#include "bignum.h"

const string bignum::signlist = "0123456789abcdef";
const bignum bignum::UP_INF("_UP_INF_");
const bignum bignum::DOWN_INF("_DOWN_INF_");
const unsigned int bignum::divbit = 16;

bignum::bignum(char ch, unsigned int bt)
{
	ch += 32 * (ch >= 'A' && ch <= 'Z');//大变小

	sign = 1;
	bit = bt;

	int num = signlist.find(ch);
	if (num != string::npos)
	{
		while (1)
		{
			body_int.push_back(bignum::signlist[num % bit]);
			num /= bit;
			if (num == 0)
			{
				break;
			}
		}
	}
	else
	{
		body_int.push_back('0');
	}
}
bignum::bignum(int num, unsigned int bt)
{
	sign = (num >= 0) * 2 - 1;
	bit = bt;

	num *= sign;
	while (1)
	{
		body_int.push_back(bignum::signlist[num % bit]);
		num /= bit;
		if (num == 0)
		{
			break;
		}
	}
}
bignum::bignum(double num, unsigned int bt)
{
	sign = (num >= 0) * 2 - 1;
	bit = bt;

	num *= sign;

	int num_int = int(num);
	while (num_int != 0)
	{
		body_int.push_back(bignum::signlist[num_int % bit]);
		num_int /= bit;
	}

	num -= int(num);

	while (std::abs(num - int(num)) > 1e-7)
	{
		num *= bit;
		body_float.push_back(bignum::signlist[int(num)]);
		num -= int(num);
	}

	if (body_int.size() == 0)
	{
		//无整数部分  .909 -> 0.909
		body_int.push_back('0');

		if (body_float.size() == 0)
		{
			//无小数部分  .  ->   0.0
			this->copy(this->getnum(0));
		}
	}

}
bignum::bignum(const string& str, unsigned int bt)
{
	int length = str.length();
	sign = 2 * (str[0] != '-') - 1;
	bit = bt;
	int fpos = str.find('.');
	if (fpos != string::npos)
	{
		//有小数点
		for (int i = fpos + 1; i < length; ++i)
		{
			char midc = str[i] + 32 * (str[i] >= 'A' && str[i] <= 'Z');
			size_t mid = bignum::signlist.find(midc);
			if (mid == string::npos)
			{
				//界外符号
				body_float.clear();
				sign = 0;
				break;
			}
			if (mid + 1 > bit)
			{
				//超过默认进制
				bit = mid + 1;
			}
			body_float.push_back(midc);
		}
	}
	else
	{
		fpos = str.length();
	}
	for (int i = (sign == -1); i < fpos; ++i)
	{
		char midc = str[i] + 32 * (str[i] >= 'A' && str[i] <= 'Z');
		size_t mid = bignum::signlist.find(midc);
		if (mid == string::npos)
		{
			//界外符号
			body_int.clear();
			sign = 0;
			break;
		}
		if (mid + 1 > bit)
		{
			//超过默认进制
			bit = mid + 1;
		}
		body_int.push_back(midc);
	}
	reverse(body_int.begin(), body_int.end());

	if (body_int.size() == 0)
	{
		//无整数部分  .909 -> 0.909
		body_int.push_back('0');

		if (body_float.size() == 0)
		{
			//无小数部分  .  ->   0.0
			this->copy(this->getnum(0));
		}
	}

	if (sign == 0)
	{
		this->copy(this->getnum(0));
	}
	this->trim();

	if (str == "_UP_INF_")
	{
		sign = 1;
		bit = 10;
		body_int = { '_','U','P','_','I','N','F','_' };
		body_float.clear();
	}
	if (str == "_DOWN_INF_")
	{
		sign = -1;
		bit = 10;
		body_int = { '_','D','O','W','N','_','I','N','F','_' };
		body_float.clear();
	}

}

bignum::bignum(const bignum& oth)
{
	body_float = oth.body_float;
	body_int = oth.body_int;
	sign = oth.sign;
	bit = oth.bit;
}
bignum::~bignum()
{
	body_float.clear();
	body_int.clear();
}

//比较运算符,返回布尔值
bool bignum::operator>(const bignum& oth)const
{
	if (*this == oth)
	{
		return false;
	}

	//符号位
	if (sign > oth.sign)
	{
		return true;
	}

	if (*this == UP_INF)
	{
		return true;
	}
	if (*this == DOWN_INF)
	{
		return false;
	}

	bignum fnum = *this;
	bignum bnum = oth;

	if (fnum.bit != bnum.bit)
	{
		//system("pause");
		return false;
	}

	int csign = sign;

	int front = fnum.body_int.size();
	int back = bnum.body_int.size();
	//最长整数
	int length = front > back ? front : back;
	if (length > 0)
	{
		//整数对齐
		for (int i = 0; i <= length; ++i)
		{
			if (i >= front)
			{
				fnum.body_int.push_back('0');
			}
			if (i >= back)
			{
				bnum.body_int.push_back('0');
			}
		}
	}

	//整数位
	for (int i = length; i >= 0; --i)
	{
		if (bignum::signlist.find(fnum.body_int[i]) > bignum::signlist.find(bnum.body_int[i]))
		{
			return csign == 1 ? true : false;
		}
		if (bignum::signlist.find(fnum.body_int[i]) < bignum::signlist.find(bnum.body_int[i]))
		{
			return csign == 1 ? false : true;
		}
	}

	front = fnum.body_float.size();
	back = bnum.body_float.size();
	//最长浮点
	length = front > back ? front : back;
	if (length > 0)
	{
		//浮点对齐
		for (int i = 0; i < length; ++i)
		{
			if (i >= front)
			{
				fnum.body_float.push_back('0');
			}
			if (i >= back)
			{
				bnum.body_float.push_back('0');
			}
		}
	}

	//小数位
	for (int i = 0; i < length; ++i)
	{
		if (bignum::signlist.find(fnum.body_float[i]) > bignum::signlist.find(bnum.body_float[i]))
		{
			return csign == 1 ? true : false;
		}
		if (bignum::signlist.find(fnum.body_float[i]) < bignum::signlist.find(bnum.body_float[i]))
		{
			return csign == 1 ? false : true;
		}
	}

	return false;
}
bool bignum::operator==(const bignum& oth)const
{
	if ((this->body_int == UP_INF.body_int && oth.body_int == UP_INF.body_int) || (this->body_int == DOWN_INF.body_int && oth.body_int == DOWN_INF.body_int))
	{
		return true;
	}
	bignum fnum = *this;
	bignum bnum = oth;

	if (fnum.bit != bnum.bit)
	{
		//system("pause");
		return false;
	}

	//fnum.bit > bnum.bit ? bnum.setbit(fnum.bit) : fnum.setbit(bnum.bit);

	return fnum.sign == bnum.sign && fnum.body_float == bnum.body_float && fnum.body_int == bnum.body_int;
}
bool bignum::operator<(const bignum& oth)const
{
	return !(*this >= oth);
}
bool bignum::operator>=(const bignum& oth)const
{
	return *this > oth || *this == oth;
}
bool bignum::operator<=(const bignum& oth)const
{
	return !(*this > oth);
}
bool bignum::operator!=(const bignum& oth)const
{
	return !(*this == oth);
}

//四则运算,返回右值
const bignum bignum::operator+(const bignum& oth)const
{
	if (this->bit != oth.bit)
	{
		cout << "进制不同!" << endl;
		return this->getnum(0);
	}
	//只处理同号运算
	if (this->sign != oth.sign)
	{
		return (*this) - (-oth);
	}

	bignum fnum = *this;
	bignum bnum = oth;
	int cbit = this->bit;

	//浮点运算预备
	int front = fnum.body_float.size();
	int back = bnum.body_float.size();
	//最长浮点
	int length = front > back ? front : back;
	if (length > 0)
	{
		//浮点对齐
		for (int i = 0; i < length; ++i)
		{
			if (i >= front)
			{
				fnum.body_float.push_back('0');
			}
			if (i >= back)
			{
				bnum.body_float.push_back('0');
			}
		}
	}
	//计算浮点
	int upbit = 0;
	for (int i = length - 1; i >= 0; --i)
	{
		int all = bignum::signlist.find(fnum.body_float[i]) + bignum::signlist.find(bnum.body_float[i]) + upbit;
		upbit = all >= cbit;
		if (upbit)
		{
			fnum.body_float[i] = bignum::signlist[all - cbit];
		}
		else
		{
			fnum.body_float[i] = bignum::signlist[all];
		}
	}
	//整数运算预备
	front = fnum.body_int.size();
	back = bnum.body_int.size();
	//最长整数
	length = front > back ? front : back;
	if (length > 0)
	{
		//整数对齐
		for (int i = 0; i <= length; ++i)
		{
			if (i >= front)
			{
				fnum.body_int.push_back('0');
			}
			if (i >= back)
			{
				bnum.body_int.push_back('0');
			}
		}
	}
	for (int i = 0; i <= length; ++i)
	{
		int all = bignum::signlist.find(fnum.body_int[i]) + bignum::signlist.find(bnum.body_int[i]) + upbit;
		upbit = all >= cbit;
		if (upbit)
		{
			fnum.body_int[i] = bignum::signlist[all - cbit];
		}
		else
		{
			fnum.body_int[i] = bignum::signlist[all];
		}
	}

	fnum.trim();

	return fnum;
}
const bignum bignum::operator-(const bignum& oth)const
{
	if (this->bit != oth.bit)
	{
		cout << "进制不同!" << endl;
		return this->getnum(0);
	}

	if (*this == oth)
	{
		return this->getnum(0);
	}

	//只处理同号运算

	if (this->sign != oth.sign)
	{
		return (*this) + (-oth);
	}

	int csign = 2 * (*this > oth) - 1;
	int cbit = this->bit;
	bignum fnum = this->abs() > oth.abs() ? this->abs() : oth.abs();
	bignum bnum = this->abs() <= oth.abs() ? this->abs() : oth.abs();
	//大前小后

	//浮点运算预备
	int front = fnum.body_float.size();
	int back = bnum.body_float.size();
	//最长浮点
	int length = front > back ? front : back;
	if (length > 0)
	{
		//浮点对齐
		for (int i = 0; i < length; ++i)
		{
			if (i >= front)
			{
				fnum.body_float.push_back('0');
			}
			if (i >= back)
			{
				bnum.body_float.push_back('0');
			}
		}
	}
	//计算浮点
	int upbit = 0;
	for (int i = length - 1; i >= 0; --i)
	{
		int all = bignum::signlist.find(fnum.body_float[i]) - bignum::signlist.find(bnum.body_float[i]) - upbit;
		upbit = all < 0;
		if (upbit)
		{
			fnum.body_float[i] = bignum::signlist[all + cbit];
		}
		else
		{
			fnum.body_float[i] = bignum::signlist[all];
		}
	}
	//整数运算预备
	front = fnum.body_int.size();
	back = bnum.body_int.size();
	//最长整数
	length = front > back ? front : back;
	if (length > 0)
	{
		//整数对齐
		for (int i = 0; i <= length; ++i)
		{
			if (i >= front)
			{
				fnum.body_int.push_back('0');
			}
			if (i >= back)
			{
				bnum.body_int.push_back('0');
			}
		}
	}
	for (int i = 0; i <= length; ++i)
	{
		int all = bignum::signlist.find(fnum.body_int[i]) - bignum::signlist.find(bnum.body_int[i]) - upbit;
		upbit = all < 0;
		if (upbit)
		{
			fnum.body_int[i] = bignum::signlist[all + cbit];
		}
		else
		{
			fnum.body_int[i] = bignum::signlist[all];
		}
	}

	fnum.trim();
	fnum.sign = csign;

	return fnum;
}
const bignum bignum::operator*(const bignum& oth)const
{
	if (this->bit != oth.bit)
	{
		cout << "进制不同!";
		return this->getnum(0);
	}
	if (*this == getnum(0) || oth == getnum(0))
	{
		return getnum(0);
	}

	bignum fnum = *this;
	bignum bnum = oth;
	int csign = fnum.sign * bnum.sign;
	int cbit = this->bit;

	if (fnum.abs() == 1)
	{
		bnum.sign = csign;
		return bnum;
	}
	if (bnum.abs() == 1)
	{
		fnum.sign = csign;
		return fnum;
	}

	//处理无穷
	if (fnum.abs() == UP_INF || bnum.abs() == UP_INF)
	{
		return csign > 0 ? UP_INF : DOWN_INF;
	}

	vector<int>fall;
	vector<int>ball;
	int fbit = fnum.body_float.size() + bnum.body_float.size();
	//小数位

	//去小数
	for_each(fnum.body_float.rbegin(), fnum.body_float.rend(), [&fall](char val)->void {fall.push_back(bignum::signlist.find(val)); });
	for_each(fnum.body_int.begin(), fnum.body_int.end(), [&fall](char val)->void {fall.push_back(bignum::signlist.find(val)); });

	for_each(bnum.body_float.rbegin(), bnum.body_float.rend(), [&ball](char val)->void {ball.push_back(bignum::signlist.find(val)); });
	for_each(bnum.body_int.begin(), bnum.body_int.end(), [&ball](char val)->void {ball.push_back(bignum::signlist.find(val)); });

	vector<int>result(fall.size() + ball.size(), 0);

	//各位相乘
	for (size_t i = 0; i < fall.size(); ++i)
	{
		for (size_t j = 0; j < ball.size(); ++j)
		{
			result[i + j] += (fall[i]) * (ball[j]);
		}
	}

	//进位
	reverse(result.begin(), result.end());
	for (int k = result.size() - 1; k > 0; --k)
	{
		if (result[k] >= cbit)
		{
			result[k - 1] += result[k] / cbit;
			result[k] = (result[k]) % cbit;
		}
	}

	//解析result
	bignum rnum;
	rnum.bit = cbit;
	rnum.sign = csign;

	for (int i = result.size() - 1; i >= 0; --i, --fbit)
	{
		if (fbit > 0)
		{
			rnum.body_float.push_back(bignum::signlist[result[i]]);
		}
		else
		{
			rnum.body_int.push_back(bignum::signlist[result[i]]);
		}
	}
	reverse(rnum.body_float.begin(), rnum.body_float.end());

	rnum.trim();
	return rnum;

}
const bignum bignum::operator/(const bignum& oth)const
{
	if (this->bit != oth.bit)
	{
		cout << "进制不同!" << endl;
		return this->getnum(0);
	}

	bignum fnum = this->abs();
	bignum bnum = oth.abs();

	int csign = this->sign * oth.sign;
	unsigned int cbit = fnum.bit;
	int fbit = 0;

	if (fnum == getnum(0))
	{
		return getnum(0);
	}
	if (fnum.abs() == bnum.abs())
	{
		return getnum(sign);
	}
	if (bnum.abs() == getnum(1))
	{
		fnum.sign = csign;
		return fnum;
	}
	if (bnum.abs() == getnum(0))
	{
		cout << "除数不可为0" << endl;
		return csign > 0 ? UP_INF : DOWN_INF;
	}

	//对齐
	while (fnum.body_float.size() > 0)
	{
		//左移一位
		fnum << 1;
		fbit++;
	}
	while (bnum.body_float.size() > 0 || fnum.abs() > bnum.abs())
	{
		//左移一位
		bnum << 1;
		fbit--;
	}
	while (fnum.abs() <= bnum.abs())
	{
		fnum << 1;
		fbit++;
	}

	int acbit = 8 + fnum.body_int.size() + bnum.body_int.size();
	vector<char>result;

	//出口为达到精度或者整除
	while (int(result.size()) < acbit)
	{
		int mbit = 0;
		while (fnum.abs() >= bnum.abs())
		{
			fnum -= bnum;
			mbit++;
		}
		result.push_back(bignum::signlist[mbit]);

		if (fnum.abs() == getnum(0))
		{
			break;
		}

		fnum << 1;
	}

	bignum rnum;
	rnum.sign = csign;
	rnum.bit = cbit;

	rnum.body_int.push_back(result[0]);

	rnum.body_float.insert(rnum.body_float.end(), result.begin() + 1, result.end());

	rnum >> fbit;

	return rnum;
}

//自增减,前置返回引用,后置返回右值
bignum& bignum::operator++()
{
	this->copy(*this + getnum(1));
	return *this;
}
bignum& bignum::operator--()
{
	this->copy(*this - getnum(1));
	return *this;
}
const bignum bignum::operator++(int)
{
	bignum mid = *this;
	this->copy(*this + getnum(1));
	return mid;
}
const bignum bignum::operator--(int)
{
	bignum mid = *this;
	this->copy(*this - getnum(1));
	return mid;
}

//赋值运算符
bignum& bignum::operator=(const bignum& oth)
{
	this->copy(oth);
	return *this;
}
bignum& bignum::operator+=(const bignum& oth)
{
	this->copy(*this + oth);
	return *this;
}
bignum& bignum::operator-=(const bignum& oth)
{
	this->copy(*this - oth);
	return *this;
}
bignum& bignum::operator*=(const bignum& oth)
{
	this->copy(*this * oth);
	return *this;
}
bignum& bignum::operator/=(const bignum& oth)
{
	this->copy(*this / oth);
	return *this;
}

//特殊运算符
bignum bignum::operator-()const
{
	if (*this == UP_INF)
	{
		return DOWN_INF;
	}
	if (*this == DOWN_INF)
	{
		return UP_INF;
	}

	bignum mid = *this;
	mid.sign *= -1;
	return mid;
}
const bignum bignum::operator%(const bignum& oth)const
{
	return *this - (*this / oth).fixedto(0) * oth;
}
const bignum bignum::operator^(const bignum& oth)const
{
	bignum i_part = oth;
	i_part.body_float.clear();
	i_part.trim();

	bignum f_part = oth;
	f_part.body_int.clear();
	f_part.trim();

	if (oth.isint())
	{
		return this->pow_i(i_part);
	}
	else
	{
		return this->pow_i(i_part) * this->pow_f(f_part);
	}
}

//位运算
bignum& bignum::operator<<(int mbit)
{
	//左移,变大
	if (mbit < 0)
	{
		return *this >> (-mbit);
	}
	//扩小数位
	for (int i = body_float.size(); i < mbit; ++i)
	{
		body_float.push_back('0');
	}
	vector<char>mid;
	mid.insert(mid.end(), body_float.rend() - mbit, body_float.rend());
	mid.insert(mid.end(), body_int.begin(), body_int.end());

	body_int = mid;

	mid.clear();
	mid.insert(mid.end(), body_float.begin() + mbit, body_float.end());
	body_float = mid;

	this->trim();
	return *this;
}
bignum& bignum::operator>>(int mbit)
{
	//右移,变小
	if (mbit < 0)
	{
		return *this << (-mbit);
	}
	//扩整数位
	for (int i = body_int.size(); i <= mbit; ++i)
	{
		body_int.push_back('0');
	}
	vector<char>mid;
	mid.insert(mid.end(), body_int.rend() - mbit, body_int.rend());
	mid.insert(mid.end(), body_float.begin(), body_float.end());

	body_float = mid;

	mid.clear();
	mid.insert(mid.end(), body_int.begin() + mbit, body_int.end());
	body_int = mid;

	this->trim();
	return *this;
}


//私有函数
bignum bignum::pow_i(const bignum& oth)const
{
	if (oth.getbit(10) == 0)
	{
		return 1;
	}
	if (oth.getbit(10) == 1)
	{
		return *this;
	}
	if (this->getbit(10) == 0)
	{
		return 0;
	}

	bignum fnum = *this;
	bignum bnum = oth;

	int csign = (fnum.sign == -1 && bnum % 2 == 1) ? -1 : 1;
	int cbit = fnum.bit;

	fnum = fnum.abs();
	if (bnum < bignum(0, bnum.bit))
	{
		return bignum(1) / fnum.pow_i(-bnum);
	}
	// 1*2*2+ 1*2+  1

	//bnum>=2
	bnum.setbit(2);
	--bnum;
	for (bignum pre = fnum; bnum > bignum(0, 2); bnum >> 1, bnum.fixedto(0), pre *= pre)
	{
		if (bnum.body_int[0] == '1')
		{
			fnum *= pre;
		}
	}

	fnum.sign = csign;
	return fnum;

}
bignum bignum::pow_f(const bignum& oth)const
{
	if (*this == getnum(0))
	{
		return getnum(0);
	}
	if (oth == bignum(0, oth.bit))
	{
		return getnum(0);
	}
	if (oth < bignum(0, oth.bit))
	{
		return getnum(1) / this->pow_f(-oth);
	}

	const int ans_bit = 15;

	bignum fnum = *this;
	bignum bnum = oth;

	if (fnum == getnum(2))
	{
		return getnum(1) / bignum("0.5", bit).pow_f(bnum);
	}

	int gbit = 0;
	while (fnum > bignum("1.5", bit))
	{
		fnum /= 2;
		++gbit;

		if (fnum <= bignum("1.5", bit))
		{
			return fnum.pow_f(bnum) * (getnum(2) ^ (bnum * gbit));
		}
	}
	while (fnum < bignum("0.5", bit))
	{
		fnum *= 2;
		++gbit;

		if (fnum >= bignum("0.5", bit))
		{
			return fnum.pow_f(bnum) / (getnum(2) ^ (bnum * gbit));
		}
	}

	bignum powf = 0, x = fnum - 1, tmpm = 1;
	bignum fix(1, bit);
	fix >> ans_bit + 4;
	for (int i = 1; tmpm.abs() > fix; i++, --bnum)
	{
		tmpm *= bnum * x / i;
		powf += tmpm;
	}
	return (powf + 1).trimto(ans_bit - 1);

}
void bignum::copy(const bignum& oth)
{
	body_float = oth.body_float;
	body_int = oth.body_int;
	sign = oth.sign;
	bit = oth.bit;
}
bignum bignum::getnum(int num)const
{
	return bignum(num, bit);
}

//公共函数
ostream& operator<<(ostream& out, const bignum& obj)
{
	if (obj == bignum::UP_INF)
	{
		out << "_UP_INF_";
		return out;
	}
	if (obj == bignum::DOWN_INF)
	{
		out << "_DOWN_INF_";
		return out;
	}

	if (obj.body_float.size() == 0 && obj.body_int.size() == 0)
	{
		out << 0;
		return out;
	}

	if (obj.sign == -1)
	{
		out << '-';
	}

	for (int i = obj.body_int.size() - 1; i >= 0; --i)
	{
		out << obj.body_int[i];
	}

	int length = obj.body_float.size();
	if (length > 0)
	{
		out << '.';
	}

	for (int i = 0; i < length; ++i)
	{
		out << obj.body_float[i];
	}

	if (obj.bit != 10)
	{
		out << "(" << obj.bit << ")";
	}

	return out;
}
istream& operator>>(istream& in, bignum& obj)
{
	string str;
	in >> str;

	bignum mid(str);
	obj.copy(mid);

	return in;
}

bool bignum::isint()const
{
	return !body_float.size();
}
bool bignum::isfloat()const
{
	return body_float.size();
}
bool bignum::is(int num)const
{
	return *this == getnum(num);
}
bool bignum::upper(int num)const
{
	return *this > getnum(num);
}
bool bignum::lower(int num)const
{
	return *this < getnum(num);
}
unsigned int bignum::gbit()const
{
	return bit;
}

bignum::operator int()const
{
	int rnum = 0;
	int tbit = bit;
	for_each(body_int.rbegin(), body_int.rend(), [&rnum, tbit](char val)->void {rnum *= tbit; rnum += signlist.find(val);  });

	return rnum;
}

bignum bignum::abs()const
{
	if (this->body_int == DOWN_INF.body_int)
	{
		return UP_INF;
	}
	bignum mid = *this;
	mid.sign = 1;
	return mid;
}
bignum& bignum::trim()
{
	while (body_float.size() > 0)
	{
		if (body_float[body_float.size() - 1] == '0')
		{
			body_float.pop_back();
		}
		else
		{
			break;
		}
	}
	while (body_int.size() > 1)
	{
		if (body_int[body_int.size() - 1] == '0')
		{
			body_int.pop_back();
		}
		else
		{
			break;
		}
	}

	if (body_int.size() == 0)
	{
		body_int.push_back('0');
	}

	return *this;
}
bignum& bignum::trimto(size_t tbit)
{
	*this += getnum(5 * this->sign) >> (tbit + 1);
	return this->fixedto(tbit);
}
bignum bignum::trimto(size_t tbit)const
{
	bignum mid = *this;
	mid.trimto(tbit);
	return mid;
}
bignum& bignum::fixedto(size_t fbit)
{
	if (fbit >= 0 && fbit < this->body_float.size())
	{
		vector<char> mid(body_float.begin(), body_float.begin() + fbit);
		body_float = mid;
	}
	trim();
	return *this;
}
bignum bignum::fixedto(size_t fbit)const
{
	bignum rnum = *this;
	if (fbit >= 0 && fbit < rnum.body_float.size())
	{
		vector<char> mid(rnum.body_float.begin(), rnum.body_float.begin() + fbit);
		rnum.body_float = mid;
	}
	return rnum.trim();
}
bignum& bignum::usebit(int numbit)
{
	trim();
	bignum mid = *this;

	int pbit = mid.body_float.size();
	mid << pbit;

	for (int to = mid.body_int.size() - numbit; to >= 0; --to)
	{
		mid.body_int[to] = '0';
	}

	mid >> pbit;

	*this = mid;
	trim();

	return *this;
}
bignum& bignum::setbit(int numbit)
{
	if (numbit < 2 || numbit >16)
	{
		cout << "转化参数越界!" << endl;
		return *this;
	}

	if (numbit == bit)
	{
		return *this;
	}

	if (abs().body_int[0] == '_')
	{
		bit = numbit;
		return *this;
	}

	bignum nbit(numbit, bit);
	bignum mid;
	int fbit = 0;

	while (this->body_float.size() > 0)
	{
		*this << 1;
		fbit++;
	}

	string result;

	while (1)
	{
		bignum midabs = this->abs();
		mid = bignum(int(midabs % nbit), numbit);
		result.push_back(mid.fixedto(0).body_int[0]);

		*this /= nbit;
		this->fixedto(0);

		if (*this == getnum(0))
		{
			break;
		}
	}

	reverse(result.begin(), result.end());

	nbit = bignum(int(bit), numbit);
	*this = result;

	bit = numbit;
	for (int i = 0; i < fbit; ++i)
	{
		*this /= nbit;
	}


	return *this;
}
bignum bignum::getbit(int numbit)const
{
	bignum mid = *this;
	mid.setbit(numbit);
	return mid;
}
bignum bignum::fact()
{
	if (body_float.size() == 0 && *this > getnum(0))
	{
		bignum rnum(1);
		for (bignum i = 1; i <= *this; ++i)
		{
			rnum.copy(rnum * i);
		}
		return rnum;
	}
	else
	{
		cout << "阶乘调用错误!";
		return 1;
	}
}

bignum bignum::rand()
{
	srand(clock());
	return bignum(std::rand());
}
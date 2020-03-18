#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <Windows.h>

using namespace std;

class bignum {

private:
	vector<char> body_int;//整数部分
	vector<char> body_float;//小数部分
	short sign;
	unsigned int bit;//进制

private:

	bignum pow_i(const bignum& oth)const;
	bignum pow_f(const bignum& oth)const;
	bignum getnum(int nbit)const;
	void copy(const bignum& oth);

public:
	bignum() { bit = 10; sign = 1; }

	bignum(char ch, unsigned int bt = 10);
	bignum(int num, unsigned int bt = 10);
	bignum(double num, unsigned int bt = 10);
	bignum(const string& str, unsigned int bt = 10);//字符串初始化

	bignum(const bignum& oth);//复制构造函数
	~bignum();

	//比较运算符,返回布尔值
	bool operator>(const bignum& oth)const;
	bool operator==(const bignum& oth)const;
	bool operator<(const bignum& oth)const;
	bool operator>=(const bignum& oth)const;
	bool operator<=(const bignum& oth)const;
	bool operator!=(const bignum& oth)const;

	//四则运算,返回右值
	const bignum operator+(const bignum& oth)const;
	const bignum operator-(const bignum& oth)const;
	const bignum operator*(const bignum& oth)const;
	const bignum operator/(const bignum& oth)const;

	//自增减,前置返回引用,后置返回右值
	bignum& operator++();//前置
	bignum& operator--();//前置
	const bignum operator++(int);//后置
	const bignum operator--(int);//后置

	//赋值运算符
	bignum& operator=(const bignum& oth);
	bignum& operator+=(const bignum& oth);
	bignum& operator-=(const bignum& oth);
	bignum& operator*=(const bignum& oth);
	bignum& operator/=(const bignum& oth);

	//特殊运算符
	const bignum operator%(const bignum& oth)const;//求模
	const bignum operator^(const bignum& oth)const;//幂次

	//位运算
	bignum& operator<<(int mbit);
	bignum& operator>>(int mbit);

	bignum operator-()const;//负数

	//公共函数
	friend ostream& operator<<(ostream& out, const bignum& obj);//输出
	friend istream& operator>>(istream& in, bignum& obj);//输入

	bool isint()const;
	bool isfloat()const;
	bool is(int num)const;
	bool upper(int num)const;
	bool lower(int num)const;
	unsigned int gbit()const;

	explicit operator int()const;

	bignum abs()const;//绝对值
	bignum& trim();//去除无效0
	bignum& trimto(size_t tbit);//精度转化(会舍入)
	bignum trimto(size_t tbit)const;
	bignum& fixedto(size_t fbit);//精度转化
	bignum fixedto(size_t fbit)const;
	bignum& usebit(int numbit);//取有效位
	bignum& setbit(int numbit);//进制转化
	bignum getbit(int numbit)const;//获取值
	bignum fact();//阶乘

	const static string signlist;
	const static bignum UP_INF;
	const static bignum DOWN_INF;
	const static unsigned int divbit;

	static bignum rand();

};


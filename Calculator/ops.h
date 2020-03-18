#pragma once
#include "calculator.h"

map<string, basenode*> calculator::varmap;
//const ops multiple{ "*",2,{-1,1} ,1 ,"乘法运算,支持高精度"};
class multiple :public ops
{
public:
	multiple()
	{
		opsdata = { "*",2,{-1,1} ,4 ,"乘法运算,支持高精度" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() * (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops divide{ "/",2,{-1,1} ,2 ,"除法运算,请勿除0" };
class divide :public ops
{
public:
	divide()
	{
		opsdata = { "/",2,{-1,1} ,4 ,"除法运算,请勿除0" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() / (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops add{ "+",2,{-1,1} ,3 ,"加法运算" };
class addition :public ops
{
public:
	addition()
	{
		opsdata = { "+",2,{-1,1} ,5 ,"加法运算" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() + (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops subtract{ "-",2,{-1,1},4 ,"减法运算" };
class subtract :public ops
{
public:
	subtract()
	{
		opsdata = { "-",2,{-1,1},5 ,"减法运算" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() - (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops power{ "^",2,{-1,1} ,5 ,"乘方运算,支持小数,速度较慢" };
class power :public ops
{
public:
	power()
	{
		opsdata = { "^",2,{-1,1} ,3 ,"乘方运算,支持小数,速度较慢" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() ^ (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops module{ "%",2,{-1,1} ,6 ,"求模运算,支持小数,请勿模0" };
class module :public ops
{
public:
	module()
	{
		opsdata = { "%",2,{-1,1} ,4 ,"求模运算,支持小数,请勿模0" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() % (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};

class upthan :public ops
{
public:
	upthan()
	{
		opsdata = { ">",2,{-1,1} ,6 ,"比较运算符" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() > (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};

class lowthan :public ops
{
public:
	lowthan()
	{
		opsdata = { "<",2,{-1,1} ,6 ,"比较运算符" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() < (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};

//const ops valcopy{ "=",2,{-1,1} ,7 ,"赋值运算,可以以xxx=123的形式注册一个变量,也可用已注册的变量初始化xxx" };
class valcopy :public ops
{
public:
	valcopy()
	{
		opsdata = { "=",2,{-1,1} ,7 ,"赋值运算,可以以xxx=123的形式注册一个变量,也可用已注册的变量初始化xxx" };
	}
	basenode* fun(const nodelist& vlist)
	{
		delete(((var*)(vlist.info[0]))->tonode);
		basenode* from = nullptr;
		if (vlist.info[1]->type == "var")
		{
			from = vlist.info[1]->getchild<var>()->tonode;
		}
		else
		{
			from = vlist.info[1];
		}

		vlist.info[0]->getchild<var>()->tonode = factory::createnode(from->type, "");
		if (from->type == "num")
		{
			vlist.info[0]->getchild<var>()->tonode->getchild<number>()->num = from->getchild<number>()->num;
		}
		else if (from->type == "mstring")
		{
			vlist.info[0]->getchild<var>()->tonode->getchild<mstring>()->info = from->getchild<mstring>()->info;
		}

		calculator::varmap[vlist.info[0]->getchild<var>()->name] = vlist.info[0]->getchild<var>()->tonode;

		return vlist.info[1];
	}
};

//const ops fact{ "!",1,{-1} ,8 ,"阶乘运算,只支持正整数,建议不要超过2000" };
class fact :public ops
{
public:
	fact()
	{
		opsdata = { "!",1,{-1} ,2 ,"阶乘运算,只支持正整数,建议不要超过2000" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum().fact();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//
//const ops all{ "sum",3 ,{1} ,9,"求和函数,可输入任意个参数" };
class all :public ops
{
public:
	all()
	{
		opsdata = { "sum",3 ,{1} ,1,"求和函数,可输入任意个参数" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans(0, (vlist.info[0])->getnum().gbit());
		for (auto index = vlist.info.begin(); index != vlist.info.end(); ++index)
		{
			rans += (*index)->getnum();
		}

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops factsum{ "factsum",3 ,{1} ,15,"求阶乘之和的函数,参数为正整数" };
class factsum :public ops
{
public:
	factsum()
	{
		opsdata = { "sum",3 ,{1} ,1,"求和函数,可输入任意个参数" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum endnum = ((number*)(vlist.info[0]))->num;
		bignum rans(0, endnum.gbit());
		bignum index(1, endnum.gbit());
		for (bignum i = index; i <= endnum; ++i)
		{
			index *= i;
			rans += index;
		}

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops forloop{ "for",3,{1},16,"将下一条语句重复执行,样例:x=1 for(10) x=x+1;show(),注意:使用空格隔开的为一条语句,使用分号是将多条语句缩合成一条语句" };
class forloop :public ops
{
public:
	forloop()
	{
		opsdata = { "for",3,{1},1,"参数为3个字符串,分别是初始化语句,条件语句,循环体语句" };
	}
	basenode* fun(const nodelist& vlist)
	{
		string start = ((mstring*)(vlist.info[0]))->info;
		string need = ((mstring*)(vlist.info[1]))->info;
		string iterate = ((mstring*)(vlist.info[2]))->info;

		calculator midcal;
		midcal << start;
		while (1)
		{
			midcal << need;
			if (!midcal.ans->isnum())
			{
				throw(exception("for的循环条件必须为数值类型"));
			}
			if (midcal.ans->getnum().is(0))
			{
				break;
			}
			midcal << iterate;
		}

		return new basenode;
	}
};
//
//const ops sbit{ "set",3,{1},11 ,"使用set(xxx,yyy)求得xxx在yyy进制下的表示,yyy的范围为2~16,(注意不同进制的数据不能进行计算!)" };
//
//const ops clear{ "clear",3 ,{1} ,10 ,"使用clear()注销所有变量" };
//const ops showval{ "show",3,{1},12,"使用show()查看所有变量" };
//
//const ops help{ "help",3,{1},13,"使用help()查看所有帮助" };
//const ops cls{ "cls",3,{},14 ,"使用cls清理屏幕" };

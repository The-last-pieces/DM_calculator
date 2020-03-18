#pragma once
#include "calculator.h"

map<string, basenode*> calculator::varmap;
//const ops multiple{ "*",2,{-1,1} ,1 ,"�˷�����,֧�ָ߾���"};
class multiple :public ops
{
public:
	multiple()
	{
		opsdata = { "*",2,{-1,1} ,4 ,"�˷�����,֧�ָ߾���" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() * (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops divide{ "/",2,{-1,1} ,2 ,"��������,�����0" };
class divide :public ops
{
public:
	divide()
	{
		opsdata = { "/",2,{-1,1} ,4 ,"��������,�����0" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() / (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops add{ "+",2,{-1,1} ,3 ,"�ӷ�����" };
class addition :public ops
{
public:
	addition()
	{
		opsdata = { "+",2,{-1,1} ,5 ,"�ӷ�����" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() + (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops subtract{ "-",2,{-1,1},4 ,"��������" };
class subtract :public ops
{
public:
	subtract()
	{
		opsdata = { "-",2,{-1,1},5 ,"��������" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() - (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops power{ "^",2,{-1,1} ,5 ,"�˷�����,֧��С��,�ٶȽ���" };
class power :public ops
{
public:
	power()
	{
		opsdata = { "^",2,{-1,1} ,3 ,"�˷�����,֧��С��,�ٶȽ���" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() ^ (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};
//const ops module{ "%",2,{-1,1} ,6 ,"��ģ����,֧��С��,����ģ0" };
class module :public ops
{
public:
	module()
	{
		opsdata = { "%",2,{-1,1} ,4 ,"��ģ����,֧��С��,����ģ0" };
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
		opsdata = { ">",2,{-1,1} ,6 ,"�Ƚ������" };
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
		opsdata = { "<",2,{-1,1} ,6 ,"�Ƚ������" };
	}
	basenode* fun(const nodelist& vlist)
	{
		bignum rans = (vlist.info[0])->getnum() < (vlist.info[1])->getnum();

		number* rnode = (number*)factory::createnode("num", "0");
		rnode->num = rans;

		return rnode;
	}
};

//const ops valcopy{ "=",2,{-1,1} ,7 ,"��ֵ����,������xxx=123����ʽע��һ������,Ҳ������ע��ı�����ʼ��xxx" };
class valcopy :public ops
{
public:
	valcopy()
	{
		opsdata = { "=",2,{-1,1} ,7 ,"��ֵ����,������xxx=123����ʽע��һ������,Ҳ������ע��ı�����ʼ��xxx" };
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

//const ops fact{ "!",1,{-1} ,8 ,"�׳�����,ֻ֧��������,���鲻Ҫ����2000" };
class fact :public ops
{
public:
	fact()
	{
		opsdata = { "!",1,{-1} ,2 ,"�׳�����,ֻ֧��������,���鲻Ҫ����2000" };
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
//const ops all{ "sum",3 ,{1} ,9,"��ͺ���,���������������" };
class all :public ops
{
public:
	all()
	{
		opsdata = { "sum",3 ,{1} ,1,"��ͺ���,���������������" };
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
//const ops factsum{ "factsum",3 ,{1} ,15,"��׳�֮�͵ĺ���,����Ϊ������" };
class factsum :public ops
{
public:
	factsum()
	{
		opsdata = { "sum",3 ,{1} ,1,"��ͺ���,���������������" };
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
//const ops forloop{ "for",3,{1},16,"����һ������ظ�ִ��,����:x=1 for(10) x=x+1;show(),ע��:ʹ�ÿո������Ϊһ�����,ʹ�÷ֺ��ǽ�����������ϳ�һ�����" };
class forloop :public ops
{
public:
	forloop()
	{
		opsdata = { "for",3,{1},1,"����Ϊ3���ַ���,�ֱ��ǳ�ʼ�����,�������,ѭ�������" };
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
				throw(exception("for��ѭ����������Ϊ��ֵ����"));
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
//const ops sbit{ "set",3,{1},11 ,"ʹ��set(xxx,yyy)���xxx��yyy�����µı�ʾ,yyy�ķ�ΧΪ2~16,(ע�ⲻͬ���Ƶ����ݲ��ܽ��м���!)" };
//
//const ops clear{ "clear",3 ,{1} ,10 ,"ʹ��clear()ע�����б���" };
//const ops showval{ "show",3,{1},12,"ʹ��show()�鿴���б���" };
//
//const ops help{ "help",3,{1},13,"ʹ��help()�鿴���а���" };
//const ops cls{ "cls",3,{},14 ,"ʹ��cls������Ļ" };

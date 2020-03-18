#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "bignum.h"
using namespace std;

class basenode;
class number;
class var;
class mstring;
class dived;
class nodelist;
class ops;

class basenode
{
public:
	string type;
	//基类型
	//数值常量,变量,操作符,字符串常量,分隔符,无类型(默认),列表(计算时生成)
public:
	basenode() { type = "none"; }
	virtual ~basenode() {}
	size_t size();
	template <typename T>
	T* getchild()
	{
		return (T*)this;
	}
	bool isnum();
	bignum getnum();
	friend ostream& operator<<(ostream& out, const basenode* base);
};

class number :public basenode
{
public:
	bignum num;
	number() { num = 0; }
	number(string val) { num = val; }
	~number() {}
};

class var :public basenode
{
public:
	string name;
	basenode* tonode;//数值常量或者字符串常量
	var() { tonode = new basenode; }
	var(string val) { name = val; tonode = new basenode; }
	~var() { }
};

class ops :public basenode
{
public:
	struct
	{
		string name;
		int type = 2;//一元,二元,函数
		vector<int> plist;//参数偏移列表

		int range = 0;//优先级

		string help;
	}opsdata;
	basenode* cal(const nodelist& vlist)
	{
		catcherr(vlist);

		return fun(vlist);
	}

private:
	virtual basenode* fun(const nodelist& vlist) { return new basenode; }// = 0;
	virtual void catcherr(const nodelist& vlist) {}

public:
	ops() {}
	ops(string val) { opsdata.name = val; }
	ops(string _name, int _type, vector<int> _plist, int _id, string _help)
	{
		opsdata.name = _name;
		opsdata.type = _type;
		opsdata.plist = _plist;
		opsdata.range = _id;
		opsdata.help = _help;
	}
	~ops() {}
};

class mstring :public basenode
{
public:
	string info;
	mstring() {}
	mstring(string val) { info = val; }
	~mstring() {}
};

class dived :public basenode
{
public:
	char div;
	dived() { div = '\0'; }
	dived(string val) { div = val[0]; }
	~dived() {}
};

class nodelist :public basenode
{
public:
	vector<basenode*> info;
	nodelist() { type = "nodelist"; }
	~nodelist() {}
};


class factory
{
public:
	static ops* createops(string name);
	static basenode* createnode(string type, string val);
};


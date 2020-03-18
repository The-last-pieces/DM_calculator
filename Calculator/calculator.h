#pragma once
#include "node.h"
#include <vector>
using namespace std;

class calculator
{
private:
	string s_str;//源表达式
	vector<basenode*> expre;//待处理的表达式
	vector<basenode*>s_node;//源节点

private:
	void parse_str();//解析表达式
	basenode* inputnode(size_t& index);//从当前下标读取节点
	basenode* parse_expre(vector<basenode*>& expre);
	basenode* parse_range(vector<basenode*>& expre);
	void clear();
	void free();

	static void split(vector<basenode*>arr, vector<vector<basenode*>>& arrlist, char div);

public:
	friend istream& operator>>(istream& in, calculator& obj);//输入
	friend calculator& operator<<(calculator& obj, const string& str);//注入语句

public:

	static map<string, basenode*> varmap;//变量表

	basenode* ans = nullptr;//结果

	void show();
};

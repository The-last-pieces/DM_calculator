#pragma once
#include "node.h"
#include <vector>
using namespace std;

class calculator
{
private:
	string s_str;//Դ���ʽ
	vector<basenode*> expre;//������ı��ʽ
	vector<basenode*>s_node;//Դ�ڵ�

private:
	void parse_str();//�������ʽ
	basenode* inputnode(size_t& index);//�ӵ�ǰ�±��ȡ�ڵ�
	basenode* parse_expre(vector<basenode*>& expre);
	basenode* parse_range(vector<basenode*>& expre);
	void clear();
	void free();

	static void split(vector<basenode*>arr, vector<vector<basenode*>>& arrlist, char div);

public:
	friend istream& operator>>(istream& in, calculator& obj);//����
	friend calculator& operator<<(calculator& obj, const string& str);//ע�����

public:

	static map<string, basenode*> varmap;//������

	basenode* ans = nullptr;//���

	void show();
};

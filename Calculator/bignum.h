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
	vector<char> body_int;//��������
	vector<char> body_float;//С������
	short sign;
	unsigned int bit;//����

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
	bignum(const string& str, unsigned int bt = 10);//�ַ�����ʼ��

	bignum(const bignum& oth);//���ƹ��캯��
	~bignum();

	//�Ƚ������,���ز���ֵ
	bool operator>(const bignum& oth)const;
	bool operator==(const bignum& oth)const;
	bool operator<(const bignum& oth)const;
	bool operator>=(const bignum& oth)const;
	bool operator<=(const bignum& oth)const;
	bool operator!=(const bignum& oth)const;

	//��������,������ֵ
	const bignum operator+(const bignum& oth)const;
	const bignum operator-(const bignum& oth)const;
	const bignum operator*(const bignum& oth)const;
	const bignum operator/(const bignum& oth)const;

	//������,ǰ�÷�������,���÷�����ֵ
	bignum& operator++();//ǰ��
	bignum& operator--();//ǰ��
	const bignum operator++(int);//����
	const bignum operator--(int);//����

	//��ֵ�����
	bignum& operator=(const bignum& oth);
	bignum& operator+=(const bignum& oth);
	bignum& operator-=(const bignum& oth);
	bignum& operator*=(const bignum& oth);
	bignum& operator/=(const bignum& oth);

	//���������
	const bignum operator%(const bignum& oth)const;//��ģ
	const bignum operator^(const bignum& oth)const;//�ݴ�

	//λ����
	bignum& operator<<(int mbit);
	bignum& operator>>(int mbit);

	bignum operator-()const;//����

	//��������
	friend ostream& operator<<(ostream& out, const bignum& obj);//���
	friend istream& operator>>(istream& in, bignum& obj);//����

	bool isint()const;
	bool isfloat()const;
	bool is(int num)const;
	bool upper(int num)const;
	bool lower(int num)const;
	unsigned int gbit()const;

	explicit operator int()const;

	bignum abs()const;//����ֵ
	bignum& trim();//ȥ����Ч0
	bignum& trimto(size_t tbit);//����ת��(������)
	bignum trimto(size_t tbit)const;
	bignum& fixedto(size_t fbit);//����ת��
	bignum fixedto(size_t fbit)const;
	bignum& usebit(int numbit);//ȡ��Чλ
	bignum& setbit(int numbit);//����ת��
	bignum getbit(int numbit)const;//��ȡֵ
	bignum fact();//�׳�

	const static string signlist;
	const static bignum UP_INF;
	const static bignum DOWN_INF;
	const static unsigned int divbit;

	static bignum rand();

};


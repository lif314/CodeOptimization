#include <iostream>
#include <string>
#include <fstream>
using namespace std;


/*
 * ����1�����β鿴L�и��������ÿ����Ԫʽ���������ÿ����������Ϊ���������߶�ֵ���� L�⣬�򽫴���Ԫʽ���Ϊ"��������";
 * ����2���ظ���3��ֱ��û���µ���Ԫʽ�����Ϊ"��������"Ϊֹ;
 * ����3�����β鿴��δ�����Ϊ"��������"����Ԫʽ���������ÿ����������Ϊ��������ֵ����L֮�⣬
 *       ��ֻ��һ������-��ֵ���Ҹõ��ϵ���Ԫʽ�ѱ����Ϊ"��������"����ѱ��鿴����Ԫʽ���Ϊ"��������"��
 *
 */

/*
 * ��Ԫʽ�ṹ��
 */
struct Tac
{
	int id;		    // ���
	string op; 	    // ������
	string arg1;	// ������1
	string arg2;	// ������2
	string res;	    // ���
} tac[100];   // ������Ԫʽ

int N = 0;   // ��Ԫʽ�ĸ���

void readTac()
{
	ifstream in("unchange.txt", ios::in);
	if (!in.is_open())
	{
		cout << "Error: opening file fail" << endl;
		exit(1);
	}
	while (!in.eof())
	{
		tac[N].id = N;
		in >> tac[N].op >> tac[N].arg1 >> tac[N].arg2 >> tac[N].res;
		N++;
	}

	in.close();
}

bool notExistRes(string  a[], string arg)
{
	for(int i = 0; i < N; i++)
	{
		if(arg == a[i])
		{
			return false;
		}
	}
	return true;
}


// ѭ���������㴦��
void findUnchange()
{
	string res[100];
	for (int i = 0; i < N; i++)
	{
		res[i] = tac[i].res;
	}

	/*
	 * �����ж�������
	 *	1. ������Ϊ����
	 *	2. ������û�г�����res������
	 */
	for(int i = 0; i < N; i++)
	{
		// ������������Ϊ����
		if(isdigit(tac[i].arg1[0]) && isdigit(tac[0].arg2[0]))
			cout << tac[i].id << ": " << tac[i].op << ", " << tac[i].arg1 << ", " << tac[i].arg2 << ", " << tac[i].res << endl;
		// ��������������L�ⲿ
		else if(notExistRes(res, tac[i].arg1) && notExistRes(res, tac[i].arg2))
			cout << tac[i].id << ": " << tac[i].op << ", " << tac[i].arg1 << ", " << tac[i].arg2 << ", " << tac[i].res << endl;

		// һ��Ϊ������һ�����ⲿ
		else if((isdigit(tac[i].arg1[0]) && notExistRes(res, tac[i].arg2)) || (isdigit(tac[0].arg2[0]) && notExistRes(res, tac[i].arg1)))
			cout << tac[i].id << ": " << tac[i].op << ", " << tac[i].arg1 << ", " << tac[i].arg2 << ", " << tac[i].res << endl;
		else
			continue;
	}
}



int main()
{
	readTac();    // ��ʼ������
	cout << "============== ��Ԫʽ ===============" << endl;
	for (int i = 0; i < N; i++)
	{
		cout << tac[i].id << ": " << tac[i].op << ", " << tac[i].arg1 << ", " << tac[i].arg2 << ", " << tac[i].res << endl;
	}

	cout << "============ ѭ���������� ==============" << endl;
	findUnchange(); // Ѱ��ѭ����������
	return 0;
}
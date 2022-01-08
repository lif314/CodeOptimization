#include <iostream>
#include <string>
#include <fstream>
using namespace std;


/*
 * 步骤1：依次查看L中各基本块的每个四元式，如果它的每个运算对象或为常数，或者定值点在 L外，则将此四元式标记为"不变运算";
 * 步骤2：重复第3步直至没有新的四元式被标记为"不变运算"为止;
 * 步骤3：依次查看尚未被标记为"不变运算"的四元式，如果它的每个运算对象或为常数，或定值点在L之外，
 *       或只有一个到达-定值点且该点上的四元式已被标记为"不变运算"，则把被查看的四元式标记为"不变运算"。
 *
 */

/*
 * 四元式结构体
 */
struct Tac
{
	int id;		    // 序号
	string op; 	    // 操作符
	string arg1;	// 操作数1
	string arg2;	// 操作数2
	string res;	    // 结果
} tac[100];   // 公用四元式

int N = 0;   // 四元式的个数

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


// 循环不变运算处理
void findUnchange()
{
	string res[100];
	for (int i = 0; i < N; i++)
	{
		res[i] = tac[i].res;
	}

	/*
	 * 两个判定条件：
	 *	1. 操作数为常数
	 *	2. 操作数没有出现在res集合中
	 */
	for(int i = 0; i < N; i++)
	{
		// 两个操作数均为常数
		if(isdigit(tac[i].arg1[0]) && isdigit(tac[0].arg2[0]))
			cout << tac[i].id << ": " << tac[i].op << ", " << tac[i].arg1 << ", " << tac[i].arg2 << ", " << tac[i].res << endl;
		// 两个操作数均在L外部
		else if(notExistRes(res, tac[i].arg1) && notExistRes(res, tac[i].arg2))
			cout << tac[i].id << ": " << tac[i].op << ", " << tac[i].arg1 << ", " << tac[i].arg2 << ", " << tac[i].res << endl;

		// 一个为常数，一个在外部
		else if((isdigit(tac[i].arg1[0]) && notExistRes(res, tac[i].arg2)) || (isdigit(tac[0].arg2[0]) && notExistRes(res, tac[i].arg1)))
			cout << tac[i].id << ": " << tac[i].op << ", " << tac[i].arg1 << ", " << tac[i].arg2 << ", " << tac[i].res << endl;
		else
			continue;
	}
}



int main()
{
	readTac();    // 初始化数据
	cout << "============== 四元式 ===============" << endl;
	for (int i = 0; i < N; i++)
	{
		cout << tac[i].id << ": " << tac[i].op << ", " << tac[i].arg1 << ", " << tac[i].arg2 << ", " << tac[i].res << endl;
	}

	cout << "============ 循环不变运算 ==============" << endl;
	findUnchange(); // 寻找循环不变运算
	return 0;
}
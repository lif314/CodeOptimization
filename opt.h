#include <iostream>
#include <string>
using namespace std;


/**
 * 公用结构
 */


/*
 * 操作符集合
 */
char opera[10] = { '=','+','-','*','/','_' };

struct Opera
{
	string EQUAL = "=";
	string ADD = "+";
	string MINUS = "-";
	string MULTI = "*"; // 乘法
	string DIVIDE = "/";
	string MORE = ">";  // 大于
	string LESS = "<";  // 小于
}Opera;


/*
 * 四元式结构体
 */
struct Tac
{
	string str;     // 四元式完整的式子
	int id;		    // 序号
	char op; 	    // 操作符
	string arg1;	// 操作数1
	int num1;       // 若操作数1为数字
	string arg2;	// 操作数2
	int num2;       // 若操作数2为数字
	string res;	    // 结果
	int ans;        // 若结果为数字
}t[100];   // 公用四元式



/*
 * =================== 读取处理模块 ========================
 */

/*
 * 三地址代码转换为四元式
 */
//void process(Tac& stm) {
//
//	// T4[T3] = T1 + 1
//	string s = stm.str;
//
//	isdigit(s[0]);
//
//
//	char op = s[1];
//	for (int i = 0; i < 5; i++)
//		if (op == opera[i]) stm.op = i;
//
//
//	int ptr = 3;
//	while (s[ptr] == ' ') ptr++;
//	while (s[ptr] != ',') stm.op1 += s[ptr++];
//	ptr++;
//	while (s[ptr] == ' ') ptr++;
//	while (s[ptr] != ',') stm.op2 += s[ptr++];
//	ptr++;
//	while (s[ptr] == ' ') ptr++;
//	while (s[ptr] != ')') stm.res += s[ptr++];
//
//	if (stm.operation == 1 && stm.op2 == " ") stm.operation = 5;
//}

// 读取测试
//void debug() {
//	for (int i = 1; i < n; i++) {
//		cout << t[i].str << ' ' << t[i].id << ' ' << t[i].op1 << ' ' << opera[t[i].operation] << ' ' << t[i].op2 << ' ' << t[i].res << endl;
//	}
//}


// 读取文件
//void read() {
//	freopen("exp6_2.in", "r", stdin);
//	char s[100];
//	n = 1;
//	while (gets(s)) {
//		for (int i = 0; i < strlen(s); i++) {
//			t[n].str += s[i];
//		}
//		t[n].id = n;
//		process(t[n]);
//		n++;
//	}
//	debug();
//}



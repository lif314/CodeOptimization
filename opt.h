#include <iostream>
#include <string>
using namespace std;


/**
 * ���ýṹ
 */


/*
 * ����������
 */
char opera[10] = { '=','+','-','*','/','_' };

struct Opera
{
	string EQUAL = "=";
	string ADD = "+";
	string MINUS = "-";
	string MULTI = "*"; // �˷�
	string DIVIDE = "/";
	string MORE = ">";  // ����
	string LESS = "<";  // С��
}Opera;


/*
 * ��Ԫʽ�ṹ��
 */
struct Tac
{
	string str;     // ��Ԫʽ������ʽ��
	int id;		    // ���
	char op; 	    // ������
	string arg1;	// ������1
	int num1;       // ��������1Ϊ����
	string arg2;	// ������2
	int num2;       // ��������2Ϊ����
	string res;	    // ���
	int ans;        // �����Ϊ����
}t[100];   // ������Ԫʽ



/*
 * =================== ��ȡ����ģ�� ========================
 */

/*
 * ����ַ����ת��Ϊ��Ԫʽ
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

// ��ȡ����
//void debug() {
//	for (int i = 1; i < n; i++) {
//		cout << t[i].str << ' ' << t[i].id << ' ' << t[i].op1 << ' ' << opera[t[i].operation] << ' ' << t[i].op2 << ' ' << t[i].res << endl;
//	}
//}


// ��ȡ�ļ�
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



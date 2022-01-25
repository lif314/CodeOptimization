#ifndef _opt_
#define _opt_

#include <string>
#include <iostream>
#include <vector>
using namespace std;

/*
* ��Ԫʽ�ṹ��
*/
struct Quaternary
{
    int id;		    // ���
    string op; 	    // ������
    string arg1;	// ������1
    string arg2;	// ������2
    string res;	    // ���
    // ����Ԫʽ�󶨵���ת��� 
    string label;

    // ��ӡ�ṹ����
    void print()
    {
        cout << id << ": " << op << " " << arg1 << " " << arg2 << " " << res << std::endl;
    }
    void Output()
    {
        cout << "  " << op << ' ' << arg1 << ' ' << arg2 << ' ' << res << endl;
    }
};
/*ostream& operator << (ostream& os, Quaternary& qua)
{
    os << qua.id << ": " << qua.op << " " << qua.arg1 << " " << qua.arg2 << " " << qua.res << std::endl;
    return os;
}*/

/**
 * ������Ϣ�ͻ�Ծ��Ϣ�Ķ�Ԫ��
 */
struct StandbyActive
{
    int standby;  // �����-1��ʾ�Ǵ��ã�i��ʾ����Ԫʽ�кδ�����
    bool active;  // true��ʾ��Ծ��false��ʾ�ǻ�Ծ
};

/*
 * ������Ĵ�����Ϣ�ͻ�Ծ��Ϣ��
 */
struct BasicBlockStandbyActiveTable
{
    // ��Ԫʽ��id
    int qua_id;
    // ��ֵ
    StandbyActive res;
    // �������
    StandbyActive arg1;
    // �Ҳ�����
    StandbyActive arg2;

    void print()
    {
        cout << qua_id << ": "
            << "(" << res.standby << "," << res.active << ")" << " "
            << "(" << arg1.standby << "," << arg1.active << ")" << " "
            << "(" << arg2.standby << "," << arg2.active << ")" << endl;
    }
};


// һ����������
struct BasicBlock
{
    // ������id
    int block_id;
    // �������к��е���Ԫʽ��id����
    vector<int> quaList;
    // �жϸû������Ƿ���ѭ���壬����ѭ����������ʹ�������
    bool loop = false;
    // ��������ѭ����������ļ���
    vector<int> unchangeOpList;

    // �������д�����������ļ���
    vector<int> out_liftedList;

};
//ostream& operator<<(ostream& os, BasicBlock& block)


#endif // !_opt_
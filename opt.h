#ifndef _opt_
#define _opt_

#include <string>
#include <iostream>
#include <vector>
using namespace std;

/*
* 四元式结构体
*/
struct Quaternary
{
    int id;		    // 序号
    string op; 	    // 操作符
    string arg1;	// 操作数1
    string arg2;	// 操作数2
    string res;	    // 结果
    // 与四元式绑定的跳转标号 
    string label;

    // 打印结构数据
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
 * 待用信息和活跃信息的二元组
 */
struct StandbyActive
{
    int standby;  // 如果是-1表示非待用，i表示在四元式中何处待用
    bool active;  // true表示活跃，false表示非活跃
};

/*
 * 基本块的待用信息和活跃信息表
 */
struct BasicBlockStandbyActiveTable
{
    // 四元式的id
    int qua_id;
    // 左值
    StandbyActive res;
    // 左操作数
    StandbyActive arg1;
    // 右操作数
    StandbyActive arg2;

    void print()
    {
        cout << qua_id << ": "
            << "(" << res.standby << "," << res.active << ")" << " "
            << "(" << arg1.standby << "," << arg1.active << ")" << " "
            << "(" << arg2.standby << "," << arg2.active << ")" << endl;
    }
};


// 一个基本块类
struct BasicBlock
{
    // 基本块id
    int block_id;
    // 基本块中含有的四元式的id集合
    vector<int> quaList;
    // 判断该基本块是否是循环体，帮助循环不变运算和代码外提
    bool loop = false;
    // 基本块中循环不变运算的集合
    vector<int> unchangeOpList;

    // 基本块中代码外提运算的集合
    vector<int> out_liftedList;

};
//ostream& operator<<(ostream& os, BasicBlock& block)


#endif // !_opt_
#include <iostream>
#include <vector>
#include "opt.h"
#include "DAGraph.h"

using namespace std;

// 读取四元式文件
vector<Quaternary> readTacToQuaternary(string filename);
// 划分基本块
vector<BasicBlock> computeBasicBlock(vector<Quaternary> qua);
// 全局四元式的待用信息和活跃信息表
vector<BasicBlockStandbyActiveTable> computeStandbyActiveTable(vector<Quaternary> qua);
// 直接读取四元式文件
vector<Quaternary> readQuaternary(string filename);
// 循环不变运算
vector<BasicBlock> getUnchangeOperation(vector<Quaternary> all_qua, vector<BasicBlockStandbyActiveTable> standby_actives, vector<BasicBlock>& basic_blocks);
// 强度削弱
void strengthWeakening(vector<BasicBlock>& all_block, vector<Quaternary>& qua);
// 代码外提运算
vector<BasicBlock> Out_Lifting(const vector<BasicBlock>& BasicB, const vector<Quaternary>& Q);

// 全局函数
int main()
{
	// [1] 获取四元式集合
	// 读取三地址代码文件
	vector<Quaternary> qua = readTacToQuaternary("all.txt");
	// 读取四元式文件
	// vector<Quaternary> qua = readQuaternary("qua.txt");
	cout << "读入文件完毕\n";
	// 读入测试
	cout << "四元式集合读入测试：" << endl;
	for (int i = 0; i < qua.size(); i++)
	{
		qua[i].print();
	}

	// [2] 基本块的划分
	cout << "\n基本块划分测试：" << endl;
	vector<BasicBlock> all_block = computeBasicBlock(qua);
	// 基本块划分测试
	for (int i = 0; i < all_block.size(); i++)
	{
		cout << all_block[i].block_id << ": ";
		for (int j = 0; j < all_block[i].quaList.size(); j++)
		{
			cout << all_block[i].quaList[j] << " ";
		}
		cout << endl;
	}


	// [3] 计算每一个整个四元式集合中的待用信息和活跃信息表
	cout << "\n四元式集合的待用信息和活跃信息表：" << endl;
	vector<BasicBlockStandbyActiveTable> basic_block_standby_active_tables = computeStandbyActiveTable(qua);
	// 待用信息和活跃信息表测试
	for (int i = 0; i < basic_block_standby_active_tables.size(); i++)
	{
		basic_block_standby_active_tables[i].print();
	}


	// [5] DAG局部优化
	cout << "\nDAG局部优化测试：" << endl;
	DAGraph dag;
	for (int i = 0; i < all_block.size(); i++)
	{
		dag.DAGOptimize(all_block[i], qua);
		dag.Clear();
	}

	// [6] 寻找循环不变运算
	vector<BasicBlock> unchangeBasicBlock = getUnchangeOperation(qua, basic_block_standby_active_tables, all_block);
	// 循环不变运算测试
	cout << "\n循环不变运算测试：" << endl;
	for (int i = 0; i < unchangeBasicBlock.size(); i++)
	{
		if (unchangeBasicBlock[i].loop)
		{
			cout << unchangeBasicBlock[i].block_id << ": ";
			for (int j = 0; j < unchangeBasicBlock[i].unchangeOpList.size(); j++)
			{
				cout << unchangeBasicBlock[i].unchangeOpList[j] << " ";
			}
			cout << endl;
		}

	}



	// [7] 代码外提

	///*
	vector<BasicBlock> out_liftingBasicBlock = Out_Lifting(unchangeBasicBlock, qua);
	// 代码外提测试
	cout << "\n代码外提运算测试：" << endl;
	for (int i = 0; i < out_liftingBasicBlock.size(); i++)
	{
		cout << out_liftingBasicBlock[i].block_id << ": ";
		for (int j = 0; j < out_liftingBasicBlock[i].quaList.size(); j++)
		{
			cout << out_liftingBasicBlock[i].quaList[j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	//*/


	// [8] 强度削弱
	cout << "强度削弱测试" << endl;
	strengthWeakening(unchangeBasicBlock, qua);
	for (int i = 0; i < qua.size(); i++) {
		qua[i].print();
	}
	cout << "--------" << endl;
	for (int i = 0; i < unchangeBasicBlock.size(); i++)
	{
		cout << "基本块号：" << unchangeBasicBlock[i].block_id << endl;
		cout << "四元式集合：" << endl;
		for (int j = 0; j < unchangeBasicBlock[i].quaList.size(); j++)
		{
			cout << unchangeBasicBlock[i].quaList[j] << " ";
		}
		cout << endl;
		cout << "是否是循环：" << unchangeBasicBlock[i].loop << endl;
		cout << "循环不变量信息：" << endl;
		for (int j = 0; j < unchangeBasicBlock[i].unchangeOpList.size(); j++)
		{
			cout << unchangeBasicBlock[i].unchangeOpList[j] << " ";
		}
		cout<<endl;
	}


	return 0;
}
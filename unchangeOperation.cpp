#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include "opt.h"
using namespace std;

/*
 * @author  llf
 *
 *
 * 循环不变运算
 */

/*
 * 步骤1：依次查看L中各基本块的每个四元式，如果它的每个运算对象或为常数，或者定值点在 L外，则将此四元式标记为"不变运算";
 * 步骤2：重复第3步直至没有新的四元式被标记为"不变运算"为止;
 * 步骤3：依次查看尚未被标记为"不变运算"的四元式，如果它的每个运算对象或为常数，或定值点在L之外，
 *       或只有一个到达-定值点且该点上的四元式已被标记为"不变运算"，则把被查看的四元式标记为"不变运算"。
 */

/*
 * standby_actives: 全局四元式的待用信息表
 *
 * basic_blocks: 基本块
 */

// 寻找循环的基本块
vector<BasicBlock> findLoop(vector<Quaternary> all_qua, vector<BasicBlock> & basic_blocks)
{
	// 如果基本块的最后一个语句跳转到该基本块第一个语句，则是循环的基本块
	for(int i = 0; i < basic_blocks.size(); i++)
	{
		// 第一个四元式语句
		Quaternary start_qua = all_qua[basic_blocks[i].quaList.front()];
		// 最后一个四元式语句
		Quaternary end_qua = all_qua[basic_blocks[i].quaList.back()];

		// 最后一个语句含有跳转，并跳到首句
		if(end_qua.op.find("j") != end_qua.op.npos && (stoi(end_qua.res) + 1) == start_qua.id)
		{
			// 标记该基本块是循环的基本块
			basic_blocks[i].loop = true;
		}
	}

	return basic_blocks;
}

// 判断一个元素是否在vector中
bool isInVector(int key, vector<int> v)
{
	return std::count(v.begin(), v.end(), key);
}
// 函数重载
bool isInVector(string key, vector<string> v)
{
	return std::count(v.begin(), v.end(), key);
}


// 循环不变运算
vector<BasicBlock> getUnchangeOperation(vector<Quaternary> all_qua , vector<BasicBlockStandbyActiveTable> basic_block_standby_active_tables, vector<BasicBlock> & basic_blocks)
{
	// 寻找循环的的基本块
	vector<BasicBlock> res_basic_blocks = findLoop(all_qua, basic_blocks);

	// 计算每一个基本块中的循环不变运算
	for (int i = 0; i < res_basic_blocks.size(); i++)
	{
		// 对循环的基本块中寻找不变运算
		if (res_basic_blocks[i].loop == true)
		{
			
			// 获取基本块的四元式集合以及非基本块内的四元式集合
			vector<Quaternary> b_qua;
			for(int j = 0; j < res_basic_blocks[i].quaList.size(); j++)
			{
				b_qua.push_back(all_qua[res_basic_blocks[i].quaList[j]]);
			}

			/*vector<Quaternary> b_qua;
			vector<Quaternary> n_qua;
			for(int k = 0; k < all_qua.size(); k++)
			{
				if (isInVector(k, res_basic_blocks[i].quaList))
					b_qua.push_back(all_qua[k]);
				else
					n_qua.push_back(all_qua[k]);
			}*/

			// 获取基本快外部的定值点集合
			//vector<string> n_node;
			//for(int x = 0; i < n_qua.size();x++)
			//{
			//	n_node.push_back(n_qua[x].res);
			//}
			//// 排序
			//sort(n_node.begin(), n_node.end());
			// n_node.erase(unique(n_node.begin(), n_node.end()), n_node.end());


			// 获取基本内部的定值点集合
			vector<string> b_node;
			for (int i = 0; i < b_qua.size(); i++)
			{
				b_node.push_back(b_qua[i].res);
			}
			// 排序
			// sort(b_node.begin(), b_node.end());
			// b_node.erase(unique(b_node.begin(), b_node.end()), b_node.end());


			// 判断循环不变运算
			for(int j = 0; j < b_qua.size(); j ++)
			{
				Quaternary q = b_qua[j];

				if(q.op == "j")
					continue;

				// 两个操作数都是常数, 并且在内部没有定值点(被重新复制)
				if(isdigit(q.arg1[0]) && isdigit(q.arg2[0]) && isInVector(q.res, b_node) == 1)
				{
					res_basic_blocks[i].unchangeOpList.push_back(q.id);
				}
				// 一个是常数，另一个没有被重新定值
				else if((isdigit(q.arg1[0]) && isInVector(q.arg2, b_node) == 0  && isInVector(q.res, b_node) == 1) || (isdigit(q.arg2[0]) && isInVector(q.arg1, b_node) == 0 && isInVector(q.res, b_node) == 1))
				{
					res_basic_blocks[i].unchangeOpList.push_back(q.id);
				}
				// 两个操作数都没有被重新定值
				else if(isInVector(q.arg1, b_node) == 0 && isInVector(q.arg2, b_node) == 0  &&  isInVector(q.res, b_node) == 1 && q.res != q.arg1 && q.res != q.arg2)
				{
					res_basic_blocks[i].unchangeOpList.push_back(q.id);
				}
			}

			// 判断基本块中每一个四元式
			/*for (int j = 0; j < basic_blocks[i].quaList.size(); j++)
			{*/
				// 获取四元式对象的id
				//int qua_id = basic_blocks[i].quaList[j];
				///*
				// * 运算对象是常数(-1, false)
				// * 定值点在L外(8) 8不在基本内
				// *
				// * 该四元式的待用信息在该基本块的外部或者为常数
				// */
				// // 两个操作数是常数
				//if (basic_block_standby_active_tables[qua_id].arg1.standby < 0 && basic_block_standby_active_tables[qua_id].arg2.active == false)
				//{
				//	// 并且运算结果在基本块中不再被赋值
				//	if(basic_block_standby_active_tables[qua_id].res.standby >= 0 && !isInVector(basic_block_standby_active_tables[qua_id].res.standby, basic_blocks[i].quaList))
				//	// 添加到不变运算集合中
				//	res_basic_blocks[i].unchangeOpList.push_back(j);
				//}

				//// 定值点在L外：待用信息大于0，但不在基本块的四元式索引中
				//else if (basic_block_standby_active_tables[qua_id].arg1.standby >= 0 && !isInVector(basic_block_standby_active_tables[qua_id].arg1.standby, basic_blocks[i].quaList))
				//{
				//	// 添加到不变运算集合中
				//	res_basic_blocks[i].unchangeOpList.push_back(j);
				//}
			//}
		}
	}

	return res_basic_blocks;
}
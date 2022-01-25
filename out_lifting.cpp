#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "opt.h"

using namespace std;

// 查找四元式所在基本块块号
int findBasic(int qID, const vector<BasicBlock>& BB)
{
	for (int i = 0; i < BB.size(); i++)
	{
		for (int j = 0; j < BB[i].quaList.size(); i++)
		{
			if (BB[i].quaList[j] == qID) return i;
		}
	}
	return -1; // 没有查找到
}


// 条件1-1
int MustNode(int round, const vector<BasicBlock>& BB)
{
	// 先找循环启示位置
	// 再找循环终止位置
	int end_loop = round + 1;
	for (int start_not_loop = round + 1;; start_not_loop++)
	{
		if (BB[start_not_loop].loop == false)
		{
			end_loop = start_not_loop;
			break;
		}
	}
	if (end_loop != round + 1)
	{
		return -1;
	}
	return 1;
}
// 
// 条件1-2
int OnlyValue(int qNum, const vector<BasicBlock>& BB, const vector<Quaternary>& Q, int round)
{
	string res_target = Q[qNum].res;

	for (int i = 0; i < BB[i].quaList.size(); i++)
	{
		string res_temp = Q[BB[i].quaList[i]].res;
		if (BB[i].quaList[i] != qNum && res_temp == res_target)
		{
			return -1;
		}
	}

	// 先找循环启示位置
	int start_loop = round - 1;
	for (int start_to_loop = round - 1;; start_to_loop--)
	{
		if (BB[start_to_loop].loop == false)
		{
			start_loop = start_to_loop + 1; // 循环终止即可，无需+1
			break;
		}
	}

	// 再找循环终止位置
	int end_loop = round + 1;
	for (int start_not_loop = round + 1;; start_not_loop++)
	{
		if (BB[start_not_loop].loop == false)
		{
			end_loop = start_not_loop - 1; // 循环终止即可，无需+1
			break;
		}
	}

	// 查看循环体之间，即L内的情况
	for (int i = 0; i < BB.size(); i++)
	{
		if (BB[i].block_id >= start_loop && BB[i].block_id <= end_loop)
		{
			vector<int> quaList_temp = BB[i].quaList;
			for (int j = 0; j < quaList_temp.size(); j++)
			{
				string res_temp = Q[quaList_temp[j]].res;
				if (quaList_temp[j] != qNum && res_temp == res_target)
				{
					return -1;
				}
			}
		}
	}

	return 1;
}


// 递归查看两个基本块之间是否有通路
int loadPath(const vector<BasicBlock>& BB, const vector<Quaternary>& Q, int startB, int endB)
{
	if (BB[startB].loop == false || (Q[BB[startB].quaList.back()].op.find("j") == string::npos))
	{
		return -1;
	}
	if (startB == endB)
	{
		return 1;
	}
	startB = findBasic(stoi(Q[BB[startB].quaList.back()].res), BB);
	loadPath(BB, Q, startB, endB);
}

// 条件1-3
int OnlyAccess(int qNum, const vector<BasicBlock>& BB, const vector<Quaternary>& Q, int round)
{
	string res_target = Q[qNum].res;
	int block_target = round;

	// 先找循环启示位置
	int start_loop = round - 1;
	for (int start_to_loop = round - 1;; start_to_loop--)
	{
		if (BB[start_to_loop].loop == false)
		{
			start_loop = start_to_loop + 1; // 循环终止即可，无需+1
			break;
		}
	}

	// 再找循环终止位置
	int end_loop = round + 1;
	for (int start_not_loop = round + 1;; start_not_loop++)
	{
		if (BB[start_not_loop].loop == false)
		{
			end_loop = start_not_loop - 1; // 循环终止即可，无需+1
			break;
		}
	}

	// 查看循环体之间，即L内的情况
	for (int i = 0; i < BB.size(); i++)
	{
		if (BB[i].block_id >= start_loop && BB[i].block_id <= end_loop)
		{
			vector<int> quaList_temp = BB[i].quaList;
			for (int j = 0; j < quaList_temp.size(); j++)
			{
				if (Q[quaList_temp[j]].arg1 == res_target || Q[quaList_temp[j]].arg2 == res_target)
				{
					//cout << "测试4 " << loadPath(BB, Q, round, i) << endl;
					if (loadPath(BB, Q, round, i) == -1)
					{
						return -1;
					}
					else
					{
						for (int temp1 = 0; temp1 < BB.size(); temp1++)
						{
							if (BB[temp1].block_id >= start_loop && BB[temp1].block_id <= end_loop)
							{
								vector<int> quaList_temp = BB[i].quaList;
								for (int temp2 = 0; temp2 < quaList_temp.size(); temp2++)
								{
									if (Q[quaList_temp[temp2]].res == res_target)
									{
										if (loadPath(BB, Q, temp1, i) == 1 && temp1 != i)
										{
											return -1;
										}
									}
								}
							}
						}
					}
				}

			}
		}
	}

	return 1;

}

// 条件2
int NotActive(int qNum, const vector<BasicBlock>& BB, const vector<Quaternary>& Q, int round)
{
	string res_target = Q[qNum].res;

	int start_notloop = round + 1;  // 默认出口基本块块号

	for (int find_notloop = round + 1;; find_notloop++)
	{
		if (BB[find_notloop].loop == false)
		{
			start_notloop = find_notloop;
			break;
		}
	}

	int lookfor = start_notloop; // 查看的基本块编号，根据lookfor进行循环
	for (;;)
	{
		vector<int> quaList_lookfor = BB[lookfor].quaList;
		for (int i = 0; i < quaList_lookfor.size(); i++)
		{
			if (Q[quaList_lookfor[i]].arg1 == res_target || Q[quaList_lookfor[i]].arg2 == res_target)
			{
				return -1;
			}
		}

		Quaternary end_qua = Q[BB[lookfor].quaList.back()];
		string::size_type idx_jump = end_qua.op.find("j");
		if (idx_jump != string::npos)
		{
			int lookfor_q = stoi(end_qua.res);
			lookfor = findBasic(lookfor_q, BB);
			continue;
		}
		else
		{
			lookfor += 1;
			if ((lookfor + 1) > BB.size())
			{
				return 1;
			}
			continue;
		}
	}


}

vector<BasicBlock> Out_Lifting(const vector<BasicBlock>& BasicB, const vector<Quaternary>& Q)
{

	vector<BasicBlock> BB = BasicB;
	for (int round = 0;; round++) // 进入一个基本块
	{

		if (round == BB.size())
		{
			break;
		}
		if (BB[round].loop == false) // 不是循环体，不针对其操作
		{
			continue;
		}

		// 确认是循环体
		cout << endl;
		vector<int> potList = BB[round].unchangeOpList;
		for (int midround = 0; midround < potList.size(); midround++)
		{
			int qNum = potList[midround];

			int condi1_1 = MustNode(round, BB); // 条件1-1
			int condi1_2 = OnlyValue(qNum, BB, Q, round); // 条件1-2
			int condi1_3 = OnlyAccess(qNum, BB, Q, round);// 条件1-3
			int condi2 = NotActive(qNum, BB, Q, round);// 条件2

			/*
			cout << condi1_1 << endl;
			cout << condi1_2 << endl;
			cout << condi1_3 << endl;
			cout << condi2 << endl;
			cout << endl;
			*/

			// 满足条件①或条件②
			if ((condi1_1 == 1 && condi1_2 == 1 && condi1_3 == 1) || (condi2 == 1 && condi1_2 == 1 && condi1_3 == 1))
			{
				BB[round].out_liftedList.push_back(qNum);
			}
		}// 得到代码外提运算的集合

		if (BB[round].out_liftedList.size() > 0) // 需要进行外提
		{
			int oldround = round;
			int changeround = round - 1; // 默认代码外提的位置
			for (int findchange = 1;; findchange++)
			{
				if (BB[round - findchange].loop == false)
				{
					changeround = round - findchange; // 求得真正代码外提的位置
					break;
				}
			}

			// 代码外提，后续所有节点标号往后一位
			for (int i = 0; i < BB.size(); i++)
			{
				if (BB[i].block_id >= changeround)
				{
					BB[i].block_id += 1;
				}
			}

			// 生成代码外提节点
			BasicBlock newBB;
			newBB.block_id = changeround;
			newBB.quaList = BB[oldround].out_liftedList;
			newBB.loop = false;
			// 把外提节点放入vector<BasicBlock>
			BB.push_back(newBB);
			// 此时BB.size()增加1
			round += 1; // 不能重复查看同一个基本块
		}

		// 由于BB.size()会发生改变，特别退出循环条件
		if ((round + 1) == BB.size()) break;

	}

	return BB;
}
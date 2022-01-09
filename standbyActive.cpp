#include "opt.h"
#include <vector>
#include <algorithm>
/*
 * @author llf
 *
 * 计算基本块的待用信息和活跃信息表
 */



// 初始状态 -- > 信息链
struct StandbyActiveLink
{
	// 变量名
	string VarName;
	StandbyActive standby_active = {-1, false}; // 信息链:初始化为非待用，非活跃
};


// 变量与下标的关联集
struct IndexVar
{
	int index;
	string var;
};


// 计算全局四元式的待用信息和活跃信息表
vector<BasicBlockStandbyActiveTable> computeStandbyActiveTable(vector<Quaternary> qua)
{
	// 获取所有变量名
	vector<string> varNameList;
	for(int i = 0; i < qua.size(); i++)
	{
		
		// 如果是goto,直接跳转，则不需要分析其操作符和操作数
		if(qua[i].op != "j")
		{
			varNameList.push_back(qua[i].res);
			// 去除空操作数和纯数字
			if(qua[i].arg1 != "#" && !isdigit(qua[i].arg1[0]))
				varNameList.push_back(qua[i].arg1);
			if (qua[i].arg2 != "#" && !isdigit(qua[i].arg2[0]))
			varNameList.push_back(qua[i].arg2);
		}
	}
	// 去重排序
	sort(varNameList.begin(), varNameList.end());
	varNameList.erase(unique(varNameList.begin(), varNameList.end()), varNameList.end());


	// 信息链表变量名索引
	vector<IndexVar> index_vars;
	for(int i = 0; i < varNameList.size(); i++)
	{
		IndexVar index_var = { i, varNameList[i] };
		index_vars.push_back(index_var);
	}

	// 获取四元式中出现的所有变量
	vector<StandbyActiveLink> standby_active_link;

	// 信息链长度
	int N = varNameList.size();

	// 信息链初始化
	for(int i = 0; i < N; i++)
	{
		StandbyActiveLink s;
		s.VarName  = varNameList[i];
		s.standby_active = { -1, false };
		standby_active_link.push_back(s);
	}

	// 初始化待用信息和活跃信息列表
	vector<BasicBlockStandbyActiveTable> basic_block_standby_active_tables;
	for (int i = 0; i < qua.size(); i++)
	{
		BasicBlockStandbyActiveTable b;
		b.qua_id = qua[i].id;
		b.res = { -1, false };
		b.arg1 = { -1, false};
		b.arg2 = { -1, false };
		basic_block_standby_active_tables.push_back(b);
	}

	// 计算待用信息和活跃信息列表
	// A := B op C
	for(int i = qua.size() - 1 ; i >= 0; i--)
	{
		// 在变量索引表中查找字符串
		for(int j = 0; j < index_vars.size(); j++)
		{
			// 左值
			if(qua[i].res == index_vars[j].var)
			{
				// 把符号表中变量A的待用信息和活跃信息附加在四元式i上
				basic_block_standby_active_tables[i].res = standby_active_link[index_vars[j].index].standby_active;
				// 把符号表中A的待用信息和活跃信息分别置为非待用和非活跃
				standby_active_link[index_vars[j].index].standby_active = { -1, false };
			}
			// 把符号表中变量B和C的待用信息和活跃信息附加在四元式i上
			// 把符号表中B和C的待用信息和活跃信息均值i和活跃
			if(qua[i].arg1 == index_vars[j].var)
			{
				basic_block_standby_active_tables[i].arg1 = standby_active_link[index_vars[j].index].standby_active;
				standby_active_link[index_vars[j].index].standby_active = { i, true };
			}
			if (qua[i].arg2 == index_vars[j].var)
			{
				basic_block_standby_active_tables[i].arg2 = standby_active_link[index_vars[j].index].standby_active;
				standby_active_link[index_vars[j].index].standby_active = { i, true };
			}
		}
	}

	return basic_block_standby_active_tables;
}


// 输入全局的四元式列表和基本块的划分信息，返回基本获得待用信息和活跃信息列表的基本块
//vector<BasicBlock> computeStandbyActiveTable(vector<Quaternary> all_qua, vector<BasicBlock> basic_blocks)
//{
//	vector<BasicBlockStandbyActiveTable> basic_block_standby_active_tables = computeStandbyActiveTable(all_qua);
//
//	// 对每一个基本块计算待用信息和活跃信息
//	for(int i = 0; i < basic_blocks.size(); i++)
//	{
//		// 获取基本块的四元式集合
//		vector<Quaternary> qua;
//		for(int i = 0; i < basic_blocks[i].quaList.size(); i++)
//		{
//			qua.push_back(all_qua[basic_blocks[i].quaList[i]]);
//		}
//
//		// 获取基本块中所有变量名
//		vector<string> varNameList;
//		for (int i = 0; i < qua.size(); i++)
//		{
//
//			// 如果是goto,直接跳转，则不需要分析其操作符和操作数
//			if (qua[i].op != "j")
//			{
//				varNameList.push_back(qua[i].res);
//				// 去除空操作数和纯数字
//				if (qua[i].arg1 != "#" && !isdigit(qua[i].arg1[0]))
//					varNameList.push_back(qua[i].arg1);
//				if (qua[i].arg2 != "#" && !isdigit(qua[i].arg2[0]))
//					varNameList.push_back(qua[i].arg2);
//			}
//		}
//		// 去重排序
//		sort(varNameList.begin(), varNameList.end());
//		varNameList.erase(unique(varNameList.begin(), varNameList.end()), varNameList.end());
//
//		// 信息链表变量名索引
//		vector<IndexVar> index_vars;
//		for (int i = 0; i < varNameList.size(); i++)
//		{
//			IndexVar index_var = { i, varNameList[i] };
//			index_vars.push_back(index_var);
//		}
//
//		// 获取四元式中出现的所有变量
//		vector<StandbyActiveLink> standby_active_link;
//
//		// 信息链长度
//		int N = varNameList.size();
//
//		// 信息链初始化
//		for (int i = 0; i < N; i++)
//		{
//			StandbyActiveLink s;
//			s.VarName = varNameList[i];
//			s.standby_active = { -1, false };
//			standby_active_link.push_back(s);
//		}
//
//		// 初始化待用信息和活跃信息列表
//		// 计算基本块的待用信息和活跃信息
//	}
//}
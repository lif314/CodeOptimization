#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>  
#include "opt.h"
using namespace std;

void strengthWeakening(vector<BasicBlock>& all_block, vector<Quaternary>& qua) {
	// 判断是否是数字的函数
	bool isnum(string s);
	// 存放循环不变量
	unordered_set<string> inVariables;
	// 存放基本归纳变量
	unordered_set<string> basIndVariables;
	// 存放新的基本归纳变量
	unordered_set<string> newBasIndVariables;
	// 存放归纳变量
	unordered_set<string> indVariables;
	// 存放归纳变量与四元式对应关系
	unordered_map<string, Quaternary> varToCode;
	// 存放新归纳变量与四元式对应关系
	unordered_map<string, Quaternary> newVarToCode;
	// 判断是否还要继续循环强度削弱
	bool isContinue = true;
	//判断一个基本块是否是循环基本块
	for (int i = 0; i < all_block.size(); i++)
	{
		if (all_block[i].loop)
		{
			// 保存基本块内部四元式id
			vector<int> codeIndex = all_block[i].quaList;


			// 将该循环体内部的循环不变量放到一个hash_set中，便于后续判断
			for (int j = 0; j < all_block[i].unchangeOpList.size(); j++)
			{
				int codeId = all_block[i].unchangeOpList[j];
				inVariables.insert(qua[codeId].res);
			}


			// 判断一个变量是否是基本归纳变量
			for (int k = 0; k < codeIndex.size(); k++)
			{
				Quaternary code = qua[codeIndex[k]];

				// 判断该变量是否是基本归纳变量: 第一操作数等于目的操作数 && 第二操作数是循环不变量 && 运算符为+/-
				if (code.arg1 == code.res && (isnum(code.arg2) || (inVariables.find(code.arg2) != inVariables.end())) && (code.op == "+" || code.op == "-")) {

					// 若是，则加入基本归纳变量数组中，并把对应的基本归纳变量-四元式关系放入hash_map中
					basIndVariables.insert(code.res);
					varToCode[code.res] = code;
				}
			}
			while (isContinue == true) {
				isContinue = false;
				// 判断一个变量是否是归纳变量，若是，则对其进行强度削弱，强度削弱后的形式为：T:=T+/-C，C为循环不变量
				for (int k = 0; k < codeIndex.size(); k++)
				{
					Quaternary code = qua[codeIndex[k]];
					bool judge = false;
					// 定义新的第二操作数
					string newArg2;
					// 获取基本归纳变量所属四元式部分信息
					string preArg2;
					string preOp;
					string newOp;
					// 情况一：T:=K*I,K为循环不变量，I为基本归纳变量
					if (code.op == "*" && (basIndVariables.find(code.arg2) != basIndVariables.end()) && ((inVariables.find(code.arg1) != inVariables.end()) || isnum(code.arg1)))
					{
						preArg2 = varToCode[code.arg2].arg2;
						preOp = varToCode[code.arg2].op;
						judge = true;
						// 计算新的第二操作数，分常数和基本归纳变量两种情况
						if (isnum(code.arg1) && isnum(preArg2)) {
							newArg2 = to_string(stoi(code.arg1) * stoi(preArg2));
						}
						else {
							newArg2 = code.arg1 + "*" + preArg2;
						}
						newOp = preOp;

					}
					//情况二：T:=I+/-C,C为循环不变量，I为基本归纳变量
					else if ((code.op == "+" || code.op == "-") && (basIndVariables.find(code.arg1) != basIndVariables.end()) && ((inVariables.find(code.arg2) != inVariables.end()) || isnum(code.arg2)) && (basIndVariables.find(code.res)==basIndVariables.end())) {
						preArg2 = varToCode[code.arg1].arg2;
						preOp = varToCode[code.arg1].op;
						judge = true;
						// 判断操作数是否是数字
						if (isnum(preArg2)) {
							// 计算新的第二操作数
							switch (preOp[0])
							{
							case '+': {
								newArg2 = to_string(0 + stoi(preArg2));
								break;
							}
							case '-': {
								newArg2 = to_string(0 - stoi(preArg2));
								break;
							}
							}
							if (stoi(newArg2) >= 0)
							{
								newOp = code.op;
							}
							else {
								newOp = code.op == "+" ? "-" : "+";
							}
						}
						else {
							newArg2 = preArg2;
							newOp = preOp;
						}
					}
					if (judge == true) {
						isContinue = true;
						// 产生强度优化后的代码
						int newCodeId = qua.size();
						Quaternary newCode = { newCodeId,newOp,code.res,newArg2,code.res };

						// 将强度优化后的代码加入到qua的尾部
						qua.push_back(newCode);

						// 删除本基本块中该四元式的信息
						all_block[i].quaList.erase(find(all_block[i].quaList.begin(), all_block[i].quaList.end(), codeIndex[k]));

						// 将原来的四元式提到第n-2个基本块中
						if (i - 2 >= 0) {
							for (int l = 0; l < all_block.size(); l++) {
								if (all_block[l].block_id == i - 2)
								{
									all_block[l].quaList.push_back(codeIndex[k]);
								}
							}
						}
						else {
							for (int l = 0; l < all_block.size(); l++) {
								all_block[l].block_id++;
							}
							vector<int> newQuaList;
							newQuaList.push_back(codeIndex[k]);
							BasicBlock b = { 0,newQuaList,false };
							all_block.push_back(b);
						}

						//将新生成的代码加入本基本块末尾
						int lastCode = all_block[i].quaList[all_block[i].quaList.size()-1];
						all_block[i].quaList[all_block[i].quaList.size()-1] = newCodeId;
						all_block[i].quaList.push_back(lastCode);

						//将新生成的基本归纳变量加入到新基本归纳变量数组
						newBasIndVariables.insert(code.res);
						newVarToCode[code.res] = newCode;
					}
				}
				//替换归纳变量
				basIndVariables = newBasIndVariables;
				varToCode = newVarToCode;
			}
			}
	}
}
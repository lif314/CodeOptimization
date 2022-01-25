#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>  
#include "opt.h"
using namespace std;

void strengthWeakening(vector<BasicBlock>& all_block, vector<Quaternary>& qua) {
	// �ж��Ƿ������ֵĺ���
	bool isnum(string s);
	// ���ѭ��������
	unordered_set<string> inVariables;
	// ��Ż������ɱ���
	unordered_set<string> basIndVariables;
	// ����µĻ������ɱ���
	unordered_set<string> newBasIndVariables;
	// ��Ź��ɱ���
	unordered_set<string> indVariables;
	// ��Ź��ɱ�������Ԫʽ��Ӧ��ϵ
	unordered_map<string, Quaternary> varToCode;
	// ����¹��ɱ�������Ԫʽ��Ӧ��ϵ
	unordered_map<string, Quaternary> newVarToCode;
	// �ж��Ƿ�Ҫ����ѭ��ǿ������
	bool isContinue = true;
	//�ж�һ���������Ƿ���ѭ��������
	for (int i = 0; i < all_block.size(); i++)
	{
		if (all_block[i].loop)
		{
			// ����������ڲ���Ԫʽid
			vector<int> codeIndex = all_block[i].quaList;


			// ����ѭ�����ڲ���ѭ���������ŵ�һ��hash_set�У����ں����ж�
			for (int j = 0; j < all_block[i].unchangeOpList.size(); j++)
			{
				int codeId = all_block[i].unchangeOpList[j];
				inVariables.insert(qua[codeId].res);
			}


			// �ж�һ�������Ƿ��ǻ������ɱ���
			for (int k = 0; k < codeIndex.size(); k++)
			{
				Quaternary code = qua[codeIndex[k]];

				// �жϸñ����Ƿ��ǻ������ɱ���: ��һ����������Ŀ�Ĳ����� && �ڶ���������ѭ�������� && �����Ϊ+/-
				if (code.arg1 == code.res && (isnum(code.arg2) || (inVariables.find(code.arg2) != inVariables.end())) && (code.op == "+" || code.op == "-")) {

					// ���ǣ������������ɱ��������У����Ѷ�Ӧ�Ļ������ɱ���-��Ԫʽ��ϵ����hash_map��
					basIndVariables.insert(code.res);
					varToCode[code.res] = code;
				}
			}
			while (isContinue == true) {
				isContinue = false;
				// �ж�һ�������Ƿ��ǹ��ɱ��������ǣ���������ǿ��������ǿ�����������ʽΪ��T:=T+/-C��CΪѭ��������
				for (int k = 0; k < codeIndex.size(); k++)
				{
					Quaternary code = qua[codeIndex[k]];
					bool judge = false;
					// �����µĵڶ�������
					string newArg2;
					// ��ȡ�������ɱ���������Ԫʽ������Ϣ
					string preArg2;
					string preOp;
					string newOp;
					// ���һ��T:=K*I,KΪѭ����������IΪ�������ɱ���
					if (code.op == "*" && (basIndVariables.find(code.arg2) != basIndVariables.end()) && ((inVariables.find(code.arg1) != inVariables.end()) || isnum(code.arg1)))
					{
						preArg2 = varToCode[code.arg2].arg2;
						preOp = varToCode[code.arg2].op;
						judge = true;
						// �����µĵڶ����������ֳ����ͻ������ɱ����������
						if (isnum(code.arg1) && isnum(preArg2)) {
							newArg2 = to_string(stoi(code.arg1) * stoi(preArg2));
						}
						else {
							newArg2 = code.arg1 + "*" + preArg2;
						}
						newOp = preOp;

					}
					//�������T:=I+/-C,CΪѭ����������IΪ�������ɱ���
					else if ((code.op == "+" || code.op == "-") && (basIndVariables.find(code.arg1) != basIndVariables.end()) && ((inVariables.find(code.arg2) != inVariables.end()) || isnum(code.arg2)) && (basIndVariables.find(code.res)==basIndVariables.end())) {
						preArg2 = varToCode[code.arg1].arg2;
						preOp = varToCode[code.arg1].op;
						judge = true;
						// �жϲ������Ƿ�������
						if (isnum(preArg2)) {
							// �����µĵڶ�������
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
						// ����ǿ���Ż���Ĵ���
						int newCodeId = qua.size();
						Quaternary newCode = { newCodeId,newOp,code.res,newArg2,code.res };

						// ��ǿ���Ż���Ĵ�����뵽qua��β��
						qua.push_back(newCode);

						// ɾ�����������и���Ԫʽ����Ϣ
						all_block[i].quaList.erase(find(all_block[i].quaList.begin(), all_block[i].quaList.end(), codeIndex[k]));

						// ��ԭ������Ԫʽ�ᵽ��n-2����������
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

						//�������ɵĴ�����뱾������ĩβ
						int lastCode = all_block[i].quaList[all_block[i].quaList.size()-1];
						all_block[i].quaList[all_block[i].quaList.size()-1] = newCodeId;
						all_block[i].quaList.push_back(lastCode);

						//�������ɵĻ������ɱ������뵽�»������ɱ�������
						newBasIndVariables.insert(code.res);
						newVarToCode[code.res] = newCode;
					}
				}
				//�滻���ɱ���
				basIndVariables = newBasIndVariables;
				varToCode = newVarToCode;
			}
			}
	}
}
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
 * ѭ����������
 */

/*
 * ����1�����β鿴L�и��������ÿ����Ԫʽ���������ÿ����������Ϊ���������߶�ֵ���� L�⣬�򽫴���Ԫʽ���Ϊ"��������";
 * ����2���ظ���3��ֱ��û���µ���Ԫʽ�����Ϊ"��������"Ϊֹ;
 * ����3�����β鿴��δ�����Ϊ"��������"����Ԫʽ���������ÿ����������Ϊ��������ֵ����L֮�⣬
 *       ��ֻ��һ������-��ֵ���Ҹõ��ϵ���Ԫʽ�ѱ����Ϊ"��������"����ѱ��鿴����Ԫʽ���Ϊ"��������"��
 */

/*
 * standby_actives: ȫ����Ԫʽ�Ĵ�����Ϣ��
 *
 * basic_blocks: ������
 */

// Ѱ��ѭ���Ļ�����
vector<BasicBlock> findLoop(vector<Quaternary> all_qua, vector<BasicBlock> & basic_blocks)
{
	// �������������һ�������ת���û������һ����䣬����ѭ���Ļ�����
	for(int i = 0; i < basic_blocks.size(); i++)
	{
		// ��һ����Ԫʽ���
		Quaternary start_qua = all_qua[basic_blocks[i].quaList.front()];
		// ���һ����Ԫʽ���
		Quaternary end_qua = all_qua[basic_blocks[i].quaList.back()];

		// ���һ����京����ת���������׾�
		if(end_qua.op.find("j") != end_qua.op.npos && (stoi(end_qua.res) + 1) == start_qua.id)
		{
			// ��Ǹû�������ѭ���Ļ�����
			basic_blocks[i].loop = true;
		}
	}

	return basic_blocks;
}

// �ж�һ��Ԫ���Ƿ���vector��
bool isInVector(int key, vector<int> v)
{
	return std::count(v.begin(), v.end(), key);
}
// ��������
bool isInVector(string key, vector<string> v)
{
	return std::count(v.begin(), v.end(), key);
}


// ѭ����������
vector<BasicBlock> getUnchangeOperation(vector<Quaternary> all_qua , vector<BasicBlockStandbyActiveTable> basic_block_standby_active_tables, vector<BasicBlock> & basic_blocks)
{
	// Ѱ��ѭ���ĵĻ�����
	vector<BasicBlock> res_basic_blocks = findLoop(all_qua, basic_blocks);

	// ����ÿһ���������е�ѭ����������
	for (int i = 0; i < res_basic_blocks.size(); i++)
	{
		// ��ѭ���Ļ�������Ѱ�Ҳ�������
		if (res_basic_blocks[i].loop == true)
		{
			
			// ��ȡ���������Ԫʽ�����Լ��ǻ������ڵ���Ԫʽ����
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

			// ��ȡ�������ⲿ�Ķ�ֵ�㼯��
			//vector<string> n_node;
			//for(int x = 0; i < n_qua.size();x++)
			//{
			//	n_node.push_back(n_qua[x].res);
			//}
			//// ����
			//sort(n_node.begin(), n_node.end());
			// n_node.erase(unique(n_node.begin(), n_node.end()), n_node.end());


			// ��ȡ�����ڲ��Ķ�ֵ�㼯��
			vector<string> b_node;
			for (int i = 0; i < b_qua.size(); i++)
			{
				b_node.push_back(b_qua[i].res);
			}
			// ����
			// sort(b_node.begin(), b_node.end());
			// b_node.erase(unique(b_node.begin(), b_node.end()), b_node.end());


			// �ж�ѭ����������
			for(int j = 0; j < b_qua.size(); j ++)
			{
				Quaternary q = b_qua[j];

				if(q.op == "j")
					continue;

				// �������������ǳ���, �������ڲ�û�ж�ֵ��(�����¸���)
				if(isdigit(q.arg1[0]) && isdigit(q.arg2[0]) && isInVector(q.res, b_node) == 1)
				{
					res_basic_blocks[i].unchangeOpList.push_back(q.id);
				}
				// һ���ǳ�������һ��û�б����¶�ֵ
				else if((isdigit(q.arg1[0]) && isInVector(q.arg2, b_node) == 0  && isInVector(q.res, b_node) == 1) || (isdigit(q.arg2[0]) && isInVector(q.arg1, b_node) == 0 && isInVector(q.res, b_node) == 1))
				{
					res_basic_blocks[i].unchangeOpList.push_back(q.id);
				}
				// ������������û�б����¶�ֵ
				else if(isInVector(q.arg1, b_node) == 0 && isInVector(q.arg2, b_node) == 0  &&  isInVector(q.res, b_node) == 1 && q.res != q.arg1 && q.res != q.arg2)
				{
					res_basic_blocks[i].unchangeOpList.push_back(q.id);
				}
			}

			// �жϻ�������ÿһ����Ԫʽ
			/*for (int j = 0; j < basic_blocks[i].quaList.size(); j++)
			{*/
				// ��ȡ��Ԫʽ�����id
				//int qua_id = basic_blocks[i].quaList[j];
				///*
				// * ��������ǳ���(-1, false)
				// * ��ֵ����L��(8) 8���ڻ�����
				// *
				// * ����Ԫʽ�Ĵ�����Ϣ�ڸû�������ⲿ����Ϊ����
				// */
				// // �����������ǳ���
				//if (basic_block_standby_active_tables[qua_id].arg1.standby < 0 && basic_block_standby_active_tables[qua_id].arg2.active == false)
				//{
				//	// �����������ڻ������в��ٱ���ֵ
				//	if(basic_block_standby_active_tables[qua_id].res.standby >= 0 && !isInVector(basic_block_standby_active_tables[qua_id].res.standby, basic_blocks[i].quaList))
				//	// ��ӵ��������㼯����
				//	res_basic_blocks[i].unchangeOpList.push_back(j);
				//}

				//// ��ֵ����L�⣺������Ϣ����0�������ڻ��������Ԫʽ������
				//else if (basic_block_standby_active_tables[qua_id].arg1.standby >= 0 && !isInVector(basic_block_standby_active_tables[qua_id].arg1.standby, basic_blocks[i].quaList))
				//{
				//	// ��ӵ��������㼯����
				//	res_basic_blocks[i].unchangeOpList.push_back(j);
				//}
			//}
		}
	}

	return res_basic_blocks;
}
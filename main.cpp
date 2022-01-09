#include <iostream>
#include <vector>
#include "opt.h"

using namespace std;

// ��ȡ��Ԫʽ�ļ�
vector<Quaternary> readTacToQuaternary(string filename);
// ���ֻ�����
vector<BasicBlock> computeBasicBlock(vector<Quaternary> qua);
// ȫ����Ԫʽ�Ĵ�����Ϣ�ͻ�Ծ��Ϣ��
vector<BasicBlockStandbyActiveTable> computeStandbyActiveTable(vector<Quaternary> qua);
// ֱ�Ӷ�ȡ��Ԫʽ�ļ�
vector<Quaternary> readQuaternary(string filename);
// ѭ����������
vector<BasicBlock> getUnchangeOperation(vector<Quaternary> all_qua, vector<BasicBlockStandbyActiveTable> standby_actives, vector<BasicBlock>& basic_blocks);


// ȫ�ֺ���
int main()
{
	// [1] ��ȡ��Ԫʽ����
	// ��ȡ����ַ�����ļ�
	// vector<Quaternary> qua = readTacToQuaternary("text_all.txt");
	// ��ȡ��Ԫʽ�ļ�
	vector<Quaternary> qua = readQuaternary("qua.txt");

	// �������
	cout << "��Ԫʽ���϶�����ԣ�" << endl;
	for (int i = 0; i < qua.size(); i++)
	{
		qua[i].print();
	}

	// [2] ������Ļ���
	cout << "\n�����黮�ֲ��ԣ�" << endl;
	vector<BasicBlock> all_block = computeBasicBlock(qua);
	// �����黮�ֲ���
	for (int i = 0; i < all_block.size(); i++)
	{
		cout << all_block[i].block_id << ": ";
		for (int j = 0; j < all_block[i].quaList.size(); j++)
		{
			cout << all_block[i].quaList[j] << " ";
		}
		cout << endl;
	}


	// [3] ����ÿһ��������Ԫʽ�����еĴ�����Ϣ�ͻ�Ծ��Ϣ��
	cout << "\n��Ԫʽ���ϵĴ�����Ϣ�ͻ�Ծ��Ϣ��" << endl;
	vector<BasicBlockStandbyActiveTable> basic_block_standby_active_tables = computeStandbyActiveTable(qua);
	// ������Ϣ�ͻ�Ծ��Ϣ�����
	for(int i = 0; i < basic_block_standby_active_tables.size(); i++)
	{
		basic_block_standby_active_tables[i].print();
	}

	// [4] ����ÿһ��������Ĵ�����Ϣ�ͻ�Ծ��Ϣ��




	// [5] DAG�ֲ��Ż�


	// [6] Ѱ��ѭ����������
	vector<BasicBlock> unchangeBasicBlock = getUnchangeOperation(qua, basic_block_standby_active_tables, all_block);
	// ѭ�������������
	cout << "\nѭ������������ԣ�" << endl;
	for (int i = 0; i < unchangeBasicBlock.size(); i++)
	{
		if(unchangeBasicBlock[i].loop)
		{
			cout << unchangeBasicBlock[i].block_id << ": ";
			for (int j = 0; j < unchangeBasicBlock[i].unchangeOpList.size(); j++)
			{
				cout << unchangeBasicBlock[i].unchangeOpList[j] << " ";
			}
			cout << endl;
		}
		
	}



	// [7] ��������


	// [8] ǿ������



	return 0;
}
#include "opt.h"
#include <vector>
#include <algorithm>
/*
 * @author llf
 *
 * ���������Ĵ�����Ϣ�ͻ�Ծ��Ϣ��
 */



// ��ʼ״̬ -- > ��Ϣ��
struct StandbyActiveLink
{
	// ������
	string VarName;
	StandbyActive standby_active = {-1, false}; // ��Ϣ��:��ʼ��Ϊ�Ǵ��ã��ǻ�Ծ
};


// �������±�Ĺ�����
struct IndexVar
{
	int index;
	string var;
};


// ����ȫ����Ԫʽ�Ĵ�����Ϣ�ͻ�Ծ��Ϣ��
vector<BasicBlockStandbyActiveTable> computeStandbyActiveTable(vector<Quaternary> qua)
{
	// ��ȡ���б�����
	vector<string> varNameList;
	for(int i = 0; i < qua.size(); i++)
	{
		
		// �����goto,ֱ����ת������Ҫ������������Ͳ�����
		if(qua[i].op != "j")
		{
			varNameList.push_back(qua[i].res);
			// ȥ���ղ������ʹ�����
			if(qua[i].arg1 != "#" && !isdigit(qua[i].arg1[0]))
				varNameList.push_back(qua[i].arg1);
			if (qua[i].arg2 != "#" && !isdigit(qua[i].arg2[0]))
			varNameList.push_back(qua[i].arg2);
		}
	}
	// ȥ������
	sort(varNameList.begin(), varNameList.end());
	varNameList.erase(unique(varNameList.begin(), varNameList.end()), varNameList.end());


	// ��Ϣ�������������
	vector<IndexVar> index_vars;
	for(int i = 0; i < varNameList.size(); i++)
	{
		IndexVar index_var = { i, varNameList[i] };
		index_vars.push_back(index_var);
	}

	// ��ȡ��Ԫʽ�г��ֵ����б���
	vector<StandbyActiveLink> standby_active_link;

	// ��Ϣ������
	int N = varNameList.size();

	// ��Ϣ����ʼ��
	for(int i = 0; i < N; i++)
	{
		StandbyActiveLink s;
		s.VarName  = varNameList[i];
		s.standby_active = { -1, false };
		standby_active_link.push_back(s);
	}

	// ��ʼ��������Ϣ�ͻ�Ծ��Ϣ�б�
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

	// ���������Ϣ�ͻ�Ծ��Ϣ�б�
	// A := B op C
	for(int i = qua.size() - 1 ; i >= 0; i--)
	{
		// �ڱ����������в����ַ���
		for(int j = 0; j < index_vars.size(); j++)
		{
			// ��ֵ
			if(qua[i].res == index_vars[j].var)
			{
				// �ѷ��ű��б���A�Ĵ�����Ϣ�ͻ�Ծ��Ϣ��������Ԫʽi��
				basic_block_standby_active_tables[i].res = standby_active_link[index_vars[j].index].standby_active;
				// �ѷ��ű���A�Ĵ�����Ϣ�ͻ�Ծ��Ϣ�ֱ���Ϊ�Ǵ��úͷǻ�Ծ
				standby_active_link[index_vars[j].index].standby_active = { -1, false };
			}
			// �ѷ��ű��б���B��C�Ĵ�����Ϣ�ͻ�Ծ��Ϣ��������Ԫʽi��
			// �ѷ��ű���B��C�Ĵ�����Ϣ�ͻ�Ծ��Ϣ��ֵi�ͻ�Ծ
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


// ����ȫ�ֵ���Ԫʽ�б�ͻ�����Ļ�����Ϣ�����ػ�����ô�����Ϣ�ͻ�Ծ��Ϣ�б�Ļ�����
//vector<BasicBlock> computeStandbyActiveTable(vector<Quaternary> all_qua, vector<BasicBlock> basic_blocks)
//{
//	vector<BasicBlockStandbyActiveTable> basic_block_standby_active_tables = computeStandbyActiveTable(all_qua);
//
//	// ��ÿһ����������������Ϣ�ͻ�Ծ��Ϣ
//	for(int i = 0; i < basic_blocks.size(); i++)
//	{
//		// ��ȡ���������Ԫʽ����
//		vector<Quaternary> qua;
//		for(int i = 0; i < basic_blocks[i].quaList.size(); i++)
//		{
//			qua.push_back(all_qua[basic_blocks[i].quaList[i]]);
//		}
//
//		// ��ȡ�����������б�����
//		vector<string> varNameList;
//		for (int i = 0; i < qua.size(); i++)
//		{
//
//			// �����goto,ֱ����ת������Ҫ������������Ͳ�����
//			if (qua[i].op != "j")
//			{
//				varNameList.push_back(qua[i].res);
//				// ȥ���ղ������ʹ�����
//				if (qua[i].arg1 != "#" && !isdigit(qua[i].arg1[0]))
//					varNameList.push_back(qua[i].arg1);
//				if (qua[i].arg2 != "#" && !isdigit(qua[i].arg2[0]))
//					varNameList.push_back(qua[i].arg2);
//			}
//		}
//		// ȥ������
//		sort(varNameList.begin(), varNameList.end());
//		varNameList.erase(unique(varNameList.begin(), varNameList.end()), varNameList.end());
//
//		// ��Ϣ�������������
//		vector<IndexVar> index_vars;
//		for (int i = 0; i < varNameList.size(); i++)
//		{
//			IndexVar index_var = { i, varNameList[i] };
//			index_vars.push_back(index_var);
//		}
//
//		// ��ȡ��Ԫʽ�г��ֵ����б���
//		vector<StandbyActiveLink> standby_active_link;
//
//		// ��Ϣ������
//		int N = varNameList.size();
//
//		// ��Ϣ����ʼ��
//		for (int i = 0; i < N; i++)
//		{
//			StandbyActiveLink s;
//			s.VarName = varNameList[i];
//			s.standby_active = { -1, false };
//			standby_active_link.push_back(s);
//		}
//
//		// ��ʼ��������Ϣ�ͻ�Ծ��Ϣ�б�
//		// ���������Ĵ�����Ϣ�ͻ�Ծ��Ϣ
//	}
//}
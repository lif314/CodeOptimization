#include <algorithm>
#include <string>
#include <vector>
#include "opt.h"
using namespace std;

/*
 * @author wwj
 *
 * �����컮���㷨
 */


// ������Ԫʽ���ϣ����ػ��������Ϣ
vector<BasicBlock> computeBasicBlock(vector<Quaternary> qua)
{
    void QuickSort(vector<int> &array, int low, int high);
    int getStandard(vector<int> &array, int i, int j);
    Quaternary code;
    vector<int> basicBlockStart;
    vector<vector <int>> basicBlock;

    // 0�ǻ������һ����ڵ�
    basicBlockStart.push_back(0);
    for(int i = 0; i < qua.size(); i++)
    {
        // goto����
	    if(qua[i].op.find("j") != qua[i].op.npos)
	    {
            if(qua[i].op.size()>1)
            {
                basicBlockStart.push_back(qua[i].id + 1);
            }
            basicBlockStart.push_back(stoi(qua[i].res));
	    }
        if(qua[i].op.find("halt") != qua[i].op.npos)
        {
            basicBlockStart.push_back(qua[i].id);
        }
    }

    //��100(���������)��Ϊ���һ������ֹ����Խ��
    basicBlockStart.push_back(qua.size());


    // �Ի����鿪ʼ������Ԥ���򣬱��ں�������
    QuickSort(basicBlockStart, 0, basicBlockStart.size() - 1);
    // ȥ������
    basicBlockStart.erase(unique(basicBlockStart.begin(), basicBlockStart.end()), basicBlockStart.end());

    // ���û���������
	// basicBlock.resize(basicBlockStart.size());

	//int blockIndex = 0;

    //int index = 0;
    //// ���ֻ�����
    //while (index < qua.size()) {
    //    //if (index < basicBlockStart[blockIndex] - 1 || code.find("goto") != code.npos || code.find("halt") != code.npos) {
    //    if (index < basicBlockStart[blockIndex]) {
    //        basicBlock[blockIndex].push_back(index);
    //    }
    //    else {
    //        blockIndex++;
    //        basicBlock[blockIndex].push_back(index);
    //    }
    //    index++;
    //}

	int blockIndex = 0;
    vector<BasicBlock> basic_blocks;
    for(int i = 0; i < basicBlockStart.size() - 1; i++)
    {
    	BasicBlock b;
    	b.block_id = blockIndex;

    	for (int k = basicBlockStart[i]; k < basicBlockStart[i + 1]; k++)
    	{
            if(k < qua.size())
    			b.quaList.push_back(k);
            else
                break;
    	}
        basic_blocks.push_back(b);
        blockIndex++;
    }

    return basic_blocks;
}


int getStandard(vector<int>& array, int i, int j) {
    // ��׼����
    int key = array[i];
    while (i < j) {
        // ��ΪĬ�ϻ�׼�Ǵ���߿�ʼ�����Դ��ұ߿�ʼ�Ƚ�
        // ����β��Ԫ�ش��ڵ��ڻ�׼���� ʱ,��һֱ��ǰŲ�� j ָ��
        while (i < j && array[j] >= key) {
            j--;
        }
        // ���ҵ��� array[i] С��ʱ���ͰѺ����ֵ array[j] ������
        if (i < j) {
            array[i] = array[j];
        }
        // ������Ԫ��С�ڵ��ڻ�׼���� ʱ,��һֱ���Ų�� i ָ��
        while (i < j && array[i] <= key) {
            i++;
        }
        // ���ҵ��� array[j] ���ʱ���Ͱ�ǰ���ֵ array[i] ������
        if (i < j) {
            array[j] = array[i];
        }
    }
    // ����ѭ��ʱ i �� j ���,��ʱ�� i �� j ���� key ����ȷ����λ��
    // �ѻ�׼���ݸ�����ȷλ��
    array[i] = key;
    return i;
}

void QuickSort(vector<int>& array, int low, int high) {
    // ��ʼĬ�ϻ�׼Ϊ low
    if (low < high) {
        // �ֶ�λ���±�
        int standard = getStandard(array, low, high);
        // �ݹ��������
        // �������
        QuickSort(array, low, standard - 1);
        // �ұ�����
        QuickSort(array, standard + 1, high);
    }
}
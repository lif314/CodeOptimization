#include <algorithm>
#include <string>
#include <vector>
#include "opt.h"
using namespace std;

/*
 * @author wwj
 *
 * 基本快划分算法
 */


// 输入四元式集合，返回基本块的信息
vector<BasicBlock> computeBasicBlock(vector<Quaternary> qua)
{
    void QuickSort(vector<int> &array, int low, int high);
    int getStandard(vector<int> &array, int i, int j);
    Quaternary code;
    vector<int> basicBlockStart;
    vector<vector <int>> basicBlock;

    // 0是基本块第一个入口点
    basicBlockStart.push_back(0);
    for(int i = 0; i < qua.size(); i++)
    {
        // goto操作
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

    //把100(假设无穷大)作为最后一个，防止容器越界
    basicBlockStart.push_back(qua.size());


    // 对基本块开始语句进行预排序，便于后续划分
    QuickSort(basicBlockStart, 0, basicBlockStart.size() - 1);
    // 去重排序
    basicBlockStart.erase(unique(basicBlockStart.begin(), basicBlockStart.end()), basicBlockStart.end());

    // 设置基本块数量
	// basicBlock.resize(basicBlockStart.size());

	//int blockIndex = 0;

    //int index = 0;
    //// 划分基本块
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
    // 基准数据
    int key = array[i];
    while (i < j) {
        // 因为默认基准是从左边开始，所以从右边开始比较
        // 当队尾的元素大于等于基准数据 时,就一直向前挪动 j 指针
        while (i < j && array[j] >= key) {
            j--;
        }
        // 当找到比 array[i] 小的时，就把后面的值 array[j] 赋给它
        if (i < j) {
            array[i] = array[j];
        }
        // 当队首元素小于等于基准数据 时,就一直向后挪动 i 指针
        while (i < j && array[i] <= key) {
            i++;
        }
        // 当找到比 array[j] 大的时，就把前面的值 array[i] 赋给它
        if (i < j) {
            array[j] = array[i];
        }
    }
    // 跳出循环时 i 和 j 相等,此时的 i 或 j 就是 key 的正确索引位置
    // 把基准数据赋给正确位置
    array[i] = key;
    return i;
}

void QuickSort(vector<int>& array, int low, int high) {
    // 开始默认基准为 low
    if (low < high) {
        // 分段位置下标
        int standard = getStandard(array, low, high);
        // 递归调用排序
        // 左边排序
        QuickSort(array, low, standard - 1);
        // 右边排序
        QuickSort(array, standard + 1, high);
    }
}
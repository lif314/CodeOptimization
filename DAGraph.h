#ifndef _DAGraph_
#define _DAGraph_

#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include "opt.h"
#include "Node.h"

using namespace std;

/**
 * @author ZHY
 *
 * DAGraph class
 * 
 */

//DAG图类
class DAGraph
{
private:
	vector<Node*> nodeList;	//结点集
	unordered_map<string, int> mappingTable;	//标识符与结点的对应表，用于Node(A)

	bool ifNodeDefine(string identifier);	//标识符的Node是否定义，1为定义
	int getNode(string identifier);	//求Node(),若未定义则返回-1
	int createLeafNode(string identifier);	//构造标记为identifier的叶结点,返回新节点的下标
	bool ifNodeConstLeaf(string identifier);	//标识符的Node是否是标记为常数的叶结点，保证结点已存在，1为是
	bool ifNodeConstLeaf(int node_id);	//Node是否是标记为常数的叶结点，保证结点已存在，1为是
	int excuteOP_1(int nodeB, string op);	//执行op B,即1类代码，保证结点是常数叶结点，返回结果结点的下标
	int excuteOP_2(int nodeB, int nodeC, string op);
	//执行B op C,即2类代码，保证结点是常数叶结点,且id2为第二个操作数，返回结果结点的下标
	void removeNode();	//删除最新的结点，并从mappingtable中移除
	int FindNode(string op, int lc, int rc = -1);	//检查是否存在一结点，其后继为lc，rc且标记为op,若没有则返回-1
	int createInnerNode(string op, int lc, int rc = -1);	//构造内部结点，其label为op,子节点为lc,rc,返回新节点的下标
	void resIdentifierAdd(string identifier, int n);	//对代码中A的操作，即步骤4,将A附加在结点n上

public:
	DAGraph();
	~DAGraph();
	void Clear();

	void outputGraph();	//输出DAG图
	void constructbyTac(Quaternary& quat);	//读入一条四元式，构造DAG图
	void outputQuaternary();	//生成DAG对应的四元式
	void deleteUselessAssign();	//删除无用赋值，假设T开头的标识符在基本块后不会被引用
	void DAGOptimize(BasicBlock& block, vector<Quaternary>& qua);	//DAG优化
	
};

#endif // !_DAGraph_
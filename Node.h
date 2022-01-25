#ifndef _Node_
#define _Node_

#include <string>
#include <vector>
#include <set>
#include <iostream>

using namespace std;

/**
 * @author ZHY
 *
 * DAGNode class
 *
 */

//DAG结点类
class Node
{
private:
	string label;	//标记
	bool ifLeaf;	//是否是叶结点，1为是
	bool ifConst;	//是否是标记为常数的叶结点，1为是
	set<string> identifierSet;	//附加标识符集
	int leftChild;	//左子结点
	int rightChild;	//右子结点

public:
	Node(string l, bool ifl, bool ifc);
	Node(string l, int lc, int rc = -1);
	string getLabel() { return label; }
	bool getIfLeaf() { return ifLeaf; }
	bool getIfConst() { return ifConst; }
	int getLeftChild() { return leftChild; }
	int getRightChild() { return rightChild; }
	set<string> getIdentifierSet() { return identifierSet; }

	void identifierAdd(string identifier);	//集合增加元素
	void identifierRemove(string identifier);	//集合删除元素,不会减为空
	void identifierRemoveCanEmpty(string identifier);	//集合删除元素,会减为空

	friend ostream& operator << (ostream& os, Node& node);
};

#endif // !_Node_
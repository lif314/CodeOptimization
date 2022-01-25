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

//DAG�����
class Node
{
private:
	string label;	//���
	bool ifLeaf;	//�Ƿ���Ҷ��㣬1Ϊ��
	bool ifConst;	//�Ƿ��Ǳ��Ϊ������Ҷ��㣬1Ϊ��
	set<string> identifierSet;	//���ӱ�ʶ����
	int leftChild;	//���ӽ��
	int rightChild;	//���ӽ��

public:
	Node(string l, bool ifl, bool ifc);
	Node(string l, int lc, int rc = -1);
	string getLabel() { return label; }
	bool getIfLeaf() { return ifLeaf; }
	bool getIfConst() { return ifConst; }
	int getLeftChild() { return leftChild; }
	int getRightChild() { return rightChild; }
	set<string> getIdentifierSet() { return identifierSet; }

	void identifierAdd(string identifier);	//��������Ԫ��
	void identifierRemove(string identifier);	//����ɾ��Ԫ��,�����Ϊ��
	void identifierRemoveCanEmpty(string identifier);	//����ɾ��Ԫ��,���Ϊ��

	friend ostream& operator << (ostream& os, Node& node);
};

#endif // !_Node_
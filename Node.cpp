#include "Node.h"

Node::Node(string l, bool ifl, bool ifc)
{
	label = l;
	ifLeaf = ifl;
	ifConst = ifc;
	leftChild = -1;
	rightChild = -1;
	identifierSet.clear();
}

Node::Node(string l, int lc, int rc)
{
	label = l;
	ifLeaf = false;
	ifConst = false;
	leftChild = lc;
	rightChild = rc;
	identifierSet.clear();
}

void Node::identifierAdd(string identifier)
{
	identifierSet.insert(identifier);
}

void Node::identifierRemove(string identifier)
{
	if (identifierSet.size() >= 2)	//不会将集合减为空
		identifierSet.erase(identifier);
}

void Node::identifierRemoveCanEmpty(string identifier)
{
	identifierSet.erase(identifier);
}

ostream& operator << (ostream& os, Node& node)
{
	cout << "  label:" << node.label << endl;
	cout << "  ifleaf:" << node.ifLeaf << endl;
	cout << "  ifconst:" << node.ifConst << endl;
	cout << "  identifierset:";
	set<string> iset = node.getIdentifierSet();
	auto it = iset.begin();
	while (it != iset.end())
	{
		cout << (*it) << ' ';
		++it;
	}
	cout << endl;
	cout << "  leftchild:" << node.leftChild << endl;
	cout << "  rightchild:" << node.rightChild << endl;
	return os;
}
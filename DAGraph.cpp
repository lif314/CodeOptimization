#include "DAGraph.h"

DAGraph::DAGraph()
{
	nodeList.clear();
	mappingTable.clear();
}

DAGraph::~DAGraph()
{
	for (int i = 0; i < nodeList.size(); i++)
		delete nodeList[i];
}

void DAGraph::Clear()
{
	for (int i = 0; i < nodeList.size(); i++)
		delete nodeList[i];
	nodeList.clear();
	mappingTable.clear();
}

bool DAGraph::ifNodeDefine(string identifier)
{
	return (mappingTable.find(identifier) != mappingTable.end());
}

int DAGraph::getNode(string identifier)
{
	if (ifNodeDefine(identifier))	//已定义
		return mappingTable[identifier];
	else
		return -1;
}

int DAGraph::createLeafNode(string identifier)
{
	char c = identifier[0];
	bool ifc = ((c >= '0' && c <= '9') || c == '-' || c == '+');	//检测ifConst
	Node* node = new Node(identifier, true, ifc);	//构造node
	int node_id = nodeList.size();
	nodeList.push_back(node);
	mappingTable.insert({ identifier, node_id });	//更新mappingTable
	return node_id;
}

bool DAGraph::ifNodeConstLeaf(string identifier)
{
	int node_id = mappingTable[identifier];
	return nodeList[node_id]->getIfConst();
}

bool DAGraph::ifNodeConstLeaf(int node_id)
{
	return nodeList[node_id]->getIfConst();
}

int DAGraph::excuteOP_1(int nodeB, string op)
{
	string newIdentifier = op + nodeList[nodeB]->getLabel();	//此处仍可修改
	if (nodeB == nodeList.size() - 1)	//若是新生成的结点，删除
		removeNode();
	if (ifNodeDefine(newIdentifier))
		return mappingTable[newIdentifier];	//newIdentifier已定义
	else
		return createLeafNode(newIdentifier);	//newIdentifier未定义
}

int DAGraph::excuteOP_2(int nodeB, int nodeC, string op)
{
	string newIdentifier;
	string numstr1 = nodeList[nodeB]->getLabel();
	string numstr2 = nodeList[nodeC]->getLabel();
	if ((numstr1.find('.') != string::npos) || (numstr2.find('.') != string::npos))	//浮点数
	{
		float num1 = atof(numstr1.c_str());
		float num2 = atof(numstr2.c_str());
		float result = 0;
		if (op == "+")
			result = num1 + num2;
		else if (op == "-")
			result = num1 - num2;
		else if (op == "*")
			result = num1 * num2;
		else if (op == "/")
			result = num1 / num2;
		//newIdentifier = to_string(result);
		ostringstream strStream;
		strStream << result;
		newIdentifier = strStream.str();
	}
	else // 整数
	{
		int num1 = atoi(numstr1.c_str());
		int num2 = atoi(numstr2.c_str());
		int result = 0;
		if (op == "+")
			result = num1 + num2;
		else if (op == "-")
			result = num1 - num2;
		else if (op == "*")
			result = num1 * num2;
		else if (op == "/")
			result = num1 / num2;
		else if (op == "mod")
			result = num1 % num2;
		newIdentifier = to_string(result);
	}
	if (nodeC == nodeList.size() - 1)	//若是新生成的结点，删除
		removeNode();
	if (nodeB == nodeList.size() - 1)	//若是新生成的结点，删除
		removeNode();
	if (ifNodeDefine(newIdentifier))
		return mappingTable[newIdentifier];	//newIdentifier已定义
	else
		return createLeafNode(newIdentifier);	//newIdentifier未定义
}

void DAGraph::removeNode()
{
	string identifier = nodeList[nodeList.size() - 1]->getLabel();
	nodeList.pop_back();
	mappingTable.erase(identifier);
}

int DAGraph::FindNode(string op, int lc, int rc)
{
	Node* node;
	for (int i = 0; i < nodeList.size(); i++)
	{
		node = nodeList[i];
		if ((node->getLabel() == op) && (node->getLeftChild() == lc) && (node->getRightChild() == rc))
			return i;	//存在该结点
	}
	return -1;	//不存在
}

int DAGraph::createInnerNode(string op, int lc, int rc)
{
	Node* node = new Node(op, lc, rc);
	nodeList.push_back(node);
	return nodeList.size() - 1;
}

void DAGraph::resIdentifierAdd(string identifier, int n)
{
	if (ifNodeDefine(identifier))	//若A的NODE已定义
	{
		nodeList[mappingTable[identifier]]->identifierRemove(identifier);	//移除
		nodeList[n]->identifierAdd(identifier);	//将A附加在结点n上
		mappingTable[identifier] = n;
	}
	else
	{
		nodeList[n]->identifierAdd(identifier);	//将A附加在结点n上
		mappingTable.insert({ identifier, n });
	}
}

void DAGraph::outputGraph()
{
	for (int i = 0; i < nodeList.size(); i++)
	{
		cout << "node" << i << endl << *nodeList[i] << endl;
	}
}

void DAGraph::constructbyTac(Quaternary& quat)
{
	string op = quat.op;
	string A = quat.res;
	string B = quat.arg1;
	string C = quat.arg2;
	if (isalpha(op[0]))	//不对这样的四元式进行处理
		return;
	int resNode;	//最终结果的结点号
	int argNode1, argNode2;	//操作数的结点号
	argNode1 = getNode(B);
	if (argNode1 == -1)	//若B未定义
		argNode1 = createLeafNode(B);	//构造标记为B的叶结点，并定义NODE(B)
	if (C == "#") //0，1类
	{
		if (op != ":=") //1类
		{
			//2(1)
			if (ifNodeConstLeaf(argNode1))	//NODE(B)为常数叶结点
			{
				//2(3)
				resNode = excuteOP_1(argNode1, op);	//合并已知量
			}
			else
			{
				//3(1)
				resNode = FindNode(op, argNode1);	//找公共子表达式
				if (resNode == -1)	//没有找到
				{
					resNode = createInnerNode(op, argNode1);
				}
			}
		}
		else //0类
		{
			resNode = argNode1;
		}
	}
	else //2类
	{
		argNode2 = getNode(C);
		if (argNode2 == -1)	//若C未定义
			argNode2 = createLeafNode(C);	//构造标记为C的叶结点，并定义NODE(C)
		//2(2)
		if (ifNodeConstLeaf(argNode2) && ifNodeConstLeaf(argNode1))	//NODE(B)和NODE(C)都是常数叶结点
		{
			//2(4)
			resNode = excuteOP_2(argNode1, argNode2, op);
		}
		else
		{
			//3(2)
			resNode = FindNode(op, argNode1, argNode2);	//找公共子表达式
			if (resNode == -1)	//没有找到
			{
				resNode = createInnerNode(op, argNode1, argNode2);
			}
		}
	}
	//4
	resIdentifierAdd(A, resNode);
}

void DAGraph::outputQuaternary()
{
	Quaternary quat;
	Node* currNode;
	int lc, rc;	//左右子结点
	for (int i = 0; i < nodeList.size(); i++)
	{
		currNode = nodeList[i];
		if (!currNode->getIfLeaf())	//内部结点生成代码
		{
			quat.op = currNode->getLabel();	//四元式的op
			set<string> identifierSet = currNode->getIdentifierSet();
			lc = currNode->getLeftChild();	//求四元式的arg1
			if (nodeList[lc]->getIfLeaf())	//子结点是叶结点
				quat.arg1 = nodeList[lc]->getLabel();
			else  //子结点是内部结点
			{
				set<string> childIdentifierSet = nodeList[lc]->getIdentifierSet();
				set<string>::iterator it = childIdentifierSet.begin();
				quat.arg1 = (*it);
			}
			rc = currNode->getRightChild();	//求四元式的arg2
			if (rc == -1)	//无arg2
				quat.arg2 = "#";
			else if (nodeList[rc]->getIfLeaf())	//子结点是叶结点
				quat.arg2 = nodeList[rc]->getLabel();
			else  //子结点是内部结点
			{
				set<string> childIdentifierSet = nodeList[rc]->getIdentifierSet();
				set<string>::iterator it = childIdentifierSet.begin();
				quat.arg2 = (*it);
			}
			auto setit = identifierSet.begin();
			quat.res = (*setit);
			quat.Output();
			++setit;
			quat.op = ":=";
			quat.arg1 = quat.res;
			quat.arg2 = "#";
			while (setit != identifierSet.end())	//求四元式的res
			{
				quat.res = (*setit);
				quat.Output();
				++setit;
			}
		}
		else  //叶结点生成代码
		{
			quat.op = ":=";
			quat.arg1 = currNode->getLabel();
			quat.arg2 = "#";
			set<string> identifierSet = currNode->getIdentifierSet();
			auto setit = identifierSet.begin();
			while (setit != identifierSet.end())	//求四元式的res
			{
				quat.res = (*setit);
				quat.Output();
				++setit;
			}
		}
	}
}

void DAGraph::deleteUselessAssign()
{
	Node* currNode;
	set<string> identifierSet;
	string identifier;
	for (int i = 0; i < nodeList.size(); i++)
	{
		currNode = nodeList[i];
		identifierSet = currNode->getIdentifierSet();
		if (currNode->getIfLeaf())	//是叶结点
		{
			auto setit = identifierSet.begin();
			while (setit != identifierSet.end())
			{
				identifier = (*setit);
				if (identifier[0] == 'T')	//删除
				{
					++setit;
					currNode->identifierRemoveCanEmpty(identifier);
					mappingTable.erase(identifier);
				}
				else
					++setit;
			}
		}
		else  //内部结点
		{
			auto setit = identifierSet.begin();
			while (setit != identifierSet.end())
			{
				identifier = (*setit);
				if (identifier[0] == 'T' && identifierSet.size() != 1)	//删除,不会减为空
				{
					++setit;
					currNode->identifierRemove(identifier);
					mappingTable.erase(identifier);
				}
				else
					++setit;
			}
		}
	}
}

void DAGraph::DAGOptimize(BasicBlock& block, vector<Quaternary>& qua)
{
	int i = 0;
	Quaternary aquat;
	for (i = 0; i < block.quaList.size() - 1; i++)
	{
		aquat = qua[block.quaList[i]];
		constructbyTac(aquat);
	}
	aquat = qua[block.quaList[i]];
	string op = aquat.op;
	if (isalpha(op[0]))	//跳转
	{
		cout << "block " << block.block_id << ":\n";
		outputQuaternary();
		aquat.Output();
	}
	else
	{
		constructbyTac(aquat);
		cout << "block " << block.block_id << ":\n";
		outputQuaternary();
	}
	
}
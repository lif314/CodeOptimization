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
	if (ifNodeDefine(identifier))	//�Ѷ���
		return mappingTable[identifier];
	else
		return -1;
}

int DAGraph::createLeafNode(string identifier)
{
	char c = identifier[0];
	bool ifc = ((c >= '0' && c <= '9') || c == '-' || c == '+');	//���ifConst
	Node* node = new Node(identifier, true, ifc);	//����node
	int node_id = nodeList.size();
	nodeList.push_back(node);
	mappingTable.insert({ identifier, node_id });	//����mappingTable
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
	string newIdentifier = op + nodeList[nodeB]->getLabel();	//�˴��Կ��޸�
	if (nodeB == nodeList.size() - 1)	//���������ɵĽ�㣬ɾ��
		removeNode();
	if (ifNodeDefine(newIdentifier))
		return mappingTable[newIdentifier];	//newIdentifier�Ѷ���
	else
		return createLeafNode(newIdentifier);	//newIdentifierδ����
}

int DAGraph::excuteOP_2(int nodeB, int nodeC, string op)
{
	string newIdentifier;
	string numstr1 = nodeList[nodeB]->getLabel();
	string numstr2 = nodeList[nodeC]->getLabel();
	if ((numstr1.find('.') != string::npos) || (numstr2.find('.') != string::npos))	//������
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
	else // ����
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
	if (nodeC == nodeList.size() - 1)	//���������ɵĽ�㣬ɾ��
		removeNode();
	if (nodeB == nodeList.size() - 1)	//���������ɵĽ�㣬ɾ��
		removeNode();
	if (ifNodeDefine(newIdentifier))
		return mappingTable[newIdentifier];	//newIdentifier�Ѷ���
	else
		return createLeafNode(newIdentifier);	//newIdentifierδ����
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
			return i;	//���ڸý��
	}
	return -1;	//������
}

int DAGraph::createInnerNode(string op, int lc, int rc)
{
	Node* node = new Node(op, lc, rc);
	nodeList.push_back(node);
	return nodeList.size() - 1;
}

void DAGraph::resIdentifierAdd(string identifier, int n)
{
	if (ifNodeDefine(identifier))	//��A��NODE�Ѷ���
	{
		nodeList[mappingTable[identifier]]->identifierRemove(identifier);	//�Ƴ�
		nodeList[n]->identifierAdd(identifier);	//��A�����ڽ��n��
		mappingTable[identifier] = n;
	}
	else
	{
		nodeList[n]->identifierAdd(identifier);	//��A�����ڽ��n��
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
	if (isalpha(op[0]))	//������������Ԫʽ���д���
		return;
	int resNode;	//���ս���Ľ���
	int argNode1, argNode2;	//�������Ľ���
	argNode1 = getNode(B);
	if (argNode1 == -1)	//��Bδ����
		argNode1 = createLeafNode(B);	//������ΪB��Ҷ��㣬������NODE(B)
	if (C == "#") //0��1��
	{
		if (op != ":=") //1��
		{
			//2(1)
			if (ifNodeConstLeaf(argNode1))	//NODE(B)Ϊ����Ҷ���
			{
				//2(3)
				resNode = excuteOP_1(argNode1, op);	//�ϲ���֪��
			}
			else
			{
				//3(1)
				resNode = FindNode(op, argNode1);	//�ҹ����ӱ��ʽ
				if (resNode == -1)	//û���ҵ�
				{
					resNode = createInnerNode(op, argNode1);
				}
			}
		}
		else //0��
		{
			resNode = argNode1;
		}
	}
	else //2��
	{
		argNode2 = getNode(C);
		if (argNode2 == -1)	//��Cδ����
			argNode2 = createLeafNode(C);	//������ΪC��Ҷ��㣬������NODE(C)
		//2(2)
		if (ifNodeConstLeaf(argNode2) && ifNodeConstLeaf(argNode1))	//NODE(B)��NODE(C)���ǳ���Ҷ���
		{
			//2(4)
			resNode = excuteOP_2(argNode1, argNode2, op);
		}
		else
		{
			//3(2)
			resNode = FindNode(op, argNode1, argNode2);	//�ҹ����ӱ��ʽ
			if (resNode == -1)	//û���ҵ�
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
	int lc, rc;	//�����ӽ��
	for (int i = 0; i < nodeList.size(); i++)
	{
		currNode = nodeList[i];
		if (!currNode->getIfLeaf())	//�ڲ�������ɴ���
		{
			quat.op = currNode->getLabel();	//��Ԫʽ��op
			set<string> identifierSet = currNode->getIdentifierSet();
			lc = currNode->getLeftChild();	//����Ԫʽ��arg1
			if (nodeList[lc]->getIfLeaf())	//�ӽ����Ҷ���
				quat.arg1 = nodeList[lc]->getLabel();
			else  //�ӽ�����ڲ����
			{
				set<string> childIdentifierSet = nodeList[lc]->getIdentifierSet();
				set<string>::iterator it = childIdentifierSet.begin();
				quat.arg1 = (*it);
			}
			rc = currNode->getRightChild();	//����Ԫʽ��arg2
			if (rc == -1)	//��arg2
				quat.arg2 = "#";
			else if (nodeList[rc]->getIfLeaf())	//�ӽ����Ҷ���
				quat.arg2 = nodeList[rc]->getLabel();
			else  //�ӽ�����ڲ����
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
			while (setit != identifierSet.end())	//����Ԫʽ��res
			{
				quat.res = (*setit);
				quat.Output();
				++setit;
			}
		}
		else  //Ҷ������ɴ���
		{
			quat.op = ":=";
			quat.arg1 = currNode->getLabel();
			quat.arg2 = "#";
			set<string> identifierSet = currNode->getIdentifierSet();
			auto setit = identifierSet.begin();
			while (setit != identifierSet.end())	//����Ԫʽ��res
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
		if (currNode->getIfLeaf())	//��Ҷ���
		{
			auto setit = identifierSet.begin();
			while (setit != identifierSet.end())
			{
				identifier = (*setit);
				if (identifier[0] == 'T')	//ɾ��
				{
					++setit;
					currNode->identifierRemoveCanEmpty(identifier);
					mappingTable.erase(identifier);
				}
				else
					++setit;
			}
		}
		else  //�ڲ����
		{
			auto setit = identifierSet.begin();
			while (setit != identifierSet.end())
			{
				identifier = (*setit);
				if (identifier[0] == 'T' && identifierSet.size() != 1)	//ɾ��,�����Ϊ��
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
	if (isalpha(op[0]))	//��ת
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
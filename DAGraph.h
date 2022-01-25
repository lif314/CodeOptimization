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

//DAGͼ��
class DAGraph
{
private:
	vector<Node*> nodeList;	//��㼯
	unordered_map<string, int> mappingTable;	//��ʶ������Ķ�Ӧ������Node(A)

	bool ifNodeDefine(string identifier);	//��ʶ����Node�Ƿ��壬1Ϊ����
	int getNode(string identifier);	//��Node(),��δ�����򷵻�-1
	int createLeafNode(string identifier);	//������Ϊidentifier��Ҷ���,�����½ڵ���±�
	bool ifNodeConstLeaf(string identifier);	//��ʶ����Node�Ƿ��Ǳ��Ϊ������Ҷ��㣬��֤����Ѵ��ڣ�1Ϊ��
	bool ifNodeConstLeaf(int node_id);	//Node�Ƿ��Ǳ��Ϊ������Ҷ��㣬��֤����Ѵ��ڣ�1Ϊ��
	int excuteOP_1(int nodeB, string op);	//ִ��op B,��1����룬��֤����ǳ���Ҷ��㣬���ؽ�������±�
	int excuteOP_2(int nodeB, int nodeC, string op);
	//ִ��B op C,��2����룬��֤����ǳ���Ҷ���,��id2Ϊ�ڶ��������������ؽ�������±�
	void removeNode();	//ɾ�����µĽ�㣬����mappingtable���Ƴ�
	int FindNode(string op, int lc, int rc = -1);	//����Ƿ����һ��㣬����Ϊlc��rc�ұ��Ϊop,��û���򷵻�-1
	int createInnerNode(string op, int lc, int rc = -1);	//�����ڲ���㣬��labelΪop,�ӽڵ�Ϊlc,rc,�����½ڵ���±�
	void resIdentifierAdd(string identifier, int n);	//�Դ�����A�Ĳ�����������4,��A�����ڽ��n��

public:
	DAGraph();
	~DAGraph();
	void Clear();

	void outputGraph();	//���DAGͼ
	void constructbyTac(Quaternary& quat);	//����һ����Ԫʽ������DAGͼ
	void outputQuaternary();	//����DAG��Ӧ����Ԫʽ
	void deleteUselessAssign();	//ɾ�����ø�ֵ������T��ͷ�ı�ʶ���ڻ�����󲻻ᱻ����
	void DAGOptimize(BasicBlock& block, vector<Quaternary>& qua);	//DAG�Ż�
	
};

#endif // !_DAGraph_
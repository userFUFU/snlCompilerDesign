#include<bits/stdc++.h>
#include "semantemeDesign.h"
using namespace std;
#define maxn 1010

smtTable* scope[maxn];

int level = 0;
int offset = 0;
int mOFF = 0;
/*
void smtMain(grammarTreeNode* r) {
	createTable(); //������
	//init();

	grammarTreeNode* p = r->child[1]; //�﷨���������ڵ�
	while (p) {
		switch (p->nodeKind){
		case TypeK:
			typeDecPart(p->child[0]);
			break;
		case VarK:
			varDecPart(p->child[0]);
			break;
		case ProcDecK:
			procDecPart(p->child[0]);
			break;
		default:
			break;
		}
		p = p->sibling;
	}

	p = p->child[2]; // �﷨���ĳ�����ڵ�
	if (p->nodeKind == StmLK) {
		procBody(p);
	}
	if (level != -1) {
		DestroyTable();
		//level--; // ����
	}

}

// ����һ���µľֲ�����λʱ�����շ��ű� 
void createTable() {
	level++;
	offset = 0;
	scope[level] = NULL;
}
// �˳�һ���ֲ�����ʱ,������ǰ���ű�
void DestroyTable(){
	level--;
}


// �ǼǱ�ʶ��������
bool Enter(char* id, AttributeIR* attribP, smtTable** entry) {

}

void typeDecPart(grammarTreeNode* r) {

}

void varDecPart(grammarTreeNode* r) {

}

void procDecPart(grammarTreeNode* r) {

}

void procBody(grammarTreeNode* r) {

}

// �����µ��������ı�ʶ��ʱ����һ���յķ��ű�
smtTable* newTable() {
	smtTable* t = new smtTable;
	if (t == NULL) {
		cout << "Out of memory." << endl;
	}
	return t;
}
*/
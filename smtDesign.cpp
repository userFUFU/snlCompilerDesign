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
	createTable(); //创建表
	//init();

	grammarTreeNode* p = r->child[1]; //语法树的声明节点
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

	p = p->child[2]; // 语法树的程序体节点
	if (p->nodeKind == StmLK) {
		procBody(p);
	}
	if (level != -1) {
		DestroyTable();
		//level--; // 撤销
	}

}

// 进入一个新的局部化单位时创建空符号表 
void createTable() {
	level++;
	offset = 0;
	scope[level] = NULL;
}
// 退出一个局部化区时,撤销当前符号表
void DestroyTable(){
	level--;
}


// 登记标识符和属性
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

// 遇到新的无声明的标识符时创建一个空的符号表
smtTable* newTable() {
	smtTable* t = new smtTable;
	if (t == NULL) {
		cout << "Out of memory." << endl;
	}
	return t;
}
*/
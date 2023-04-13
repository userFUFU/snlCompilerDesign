#pragma once
#include<bits/stdc++.h>
#include "grammarDesign.h"
using namespace std;
// Level Name Kind TypePtr Offset Access(dir/indir)

// SNL 的类型包括：整数类型，字符类型，数组类型，记录类型，布尔类型
typedef enum{ intTy, charTy , arrayTy, recordTy, boolTy } Typekind;

typedef  enum { typeKind, varKind, procKind }IdKind;

// 形参表
typedef struct  paramTable{
	struct symbtable* entry;
	struct paramTable* next;
}ParamNode;

typedef enum { dir, indir } AccessKind;


// 类型的内部表示

struct fieldChain {
	string name;
	Typekind unitTy; // 成员类型
	int off;
	fieldChain* next;
};

typedef struct typeNode {
	int size;
	Typekind type;
	union {
		struct {
			struct typeNode* indexTy;
			struct typeNode* elemTy;
		}arrayAttr; // 数组
		fieldChain* body; // 域类型链表
	};
}typeIR;

// 标识符的结构
typedef struct {
	//int level;
	//string name;
	IdKind mykind;
	typeIR* myType;

	union {
		struct {
			AccessKind access;
			int level;
			int off;
			bool isparam; // 是不是参数
		}varAttr; // 变量
		struct {
			int level;
			ParamNode* param;
			int size; //过程活动记录大小
			int off; // sp指针到display表的偏移
			int procEntry;
		}procAttr; //  函数/过程
	}Attr;

	//int offset;
	//AccessKind access;
}AttributeIR;

// 符号表的结构
typedef struct tNode{
	string name;
	AttributeIR attrIR;
	tNode* next;

	tNode() {
		next = NULL;
		attrIR.mykind = typeKind;
		attrIR.myType = NULL;
		attrIR.Attr.varAttr.isparam = false;
	}
}smtTable;


void smtMain(grammarTreeNode* r);

void typeDecPart(grammarTreeNode* r);
void varDecPart(grammarTreeNode* r);
void procDecPart(grammarTreeNode* r);
void procBody(grammarTreeNode* r);

void createTable();
smtTable* newTable();
void DestroyTable();
bool Enter(char* id, AttributeIR* attribP, smtTable** entry);


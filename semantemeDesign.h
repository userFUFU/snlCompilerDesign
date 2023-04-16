#pragma once
#include<bits/stdc++.h>
#include "grammarDesign.h"
using namespace std;
// Level Name Kind TypePtr Offset Access(dir/indir)

// SNL 的类型包括：整数类型，字符类型，数组类型，记录类型，布尔类型
typedef enum{ intTy, charTy , arrayTy, recordTy, boolTy } Typekind;

typedef  enum { typeKind, varKind, procKind }IdKind;

// 形参表
struct tNode;
typedef struct  paramTable{
	struct tNode* entry;
	struct paramTable* next;
}ParamNode;

typedef enum { dir, indir } AccessKind;


// 类型的内部表示
struct typeNode;
typedef struct FieldChain {
	string name;
	struct typeNode* unitTy;  // 成员类型
	int off;
	FieldChain* next;
}fieldChain;

typedef struct typeNode {
	int size;
	Typekind type;
	union {
		struct {
			struct typeNode* indexTy;
			struct typeNode* elemTy;
			int low, up;
		}arrayAttr; // 数组
		fieldChain* body;  // 域类型链表
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

	int offset;
	//AccessKind access;
}AttributeIR;

// 符号表的结构
typedef struct tNode{
	int level;
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
void init();
void typeDecPart(grammarTreeNode* r);
typeIR* typeProcess(grammarTreeNode* t, DecKind deckind);
typeIR* arrayType(grammarTreeNode* r);
typeIR* charType();
typeIR* intType();
typeIR* boolType();
typeIR* recordType(grammarTreeNode* r);
typeIR* nameType(grammarTreeNode* r);
void varDecList(grammarTreeNode* r);
void procDecPart(grammarTreeNode* r);
smtTable* HeadProcess(grammarTreeNode* r);
paramTable* paramDecList(grammarTreeNode* r);
void Body(grammarTreeNode* r);
void statement(grammarTreeNode* r);
typeIR* Expr(grammarTreeNode* r, AccessKind* Ekind);
void ifStatement(grammarTreeNode* r);
void whileStatement(grammarTreeNode* r);
void assignStatement(grammarTreeNode* r);
typeIR* arrayVar(grammarTreeNode* r);
typeIR* recordVar(grammarTreeNode* r);
void readStatement(grammarTreeNode* r);
void writeStatement(grammarTreeNode* r);
void callStatement(grammarTreeNode* r);
void returnStatement(grammarTreeNode* r);
smtTable* newTable();
void createTable();
void DestroyTable();
int FindEntry(string id, smtTable** entry);
int searchOneTable(string id, int now, smtTable** entry);
bool findField(string id, fieldChain* head, fieldChain** entry);
bool Enter(string id, AttributeIR* attribP, smtTable** entry);
void printSymbTable();

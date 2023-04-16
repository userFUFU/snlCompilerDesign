#pragma once
#include<bits/stdc++.h>
#include "grammarDesign.h"
using namespace std;
// Level Name Kind TypePtr Offset Access(dir/indir)

// SNL �����Ͱ������������ͣ��ַ����ͣ��������ͣ���¼���ͣ���������
typedef enum{ intTy, charTy , arrayTy, recordTy, boolTy } Typekind;

typedef  enum { typeKind, varKind, procKind }IdKind;

// �βα�
struct tNode;
typedef struct  paramTable{
	struct tNode* entry;
	struct paramTable* next;
}ParamNode;

typedef enum { dir, indir } AccessKind;


// ���͵��ڲ���ʾ
struct typeNode;
typedef struct FieldChain {
	string name;
	struct typeNode* unitTy;  // ��Ա����
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
		}arrayAttr; // ����
		fieldChain* body;  // ����������
	};
}typeIR;




// ��ʶ���Ľṹ
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
			bool isparam; // �ǲ��ǲ���
		}varAttr; // ����
		struct {
			int level;
			ParamNode* param;
			int size; //���̻��¼��С
			int off; // spָ�뵽display���ƫ��
			int procEntry;
		}procAttr; //  ����/����
	}Attr;

	int offset;
	//AccessKind access;
}AttributeIR;

// ���ű�Ľṹ
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

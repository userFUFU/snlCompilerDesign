#pragma once
#include<bits/stdc++.h>
#include "grammarDesign.h"
using namespace std;
// Level Name Kind TypePtr Offset Access(dir/indir)

// SNL �����Ͱ������������ͣ��ַ����ͣ��������ͣ���¼���ͣ���������
typedef enum{ intTy, charTy , arrayTy, recordTy, boolTy } Typekind;

typedef  enum { typeKind, varKind, procKind }IdKind;

// �βα�
typedef struct  paramTable{
	struct symbtable* entry;
	struct paramTable* next;
}ParamNode;

typedef enum { dir, indir } AccessKind;


// ���͵��ڲ���ʾ

struct fieldChain {
	string name;
	Typekind unitTy; // ��Ա����
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
		}arrayAttr; // ����
		fieldChain* body; // ����������
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

	//int offset;
	//AccessKind access;
}AttributeIR;

// ���ű�Ľṹ
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


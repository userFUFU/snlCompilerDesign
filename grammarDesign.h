#pragma once
#include<bits/stdc++.h>
#include "LEXsnl.h"
using namespace std;
// ��104������ʽ
#define LL1TableSize 120 

// LL(1)
// ���з��ռ��� ��68��
typedef enum {
	Program=1, ProgramHead, ProgramName, DeclarePart,TypeDec, 
	TypeDeclaration, TypeDecList, TypeDecMore,TypeId, TypeDef,
	BaseType, StructureType,ArrayType, Low, Top, 
	RecType,FieldDecList, FieldDecMore, IdList, IdMore,
	VarDec, VarDeclaration, VarDecList, VarDecMore,VarIdList,
	VarIdMore, ProcDec, ProcDeclaration,ProcDecMore, ProcName, 
	ParamList, ParamDecList,ParamMore, Param, FormList, 
	FidMore,ProcDecPart, ProcBody, ProgramBody, StmList,
	StmMore, Stm, AssCall, AssignmentRest,ConditionalStm,
	StmL, LoopStm, InputStm,InVar, OutputStm, 
	ReturnStm, CallStmRest,ActParamList, ActParamMore, RelExp, 
	OtherRelE,Exp, OtherTerm, Term, OtherFactor,
	Factor, Variable, VariMore, FieldVar,FieldVarMore,
	CmpOp, AddOp, MultOp
} NontmlWord;
// �ռ��� ��42��
typedef LexType  TmlWord;

// ����ջ,����ջ�Ľڵ�ṹ
typedef struct node {
	int isTmlWord; // 1:�ռ���,0:���ռ���
	union unionname{
		NontmlWord ntWord;
		TmlWord tWord;
	}content;

	node(NontmlWord c) {
		content.ntWord = c;
		isTmlWord = 0;
	}
	node(TmlWord c) {
		content.tWord = c;
		isTmlWord = 1;
	}
}stackNode;

// ��־�ڵ�
typedef enum { ProK=1, PheadK, DecK, TypeK, VarK, ProcDecK, StmLK, StmtK, ExpK }NodeKind;
static map<NodeKind, string>displayNodeKind{
	{ProK,"ProK"},{PheadK,"PheadK"},{DecK,"DecK"},{TypeK,"TypeK"},{VarK,"VarK"},
	{ProcDecK,"ProcDecK"},{StmLK,"StmLK"},{StmtK,"StmtK"},{ExpK,"ExpK"}
};
// ��ΪDecK-���������ڵ㣬���������Ϊ��������
typedef enum { ArrayK=1, CharK, IntegerK, RecordK, DecIdK }  DecKind;
static map<DecKind, string>displayDecKind{
	{ArrayK,"ArrayK"},{CharK,"CharK"},{IntegerK,"IntegerK"},{RecordK,"RecordK"},{DecIdK,"DecIdK"}
};
// ��ΪStmtK-������ͽڵ㣬���������Ϊ��������
typedef enum { IfK=1, WhileK, AssignK, ReadK, WriteK, CallK, ReturnK } StmtKind;
static map<StmtKind, string>displayStmtKind{
	{IfK,"IfK"},{WhileK,"WhileK"},{AssignK,"AssignK"},{ReadK,"ReadK"},{WriteK,"WriteK"},{CallK,"CallK"},{ReturnK,"ReturnK"}
};
// ��ΪExpK-���ʽ���ͽڵ㣬���������Ϊ��������
typedef enum { OpK=1, ConstK, ExpIdK } ExpKind;
static map<ExpKind, string>displayExptKind{{OpK,"OpK"},{ConstK,"ConstK"},{ExpIdK,"ExpIdK"}};
// ��ΪVarK-�������ͽڵ㣬���������Ϊ��������
typedef enum { IdV=1, ArrayMembV, FieldMembV } VarKind;
static map<VarKind, string>displayVarKind{ {IdV,"IdV"},{ArrayMembV,"ArrayMembV"},{FieldMembV,"FieldMembV"} };
// ��ΪParamK-�������ͽڵ㣬���������Ϊ��������
typedef enum { valparamType=1, varparamType } ParamType;
static map<ParamType, string>displayParamType{ {valparamType,"valparamType"},{varparamType,"varparamType"} };
// 
typedef enum { Void=1, Integer, Boolean } ExpType;
static map<ExpType, string>displayExpType{ {Void,"Void"},{Integer,"Integer"},{Boolean,"Boolean"} };

// �﷨���ڵ�ṹ
typedef struct grammarnode{
	int linenum;
	struct grammarnode* child[3];
	struct grammarnode* sibling; // �ֵܽڵ�ָ��
	NodeKind nodeKind; // �ڵ�����

	// �ýڵ�ı����������֣�int a,b,c,d; nameNum=4...
	int nameNum;
	string name[10];

	// �������� 
	union{
		DecKind  dec;
		StmtKind stmt;
		ExpKind  exp;
	}kind;

	struct{ // ����
		struct { // ����
			int low, up;
			DecKind arrayKind;
		}arrayAttr;
		struct { // ������������
			ParamType paramType;
		}paramAttr;
		struct { // ���ʽ
			LexType op;
			int val;
			VarKind varKind; // �������
			//ExpType expType;//�������ͼ��(����)
		}expAttr;
	}Attr;

	string typeName;   // type���¶�������͵�����

	grammarnode() {
		for (int i = 0; i < 3; i++)child[i] = NULL;
		sibling = NULL;
		nameNum = 0;
	}
	grammarnode(NodeKind myKind,int line) {
		for (int i = 0; i < 3; i++)child[i] = NULL;
		sibling = NULL;
		nameNum = 0;
		nodeKind = myKind;
		linenum = line;
	}
}grammarTreeNode;




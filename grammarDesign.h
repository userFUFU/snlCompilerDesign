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
static map<ExpKind, string>displayExpKind{{OpK,"OpK"},{ConstK,"ConstK"},{ExpIdK,"ExpIdK"}};
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





inline void printGrammarTree(int spaceNum, grammarTreeNode* r) {
	if (r == NULL)return;

	for (int i = 0; i < spaceNum; i++)cout << " ";

	//cout << r->nodeKind << " ";

	string str = displayNodeKind[r->nodeKind];
	switch (r->nodeKind) {
	case ProK:
		cout << str << " ";
		break;
	case PheadK: // PheadK p 
		cout << str << " " << r->name[0];
		break;
		// Deck ArrayK a 1 20 IntegerK 5 25 
		// DecK IntegerK v1 v2
		// Deck IntegerK num valparam 
	case DecK:
	{
		cout << str << " ";

		// �����﷨��Ϣ
		string decKstr = displayDecKind[r->kind.dec];
		switch (r->kind.dec) {
		case ArrayK:
		{
			cout << decKstr << " ";
			cout << r->Attr.arrayAttr.low << " " << r->Attr.arrayAttr.up << " ";
			// ����Ļ������ͣ�char��int
			if (r->Attr.arrayAttr.arrayKind == CharK) {
				cout << "CharK" << " ";
			}
			else if (r->Attr.arrayAttr.arrayKind == IntegerK) {
				cout << "IntegerK" << " ";
			}
			break;
		}
		case CharK:
			cout << decKstr << " ";
			break;
		case IntegerK:
			cout << decKstr << " ";
			break;
		case RecordK:
			cout << decKstr << " ";
			break;
		case DecIdK:
			cout << decKstr << " " << r->typeName << " ";
			break;
		}
		// �������
		if (r->nameNum) {
			for (int i = 0; i < r->nameNum; i++)cout << r->name[i] << " ";
		}
		else {
			cout << "error!DecK . no name.";
		}

		// �ǲ��Ǻ�����������
		string paramTystr = displayParamType[r->Attr.paramAttr.paramType];
		if (r->Attr.paramAttr.paramType == valparamType) { // ֵ��
			cout << paramTystr << " ";
		}
		else if (r->Attr.paramAttr.paramType == varparamType) { // ���
			cout << paramTystr << " ";
		}
		break;
	}
	case TypeK:
		cout << str << " ";
		break;
	case VarK:
		cout << str << " ";
		break;
	case ProcDecK: // ProcDecK q 
		cout << str << " " << r->name[0];
		break;
	case StmLK:
		cout << str << " ";
		break;
	case StmtK: // StmtK Assign
	{
		cout << str << " ";

		string stmtKstr = displayStmtKind[r->kind.stmt];
		switch (r->kind.stmt) {
		case IfK:
			cout << stmtKstr << " ";
			break;
		case WhileK:
			cout << stmtKstr << " ";
			break;
		case AssignK:
			cout << stmtKstr << " ";
			break;
		case ReadK:
			cout << stmtKstr << " " << r->name[0] << " ";
			break;
		case WriteK:
			cout << stmtKstr << " ";
			break;
		case CallK:// StmtK CallK q 
			cout << stmtKstr << " " << r->name[0] << " ";
			break;
		case ReturnK:
			cout << stmtKstr << " ";
			break;
		default:
			cout << "error! StmtK.";
			break;
		}
		break;
	}
	// ExpK Op <
	//      ExpK IdV v1
	//      ExpK Const 10
	case ExpK:
	{
		cout << str << " ";

		string expKstr = displayExpKind[r->kind.exp];
		switch (r->kind.exp) {
			// + - * / = <
		case OpK: {
			cout << expKstr << " ";
			switch (r->Attr.expAttr.op) {
			case PLUS:cout << "+" << " "; break;
			case MINUS:cout << "-" << " "; break;
			case TIMES:cout << "*" << " "; break;
			case OVER:cout << "/" << " "; break;
			case EQ:cout << "=" << " "; break;
			case LT:cout << "<" << " "; break;
			default:
				cout << "error! op not exist.";
				break;
			}
			break;
		}
				// VarKind
		case ConstK: {
			cout << expKstr << " ";

			string varKstr = displayVarKind[r->Attr.expAttr.varKind];
			switch (r->Attr.expAttr.varKind) {
			case IdV:cout << varKstr << " "; break;
			case ArrayMembV:cout << varKstr << " "; break;
			case FieldMembV:cout << varKstr << " "; break;
				/*default:
					cout << "error! var type.";
					break;*/
			}
			cout << r->Attr.expAttr.val << " ";
			break;
		}
				   // 
		case ExpIdK: {
			cout << expKstr << " ";

			string varKstr = displayVarKind[r->Attr.expAttr.varKind];
			switch (r->Attr.expAttr.varKind) {
			case IdV:cout << varKstr << " " << r->name[0] << " "; break;
			case ArrayMembV:cout << varKstr << " " << r->name[0] << " "; break;
			case FieldMembV:cout << varKstr << " " << r->name[0] << " "; break;
			default:
				cout << "error! var type.";
				break;
			}
			break;
		}

		default:
			cout << "error! ExpK";
			break;
		}
		break;
	}
	default:
		cout << "error in print grammar tree." << endl;
		break;
	}

	cout << endl;

	for (int i = 0; i < 3; i++) {
		printGrammarTree(spaceNum + 4, r->child[i]);
	}
	printGrammarTree(spaceNum, r->sibling);
}

inline string generateCode(){}
#pragma once
#include<bits/stdc++.h>
#include "lexicalDesign.h"
#include "grammarDesign.h"
#include "LLDesign.h"
using namespace std;
// ���ű�
int LLTable[LL1TableSize][LL1TableSize];
// ����ջ,�﷨ջ,������ջ,������ջ
stack<stackNode*> analyseStack;
stack<grammarTreeNode**> grammarStack;
stack<grammarTreeNode*> opStack;
stack<grammarTreeNode*> numberStack;

// ����
grammarTreeNode* LLroot = NULL;
int num = 0; // ѡ�еĲ���ʽ���
bool flag = true; // ջ�ձ�־
int lineno=1,tokenno=1; // �к�,��ǰtoken��
Token* curTokenLL; // token
bool getExpResult = 1, getExpResult2; // getExpResult2 ר���������Ա����
int expFlag; // ��������ƥ��



grammarTreeNode* LLmain(Token token[], int num) {
	curTokenLL = token;
	// ��ʼ��LL(1)���ű�
	for (int i = 0; i < LL1TableSize; i++) 
		for (int j = 0; j < LL1TableSize; j++)
			LLTable[i][j] = 0;
	// �������ű�
	create_LL1table();
	// �γ�LL(1)�﷨��
	tokenno = 1;
	// �����﷨�����ڵ�ProK
	grammarTreeNode* root = new grammarTreeNode();
	root->nodeKind = ProK;
	// ����������ָ��ѹ���﷨��ջ
	grammarStack.push(&(root->child[2]));
	grammarStack.push(&(root->child[1]));
	grammarStack.push(&(root->child[0]));

	analyseStack.push(new stackNode(Program));

	while (!analyseStack.empty()) {
		lineno = token[tokenno].linenum;
		stackNode* now = analyseStack.top();
		analyseStack.pop();
		if (now->isTmlWord == 1) { // ջ��Ϊ�ռ���
			if (tokenno<num && token[tokenno].type_inf == now->content.tWord) {
				//cout <<  "Match!!!  ��ǰtoken:" << token[tokenno].type_inf << "  " << token[tokenno].content << " ; ��ǰ����ջ��(�ռ���):" << now->content.tWord << endl;
				tokenno++;
			}
			else { // ƥ�䲻�ɹ�
				cout << "��" << token[tokenno].linenum << "��,�﷨����:unexpected  token." << endl;
				cout <<"��ǰtoken:" <<token[tokenno].type_inf<<"  " << token[tokenno].content << " ; ��ǰ����ջ��(�ռ���):" << now->content.tWord <<endl;
				//printGrammarTree(0, root);
				//exit(0);
			}
		}
		else { // ջ��Ϊ���ռ���
			int PFnum = LLTable[now->content.ntWord][token[tokenno].type_inf]; // ����ʽ���
			//cout <<"predict num :"<<PFnum<< " ; ��ǰtoken:" << token[tokenno].type_inf << "  " << token[tokenno].content << " ; ��ǰ����ջ��(���ռ���):" << now->content.tWord << endl;
			predict(PFnum);
		}
	}
	if (token[tokenno].type_inf != ENDFILE) { // ջ�ѿգ�����������
		cout << "EOF error!" << endl;
		//exit(0);
	}
	//cout << "no error! print LL(1) Tree!" << endl;
	// printGrammarTree(0, root);
	return root;
}


// ����LL(1)���ű�
// ���Զ�����,,,��˵��,,,
void create_LL1table() {
	LLTable[Program][PROGRAM] = 1;

	LLTable[ProgramHead][PROGRAM] = 2;

	LLTable[ProgramName][ID] = 3;

	LLTable[DeclarePart][TYPE] = 4;
	LLTable[DeclarePart][VAR] = 4;
	LLTable[DeclarePart][PROCEDURE] = 4;
	LLTable[DeclarePart][BEGIN1] = 4;

	LLTable[TypeDec][VAR] = 5;
	LLTable[TypeDec][PROCEDURE] = 5;
	LLTable[TypeDec][BEGIN1] = 5;

	LLTable[TypeDec][TYPE] = 6;

	LLTable[TypeDeclaration][TYPE] = 7;

	LLTable[TypeDecList][ID] = 8;

	LLTable[TypeDecMore][VAR] = 9;
	LLTable[TypeDecMore][PROCEDURE] = 9;
	LLTable[TypeDecMore][BEGIN1] = 9;


	LLTable[TypeDecMore][ID] = 10;

	LLTable[TypeId][ID] = 11;

	LLTable[TypeDef][INTEGER] = 12;
	LLTable[TypeDef][CHAR1] = 12;

	LLTable[TypeDef][ARRAY] = 13;
	LLTable[TypeDef][RECORD] = 13;

	LLTable[TypeDef][ID] = 14;

	LLTable[BaseType][INTEGER] = 15;

	LLTable[BaseType][CHAR1] = 16;

	LLTable[StructureType][ARRAY] = 17;

	LLTable[StructureType][RECORD] = 18;

	LLTable[ArrayType][ARRAY] = 19;

	LLTable[Low][INTC] = 20;

	LLTable[Top][INTC] = 21;

	LLTable[RecType][RECORD] = 22;

	LLTable[FieldDecList][INTEGER] = 23;
	LLTable[FieldDecList][CHAR1] = 23;

	LLTable[FieldDecList][ARRAY] = 24;

	LLTable[FieldDecMore][END1] = 25;

	LLTable[FieldDecMore][INTEGER] = 26;
	LLTable[FieldDecMore][CHAR1] = 26;
	LLTable[FieldDecMore][ARRAY] = 26;

	LLTable[IdList][ID] = 27;

	LLTable[IdMore][SEMI] = 28;

	LLTable[IdMore][COMMA] = 29;

	LLTable[VarDec][PROCEDURE] = 30;
	LLTable[VarDec][BEGIN1] = 30;

	LLTable[VarDec][VAR] = 31;

	LLTable[VarDeclaration][VAR] = 32;

	LLTable[VarDecList][INTEGER] = 33;
	LLTable[VarDecList][CHAR1] = 33;
	LLTable[VarDecList][ARRAY] = 33;
	LLTable[VarDecList][RECORD] = 33;
	LLTable[VarDecList][ID] = 33;

	LLTable[VarDecMore][PROCEDURE] = 34;
	LLTable[VarDecMore][BEGIN1] = 34;


	LLTable[VarDecMore][INTEGER] = 35;
	LLTable[VarDecMore][CHAR1] = 35;
	LLTable[VarDecMore][ARRAY] = 35;
	LLTable[VarDecMore][RECORD] = 35;
	LLTable[VarDecMore][ID] = 35;

	LLTable[VarIdList][ID] = 36;

	LLTable[VarIdMore][SEMI] = 37;

	LLTable[VarIdMore][COMMA] = 38;

	LLTable[ProcDec][BEGIN1] = 39;

	LLTable[ProcDec][PROCEDURE] = 40;

	LLTable[ProcDeclaration][PROCEDURE] = 41;

	LLTable[ProcDecMore][BEGIN1] = 42;

	LLTable[ProcDecMore][PROCEDURE] = 43;

	LLTable[ProcName][ID] = 44;

	LLTable[ParamList][RPAREN] = 45;

	LLTable[ParamList][INTEGER] = 46;
	LLTable[ParamList][CHAR1] = 46;
	LLTable[ParamList][ARRAY] = 46;
	LLTable[ParamList][RECORD] = 46;
	LLTable[ParamList][ID] = 46;
	LLTable[ParamList][VAR] = 46;

	LLTable[ParamDecList][INTEGER] = 47;
	LLTable[ParamDecList][CHAR1] = 47;
	LLTable[ParamDecList][ARRAY] = 47;
	LLTable[ParamDecList][RECORD] = 47;
	LLTable[ParamDecList][ID] = 47;
	LLTable[ParamDecList][VAR] = 47;

	LLTable[ParamMore][RPAREN] = 48;

	LLTable[ParamMore][SEMI] = 49;

	LLTable[Param][INTEGER] = 50;
	LLTable[Param][CHAR1] = 50;
	LLTable[Param][ARRAY] = 50;
	LLTable[Param][RECORD] = 50;
	LLTable[Param][ID] = 50;

	LLTable[Param][VAR] = 51;

	LLTable[FormList][ID] = 52;

	LLTable[FidMore][SEMI] = 53;
	LLTable[FidMore][RPAREN] = 53;

	LLTable[FidMore][COMMA] = 54;

	LLTable[ProcDecPart][TYPE] = 55;
	LLTable[ProcDecPart][VAR] = 55;
	LLTable[ProcDecPart][PROCEDURE] = 55;
	LLTable[ProcDecPart][BEGIN1] = 55;

	LLTable[ProcBody][BEGIN1] = 56;

	LLTable[ProgramBody][BEGIN1] = 57;

	LLTable[StmList][ID] = 58;
	LLTable[StmList][IF] = 58;
	LLTable[StmList][WHILE] = 58;
	LLTable[StmList][RETURN1] = 58;
	LLTable[StmList][READ] = 58;
	LLTable[StmList][WRITE] = 58;

	LLTable[StmMore][END1] = 59;
	LLTable[StmMore][ENDWH] = 59;
	LLTable[StmMore][ELSE] = 59;
	LLTable[StmMore][FI] = 59;

	LLTable[StmMore][SEMI] = 60;

	LLTable[Stm][IF] = 61;

	LLTable[Stm][WHILE] = 62;

	LLTable[Stm][READ] = 63;

	LLTable[Stm][WRITE] = 64;

	LLTable[Stm][RETURN1] = 65;

	LLTable[Stm][ID] = 66;

	LLTable[AssCall][ASSIGN] = 67;
	LLTable[AssCall][LMIDPAREN] = 67;
	LLTable[AssCall][DOT] = 67;


	LLTable[AssCall][LPAREN] = 68;

	LLTable[AssignmentRest][ASSIGN] = 69;
	LLTable[AssignmentRest][LMIDPAREN] = 69;
	LLTable[AssignmentRest][DOT] = 69;

	LLTable[ConditionalStm][IF] = 70;


	LLTable[LoopStm][WHILE] = 71;

	LLTable[InputStm][READ] = 72;

	LLTable[InVar][ID] = 73;

	LLTable[OutputStm][WRITE] = 74;

	LLTable[ReturnStm][RETURN1] = 75;

	LLTable[CallStmRest][LPAREN] = 76;

	LLTable[ActParamList][RPAREN] = 77;

	LLTable[ActParamList][ID] = 78;
	LLTable[ActParamList][INTC] = 78;
	LLTable[ActParamList][LPAREN] = 78;

	LLTable[ActParamMore][RPAREN] = 79;

	LLTable[ActParamMore][COMMA] = 80;

	LLTable[RelExp][LPAREN] = 81;
	LLTable[RelExp][INTC] = 81;
	LLTable[RelExp][ID] = 81;

	LLTable[OtherRelE][LT] = 82;
	LLTable[OtherRelE][EQ] = 82;

	LLTable[Exp][LPAREN] = 83;
	LLTable[Exp][INTC] = 83;
	LLTable[Exp][ID] = 83;

	LLTable[OtherTerm][LT] = 84;
	LLTable[OtherTerm][EQ] = 84;
	LLTable[OtherTerm][THEN] = 84;
	LLTable[OtherTerm][DO] = 84;
	LLTable[OtherTerm][RPAREN] = 84;
	LLTable[OtherTerm][END1] = 84;
	LLTable[OtherTerm][SEMI] = 84;
	LLTable[OtherTerm][COMMA] = 84;
	LLTable[OtherTerm][ENDWH] = 84;
	LLTable[OtherTerm][ELSE] = 84;
	LLTable[OtherTerm][FI] = 84;
	LLTable[OtherTerm][RMIDPAREN] = 84;


	LLTable[OtherTerm][PLUS] = 85;
	LLTable[OtherTerm][MINUS] = 85;

	LLTable[Term][LPAREN] = 86;
	LLTable[Term][INTC] = 86;
	LLTable[Term][ID] = 86;

	LLTable[OtherFactor][PLUS] = 87;
	LLTable[OtherFactor][MINUS] = 87;
	LLTable[OtherFactor][LT] = 87;
	LLTable[OtherFactor][EQ] = 87;
	LLTable[OtherFactor][THEN] = 87;
	LLTable[OtherFactor][ELSE] = 87;
	LLTable[OtherFactor][FI] = 87;
	LLTable[OtherFactor][DO] = 87;
	LLTable[OtherFactor][ENDWH] = 87;
	LLTable[OtherFactor][RPAREN] = 87;
	LLTable[OtherFactor][END1] = 87;
	LLTable[OtherFactor][SEMI] = 87;
	LLTable[OtherFactor][COMMA] = 87;
	LLTable[OtherFactor][RMIDPAREN] = 87;

	LLTable[OtherFactor][TIMES] = 88;
	LLTable[OtherFactor][OVER] = 88;

	LLTable[Factor][LPAREN] = 89;

	LLTable[Factor][INTC] = 90;

	LLTable[Factor][ID] = 91;

	LLTable[Variable][ID] = 92;

	LLTable[VariMore][ASSIGN] = 93;
	LLTable[VariMore][TIMES] = 93;
	LLTable[VariMore][OVER] = 93;
	LLTable[VariMore][PLUS] = 93;
	LLTable[VariMore][MINUS] = 93;
	LLTable[VariMore][LT] = 93;
	LLTable[VariMore][EQ] = 93;
	LLTable[VariMore][THEN] = 93;
	LLTable[VariMore][ELSE] = 93;
	LLTable[VariMore][FI] = 93;
	LLTable[VariMore][DO] = 93;
	LLTable[VariMore][ENDWH] = 93;
	LLTable[VariMore][RPAREN] = 93;
	LLTable[VariMore][END1] = 93;
	LLTable[VariMore][SEMI] = 93;
	LLTable[VariMore][COMMA] = 93;
	LLTable[VariMore][RMIDPAREN] = 93;

	LLTable[VariMore][LMIDPAREN] = 94;

	LLTable[VariMore][DOT] = 95;

	LLTable[FieldVar][ID] = 96;

	LLTable[FieldVarMore][ASSIGN] = 97;
	LLTable[FieldVarMore][TIMES] = 97;
	LLTable[FieldVarMore][OVER] = 97;
	LLTable[FieldVarMore][PLUS] = 97;
	LLTable[FieldVarMore][MINUS] = 97;
	LLTable[FieldVarMore][LT] = 97;
	LLTable[FieldVarMore][EQ] = 97;
	LLTable[FieldVarMore][THEN] = 97;
	LLTable[FieldVarMore][ELSE] = 97;
	LLTable[FieldVarMore][FI] = 97;
	LLTable[FieldVarMore][DO] = 97;
	LLTable[FieldVarMore][ENDWH] = 97;
	LLTable[FieldVarMore][RPAREN] = 97;
	LLTable[FieldVarMore][END1] = 97;
	LLTable[FieldVarMore][SEMI] = 97;
	LLTable[FieldVarMore][COMMA] = 97;

	LLTable[FieldVarMore][LMIDPAREN] = 98;

	LLTable[CmpOp][LT] = 99;

	LLTable[CmpOp][EQ] = 100;

	LLTable[AddOp][PLUS] = 101;

	LLTable[AddOp][MINUS] = 102;

	LLTable[MultOp][TIMES] = 103;

	LLTable[MultOp][OVER] = 104;
}


// �趨���������ȼ�
int priority(LexType op) {
	int p = 0;
	// * / > + - >  < = > ( > END
	if (op == END1)p = -1;
	else if (op == LPAREN)p = 0;
	else if (op == LT || op == EQ)p = 1;
	else if (op == PLUS || op == MINUS)p = 2;
	else if (op == TIMES || op == OVER)p = 3;
	else {
		p = -1;
		cout << curTokenLL[tokenno].linenum << "��, operator not match!"<<endl;
	}
	return p;
}

grammarTreeNode* cur = NULL; // ��ǰ�ڵ�
grammarTreeNode* saveP = NULL; // ���ڱ��浱ǰ�ڵ�
DecKind* tmp = NULL; // ��ʱ��¼��ʶ��������Ϣ�Ĳ��ֵĵ�ַ

// �������ʽ��ţ�ִ����Ӧ����
void predict(int num) {

	switch (num) {
	case 1:
	{
		analyseStack.push(new stackNode(DOT));
		analyseStack.push(new stackNode(ProgramBody));
		analyseStack.push(new stackNode(DeclarePart));
		analyseStack.push(new stackNode(ProgramHead));
		break;
	}
	case 2:
	{
		analyseStack.push(new stackNode(ProgramName));
		analyseStack.push(new stackNode(PROGRAM));
		// ����ͷ�ڵ�, ��ʼ��һ��PheadK, ��ֵ���﷨ջ���ڵ�
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = PheadK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		break;
	}
	case 3:
	{
		analyseStack.push(new stackNode(ID));
		// ��ID���ָ�����ǰ�ڵ��name[nameNum];
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	}
	case 4:
	{
		// ���鲻ͬ
		analyseStack.push(new stackNode(ProcDec));
		analyseStack.push(new stackNode(VarDec));
		analyseStack.push(new stackNode(TypeDec));
		break;
	}
	case 5:
		break; // ��
	case 6:
	{
		analyseStack.push(new stackNode(TypeDeclaration)); // ���鲻ͬ,����ΪTypeDec
		break;
	}
	case 7:
	{
		analyseStack.push(new stackNode(TypeDecList));
		analyseStack.push(new stackNode(TYPE));
		// ����������ʶ�ڵ�, ��ʼ��һ��TypeK, ��ֵ���﷨ջ���ڵ�.
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = TypeK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		// ��cur���ֵ�ָ��Ͷ���ָ�붼ѹ���﷨ջ
		grammarStack.push(& cur->sibling);
		grammarStack.push(& cur->child[0]);
		break;
	}
	case 8:
	{
		// �������������
		analyseStack.push(new stackNode(TypeDecMore));
		analyseStack.push(new stackNode(SEMI));
		analyseStack.push(new stackNode(TypeDef)); // TypeName
		analyseStack.push(new stackNode(EQ));
		analyseStack.push(new stackNode(TypeId));
		// ���������ڵ�
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = DecK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		// ѹ��cur���ֵܽڵ�
		grammarStack.push(& cur->sibling);
		break;
	}
	case 9:
	{
		// ��ʱ�﷨��ջ����ŵ������һ�����������ڵ���ֵܽڵ㣬����
		grammarStack.pop();
		break;
	}
	case 10:
	{
		analyseStack.push(new stackNode(TypeDecList));
		break;
	}
	case 11:
	{
		analyseStack.push(new stackNode(ID));
		// ��ID���ָ�����ǰ�ڵ��name[nameNum];
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	}
	case 12:
	{
		analyseStack.push(new stackNode(BaseType));
		// ������������͵�����
		// �ñ��� tmp ��¼�ڵ�����д��ʶ��������Ϣ�Ĳ��ֵĵ�ַ
		// ������Ĳ���ʽ����� temp ������ݽ��и�ֵ
		tmp = &(cur->kind.dec);
		break;
	}
	case 13:
	{
		analyseStack.push(new stackNode(StructureType));
		break;
	}
	case 14: // ��ͬ���� ???
	{
		analyseStack.push(new stackNode(ID));
		// ��ID���ָ�����ǰ�ڵ��typeName;
		cur->kind.dec = DecIdK; // ???
		cur->typeName = curTokenLL[tokenno].content;
		break;
	}
	case 15:
	{
		analyseStack.push(new stackNode(INTEGER));
		// ��������Ϊinteger ����tmp���ڵĵط�����ֵ
		(*tmp) = IntegerK;
		break;
	}
	case 16:
	{
		analyseStack.push(new stackNode(CHAR1));
		// ��������Ϊchar ����tmp���ڵĵط�����ֵ
		(*tmp) = CharK;
		break;
	}
	case 17:
	{
		analyseStack.push(new stackNode(ArrayType));
		break;
	}
	case 18:
	{
		analyseStack.push(new stackNode(RecType));
		break;
	}
	case 19:
	{
		analyseStack.push(new stackNode(BaseType));
		analyseStack.push(new stackNode(OF));
		analyseStack.push(new stackNode(RMIDPAREN));
		analyseStack.push(new stackNode(Top));
		analyseStack.push(new stackNode(UNDERANGE));
		analyseStack.push(new stackNode(Low));
		analyseStack.push(new stackNode(LMIDPAREN));
		analyseStack.push(new stackNode(ARRAY));
		// ��������Ϊ��������
		cur->kind.dec = ArrayK;
		tmp = &(cur->Attr.arrayAttr.arrayKind); // ������Ӧ��д����Ļ�������
		break;
	}
	case 20:
		analyseStack.push(new stackNode(INTC));
		// �����½�
		cur->Attr.arrayAttr.low = stoi(curTokenLL[tokenno].content);
		break;
	case 21:
		analyseStack.push(new stackNode(INTC));
		// �����Ͻ�
		cur->Attr.arrayAttr.up = stoi(curTokenLL[tokenno].content);
		break;
	case 22:
	{
		analyseStack.push(new stackNode(END1));
		analyseStack.push(new stackNode(FieldDecList));
		analyseStack.push(new stackNode(RECORD));
		// ��������Ϊ��¼����
		cur->kind.dec = RecordK;
		saveP = cur;
		grammarStack.push(& cur->child[0]);
		break;
	}
	case 23:
	{
		analyseStack.push(new stackNode(FieldDecMore));
		analyseStack.push(new stackNode(SEMI));
		analyseStack.push(new stackNode(IdList));
		analyseStack.push(new stackNode(BaseType));
		// ���ɼ�¼���͵�һ����
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = DecK;
		cur->linenum = curTokenLL[tokenno].linenum;
		tmp = &(cur->kind.dec);
		*t = cur;
		grammarStack.push(& cur->sibling);
		break;
	}
	case 24:
	{
		analyseStack.push(new stackNode(FieldDecMore));
		analyseStack.push(new stackNode(SEMI));
		analyseStack.push(new stackNode(IdList));
		analyseStack.push(new stackNode(ArrayType));
		// ���ɼ�¼���͵�һ���򣬽ڵ�Ϊ�����ڵ㣬��������������
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = DecK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		grammarStack.push(& cur->sibling);
		break;
	}
	case 25:
		// û�м�¼���͵���һ������,���� saveP �ָ���ǰ��¼���ͽڵ��ָ��
		cur = saveP;
		break;
	case 26:
		analyseStack.push(new stackNode(FieldDecList));
		break;
	case 27:
		analyseStack.push(new stackNode(IdMore));
		analyseStack.push(new stackNode(ID));
		// ����ǰ��ʶ�������֣�����cur
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	case 28:
		break;
	case 29:
		analyseStack.push(new stackNode(IdList));
		analyseStack.push(new stackNode(COMMA));
		break;
	case 30:
		// ѡ���������ʽ����ʾ����û�б�������

		break;
	case 31:
		analyseStack.push(new stackNode(VarDeclaration)); // ���鲻ͬ
		break;
	case 32:
	{
		analyseStack.push(new stackNode(VarDecList));
		analyseStack.push(new stackNode(VAR));
		// �����������
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = VarK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		grammarStack.push(& cur->sibling);
		grammarStack.push(& cur->child[0]);
		break;
	}
	case 33:
	{
		// �������ı�������
		analyseStack.push(new stackNode(VarDecMore));
		analyseStack.push(new stackNode(SEMI));
		analyseStack.push(new stackNode(VarIdList));
		analyseStack.push(new stackNode(TypeDef));
		// ����һ���������ͽڵ�
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = DecK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		grammarStack.push(& cur->sibling);
		break;
	}
	case 34:
		// ����û�б���������,����ջ��ָ����һ������������ָ��
		grammarStack.pop();
		break;
	case 35:
		analyseStack.push(new stackNode(VarDecList));
		break;
	case 36:
		analyseStack.push(new stackNode(VarIdMore));
		analyseStack.push(new stackNode(ID));
		// �������������ɸ���ʶ��
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	case 37:
		break;
	case 38:
		analyseStack.push(new stackNode(VarIdList));
		analyseStack.push(new stackNode(COMMA));
		break;
	case 39:
		//???
		grammarStack.pop();
		break;
	case 40:
		analyseStack.push(new stackNode(ProcDeclaration)); 
		break;
	case 41:
	{
		// �����������
		analyseStack.push(new stackNode(ProcDecMore));
		analyseStack.push(new stackNode(ProcBody));
		analyseStack.push(new stackNode(ProcDecPart));
		analyseStack.push(new stackNode(SEMI));
		// analyseStack.push(new stackNode(BaseType)); // ����֤ ...

		analyseStack.push(new stackNode(RPAREN));
		analyseStack.push(new stackNode(ParamList));
		analyseStack.push(new stackNode(LPAREN));
		analyseStack.push(new stackNode(ProcName));
		analyseStack.push(new stackNode(PROCEDURE));
		// ���ɹ���ͷ��� ProcDecK, �ֵ�������������ջ
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = ProcDecK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;

		grammarStack.push(& cur->sibling);
		grammarStack.push(& cur->child[2]);
		grammarStack.push(& cur->child[1]);
		grammarStack.push(& cur->child[0]);
		break;
	}
	case 42:
		break;
	case 43:
		analyseStack.push(new stackNode(ProcDeclaration));
		break;
	case 44:
		analyseStack.push(new stackNode(ID));
		// ��������д��
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	case 45:
		// �ββ���Ϊ��, ����ָ���ββ��ֵ�ָ���ַ
		grammarStack.pop();
		break;
	case 46:
		analyseStack.push(new stackNode(ParamDecList));
		break;
	case 47:
		analyseStack.push(new stackNode(ParamMore));
		analyseStack.push(new stackNode(Param));
		break;
	case 48:
		// �ββ��ִ������, �������һ���β��������ֵܽڵ�
		grammarStack.pop();
		// cur = saveP;
		// tmp = 
		// ����֤, ���鲻ͬ
		break;
	case 49:
		analyseStack.push(new stackNode(ParamDecList));
		analyseStack.push(new stackNode(SEMI));
		break;
	case 50:
	{
		analyseStack.push(new stackNode(FormList));
		analyseStack.push(new stackNode(TypeDef));
		// �����β����� , ֵ��
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = DecK;
		cur->linenum = curTokenLL[tokenno].linenum;
		cur->Attr.paramAttr.paramType = valparamType;
		*t = cur;

		grammarStack.push(& cur->sibling);
		break;
	}
	case 51:
	{
		analyseStack.push(new stackNode(FormList));
		analyseStack.push(new stackNode(TypeDef));
		analyseStack.push(new stackNode(VAR));
		// ���
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = DecK;
		cur->linenum = curTokenLL[tokenno].linenum;
		cur->Attr.paramAttr.paramType = varparamType;
		*t = cur;

		grammarStack.push(& cur->sibling);
		break;
	}
	case 52:
		analyseStack.push(new stackNode(FidMore));
		analyseStack.push(new stackNode(ID));
		// ����ʶ��������д�뵱ǰ�﷨���ڵ�
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	case 53:
		break;
	case 54:
		analyseStack.push(new stackNode(FormList));
		analyseStack.push(new stackNode(COMMA));
		break;
	case 55:
		analyseStack.push(new stackNode(DeclarePart));
		break;
	case 56:
		analyseStack.push(new stackNode(ProgramBody));
		break;
	case 57:
	{
		analyseStack.push(new stackNode(END1));
		analyseStack.push(new stackNode(StmList));
		analyseStack.push(new stackNode(BEGIN1));
		// ����������б�ʶ�ڵ�
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = StmLK; // ����֤
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		grammarStack.push(& cur->child[0]);
		break;
	}
	case 58:
		analyseStack.push(new stackNode(StmMore));
		analyseStack.push(new stackNode(Stm));
		break;
	case 59:
		// ����û���������, ��ջ����ŵ����һ�������ֵܽڵ㵯��
		grammarStack.pop();
		break;
	case 60:
		analyseStack.push(new stackNode(StmList));
		analyseStack.push(new stackNode(SEMI));
		break;
	case 61:
	{
		analyseStack.push(new stackNode(ConditionalStm));
		// ����һ��������ͽڵ�, ����ΪIfK
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = StmtK; 
		cur->kind.stmt = IfK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		grammarStack.push(& cur->sibling);
		break;
	}
	case 62:
	{
		analyseStack.push(new stackNode(LoopStm));
		// ����һ��������ͽڵ�, ����ΪWhileK
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = StmtK; 
		cur->kind.stmt = WhileK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		grammarStack.push(& cur->sibling);
		break;
	}
	case 63:
	{
		analyseStack.push(new stackNode(InputStm));
		// ����һ��������ͽڵ�, ����ΪReadK
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = StmtK;  // ����֤
		cur->kind.stmt = ReadK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		grammarStack.push(& cur->sibling);
		break;
	}
	case 64: // write ��䴦��
	{
		analyseStack.push(new stackNode(OutputStm));
		// ����һ��������ͽڵ�, ����ΪWriteK
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = StmtK; 
		cur->kind.stmt = WriteK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		//grammarStack.push(&cur->child[0]);
		grammarStack.push(& cur->sibling);
		break;
	}
	case 65:
	{
		analyseStack.push(new stackNode(ReturnStm));
		// ����һ��������ͽڵ�, ����ΪReturnK
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = StmtK;  // ����֤
		cur->kind.stmt = ReturnK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		grammarStack.push(& cur->sibling);
		break;
	}
	case 66:
	{
		analyseStack.push(new stackNode(AssCall));
		analyseStack.push(new stackNode(ID));
		// ���鲻ͬ
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		// ���˴��� ��??? 
		// ����һ��������ͽڵ�, ����ȷ�������ľ������� (����ΪAssignK
		cur = new grammarTreeNode();
		cur->nodeKind = StmtK;  // ����֤
		// cur->kind.stmt = AssignK;
		cur->linenum = curTokenLL[tokenno].linenum;
		// ������ֵ����󲿱����ڵ�, ��Ϊcur�Ķ���0
		grammarTreeNode* left = new grammarTreeNode();
		left->nodeKind = ExpK;
		left->kind.exp = ExpIdK;
		left->Attr.expAttr.varKind = IdV;
		left->linenum = curTokenLL[tokenno].linenum;
		left->name[left->nameNum++] = curTokenLL[tokenno].content;
		cur->child[0] = left;
		*t = cur;
		grammarStack.push(&(cur->sibling));
		break;
	}
	case 67:
		analyseStack.push(new stackNode(AssignmentRest));
		// ���ľ�������ֵȷ��ΪAssignK
		cur->kind.stmt = AssignK;
		break;
	case 68:
		analyseStack.push(new stackNode(CallStmRest));
		// ���ľ�������ֵȷ��ΪCallK
		cur->kind.stmt = CallK;
		// cur->child[0]->Attr.expAttr.varKind = IdV; // ����66���
		break;
	case 69:  // ����֤
	{
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(ASSIGN));
		analyseStack.push(new stackNode(VariMore));
		// ��ֵ�Ҳ��ӽڵ���ջ
		grammarStack.push(& cur->child[1]);
		cur = cur->child[0]; // curָ����
		// OPջ�� ѹ��һ�������־,���ȼ����
		grammarTreeNode* t = new grammarTreeNode();
		t->nodeKind = ExpK;
		t->kind.exp = OpK;
		t->linenum = curTokenLL[tokenno].linenum;
		t->Attr.expAttr.op = END1;
		opStack.push(t);
		break;
	}
	case 70:
	{
		analyseStack.push(new stackNode(FI));
		analyseStack.push(new stackNode(StmList));
		analyseStack.push(new stackNode(ELSE));
		analyseStack.push(new stackNode(StmList));
		analyseStack.push(new stackNode(THEN));
		analyseStack.push(new stackNode(RelExp));
		analyseStack.push(new stackNode(IF));
		// �������ʽ, then����, else���� �ֱ��Ӧ����������
		grammarStack.push(& cur->child[2]);
		grammarStack.push(& cur->child[1]);
		grammarStack.push(& cur->child[0]);
	}
	break;
	case 71:
	{
		analyseStack.push(new stackNode(ENDWH));
		analyseStack.push(new stackNode(StmList));
		analyseStack.push(new stackNode(DO));
		analyseStack.push(new stackNode(RelExp));
		analyseStack.push(new stackNode(WHILE));
		// �������ʽ, ѭ����
		grammarStack.push(& cur->child[1]);
		grammarStack.push(& cur->child[0]);
		break;
	}
	case 72:
	{
		analyseStack.push(new stackNode(RPAREN));
		analyseStack.push(new stackNode(InVar));
		analyseStack.push(new stackNode(LPAREN));
		analyseStack.push(new stackNode(READ));
		break;
	}
	case 73:
		analyseStack.push(new stackNode(ID));
		// ����ʶ�����ִ���
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	case 74:   // write(a)
	{
		analyseStack.push(new stackNode(RPAREN));
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(LPAREN));
		analyseStack.push(new stackNode(WRITE));
		// ��һ�����ӽڵ���ջ
		grammarStack.push(& cur->child[0]);
		// OPջ�� ѹ��һ�������־,���ȼ����
		grammarTreeNode* t = new grammarTreeNode();
		t->nodeKind = ExpK;
		t->kind.exp = OpK;
		t->linenum = curTokenLL[tokenno].linenum;
		t->Attr.expAttr.op = END1;
		opStack.push(t);
		break;
	}
	case 75:
		analyseStack.push(new stackNode(RETURN1));
		break;
	case 76:
	{
		analyseStack.push(new stackNode(RPAREN));
		analyseStack.push(new stackNode(ActParamList));
		analyseStack.push(new stackNode(LPAREN));
		// ��ѹ�뺯���������ĵڶ������ӽڵ�  ��һ�� or �ڶ���??? ����֤
		grammarStack.push(& cur->child[1]);
		break;
	}
	case 77:
		// ʵ�β���Ϊ��, ��ָ��ʵ�β��ֵ�ָ����﷨��ջ�е�����
		grammarStack.pop();
		break;
	case 78:
	{
		// ʵ���Ǳ��ʽ
		analyseStack.push(new stackNode(ActParamMore));
		analyseStack.push(new stackNode(Exp));
		// ��ʼ��������ջ������һ����������ȼ���͵Ĳ�����
		grammarTreeNode* t = new grammarTreeNode();
		t->nodeKind = ExpK;
		t->kind.exp = OpK;
		t->linenum = curTokenLL[tokenno].linenum;
		t->Attr.expAttr.op = END1;
		opStack.push(t);
		break;
	}
	case 79:
		// û��������ʵ�Σ�����������䴦�����
		break;
	case 80:
		analyseStack.push(new stackNode(ActParamList));
		analyseStack.push(new stackNode(COMMA));
		// ���б��ʵ��
		grammarStack.push(& cur->sibling);
		break;
	case 81:
	{
		// �����������ʽ
		analyseStack.push(new stackNode(OtherRelE));
		analyseStack.push(new stackNode(Exp));

		getExpResult = false;
		// ��ʼ��������ջ������һ����������ȼ���͵Ĳ�����
		grammarTreeNode* t = new grammarTreeNode();
		t->nodeKind = ExpK;
		t->kind.exp = OpK;
		t->linenum = curTokenLL[tokenno].linenum;
		t->Attr.expAttr.op = END1;
		opStack.push(t);
		break;
	}
	case 82:
	{
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(CmpOp));
		// ����һ�����������ͱ��ʽ�ڵ�
		grammarTreeNode* t0 = new grammarTreeNode();
		t0->nodeKind = ExpK;
		t0->kind.exp = OpK;
		t0->linenum = curTokenLL[tokenno].linenum;
		t0->Attr.expAttr.op = curTokenLL[tokenno].type_inf;

		LexType opTop = opStack.top()->Attr.expAttr.op;
		while (priority(opTop) >= priority(curTokenLL[tokenno].type_inf)) {
			grammarTreeNode* t = opStack.top();
			opStack.pop();
			//grammarTreeNode* left = numberStack.top(); numberStack.pop();
			//grammarTreeNode* right = numberStack.top(); numberStack.pop();
			grammarTreeNode* right = numberStack.top(); numberStack.pop();
			grammarTreeNode* left = numberStack.top(); numberStack.pop();

			t->child[0] = left;
			t->child[1] = right;
			numberStack.push(t);
			opTop = opStack.top()->Attr.expAttr.op;
		}
		// ��curѹ��opջ
		opStack.push(t0);

		getExpResult = true;
		break;
	}
	case 83:
		analyseStack.push(new stackNode(OtherTerm));
		analyseStack.push(new stackNode(Term));
		break;
	case 84:
	{
		// ���ʽ�Ľ���, ������������Ҫ���������
		// �����ǰ��������Ϊ������
		if (curTokenLL[tokenno].type_inf == RPAREN && expFlag != 0) {
			while (opStack.top()->Attr.expAttr.op != LPAREN) {
				grammarTreeNode* t = opStack.top();
				opStack.pop();
				//grammarTreeNode* left = numberStack.top(); numberStack.pop();
				//grammarTreeNode* right = numberStack.top(); numberStack.pop();
				grammarTreeNode* right = numberStack.top(); numberStack.pop();
				grammarTreeNode* left = numberStack.top(); numberStack.pop();

				t->child[0] = left;
				t->child[1] = right;
				numberStack.push(t);
			}
			opStack.pop();
			expFlag -= 1;
		}
		else {
			if (getExpResult || getExpResult2) {
				while (opStack.top()->Attr.expAttr.op != END1) {
					grammarTreeNode* t = opStack.top();
					opStack.pop();
					//grammarTreeNode* left = numberStack.top(); numberStack.pop();
					//grammarTreeNode* right = numberStack.top(); numberStack.pop();
					grammarTreeNode* right = numberStack.top(); numberStack.pop();
					grammarTreeNode* left = numberStack.top(); numberStack.pop();

					t->child[0] = left;
					t->child[1] = right;
					numberStack.push(t);
				}
				opStack.pop();
				grammarTreeNode** t = grammarStack.top();
				grammarStack.pop();
				cur = numberStack.top();
				numberStack.pop();
				*t = cur;
			}
			// ������һ�������±���ʽʱ,����1  �ƺ�û��ʲô�ر��������..???
			if (getExpResult2 == true)getExpResult2 = false;
		}
		break;
	}
	case 85:
	{
		// �����ӷ������
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(AddOp));
		// ����һ�����ʽ�ڵ�
		grammarTreeNode* t0 = new grammarTreeNode();
		t0->nodeKind = ExpK;
		t0->kind.exp = OpK;
		t0->linenum = curTokenLL[tokenno].linenum;
		t0->Attr.expAttr.op = curTokenLL[tokenno].type_inf;

		LexType opTop = opStack.top()->Attr.expAttr.op;
		while (priority(opTop) >= priority(curTokenLL[tokenno].type_inf)) {
			grammarTreeNode* t = opStack.top();
			opStack.pop();
			//grammarTreeNode* left = numberStack.top(); numberStack.pop();
			//grammarTreeNode* right = numberStack.top(); numberStack.pop();
			grammarTreeNode* right = numberStack.top(); numberStack.pop();
			grammarTreeNode* left = numberStack.top(); numberStack.pop();

			t->child[0] = left;
			t->child[1] = right;
			numberStack.push(t);
			opTop = opStack.top()->Attr.expAttr.op;
		}
		opStack.push(t0);
		break;
	}
	case 86:
		analyseStack.push(new stackNode(OtherFactor));
		analyseStack.push(new stackNode(Factor));
		break;
	case 87:
		break;
	case 88:
	{
		analyseStack.push(new stackNode(Term));
		analyseStack.push(new stackNode(MultOp));
		// ����һ�����ʽ�ڵ� ͬ82
		grammarTreeNode* t0 = new grammarTreeNode();
		t0->nodeKind = ExpK;
		t0->kind.exp = OpK;
		t0->linenum = curTokenLL[tokenno].linenum;
		t0->Attr.expAttr.op = curTokenLL[tokenno].type_inf;

		LexType opTop = opStack.top()->Attr.expAttr.op;
		while (priority(opTop) >= priority(curTokenLL[tokenno].type_inf)) {
			grammarTreeNode* t = opStack.top();
			opStack.pop();
			//grammarTreeNode* left = numberStack.top(); numberStack.pop();
			//grammarTreeNode* right = numberStack.top(); numberStack.pop();
			grammarTreeNode* right = numberStack.top(); numberStack.pop();
			grammarTreeNode* left = numberStack.top(); numberStack.pop();

			t->child[0] = left;
			t->child[1] = right;
			numberStack.push(t);
			opTop = opStack.top()->Attr.expAttr.op;
		}
		opStack.push(t0);
		break;
	}
	case 89:
	{
		analyseStack.push(new stackNode(RPAREN));
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(LPAREN));
		// ��ǰ����Ϊ������
		grammarTreeNode* t = new grammarTreeNode();
		t->nodeKind = ExpK;
		t->kind.exp = OpK;
		t->linenum = curTokenLL[tokenno].linenum;
		t->Attr.expAttr.op = curTokenLL[tokenno].type_inf;

		opStack.push(t);
		expFlag += 1;
		break;
	}
	case 90:
	{
		analyseStack.push(new stackNode(INTC));
		// ����һ�����������ʽ�ڵ�, ��������Ϊ�������ʽ
		cur = new grammarTreeNode();
		cur->nodeKind = ExpK;
		cur->kind.exp = ConstK;
		cur->linenum = curTokenLL[tokenno].linenum;
		cur->Attr.expAttr.val = stoi(curTokenLL[tokenno].content);
		numberStack.push(cur);
		break;
	}
	case 91:
		analyseStack.push(new stackNode(Variable));
		break;
	case 92:
	{
		analyseStack.push(new stackNode(VariMore));
		analyseStack.push(new stackNode(ID));
		// ����һ�������ı��ʽ�ڵ�
		cur = new grammarTreeNode();
		cur->nodeKind = ExpK;
		cur->kind.exp = ExpIdK;
		cur->linenum = curTokenLL[tokenno].linenum;
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		numberStack.push(cur);
		break;
	}
	case 93:
		// �������ʽ�ľ��������Ǳ�־������
		cur->Attr.expAttr.varKind = IdV;
		break;
	case 94:
	{
		analyseStack.push(new stackNode(RMIDPAREN));
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(LMIDPAREN));
		// �����Ա����
		cur->Attr.expAttr.varKind = ArrayMembV;
		grammarStack.push(& cur->child[0]);
		// OPջ�� ѹ��һ�������־,���ȼ����
		grammarTreeNode* t = new grammarTreeNode();
		t->nodeKind = ExpK;
		t->kind.exp = OpK;
		t->linenum = curTokenLL[tokenno].linenum;
		t->Attr.expAttr.op = END1;
		opStack.push(t);
		// �����±���ʽ����
		getExpResult2 = true;
		break;
	}
	case 95:
		analyseStack.push(new stackNode(FieldVar));
		analyseStack.push(new stackNode(DOT));
		// ���Ա���ͱ���
		cur->Attr.expAttr.varKind = FieldMembV;
		grammarStack.push(& cur->child[0]);
		break;
	case 96:
	{
		analyseStack.push(new stackNode(FieldVarMore));
		analyseStack.push(new stackNode(ID));
		// ����һ�������ı��ʽ�ڵ�, ��¼��ĳ�Ա
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = ExpK;
		cur->kind.exp = ExpIdK;
		cur->linenum = curTokenLL[tokenno].linenum;
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		*t = cur;
		break;
	}
	case 97:
		// �������ʽ�ľ��������Ǳ�־������
		cur->Attr.expAttr.varKind = IdV;
		break;
	case 98:
	{
		analyseStack.push(new stackNode(RMIDPAREN));
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(LMIDPAREN));
		// ͬ 94 �Ĵ��� 
		cur->Attr.expAttr.varKind = ArrayMembV;
		grammarStack.push(& cur->child[0]);
		// OPջ�� ѹ��һ�������־,���ȼ����
		grammarTreeNode* t = new grammarTreeNode();
		t->nodeKind = ExpK;
		t->kind.exp = OpK;
		t->linenum = curTokenLL[tokenno].linenum;
		t->Attr.expAttr.op = END1;
		opStack.push(t);
		// �����±���ʽ����
		getExpResult2 = true;
		break;
	}
	case 99:
		analyseStack.push(new stackNode(LT));
		break;
	case 100:
		analyseStack.push(new stackNode(EQ));
		break;
	case 101:
		analyseStack.push(new stackNode(PLUS));
		break;
	case 102:
		analyseStack.push(new stackNode(MINUS));
		break;
	case 103:
		analyseStack.push(new stackNode(TIMES));
		break;
	case 104:
		analyseStack.push(new stackNode(OVER));
		break;
	case 0:
	default: {
		cout << "��" << lineno << "��,�﷨����:unexpected  token." << endl;
		cout << "predict num : " << num << endl;
		//exit(0);
		break;
	}
	}
}

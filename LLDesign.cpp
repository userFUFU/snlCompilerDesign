#pragma once
#include<bits/stdc++.h>
#include "lexicalDesign.h"
#include "grammarDesign.h"
#include "LLDesign.h"
using namespace std;
// 符号表
int LLTable[LL1TableSize][LL1TableSize];
// 符号栈,语法栈,操作符栈,操作数栈
stack<stackNode*> analyseStack;
stack<grammarTreeNode**> grammarStack;
stack<grammarTreeNode*> opStack;
stack<grammarTreeNode*> numberStack;

// 声明
grammarTreeNode* LLroot = NULL;
int num = 0; // 选中的产生式编号
bool flag = true; // 栈空标志
int lineno=1,tokenno=1; // 行号,当前token号
Token* curTokenLL; // token
bool getExpResult = 1, getExpResult2; // getExpResult2 专用于数组成员变量
int expFlag; // 用于括号匹配



grammarTreeNode* LLmain(Token token[], int num) {
	curTokenLL = token;
	// 初始化LL(1)符号表
	for (int i = 0; i < LL1TableSize; i++) 
		for (int j = 0; j < LL1TableSize; j++)
			LLTable[i][j] = 0;
	// 创建符号表
	create_LL1table();
	// 形成LL(1)语法树
	tokenno = 1;
	// 创建语法树根节点ProK
	grammarTreeNode* root = new grammarTreeNode();
	root->nodeKind = ProK;
	// 将三个儿子指针压入语法树栈
	grammarStack.push(&(root->child[2]));
	grammarStack.push(&(root->child[1]));
	grammarStack.push(&(root->child[0]));

	analyseStack.push(new stackNode(Program));

	while (!analyseStack.empty()) {
		lineno = token[tokenno].linenum;
		stackNode* now = analyseStack.top();
		analyseStack.pop();
		if (now->isTmlWord == 1) { // 栈顶为终极符
			if (tokenno<num && token[tokenno].type_inf == now->content.tWord) {
				//cout <<  "Match!!!  当前token:" << token[tokenno].type_inf << "  " << token[tokenno].content << " ; 当前符号栈顶(终极符):" << now->content.tWord << endl;
				tokenno++;
			}
			else { // 匹配不成功
				cout << "第" << token[tokenno].linenum << "行,语法错误:unexpected  token." << endl;
				cout <<"当前token:" <<token[tokenno].type_inf<<"  " << token[tokenno].content << " ; 当前符号栈顶(终极符):" << now->content.tWord <<endl;
				//printGrammarTree(0, root);
				//exit(0);
			}
		}
		else { // 栈顶为非终极符
			int PFnum = LLTable[now->content.ntWord][token[tokenno].type_inf]; // 产生式编号
			//cout <<"predict num :"<<PFnum<< " ; 当前token:" << token[tokenno].type_inf << "  " << token[tokenno].content << " ; 当前符号栈顶(非终极符):" << now->content.tWord << endl;
			predict(PFnum);
		}
	}
	if (token[tokenno].type_inf != ENDFILE) { // 栈已空，输入流不空
		cout << "EOF error!" << endl;
		//exit(0);
	}
	//cout << "no error! print LL(1) Tree!" << endl;
	// printGrammarTree(0, root);
	return root;
}


// 创建LL(1)符号表
// 可自动生成,,,再说吧,,,
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


// 设定操作符优先级
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
		cout << curTokenLL[tokenno].linenum << "行, operator not match!"<<endl;
	}
	return p;
}

grammarTreeNode* cur = NULL; // 当前节点
grammarTreeNode* saveP = NULL; // 用于保存当前节点
DecKind* tmp = NULL; // 暂时记录标识符类型信息的部分的地址

// 传入产生式编号，执行相应动作
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
		// 程序头节点, 初始化一个PheadK, 赋值给语法栈顶节点
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
		// 将ID名字赋给当前节点的name[nameNum];
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	}
	case 4:
	{
		// 与书不同
		analyseStack.push(new stackNode(ProcDec));
		analyseStack.push(new stackNode(VarDec));
		analyseStack.push(new stackNode(TypeDec));
		break;
	}
	case 5:
		break; // 空
	case 6:
	{
		analyseStack.push(new stackNode(TypeDeclaration)); // 与书不同,书中为TypeDec
		break;
	}
	case 7:
	{
		analyseStack.push(new stackNode(TypeDecList));
		analyseStack.push(new stackNode(TYPE));
		// 类型声明标识节点, 初始化一个TypeK, 赋值给语法栈顶节点.
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = TypeK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		// 将cur的兄弟指针和儿子指针都压入语法栈
		grammarStack.push(& cur->sibling);
		grammarStack.push(& cur->child[0]);
		break;
	}
	case 8:
	{
		// 具体的类型声明
		analyseStack.push(new stackNode(TypeDecMore));
		analyseStack.push(new stackNode(SEMI));
		analyseStack.push(new stackNode(TypeDef)); // TypeName
		analyseStack.push(new stackNode(EQ));
		analyseStack.push(new stackNode(TypeId));
		// 类型声明节点
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = DecK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		// 压入cur的兄弟节点
		grammarStack.push(& cur->sibling);
		break;
	}
	case 9:
	{
		// 这时语法树栈顶存放的是最后一个类型声明节点的兄弟节点，弹出
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
		// 将ID名字赋给当前节点的name[nameNum];
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	}
	case 12:
	{
		analyseStack.push(new stackNode(BaseType));
		// 由于数组基类型的问题
		// 用变量 tmp 记录节点上填写标识符类型信息的部分的地址
		// 在下面的产生式处理对 temp 里的内容进行赋值
		tmp = &(cur->kind.dec);
		break;
	}
	case 13:
	{
		analyseStack.push(new stackNode(StructureType));
		break;
	}
	case 14: // 不同于书 ???
	{
		analyseStack.push(new stackNode(ID));
		// 将ID名字赋给当前节点的typeName;
		cur->kind.dec = DecIdK; // ???
		cur->typeName = curTokenLL[tokenno].content;
		break;
	}
	case 15:
	{
		analyseStack.push(new stackNode(INTEGER));
		// 声明部分为integer 即给tmp所在的地方回填值
		(*tmp) = IntegerK;
		break;
	}
	case 16:
	{
		analyseStack.push(new stackNode(CHAR1));
		// 声明部分为char 即给tmp所在的地方回填值
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
		// 声明部分为数组类型
		cur->kind.dec = ArrayK;
		tmp = &(cur->Attr.arrayAttr.arrayKind); // 接下来应填写数组的基础类型
		break;
	}
	case 20:
		analyseStack.push(new stackNode(INTC));
		// 数组下界
		cur->Attr.arrayAttr.low = stoi(curTokenLL[tokenno].content);
		break;
	case 21:
		analyseStack.push(new stackNode(INTC));
		// 数组上界
		cur->Attr.arrayAttr.up = stoi(curTokenLL[tokenno].content);
		break;
	case 22:
	{
		analyseStack.push(new stackNode(END1));
		analyseStack.push(new stackNode(FieldDecList));
		analyseStack.push(new stackNode(RECORD));
		// 声明部分为记录类型
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
		// 生成记录类型的一个域
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
		// 生成记录类型的一个域，节点为声明节点，类型是数组类型
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
		// 没有记录类型的下一个域了,利用 saveP 恢复当前记录类型节点的指针
		cur = saveP;
		break;
	case 26:
		analyseStack.push(new stackNode(FieldDecList));
		break;
	case 27:
		analyseStack.push(new stackNode(IdMore));
		analyseStack.push(new stackNode(ID));
		// 将当前标识符的名字，存入cur
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	case 28:
		break;
	case 29:
		analyseStack.push(new stackNode(IdList));
		analyseStack.push(new stackNode(COMMA));
		break;
	case 30:
		// 选择这个产生式，表示程序没有变量声明

		break;
	case 31:
		analyseStack.push(new stackNode(VarDeclaration)); // 与书不同
		break;
	case 32:
	{
		analyseStack.push(new stackNode(VarDecList));
		analyseStack.push(new stackNode(VAR));
		// 处理变量声明
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
		// 进入具体的变量声明
		analyseStack.push(new stackNode(VarDecMore));
		analyseStack.push(new stackNode(SEMI));
		analyseStack.push(new stackNode(VarIdList));
		analyseStack.push(new stackNode(TypeDef));
		// 生成一个声明类型节点
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
		// 后面没有变量声明了,弹出栈中指向下一个变量声明的指针
		grammarStack.pop();
		break;
	case 35:
		analyseStack.push(new stackNode(VarDecList));
		break;
	case 36:
		analyseStack.push(new stackNode(VarIdMore));
		analyseStack.push(new stackNode(ID));
		// 变量声明的若干个标识符
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
		// 处理过程声明
		analyseStack.push(new stackNode(ProcDecMore));
		analyseStack.push(new stackNode(ProcBody));
		analyseStack.push(new stackNode(ProcDecPart));
		analyseStack.push(new stackNode(SEMI));
		// analyseStack.push(new stackNode(BaseType)); // 待验证 ...

		analyseStack.push(new stackNode(RPAREN));
		analyseStack.push(new stackNode(ParamList));
		analyseStack.push(new stackNode(LPAREN));
		analyseStack.push(new stackNode(ProcName));
		analyseStack.push(new stackNode(PROCEDURE));
		// 生成过程头结点 ProcDecK, 兄弟与三个儿子入栈
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
		// 函数名字写入
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	case 45:
		// 形参部分为空, 弹出指向形参部分的指针地址
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
		// 形参部分处理完毕, 弹出最后一个形参声明的兄弟节点
		grammarStack.pop();
		// cur = saveP;
		// tmp = 
		// 待验证, 与书不同
		break;
	case 49:
		analyseStack.push(new stackNode(ParamDecList));
		analyseStack.push(new stackNode(SEMI));
		break;
	case 50:
	{
		analyseStack.push(new stackNode(FormList));
		analyseStack.push(new stackNode(TypeDef));
		// 进入形参声明 , 值参
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
		// 变参
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
		// 将标识符的名字写入当前语法树节点
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
		// 建立语句序列标识节点
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = StmLK; // 待验证
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
		// 后面没有其他语句, 将栈顶存放的最后一条语句的兄弟节点弹出
		grammarStack.pop();
		break;
	case 60:
		analyseStack.push(new stackNode(StmList));
		analyseStack.push(new stackNode(SEMI));
		break;
	case 61:
	{
		analyseStack.push(new stackNode(ConditionalStm));
		// 创建一个语句类型节点, 类型为IfK
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
		// 创建一个语句类型节点, 类型为WhileK
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
		// 创建一个语句类型节点, 类型为ReadK
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = StmtK;  // 待验证
		cur->kind.stmt = ReadK;
		cur->linenum = curTokenLL[tokenno].linenum;
		*t = cur;
		grammarStack.push(& cur->sibling);
		break;
	}
	case 64: // write 语句处理
	{
		analyseStack.push(new stackNode(OutputStm));
		// 创建一个语句类型节点, 类型为WriteK
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
		// 创建一个语句类型节点, 类型为ReturnK
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		cur = new grammarTreeNode();
		cur->nodeKind = StmtK;  // 待验证
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
		// 与书不同
		grammarTreeNode** t = grammarStack.top();
		grammarStack.pop();
		// 无伤大雅 但??? 
		// 创建一个语句类型节点, 不能确定此语句的具体类型 (类型为AssignK
		cur = new grammarTreeNode();
		cur->nodeKind = StmtK;  // 待验证
		// cur->kind.stmt = AssignK;
		cur->linenum = curTokenLL[tokenno].linenum;
		// 创建赋值语句左部变量节点, 作为cur的儿子0
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
		// 语句的具体类型值确定为AssignK
		cur->kind.stmt = AssignK;
		break;
	case 68:
		analyseStack.push(new stackNode(CallStmRest));
		// 语句的具体类型值确定为CallK
		cur->kind.stmt = CallK;
		// cur->child[0]->Attr.expAttr.varKind = IdV; // 已在66完成
		break;
	case 69:  // 待验证
	{
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(ASSIGN));
		analyseStack.push(new stackNode(VariMore));
		// 赋值右部子节点入栈
		grammarStack.push(& cur->child[1]);
		cur = cur->child[0]; // cur指向左部
		// OP栈底 压入一个特殊标志,优先级最低
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
		// 条件表达式, then部分, else部分 分别对应于三个儿子
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
		// 条件表达式, 循环体
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
		// 将标识符名字存入
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		break;
	case 74:   // write(a)
	{
		analyseStack.push(new stackNode(RPAREN));
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(LPAREN));
		analyseStack.push(new stackNode(WRITE));
		// 第一个儿子节点入栈
		grammarStack.push(& cur->child[0]);
		// OP栈底 压入一个特殊标志,优先级最低
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
		// 先压入函数调用语句的第二个儿子节点  第一个 or 第二个??? 待验证
		grammarStack.push(& cur->child[1]);
		break;
	}
	case 77:
		// 实参部分为空, 将指向实参部分的指针从语法树栈中弹出来
		grammarStack.pop();
		break;
	case 78:
	{
		// 实参是表达式
		analyseStack.push(new stackNode(ActParamMore));
		analyseStack.push(new stackNode(Exp));
		// 初始化操作符栈，放入一个特殊的优先级最低的操作符
		grammarTreeNode* t = new grammarTreeNode();
		t->nodeKind = ExpK;
		t->kind.exp = OpK;
		t->linenum = curTokenLL[tokenno].linenum;
		t->Attr.expAttr.op = END1;
		opStack.push(t);
		break;
	}
	case 79:
		// 没有其他的实参，函数调用语句处理结束
		break;
	case 80:
		analyseStack.push(new stackNode(ActParamList));
		analyseStack.push(new stackNode(COMMA));
		// 还有别的实参
		grammarStack.push(& cur->sibling);
		break;
	case 81:
	{
		// 处理条件表达式
		analyseStack.push(new stackNode(OtherRelE));
		analyseStack.push(new stackNode(Exp));

		getExpResult = false;
		// 初始化操作符栈，放入一个特殊的优先级最低的操作符
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
		// 建立一个操作符类型表达式节点
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
		// 将cur压入op栈
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
		// 表达式的结束, 遇到右括号需要分情况处理
		// 如果当前所读单词为右括号
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
			// 遇到下一个数组下标表达式时,再置1  似乎没有什么特别大的意义吧..???
			if (getExpResult2 == true)getExpResult2 = false;
		}
		break;
	}
	case 85:
	{
		// 遇到加法运算符
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(AddOp));
		// 建立一个表达式节点
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
		// 建立一个表达式节点 同82
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
		// 当前单词为左括号
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
		// 创建一个操作数表达式节点, 具体类型为常数表达式
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
		// 创建一个变量的表达式节点
		cur = new grammarTreeNode();
		cur->nodeKind = ExpK;
		cur->kind.exp = ExpIdK;
		cur->linenum = curTokenLL[tokenno].linenum;
		cur->name[cur->nameNum++] = curTokenLL[tokenno].content;
		numberStack.push(cur);
		break;
	}
	case 93:
		// 变量表达式的具体类型是标志符变量
		cur->Attr.expAttr.varKind = IdV;
		break;
	case 94:
	{
		analyseStack.push(new stackNode(RMIDPAREN));
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(LMIDPAREN));
		// 数组成员变量
		cur->Attr.expAttr.varKind = ArrayMembV;
		grammarStack.push(& cur->child[0]);
		// OP栈底 压入一个特殊标志,优先级最低
		grammarTreeNode* t = new grammarTreeNode();
		t->nodeKind = ExpK;
		t->kind.exp = OpK;
		t->linenum = curTokenLL[tokenno].linenum;
		t->Attr.expAttr.op = END1;
		opStack.push(t);
		// 数组下标表达式处理
		getExpResult2 = true;
		break;
	}
	case 95:
		analyseStack.push(new stackNode(FieldVar));
		analyseStack.push(new stackNode(DOT));
		// 域成员类型变量
		cur->Attr.expAttr.varKind = FieldMembV;
		grammarStack.push(& cur->child[0]);
		break;
	case 96:
	{
		analyseStack.push(new stackNode(FieldVarMore));
		analyseStack.push(new stackNode(ID));
		// 创建一个变量的表达式节点, 记录域的成员
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
		// 变量表达式的具体类型是标志符变量
		cur->Attr.expAttr.varKind = IdV;
		break;
	case 98:
	{
		analyseStack.push(new stackNode(RMIDPAREN));
		analyseStack.push(new stackNode(Exp));
		analyseStack.push(new stackNode(LMIDPAREN));
		// 同 94 的处理。 
		cur->Attr.expAttr.varKind = ArrayMembV;
		grammarStack.push(& cur->child[0]);
		// OP栈底 压入一个特殊标志,优先级最低
		grammarTreeNode* t = new grammarTreeNode();
		t->nodeKind = ExpK;
		t->kind.exp = OpK;
		t->linenum = curTokenLL[tokenno].linenum;
		t->Attr.expAttr.op = END1;
		opStack.push(t);
		// 数组下标表达式处理
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
		cout << "第" << lineno << "行,语法错误:unexpected  token." << endl;
		cout << "predict num : " << num << endl;
		//exit(0);
		break;
	}
	}
}

#include<bits/stdc++.h>
#include "lexicalDesign.h"
#include "grammarDesign.h"
#include "RDDesign.h"
using namespace std;

Token* curToken;
int tokenNum; //token从下标1开始


grammarTreeNode* RDmain(Token token[], int num) {
	curToken = token;
	tokenNum = 1;
	grammarTreeNode* t = RDProgram();
	Match(ENDFILE);
	return t;
}
void Match(LexType lex) {
	if (curToken[tokenNum].type_inf == lex) {
		tokenNum++;
	}
	
	else {
		cout << curToken[tokenNum].linenum<<" "<< curToken[tokenNum].content<< " error! Not Match!" << endl;
		//exit(0);
		// throw new exception("Match Error at line " + curToken[tokenNum].linenum);
	}
}

// grammarTreeNode* curt;

// program ::= programHead declarePart programBody .
// 创建程序根节点
grammarTreeNode* RDProgram() {
	grammarTreeNode* root = new grammarTreeNode(ProK, 0);
	
	root->child[0] = RDProgramHead();
	root->child[1] = RDDeclarePart();
	root->child[2] = RDProgramBody();
	Match(DOT);
	cout << "no error! print RD Tree!" << endl;
	return root;
}

// programHead ::= PROGRAM  ProgramName
// 创建程序头节点
grammarTreeNode* RDProgramHead() {
	grammarTreeNode* ph = new grammarTreeNode(PheadK,curToken[tokenNum].linenum);

	Match(PROGRAM);
	if (curToken[tokenNum].type_inf == ID) {
		ph->name[ph->nameNum++] = curToken[tokenNum].content;
	}
	Match(ID);
	return ph;
}

// declarePart ::= typeDec  varDec  procDec 
// 函数声明节点  Type Var Func
grammarTreeNode* RDDeclarePart() {
	grammarTreeNode* typeP = new grammarTreeNode(TypeK, curToken[tokenNum].linenum);
	
	grammarTreeNode* pp = typeP;

	grammarTreeNode* tp1 = RDTypeDec();
	typeP->child[0] = tp1;

	grammarTreeNode* varP = new grammarTreeNode(VarK, curToken[tokenNum].linenum);

	grammarTreeNode* tp2 = RDVarDec();
	varP->child[0] = tp2;

	grammarTreeNode* s = RDProcDec();

	// typeP varP s
	if (tp2 == NULL) varP = s;
	else varP->sibling = s;

	if (tp1 != NULL) {
		pp = typeP;
		pp->sibling = varP;
	}
	else {
		pp = varP;
	}
	return pp;
}

// typeDec ::= ε | TypeDeclaration
// Type声明部分处理, 判断是否是TYPE
grammarTreeNode* RDTypeDec() {
	grammarTreeNode* t = NULL;
	if (curToken[tokenNum].type_inf == TYPE) { 
		t = RDTypeDeclaration();
		return t;
	}
	else if (curToken[tokenNum].type_inf == VAR || curToken[tokenNum].type_inf == PROCEDURE || curToken[tokenNum].type_inf == BEGIN1) {
		return t;
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
		tokenNum++;
		//exit(0);
		return t;
	}
}

// TypeDeclaration ::= TYPE  TypeDecList  
// Type声明部分处理,MATCH(TYPE)...
grammarTreeNode* RDTypeDeclaration() {
	Match(TYPE);
	grammarTreeNode* t = RDTypeDecList();
	if (t == NULL) {
		cout << "t is NULL"<<endl;
	}
	return t;
}

// TypeDecList ::= typeId = typeName ; typeDecMore
// Type声明部分处理, 生成type声明节点
grammarTreeNode* RDTypeDecList() {
	grammarTreeNode* t = new grammarTreeNode(DecK, curToken[tokenNum].linenum);

	RDtypeId(t);
	Match(EQ);
	RDTypeName(t);
	Match(SEMI);
	grammarTreeNode* p = RDTypeDecMore();
	if (p != NULL)t->sibling = p;
	return t;
}

// typeDecMore ::=  ε | TypeDecList
// Type声明部分处理, 有多个type
grammarTreeNode* RDTypeDecMore() {
	grammarTreeNode* t = NULL;
	if (curToken[tokenNum].type_inf == VAR || curToken[tokenNum].type_inf == PROCEDURE || curToken[tokenNum].type_inf == BEGIN1)return t;
	else if (curToken[tokenNum].type_inf == ID) {
		t = RDTypeDecList();
		return t;
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
		tokenNum++;
		//exit(0);
		return t;
	}
	//curToken++;
	return t;
}

// typeId ::= id
// Type声明部分处理, id
void RDtypeId(grammarTreeNode* t) {
	if (curToken[tokenNum].type_inf == ID && t != NULL) { 
		t->name[t->nameNum++] = curToken[tokenNum].content; 
	}
	Match(ID);
}

// typeName ::= baseType | structureType | id
// Type声明部分处理, name
void RDTypeName(grammarTreeNode* t) {
	if (t != NULL) {
		if (curToken[tokenNum].type_inf == INTEGER || curToken[tokenNum].type_inf == CHAR1) { 
			RDBaseType(t); 
			return; 
		}
		else if (curToken[tokenNum].type_inf == ARRAY || curToken[tokenNum].type_inf == RECORD) { 
			RDStructureType(t); 
			return; 
		}
		else if (curToken[tokenNum].type_inf == ID) {
			t->kind.dec = DecIdK;
			//t->name[0] = curToken[tokenNum].content;
			t->typeName = curToken[tokenNum].content; // type_name
			Match(ID);
			return;
		}
		else {
			cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
			tokenNum++;
		}
	}

}

// baseType ::=  INTEGER | CHAR
// INTEGER or CHAR
void RDBaseType(grammarTreeNode* t) {
	if (curToken[tokenNum].type_inf == INTEGER) {
		Match(INTEGER);
		t->kind.dec = IntegerK;
		return;
	}
	else if (curToken[tokenNum].type_inf == CHAR1) {
		Match(CHAR1);
		t->kind.dec = CharK;
		return;
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
		tokenNum++;
	}

}

// structureType ::=  arrayType | recType
// ARRAY or RECORD
void RDStructureType(grammarTreeNode* t) {
	if (curToken[tokenNum].type_inf == ARRAY) {
		RDArrayType(t);
	}
	else if (curToken[tokenNum].type_inf == RECORD) {
		t->kind.dec = RecordK;
		RDRecType(t);
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
		tokenNum++;
	}
}

// arrayType ::=  ARRAY [low..top] OF baseType
// ARRAY处理
void RDArrayType(grammarTreeNode* t) {
	Match(ARRAY);
	Match(LMIDPAREN);
	if (curToken[tokenNum].type_inf == INTC) {
		t->Attr.arrayAttr.low = stoi(curToken[tokenNum].content);
	}
	Match(INTC);
	Match(UNDERANGE);
	if (curToken[tokenNum].type_inf == INTC) {
		t->Attr.arrayAttr.up = stoi(curToken[tokenNum].content);
	}
	Match(INTC);
	Match(RMIDPAREN);
	Match(OF);
	RDBaseType(t);
	t->Attr.arrayAttr.arrayKind = t->kind.dec;
	t->kind.dec = ArrayK;
}

// recType ::=  RECORD fieldDecList END
// RECORD处理
void RDRecType(grammarTreeNode* t) {
	Match(RECORD);
	grammarTreeNode* p = RDFieldDecList();
	if (p != NULL) {
		t->child[0] = p;
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
		//tokenNum++;
	}
	Match(END1);
}

// fieldDecList ::=   baseType idList ; fieldDecMore  | arrayType idList; fieldDecMore    
// recBody处理
grammarTreeNode* RDFieldDecList() {
	grammarTreeNode* t = new grammarTreeNode(DecK, curToken[tokenNum].linenum);
	grammarTreeNode* p = NULL;
	if (t != NULL) {
		if (curToken[tokenNum].type_inf == INTEGER || curToken[tokenNum].type_inf == CHAR1) {
			RDBaseType(t);
			RDIdList(t);
			Match(SEMI);
			p = RDFieldDecMore();
		}
		else if (curToken[tokenNum].type_inf == ARRAY) {
			RDArrayType(t);
			RDIdList(t);
			Match(SEMI);
			p = RDFieldDecMore();
		}
		else {
			cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
			tokenNum++;
		}
		t->sibling = p;
	}
	return t;
}

// fieldDecMore ::=  ε | fieldDecList
// recBody处理,进一步
grammarTreeNode* RDFieldDecMore() {
	grammarTreeNode* t = NULL;
	if (curToken[tokenNum].type_inf == INTEGER || curToken[tokenNum].type_inf == CHAR1 || curToken[tokenNum].type_inf == ARRAY) {
		t = RDFieldDecList();
		return t;
	}
	else if (curToken[tokenNum].type_inf == END1) {
		return t;
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
		tokenNum++;
	}
	return t;
}

// idList ::=  id  idMore 
// id处理
void RDIdList(grammarTreeNode* t) {
	if (curToken[tokenNum].type_inf == ID) {
		t->name[t->nameNum++] = curToken[tokenNum].content;
		Match(ID);
	}
	RDIdMore(t);
}

// idMore ::=  ε |  , idList
// 更多id
void RDIdMore(grammarTreeNode* t) {
	if (curToken[tokenNum].type_inf != SEMI) {
		if (curToken[tokenNum].type_inf == COMMA) {
			Match(COMMA);
			RDIdList(t);
		}
		else {
			cout << curToken[tokenNum].linenum << " " << curToken[tokenNum]. content << " error! unexpected token!" << endl;
			tokenNum++;
		}
	}
}

// varDec ::=  ε |  varDeclaration 
// Var声明部分处理
grammarTreeNode* RDVarDec() {
	grammarTreeNode* t =  NULL;
	if (curToken[tokenNum].type_inf == PROCEDURE || curToken[tokenNum].type_inf == BEGIN1) {
		return t;
	}
	if (curToken[tokenNum].type_inf == VAR) {
		t = RDVarDeclaration();
		return t;
	}
	else {		
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
		tokenNum++;
	}
	return t;
}

// varDeclaration ::=  VAR  varDecList 
// Var声明部分处理
grammarTreeNode* RDVarDeclaration() {
	grammarTreeNode* t = NULL;
	Match(VAR);
	t = RDVarDecList();
	if (t == NULL) {
		cout<<curToken[tokenNum].linenum<<" " << curToken[tokenNum].content << " error! var declaration is NULL";
	}
	return t;
}

// varDecList ::=  typeName varIdList; varDecMore
// Var声明部分处理,具体
grammarTreeNode* RDVarDecList() {
	grammarTreeNode* t = new grammarTreeNode(DecK, curToken[tokenNum].linenum);
	grammarTreeNode* p =  NULL;
	if (t != NULL) {
		RDTypeName(t);
		RDVarIdList(t);
		Match(SEMI);
		p = RDVarDecMore();
		t->sibling = p;
	}
	return t;
}

// varDecMore ::=  ε |  varDecList
// 更多var声明
grammarTreeNode* RDVarDecMore() {
	grammarTreeNode* t = NULL;
	if (curToken[tokenNum].type_inf == PROCEDURE || curToken[tokenNum].type_inf == BEGIN1) {
		return t;
	}
	else if (curToken[tokenNum].type_inf == INTEGER || curToken[tokenNum].type_inf == CHAR1 || curToken[tokenNum].type_inf == ARRAY || curToken[tokenNum].type_inf == RECORD || curToken[tokenNum].type_inf == ID) {
		// t = RDVarDeclaration();
		t = RDVarDecList();
		return t;
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
		tokenNum++;
	}
	return t;
}

// varIdList ::=  id  varIdMore
// id
void RDVarIdList(grammarTreeNode* t) {
	if (curToken[tokenNum].type_inf == ID) {
		t->name[t->nameNum++] = curToken[tokenNum].content;
		Match(ID);
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! ID without name ." << endl;
		tokenNum++;
	}
	RDvarIdMore(t);
}

// varIdMore ::=  ε |  , varIdList
// 更多id
void RDvarIdMore(grammarTreeNode* t) {
	if (curToken[tokenNum].type_inf != SEMI) {
		if (curToken[tokenNum].type_inf == COMMA) {
			Match(COMMA);
			RDVarIdList(t);
		}
		else {
			cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
			tokenNum++;
		}
	}
}

// procDec ::=  ε |  procDeclaration
// Proc声明部分处理
grammarTreeNode* RDProcDec() {
	grammarTreeNode* t =  NULL;
	if (curToken[tokenNum].type_inf != BEGIN1) {
		if (curToken[tokenNum].type_inf == PROCEDURE) {
			t = RDProcDeclaration();
		}
		else {
			cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
			tokenNum++;
		}
	}
	return t;
}

/* procDeclaration > ::=  PROCEDURE
						  ProcName(paramList);
						  procDecPart                   
                          procBody                        
                          procDec 
*/
grammarTreeNode* RDProcDeclaration() {
	grammarTreeNode* t = new grammarTreeNode(ProcDecK, curToken[tokenNum].linenum);
	Match(PROCEDURE);
	if (curToken[tokenNum].type_inf == ID) {
		t->name[t->nameNum++] = curToken[tokenNum].content;
		Match(ID);
	}
	Match(LPAREN);
	RDparamlist(t);
	Match(RPAREN);
	Match(SEMI);
	t->child[1] = RDProcDecPart();
	t->child[2] = RDProcBody();
	t->sibling = RDProcDec();
	return t;
}

// paramList ::=  ε |  paramDecList
// 形参列表
void RDparamlist(grammarTreeNode* t) {
	grammarTreeNode* p =  NULL;
	if (curToken[tokenNum].type_inf != RPAREN) {
		if (curToken[tokenNum].type_inf == INTEGER || curToken[tokenNum].type_inf == CHAR1 || curToken[tokenNum].type_inf == RECORD || curToken[tokenNum].type_inf == ARRAY || curToken[tokenNum].type_inf == ID || curToken[tokenNum].type_inf == VAR) {
			p = RDParamDecList();
			t->child[0] = p;
		}
		else {
			cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! unexpected token!" << endl;
			tokenNum++;
		}
	}
}

// paramDecList ::=  param  paramMore
// 形参声明
grammarTreeNode* RDParamDecList() {
	grammarTreeNode* t = RDParam();
	grammarTreeNode* p = RDParamMore();
	if (p != NULL) {
		t->sibling = p;
	}
	return t;
}

// paramMore ::=  ε | ; paramDecList
// 更多形参
grammarTreeNode* RDParamMore() {
	grammarTreeNode* t = NULL;
	if (curToken[tokenNum].type_inf != RPAREN) {
		if (curToken[tokenNum].type_inf == SEMI) {
			Match(SEMI);
			t = RDParamDecList();
			if (t == NULL) {
				cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! Without param declaration!";
			}
			else {
				cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error!unexpected token." << endl;
				tokenNum++;
			}
		}
	}
	return t;
}

// param ::=  typeName formList | VAR typeName formList
// 形参声明处理
grammarTreeNode* RDParam() {
	grammarTreeNode* t = new grammarTreeNode(DecK, curToken[tokenNum].linenum);

	if (curToken[tokenNum].type_inf == INTEGER || curToken[tokenNum].type_inf == CHAR1 || curToken[tokenNum].type_inf == RECORD || curToken[tokenNum].type_inf == ARRAY || curToken[tokenNum].type_inf == ID) {
		// 值参
		t->Attr.paramAttr.paramType = valparamType;
		RDTypeName(t);
		RDFormList(t);
	}
	else if (curToken[tokenNum].type_inf == VAR) {
		Match(VAR);
		// 变参
		t->Attr.paramAttr.paramType = varparamType;
		RDTypeName(t);
		RDFormList(t);
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error!unexpected token." << endl;
		tokenNum++;
	}
	return t;
}

// formList ::=  id  fidMore
void RDFormList(grammarTreeNode* t) {
	if (curToken[tokenNum].type_inf == ID) {
		t->name[t->nameNum++] = curToken[tokenNum].content;
		Match(ID);
	}
	RDFidMore(t);
}

// fidMore ::=   ε |  , formList
void RDFidMore(grammarTreeNode* t) {
	if (curToken[tokenNum].type_inf == SEMI || curToken[tokenNum].type_inf == RPAREN);
	else if (curToken[tokenNum].type_inf == COMMA) {
		Match(COMMA);
		RDFormList(t);
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error!unexpected token." << endl;
		tokenNum++;
	}
	
}

// procDecPart ::=  declarePart 
grammarTreeNode* RDProcDecPart() {
	grammarTreeNode* t =  RDDeclarePart();
	return t;
}

// procBody ::=  programBody  
grammarTreeNode* RDProcBody() {
	grammarTreeNode* t = NULL;
	t = RDProgramBody();
	if (t == NULL) {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error! Without procBody!";
	}
	return t;
}

// programBody ::=  BEGIN  stmList   END 
grammarTreeNode* RDProgramBody() {
	grammarTreeNode* t = new grammarTreeNode(StmLK, curToken[tokenNum].linenum);  // STML OR STMT

	Match(BEGIN1);
	t->child[0] = RDStmList();
	Match(END1);
	return t;
}

// stmList ::=  stm    stmMore
grammarTreeNode* RDStmList() {
	grammarTreeNode* t = RDStm();
	grammarTreeNode* p = RDStmMore();
	if (t != NULL && p != NULL) {
		t->sibling = p;
	}
	return t;
}

// stmMore ::=   ε |  ; stmList
grammarTreeNode* RDStmMore() {
	grammarTreeNode* t =  NULL;
	if (curToken[tokenNum].type_inf == END1 || curToken[tokenNum].type_inf == ENDWH || curToken[tokenNum].type_inf == FI || curToken[tokenNum].type_inf == ELSE);
	else if (curToken[tokenNum].type_inf == SEMI) {
		Match(SEMI);
		t = RDStmList();
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error!unexpected token." << endl;
		tokenNum++;
	}
	return t;
}


string temp_name;
/* 
stm ::=   conditionalStm    {IF}                       
         | loopStm          {WHILE}                    
         | inputStm         {READ}                     
         | outputStm        {WRITE}                    
         | returnStm        {RETURN}                   
         | id  assCall      {id}
*/
grammarTreeNode* RDStm() {
	grammarTreeNode* t = NULL;
	if (curToken[tokenNum].type_inf == IF) {
		t = RDConditionalStm();
	}
	else if (curToken[tokenNum].type_inf == WHILE) {
		t = RDLoopStm();
	}
	else if (curToken[tokenNum].type_inf == RETURN1) {
		t = RDReturnStm();
	}
	else if (curToken[tokenNum].type_inf == READ) {
		t = RDInputStm();
	}
	else if (curToken[tokenNum].type_inf == WRITE) {
		t = RDOutputStm();
	}
	else if (curToken[tokenNum].type_inf == ID) {
		temp_name = curToken[tokenNum].content;
		Match(ID);
		t = RDAssCall();
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error!unexpected token." << endl;
		tokenNum++;
	}
	return t;
}

// assCall > ::=   assignmentRest  |   callStmRest  
// eg: {:=,LMIDPAREN,DOT}  or  {(}
grammarTreeNode* RDAssCall() {
	grammarTreeNode* t = NULL;
	if (curToken[tokenNum].type_inf == ASSIGN || curToken[tokenNum].type_inf == LMIDPAREN || curToken[tokenNum].type_inf == DOT) {
		t = RDAssignmentRest();
	}
	else if (curToken[tokenNum].type_inf == LPAREN) {
		t = RDCallStmRest();
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error!unexpected token." << endl;
		tokenNum++;
	}
	return t;
}

// assignmentRest ::=  variMore : = exp 
grammarTreeNode* RDAssignmentRest() {
	grammarTreeNode* t = new grammarTreeNode(StmtK, curToken[tokenNum].linenum); 
	t->kind.stmt = AssignK;

	grammarTreeNode* c = new grammarTreeNode(ExpK, curToken[tokenNum].linenum);
	c->kind.exp = ExpIdK;
	c->Attr.expAttr.varKind = IdV;
	c->name[c->nameNum++] = temp_name;

	RDvariMore(c);
	t->child[0] = c;

	Match(ASSIGN);
	t->child[1] = RDExp();

	return t;
}
// conditionalStm ::= IF exp THEN stmList ELSE stmList FI
grammarTreeNode* RDConditionalStm() {
	grammarTreeNode* t = new grammarTreeNode(StmtK, curToken[tokenNum].linenum);
	t->kind.stmt = IfK;
	Match(IF);
	t->child[0] = RDExp();
	Match(THEN);
	t->child[1] = RDStmList();
	if (curToken[tokenNum].type_inf == ELSE) {
		Match(ELSE);
		t->child[2] = RDStmList();
	}
	Match(FI);
	return t;
}
// loopStm ::=      WHILE exp DO stmList ENDWH 
grammarTreeNode* RDLoopStm() {
	grammarTreeNode* t = new grammarTreeNode(StmtK, curToken[tokenNum].linenum);
	t->kind.stmt = WhileK;
	Match(WHILE);
	t->child[0] = RDExp();
	Match(DO);
	t->child[1] = RDStmList();
	Match(ENDWH);
	return t;
}
// inputStm ::=    READ(id)
grammarTreeNode* RDInputStm() {
	grammarTreeNode* t = new grammarTreeNode(StmtK, curToken[tokenNum].linenum);
	t->kind.stmt = ReadK;
	Match(READ);
	Match(RPAREN);
	if (curToken[tokenNum].type_inf == ID) {
		t->name[t->nameNum++] = curToken[tokenNum].content;
	}
	Match(ID);
	Match(LPAREN);
	return t;
}
// outputStm ::=   WRITE(exp)
grammarTreeNode* RDOutputStm() {
	grammarTreeNode* t = new grammarTreeNode(StmtK, curToken[tokenNum].linenum);
	t->kind.stmt = WriteK;
	Match(WRITE);
	Match(LPAREN);
	t->child[0] = RDExp();
	Match(RPAREN);
	return t;
}
// returnStm ::=   RETURN
grammarTreeNode* RDReturnStm() {
	grammarTreeNode* t = new grammarTreeNode(StmtK, curToken[tokenNum].linenum);
	t->kind.stmt = ReturnK;
	Match(RETURN1);
	return t;
}

// callStmRest ::=  (actParamList)
grammarTreeNode* RDCallStmRest() {
	grammarTreeNode* t = new grammarTreeNode(StmtK, curToken[tokenNum].linenum);
	t->kind.stmt = CallK;
	Match(LPAREN);

	grammarTreeNode* c = new grammarTreeNode(ExpK, curToken[tokenNum].linenum);
	c->kind.exp = ExpIdK;
	c->Attr.expAttr.varKind = IdV;
	c->name[c->nameNum++] = temp_name;
	t->child[0] = c;
	t->child[1] = RDActParamList();
	Match(RPAREN);
	return t;
}

// actParamList ::=     ε |  exp actParamMore
grammarTreeNode* RDActParamList() {  
	grammarTreeNode* t = new grammarTreeNode();
	if (curToken[tokenNum].type_inf == RPAREN);
	else if (curToken[tokenNum].type_inf == ID || curToken[tokenNum].type_inf == INTC) {
		t = RDExp();
		if (t != NULL) {
			t->sibling = RDActParamMore();
		}
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error!unexpected token." << endl;
		tokenNum++;
	}
	return t;
}

// actParamMore ::=     ε |  , actParamList
grammarTreeNode* RDActParamMore() {
	grammarTreeNode* t = NULL;
	if (curToken[tokenNum].type_inf == RPAREN) {
		return t;
	}
	else if (curToken[tokenNum].type_inf == COMMA) {
		Match(COMMA);
		t = RDActParamList();
		return t;
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error!unexpected token." << endl;
		tokenNum++;
	}
}

// RelExp ::=  Exp  OtherRelE 
// OtherRelE ::=  CmpOp   Exp 
// Exp ::=  Term   OtherTerm
grammarTreeNode* RDExp() {
	grammarTreeNode* t =  NULL;
	t = RDSimple_exp();
	if (curToken[tokenNum].type_inf == LT || curToken[tokenNum].type_inf == EQ) {
		grammarTreeNode* p = new grammarTreeNode(ExpK, curToken[tokenNum].linenum);
		p->kind.exp = OpK;
		p->Attr.expAttr.varKind = IdV; // ...
		p->child[0] = t;
		p->Attr.expAttr.op = curToken[tokenNum].type_inf;
		t = p;
		Match(curToken[tokenNum].type_inf);
		if (t != NULL) {
			t->child[1] = RDSimple_exp();
		}
	}
	return t;
}

// OtherTerm ::=  ε  |   AddOp   Exp
grammarTreeNode* RDSimple_exp() {
	grammarTreeNode* t = NULL;
	t = RDterm();
	while (curToken[tokenNum].type_inf == PLUS || curToken[tokenNum].type_inf == MINUS) {
		grammarTreeNode* p = new grammarTreeNode(ExpK, curToken[tokenNum].linenum);
		p->kind.exp = OpK;
		//p->Attr.expAttr.varKind = IdV; // ...
		p->child[0] = t;
		p->Attr.expAttr.op = curToken[tokenNum].type_inf;
		t = p;
		Match(curToken[tokenNum].type_inf);
		t->child[1] = RDterm();
	}
	return t;
}

// Term	::=   Factor   OtherFactor
grammarTreeNode* RDterm() {
	grammarTreeNode* t = NULL;
	t = RDfactor();
	while (curToken[tokenNum].type_inf == TIMES || curToken[tokenNum].type_inf == OVER) {
		grammarTreeNode* p = new grammarTreeNode(ExpK, curToken[tokenNum].linenum);
		p->kind.exp = OpK;
		//p->Attr.expAttr.varKind = IdV;  // ...
		p->child[0] = t;
		p->Attr.expAttr.op = curToken[tokenNum].type_inf;
		t = p;
		Match(curToken[tokenNum].type_inf);
		t->child[1] = RDfactor();
	}
	return t;
}

// factor ::= ( exp ) | INTC | variable
grammarTreeNode* RDfactor() {
	grammarTreeNode* t = NULL;
	if (curToken[tokenNum].type_inf == INTC) {
		//数字处理部分；
		t = new grammarTreeNode(ExpK, curToken[tokenNum].linenum);
		t->kind.exp = ConstK;
		t->Attr.expAttr.varKind = IdV;
		t->Attr.expAttr.val = stoi(curToken[tokenNum].content);
		Match(INTC);
	}
	else if (curToken[tokenNum].type_inf == ID) {
		t = RDVariable();
	}
	else if (curToken[tokenNum].type_inf == LPAREN) {
		Match(LPAREN);
		t = RDExp();
		Match(RPAREN);
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error!unexpected token." << endl;
		tokenNum++;
	}
	return t;
}
string tmp;

// variable  ::=  id variMore
grammarTreeNode* RDVariable() {
	grammarTreeNode* t = new grammarTreeNode(ExpK, curToken[tokenNum].linenum);
	t->kind.exp = ExpIdK;
	t->Attr.expAttr.varKind = IdV;

	if (curToken[tokenNum].type_inf == ID) {
		t->name[t->nameNum++] = curToken[tokenNum].content;
	}
	Match(ID);
	RDvariMore(t);
	return t;
}

// variMore  ::=  ε | [exp]  |  . fieldvar   
void RDvariMore(grammarTreeNode* t) {
	switch (curToken[tokenNum].type_inf){
	case ASSIGN:
	case TIMES:
	case EQ:
	case LT:
	case PLUS:
	case MINUS:
	case OVER:
	case RPAREN:
	case RMIDPAREN:
	case SEMI:
	case COMMA:
	case THEN:
	case ELSE:
	case FI:
	case DO:
	case ENDWH:
	case END1:
		return;
	}
	if (curToken[tokenNum].type_inf == LMIDPAREN) {
		Match(LMIDPAREN);
		t->child[0] = RDExp();
		t->Attr.expAttr.varKind = ArrayMembV;
		t->child[0]->Attr.expAttr.varKind = IdV;
		Match(RMIDPAREN);
	}
	else if (curToken[tokenNum].type_inf == DOT) {
		Match(DOT);
		t->child[0] = RDFieldVar();
		t->Attr.expAttr.varKind = FieldMembV;
		t->child[0]->Attr.expAttr.varKind = IdV;
	}
	else {
		cout << curToken[tokenNum].linenum << " " << curToken[tokenNum].content << " error!unexpected token." << endl;
		tokenNum++;
	}
}

// fieldvar  ::=  id  fieldvarMore
grammarTreeNode* RDFieldVar() {
	grammarTreeNode* t = new grammarTreeNode(ExpK, curToken[tokenNum].linenum);
	t->kind.exp = ExpIdK;
	t->Attr.expAttr.varKind = IdV;
	if (curToken[tokenNum].type_inf == ID) {
		t->name[t->nameNum++] = curToken[tokenNum].content;
	}
	Match(ID);
	RDfieldvarMore(t);
	return  t;
}

// fieldvarMore ::=  ε |  [exp] 
void RDfieldvarMore(grammarTreeNode* t) {
	if (curToken[tokenNum].type_inf == LMIDPAREN) {
		Match(LMIDPAREN);
		t->child[0] = RDExp();
		t->child[0]->Attr.expAttr.varKind = ArrayMembV;
		Match(RMIDPAREN);
	}
}
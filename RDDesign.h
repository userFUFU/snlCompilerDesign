#pragma once
#include<bits/stdc++.h>
using namespace std;


grammarTreeNode* RDmain(Token[], int);

void Match(LexType lex);

grammarTreeNode* RDProgram();
grammarTreeNode* RDProgramHead();
grammarTreeNode* RDDeclarePart();
grammarTreeNode* RDTypeDec();
grammarTreeNode* RDTypeDeclaration();
grammarTreeNode* RDTypeDecList();
grammarTreeNode* RDTypeDecMore();
void RDtypeId(grammarTreeNode* t);
void RDTypeName(grammarTreeNode* t);
void RDBaseType(grammarTreeNode* t);
void RDStructureType(grammarTreeNode* t);
void RDArrayType(grammarTreeNode* t);
void RDRecType(grammarTreeNode* t);
grammarTreeNode* RDFieldDecList();
grammarTreeNode* RDFieldDecMore();
void RDIdList(grammarTreeNode* t);
void RDIdMore(grammarTreeNode* t);
grammarTreeNode* RDVarDec();
grammarTreeNode* RDVarDeclaration();
grammarTreeNode* RDVarDecList();
grammarTreeNode* RDVarDecMore();
void RDVarIdList(grammarTreeNode* t);
void RDvarIdMore(grammarTreeNode* t);
grammarTreeNode* RDProcDec();
grammarTreeNode* RDProcDeclaration();
void RDparamlist(grammarTreeNode* t);
grammarTreeNode* RDParamDecList();//
grammarTreeNode* RDParamMore();
grammarTreeNode* RDParam();
void RDFormList(grammarTreeNode* t);
void RDFidMore(grammarTreeNode* t);
grammarTreeNode* RDProcDecPart();
grammarTreeNode* RDProcBody();
grammarTreeNode* RDProgramBody();
grammarTreeNode* RDStmList();
grammarTreeNode* RDStmMore();
grammarTreeNode* RDStm();
grammarTreeNode* RDAssCall();
grammarTreeNode* RDAssignmentRest();
grammarTreeNode* RDConditionalStm();
grammarTreeNode* RDLoopStm();
grammarTreeNode* RDInputStm();
grammarTreeNode* RDOutputStm();
grammarTreeNode* RDReturnStm();
grammarTreeNode* RDCallStmRest();
grammarTreeNode* RDActParamList();
grammarTreeNode* RDActParamMore();
grammarTreeNode* RDExp();
grammarTreeNode* RDSimple_exp();
grammarTreeNode* RDterm();
grammarTreeNode* RDfactor();
grammarTreeNode* RDVariable();
void RDvariMore(grammarTreeNode* t);
grammarTreeNode*  RDFieldVar();
void RDfieldvarMore(grammarTreeNode* t);
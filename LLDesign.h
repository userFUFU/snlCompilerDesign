#pragma once
#include<bits/stdc++.h>
using namespace std;

grammarTreeNode* LLmain(Token token[], int num);

// 建立LL(1)符号表
void create_LL1table();

// 根据产生式编号转向
void predict(int num);


void printGrammarTree(int, grammarTreeNode*);
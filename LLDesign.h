#pragma once
#include<bits/stdc++.h>
using namespace std;

grammarTreeNode* LLmain(Token token[], int num);

// ����LL(1)���ű�
void create_LL1table();

// ���ݲ���ʽ���ת��
void predict(int num);


void printGrammarTree(int, grammarTreeNode*);
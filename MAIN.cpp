#define _CRT_SECURE_NO_WARNINGS     //�궨��
#pragma warning(disable:4996)

#include<bits/stdc++.h>
#include "lexicalDesign.h"
#include "grammarDesign.h"
#include "RDDesign.h"
#include "LLDesign.h"
#include "semantemeDesign.h"

using namespace std;
#define maxn 100086

Token token[maxn];
int tokencnt=1;

extern map<LexType, string> enumToStr;

int main() {
	// freopen("./examples/errorTest/contentAfterEnd.txt", "r", stdin);
	 freopen("./examples/var_val.txt", "r", stdin);
	// freopen("./source.txt", "r", stdin);

	string tokenList = ""; // �ʷ��������
	string sourceList = "";  // Դ�����һ��
	int linecnt = 1; // �м�����


	/* �ʷ����� */

	// ����ÿһ�У���ÿһ�н��дʷ�����
	while (getline(cin, sourceList)){
		cout << sourceList<<endl;
		tokencnt = getToken(sourceList,linecnt,token,tokencnt);
		linecnt++;
	}
	// EOF
	token[tokencnt].type_inf = ENDFILE;
	token[tokencnt].content = "EOF";
	token[tokencnt].linenum = linecnt;
	token[tokencnt].tokennum = tokencnt;
	// ���tokenlist
	for (int i = 1; i <= tokencnt; i++) {
		cout << token[i].tokennum << " " << token[i].linenum << " " << enumToStr[token[i].type_inf] << " " << token[i].content << endl;
	}
	cout << endl << "*************************************************************************************************" << endl;

	/* �﷨���� */

	// �ݹ��½�������
	grammarTreeNode* RDRoot = RDmain(token, tokencnt);
	printGrammarTree(0, RDRoot);
	cout << endl << "*************************************************************************************************" << endl;
	// LL(1)������
	grammarTreeNode* LLRoot = LLmain(token, tokencnt);
	printGrammarTree(0, LLRoot);
	cout << endl << "*************************************************************************************************" << endl;

	// �������
	//smtMain(RDRoot);

	return 0;
}
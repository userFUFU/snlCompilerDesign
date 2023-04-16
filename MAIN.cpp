#define _CRT_SECURE_NO_WARNINGS     //宏定义
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
	 //freopen("./examples/errorTest/IfWithoutFI.txt", "r", stdin);
	 //freopen("./examples/easy.txt", "r", stdin);
	 //freopen("./source.txt", "r", stdin);
	freopen("./test.txt", "r", stdin);


	string tokenList = ""; // 词法分析结果
	string sourceList = "";  // 源程序的一行
	int linecnt = 1; // 行计数器


	/* 词法分析 */

	// 读入每一行，对每一行进行词法分析
	//freopen("./Result/tokenError.txt", "w", stdout);
	while (getline(cin, sourceList)){
		//cout << sourceList<<endl;
		tokencnt = getToken(sourceList,linecnt,token,tokencnt);
		linecnt++;
	}
	// EOF
	token[tokencnt].type_inf = ENDFILE;
	token[tokencnt].content = "EOF";
	token[tokencnt].linenum = linecnt;
	token[tokencnt].tokennum = tokencnt;


	fclose(stdin);
	// 输出tokenlist
	//freopen("./Result/tokenList.txt", "w",stdout);
	for (int i = 1; i <= tokencnt; i++) {
		cout << token[i].tokennum << " " << token[i].linenum << " " << enumToStr[token[i].type_inf] << " " << token[i].content << endl;
	}


	//cout << endl << "*************************************************************************************************" << endl;

	/* 语法分析 */

	// 递归下降分析法
	//freopen("./Result/RDError.txt", "w", stdout);
	grammarTreeNode* RDRoot = RDmain(token, tokencnt);

	//freopen("./Result/RDGrammarTree.txt", "w", stdout);
	printGrammarTree(0, RDRoot);

	//cout << endl << "*************************************************************************************************" << endl;
	// LL(1)分析法
	//freopen("./Result/LLError.txt", "w", stdout);
	grammarTreeNode* LLRoot = LLmain(token, tokencnt);

	//freopen("./Result/LLGrammarTree.txt", "w", stdout);
	printGrammarTree(0, LLRoot);

	//cout << endl << "*************************************************************************************************" << endl;


	// Format Code
	//freopen("./Result/GencodeResult.txt", "w", stdout);
	generateCode(0, RDRoot, NULL);

	cout << endl << "*************************************************************************************************" << endl;

	// 语义分析
	//freopen("./semantemeResult.txt", "w", stdout);
	smtMain(RDRoot);

	fclose(stdin);
	//fclose(stdout);
	return 0;
}
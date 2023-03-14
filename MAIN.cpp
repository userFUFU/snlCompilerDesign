#define _CRT_SECURE_NO_WARNINGS     //宏定义
#pragma warning(disable:4996)

#include<bits/stdc++.h>
#include "lexicalDesign.h"
using namespace std;

int main() {
	freopen("source.txt", "r", stdin);

	string tokenList = ""; // 词法分析结果
	string sourceList = "";  // 源程序的一行
	int linecnt = 1; // 行计数器


	// 读入每一行，对每一行进行词法分析
	while (getline(cin, sourceList)){
		// cout << sourceList<<endl;
		getToken(sourceList,linecnt);
		linecnt++;
	}

	return 0;
}
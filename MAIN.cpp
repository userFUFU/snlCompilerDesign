#define _CRT_SECURE_NO_WARNINGS     //�궨��
#pragma warning(disable:4996)

#include<bits/stdc++.h>
#include "lexicalDesign.h"
using namespace std;

int main() {
	freopen("source.txt", "r", stdin);

	string tokenList = ""; // �ʷ��������
	string sourceList = "";  // Դ�����һ��
	int linecnt = 1; // �м�����


	// ����ÿһ�У���ÿһ�н��дʷ�����
	while (getline(cin, sourceList)){
		// cout << sourceList<<endl;
		getToken(sourceList,linecnt);
		linecnt++;
	}

	return 0;
}
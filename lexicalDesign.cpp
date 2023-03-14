#include<bits/stdc++.h>
#include "lexicalDesign.h"
#include "LEXsnl.h"
using namespace std;


/* DFA�Ÿ�״̬����ʼ����ʶ������������ɣ���ֵ��ע�ͣ������±꣬�ַ���־������ */
enum states{START,INID,INNUM,DONE,INASSIGN,INCOMMENT,INRANGE,INCHAR,ERRORSTATE};

// ���������ֲ��Һ���
void formToken(string word,int linenum) {
	//LexType mem = reservedLookup(word);
	if (reserved_words.count(word)) {
		cout << linenum << " ������ " << reserved_words[word] << " " << "��" << endl;
	}
	else {
		cout << linenum << " ��ʶ�� " << "ID" << " " << word << endl;
	}
}

bool isCharOrNum(char c) {
	return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}
string SCDelimiter(char c) {
	string ans = "";
	switch (c){
	case '=':ans = "EQ"; break;
	case '<':ans = "LT"; break;
	case '+':ans = "PLUS"; break;
	case '-':ans = "MINUS"; break;
	case '*':ans = "TIMES"; break;
	case '/':ans = "DIV"; break; // OVER 
	case '(':ans = "LPAREN"; break;
	case ')':ans = "RPAREN"; break;
	case ';':ans = "SEMI"; break;
	case ',':ans = "COMMA"; break;
	case '[':ans = "LMIDPAREN"; break;
	case ']':ans = "RMIDPAREN"; break;

	default:ans = "ERROR";
		break;
	}
	return ans;
}
// ����һ�У�ʶ�𵥴ʣ���������еĵ��ʵ�token
void getToken(string str,int linenum){
	//cout << linenum <<" " <<str<< endl;
	string tmp = "";
	int len = str.length();
	int index = 0;
	while (index < len){
		if (str[index] == ' ') {
			index++;
			continue;
		}
		// ����ĸ��ͷ������Ϊ�����֣�����Ϊ��ʶ����
		if (str[index] >= 'a' && str[index] <= 'z') {
			tmp += str[index];
			index++;
			while (index < len && isCharOrNum(str[index]))tmp += str[index++];
			formToken(tmp,linenum);
			tmp = "";
			continue;
		}
		// �����ֿ�ͷ
		else if (str[index] >= '0' && str[index] <= '9') {
			tmp += str[index];
			index++;
			while (index < len && str[index] >= '0' && str[index] <= '9')tmp += str[index++];
			cout << linenum << " �޷������� " << "INTC" << " " << tmp << endl;
			tmp = "";
			continue;
		}
		// �� : ��ͷ
		else if (str[index] == ':') {
			if (str[++index] == '=') {
				cout << linenum << " ˫�ֽ�� " << "ASSIGN(:=)" << " " << "��" << endl;
			}
			else {
				cout << "error! �� " << linenum << " ��ֵ���ų���" << endl;
			}
			index++;
			continue;
		}
		// �� { ��ͷ ��ע�Ͳ��֣�ֱ�Ӻ���
		// ��δ���� �������� �Ĵ����� ������
		else if (str[index] == '{') {
			while (index < len && str[index] != '}')index++;
			index++;
			continue;
		}
		// �� . ��ͷ
		else if (str[index] == '.') {
			if ((index+1)<len && str[++index] == '.') { // �����д��󣿣���
				cout << linenum << " �����±���޷� " << "UNDERANGE(..)" << " " << "��" << endl;
				index++;
				continue;
			}
			else {
				cout << linenum << " ������� " << "END" << " " << "��" << endl;
				break;
			}
		}
		// ���ַ��ֽ��
		else{
			string mem = SCDelimiter(str[index]);
			if (mem != "ERROR") {
				cout<< linenum << " ���ַ��ֽ�� " << mem << " " << "��" << endl;
			}
			else {
				cout << "error! �� " << linenum << " ���ַ��ֽ������" << endl;
			}
			index++;
			continue;
		}
	}

}
#include<bits/stdc++.h>
#include "lexicalDesign.h"
using namespace std;

/* DFA�Ÿ�״̬����ʼ����ʶ������������ɣ���ֵ��ע�ͣ������±꣬�ַ���־������ *//*
enum states{START,INID,INNUM,DONE,INASSIGN,INCOMMENT,INRANGE,INCHAR,ERRORSTATE};*/

 
/* //���������ֲ��Һ���
void formToken(string word,int linenum) {
	//LexType mem = reservedLookup(word);
	if (reserved_words.count(word)) {
		cout << linenum << " ������ " << reserved_words[word] << " " << "��" << endl;
	}
	else {
		cout << linenum << " ��ʶ�� " << "ID" << " " << word << endl;
	}
}
*/
map<string, LexType> reserved_words = {
	{"program",PROGRAM},{"type",TYPE},{"var",VAR},
	{"procedure",PROCEDURE},{"begin",BEGIN1},{"end",END1},
	{"array",ARRAY},{"of",OF},{"record",RECORD},
	{"if",IF},{"then",THEN},{"else",ELSE},{"fi",FI},
	{"while",WHILE},{"do",DO},{"endwh",ENDWH},
	{"read",READ},{"write",WRITE},{"return",RETURN1},
	{"integer",INTEGER},{"char",CHAR1},
	{"+", PLUS},   {"-", MINUS},     {"*", TIMES},
	{"/", OVER}, {"(", LPAREN},    {")", RPAREN},
	{";", SEMI},   {"[", LMIDPAREN}, {"]", RMIDPAREN},
	{"=", EQ},     {"<", LT},        {",", COMMA}
};
// enum->stringӳ��
map<LexType, string>enumToStr = {
	{ENDFILE,"ENDFILE"}, {ERROR,"ERROR"},
	/* ������ */
	{PROGRAM,"PROGRAM"}, {PROCEDURE,"PROCEDURE"}, {TYPE,"TYPE"}, {VAR,"VAR"}, {IF,"IF"},
	{THEN,"THEN"}, {ELSE,"ELSE"}, {FI,"FI"}, {WHILE,"WHILE"}, {DO,"DO"},
	{ENDWH,"ENDWH"}, {BEGIN1,"BEGIN1"}, {END1,"END1"}, {READ,"READ"}, {WRITE,"WRITE"},
	{ARRAY,"ARRAY"}, {OF,"OF"}, {RECORD,"RECORD"}, {RETURN1,"RETURN1"},
	/* ���� */
	{INTEGER,"INTEGER"}, {CHAR1,"CHAR1"},
	/* ���ַ����ʷ��� */
	{ID,"ID"}, {INTC,"INTC"}, {CHARC,"CHARC"},
	/* ���ֽ��ַ���������� */
	{ASSIGN,"ASSIGN"}, {EQ,"EQ"}, {LT,"LT"}, {PLUS,"PLUS"}, {MINUS,"MINUS"},
	{TIMES,"TIMES"}, {OVER,"OVER"}, {LPAREN,"LPAREN"}, {RPAREN,"RPAREN"}, {DOT,"DOT"},
	{COLON,"COLON"}, {SEMI,"SEMI"}, {COMMA,"COMMA"}, {LMIDPAREN,"LMIDPAREN"}, {RMIDPAREN,"RMIDPAREN"},
	{UNDERANGE,"UNDERANGE"}
};



bool isCharOrNum(char c) {
	return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}
string SCDelimiter(char c) {
	string ans = "";
	ans += c;
	switch (c){
	case '=':
	case '<':
	case '+':
	case '-':
	case '*':
	case '/':
	case '(':
	case ')':
	case ';':
	case ',':
	case '[':
	case ']': break;

	default:ans = "ERROR";
		break;
	}
	return ans;
}
// ����һ�У�ʶ�𵥴ʣ���������еĵ��ʵ�token
int getToken(string str,int linenum,Token* token,int tokencnt){
	//cout << linenum <<" " <<str<< endl;
	string tmp = "";
	int len = str.length();
	int index = 0;
	while (index < len){
		// �ո񣬲���
		if (str[index] == ' ' || str[index] == '\t') {
			index++;
			continue;
		}
		// ����ĸ��ͷ������Ϊ�����֣�����Ϊ��ʶ����
		if (str[index] >= 'a' && str[index] <= 'z') {
			tmp += str[index];
			index++;
			while (index < len && isCharOrNum(str[index]))tmp += str[index++];
			//formToken(tmp,linenum);
			if (reserved_words.count(tmp)) {
				//cout << linenum << " ������ " << reserved_words[tmp] << " " << "��" << endl;
				token[tokencnt].type_inf = reserved_words[tmp];
				token[tokencnt].content = tmp;
			}
			else {
				//cout << linenum << " ��ʶ�� " << "ID" << " " << tmp << endl;
				token[tokencnt].type_inf = ID;
				token[tokencnt].content = tmp;
			}
			token[tokencnt].linenum = linenum;
			token[tokencnt].tokennum = tokencnt;
			tokencnt++;
			tmp = "";
			continue;
		}
		// �����ֿ�ͷ
		else if (str[index] >= '0' && str[index] <= '9') {
			tmp += str[index];
			index++;
			while (index < len && str[index] >= '0' && str[index] <= '9')tmp += str[index++];
			//cout << linenum << " �޷������� " << "INTC" << " " << tmp << endl;
			token[tokencnt].type_inf = INTC;
			token[tokencnt].content = tmp;
			token[tokencnt].linenum = linenum;
			token[tokencnt].tokennum = tokencnt;
			tokencnt++;
			tmp = "";
			continue;
		}
		// �� : ��ͷ
		else if (str[index] == ':') {
			if (str[++index] == '=') {
				//cout << linenum << " ˫�ֽ�� " << "ASSIGN(:=)" << " " << "��" << endl;
				token[tokencnt].type_inf = ASSIGN;
				token[tokencnt].content = ":=";
				token[tokencnt].linenum = linenum;
				token[tokencnt].tokennum = tokencnt;
				tokencnt++;
			}
			else {
				cout << "error! �� " << linenum << " ��,��ֵ���ų���" << endl;
			}
			index++;
			continue;
		}
		// �� { ��ͷ ��ע�Ͳ��֣�ֱ�Ӻ���
		// ��δ���� �������Ż�������Ŀƥ�� �Ĵ����� ������
		else if (str[index] == '{') {
			while (index < len && str[index] != '}')index++;
			index++;
			continue;
		}
		// �� . ��ͷ
		else if (str[index] == '.') {
			if ((index+1)<len && str[++index] == '.') { // �����д��󣿣���
				//cout << linenum << " �����±���޷� " << "UNDERANGE(..)" << " " << "��" << endl;
				token[tokencnt].type_inf = UNDERANGE;
				token[tokencnt].content = "..";
				token[tokencnt].linenum = linenum;
				token[tokencnt].tokennum = tokencnt;
				tokencnt++;
				index++;
				continue;
			}
			else if ((index + 1) < len) {
				token[tokencnt].type_inf = DOT;
				token[tokencnt].content = ".(Record Member Symbol)";
				token[tokencnt].linenum = linenum;
				token[tokencnt].tokennum = tokencnt;
				tokencnt++;
				continue;
			}
			else{
				//cout << linenum << " ������� " << "END" << " " << "��" << endl;
				token[tokencnt].type_inf = DOT;
				token[tokencnt].content = ".";
				token[tokencnt].linenum = linenum;
				token[tokencnt].tokennum = tokencnt;
				tokencnt++;
				break;
			}
		}
		// ���ַ��ֽ��
		else{
			string mem = SCDelimiter(str[index]);
			if (mem != "ERROR") {
				//cout<< linenum << " ���ַ��ֽ�� " << mem << " " << "��" << endl;
				token[tokencnt].type_inf = reserved_words[mem];
				token[tokencnt].content = mem;
				token[tokencnt].linenum = linenum;
				token[tokencnt].tokennum = tokencnt;
				tokencnt++;
			}
			else {
				cout << "error! �� " << linenum << " ��,���ַ��ֽ������" << endl;
			}
			index++;
			continue;
		}
	}
	return tokencnt;
}


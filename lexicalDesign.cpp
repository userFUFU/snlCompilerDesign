#include<bits/stdc++.h>
#include "lexicalDesign.h"
using namespace std;

/* DFA九个状态：开始，标识符，整数，完成，赋值，注释，数组下标，字符标志，错误 *//*
enum states{START,INID,INNUM,DONE,INASSIGN,INCOMMENT,INRANGE,INCHAR,ERRORSTATE};*/

 
/* //声明保留字查找函数
void formToken(string word,int linenum) {
	//LexType mem = reservedLookup(word);
	if (reserved_words.count(word)) {
		cout << linenum << " 保留字 " << reserved_words[word] << " " << "无" << endl;
	}
	else {
		cout << linenum << " 标识符 " << "ID" << " " << word << endl;
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
// enum->string映射
map<LexType, string>enumToStr = {
	{ENDFILE,"ENDFILE"}, {ERROR,"ERROR"},
	/* 保留字 */
	{PROGRAM,"PROGRAM"}, {PROCEDURE,"PROCEDURE"}, {TYPE,"TYPE"}, {VAR,"VAR"}, {IF,"IF"},
	{THEN,"THEN"}, {ELSE,"ELSE"}, {FI,"FI"}, {WHILE,"WHILE"}, {DO,"DO"},
	{ENDWH,"ENDWH"}, {BEGIN1,"BEGIN1"}, {END1,"END1"}, {READ,"READ"}, {WRITE,"WRITE"},
	{ARRAY,"ARRAY"}, {OF,"OF"}, {RECORD,"RECORD"}, {RETURN1,"RETURN1"},
	/* 类型 */
	{INTEGER,"INTEGER"}, {CHAR1,"CHAR1"},
	/* 多字符单词符号 */
	{ID,"ID"}, {INTC,"INTC"}, {CHARC,"CHARC"},
	/* 单分界字符、特殊符号 */
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
// 输入一行，识别单词，并输出该行的单词的token
int getToken(string str,int linenum,Token* token,int tokencnt){
	//cout << linenum <<" " <<str<< endl;
	string tmp = "";
	int len = str.length();
	int index = 0;
	while (index < len){
		// 空格，不看
		if (str[index] == ' ' || str[index] == '\t') {
			index++;
			continue;
		}
		// 以字母开头，可能为保留字，可能为标识符，
		if (str[index] >= 'a' && str[index] <= 'z') {
			tmp += str[index];
			index++;
			while (index < len && isCharOrNum(str[index]))tmp += str[index++];
			//formToken(tmp,linenum);
			if (reserved_words.count(tmp)) {
				//cout << linenum << " 保留字 " << reserved_words[tmp] << " " << "无" << endl;
				token[tokencnt].type_inf = reserved_words[tmp];
				token[tokencnt].content = tmp;
			}
			else {
				//cout << linenum << " 标识符 " << "ID" << " " << tmp << endl;
				token[tokencnt].type_inf = ID;
				token[tokencnt].content = tmp;
			}
			token[tokencnt].linenum = linenum;
			token[tokencnt].tokennum = tokencnt;
			tokencnt++;
			tmp = "";
			continue;
		}
		// 以数字开头
		else if (str[index] >= '0' && str[index] <= '9') {
			tmp += str[index];
			index++;
			while (index < len && str[index] >= '0' && str[index] <= '9')tmp += str[index++];
			//cout << linenum << " 无符号整数 " << "INTC" << " " << tmp << endl;
			token[tokencnt].type_inf = INTC;
			token[tokencnt].content = tmp;
			token[tokencnt].linenum = linenum;
			token[tokencnt].tokennum = tokencnt;
			tokencnt++;
			tmp = "";
			continue;
		}
		// 以 : 开头
		else if (str[index] == ':') {
			if (str[++index] == '=') {
				//cout << linenum << " 双分界符 " << "ASSIGN(:=)" << " " << "无" << endl;
				token[tokencnt].type_inf = ASSIGN;
				token[tokencnt].content = ":=";
				token[tokencnt].linenum = linenum;
				token[tokencnt].tokennum = tokencnt;
				tokencnt++;
			}
			else {
				cout << "error! 第 " << linenum << " 行,赋值符号出错。" << endl;
			}
			index++;
			continue;
		}
		// 以 { 开头 ，注释部分，直接忽略
		// 暂未考虑 无右括号或括号数目匹配 的错误处理 ？？？
		else if (str[index] == '{') {
			while (index < len && str[index] != '}')index++;
			index++;
			continue;
		}
		// 以 . 开头
		else if (str[index] == '.') {
			if ((index+1)<len && str[++index] == '.') { // 可能有错误？？？
				//cout << linenum << " 数组下标界限符 " << "UNDERANGE(..)" << " " << "无" << endl;
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
				//cout << linenum << " 程序结束 " << "END" << " " << "无" << endl;
				token[tokencnt].type_inf = DOT;
				token[tokencnt].content = ".";
				token[tokencnt].linenum = linenum;
				token[tokencnt].tokennum = tokencnt;
				tokencnt++;
				break;
			}
		}
		// 单字符分界符
		else{
			string mem = SCDelimiter(str[index]);
			if (mem != "ERROR") {
				//cout<< linenum << " 单字符分界符 " << mem << " " << "无" << endl;
				token[tokencnt].type_inf = reserved_words[mem];
				token[tokencnt].content = mem;
				token[tokencnt].linenum = linenum;
				token[tokencnt].tokennum = tokencnt;
				tokencnt++;
			}
			else {
				cout << "error! 第 " << linenum << " 行,单字符分界符出错。" << endl;
			}
			index++;
			continue;
		}
	}
	return tokencnt;
}


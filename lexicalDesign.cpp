#include<bits/stdc++.h>
#include "lexicalDesign.h"
#include "LEXsnl.h"
using namespace std;


/* DFA九个状态：开始，标识符，整数，完成，赋值，注释，数组下标，字符标志，错误 */
enum states{START,INID,INNUM,DONE,INASSIGN,INCOMMENT,INRANGE,INCHAR,ERRORSTATE};

// 声明保留字查找函数
void formToken(string word,int linenum) {
	//LexType mem = reservedLookup(word);
	if (reserved_words.count(word)) {
		cout << linenum << " 保留字 " << reserved_words[word] << " " << "无" << endl;
	}
	else {
		cout << linenum << " 标识符 " << "ID" << " " << word << endl;
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
// 输入一行，识别单词，并输出该行的单词的token
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
		// 以字母开头，可能为保留字，可能为标识符，
		if (str[index] >= 'a' && str[index] <= 'z') {
			tmp += str[index];
			index++;
			while (index < len && isCharOrNum(str[index]))tmp += str[index++];
			formToken(tmp,linenum);
			tmp = "";
			continue;
		}
		// 以数字开头
		else if (str[index] >= '0' && str[index] <= '9') {
			tmp += str[index];
			index++;
			while (index < len && str[index] >= '0' && str[index] <= '9')tmp += str[index++];
			cout << linenum << " 无符号整数 " << "INTC" << " " << tmp << endl;
			tmp = "";
			continue;
		}
		// 以 : 开头
		else if (str[index] == ':') {
			if (str[++index] == '=') {
				cout << linenum << " 双分界符 " << "ASSIGN(:=)" << " " << "无" << endl;
			}
			else {
				cout << "error! 第 " << linenum << " 赋值符号出错。" << endl;
			}
			index++;
			continue;
		}
		// 以 { 开头 ，注释部分，直接忽略
		// 暂未考虑 无右括号 的错误处理 ？？？
		else if (str[index] == '{') {
			while (index < len && str[index] != '}')index++;
			index++;
			continue;
		}
		// 以 . 开头
		else if (str[index] == '.') {
			if ((index+1)<len && str[++index] == '.') { // 可能有错误？？？
				cout << linenum << " 数组下标界限符 " << "UNDERANGE(..)" << " " << "无" << endl;
				index++;
				continue;
			}
			else {
				cout << linenum << " 程序结束 " << "END" << " " << "无" << endl;
				break;
			}
		}
		// 单字符分界符
		else{
			string mem = SCDelimiter(str[index]);
			if (mem != "ERROR") {
				cout<< linenum << " 单字符分界符 " << mem << " " << "无" << endl;
			}
			else {
				cout << "error! 第 " << linenum << " 单字符分界符出错。" << endl;
			}
			index++;
			continue;
		}
	}

}
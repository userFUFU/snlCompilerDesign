#pragma once
#include "string" 
#include "stdio.h"
#include "map"
using namespace std;

typedef enum{
	/* */
	ENDFILE, ERROR,
	/* 保留字 */
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO,
	ENDWH, BEGIN1, END1, READ, WRITE,
	ARRAY, OF, RECORD, RETURN1,
	/* 类型 */
	INTEGER, CHAR1,
	/* 多字符单词符号 */
	ID, INTC, CHARC,
	/* 单分界字符 */
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE
} LexType;

//保留字映射
map<string, string> reserved_words = {
	{"program","PROGRAM"},{"type","TYPE"},{"var","VAR"},
	{"procedure","PROCEDURE"},{"begin","BEGIN1"},{"end","END1"},
	{"array","ARRAY"},{"of","OF"},{"record","RECORD"},
	{"if","IF"},{"then","THEN"},{"else","ELSE"},{"fi","FI"},
	{"while","WHILE"},{"do","DO"},{"endwh","ENDWH"},
	{"read","READ"},{"write","WRITE"},{"return","RETURN1"},
	{"integer","INTEGER"},{"char","CHAR1"} 
};



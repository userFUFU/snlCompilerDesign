#pragma once
#include<bits/stdc++.h>
#include "LEXsnl.h"
using namespace std;

typedef struct{
	LexType type_inf;
	string content;
	int linenum;
	int tokennum;
}Token;

extern bool endfile;
int getToken(string str,int linenum,Token* token,int tokencnt);



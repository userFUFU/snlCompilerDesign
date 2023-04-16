#include<bits/stdc++.h>
#include "semantemeDesign.h"
using namespace std;
#define maxn 1010
#define initOFF 7

smtTable* scope[maxn];
vector<smtTable*> scope_vec;

int level = 0;
int offset = 0; // 一层的偏移
int mOFF = 0; // 主程序的偏移
int noff = 0; // 过程活动记录大小记录
typeIR* boolptr = NULL;
typeIR* intptr = NULL;
typeIR* charptr = NULL;


void smtMain(grammarTreeNode* r) {
	createTable(); //创建表
	init();

	grammarTreeNode* p = r->child[1]; //语法树的声明节点
	while (p) {
		switch (p->nodeKind){
		case TypeK:
			typeDecPart(p->child[0]);
			break;
		case VarK:
			varDecList(p->child[0]);
			break;
		case ProcDecK:
			procDecPart(p);
			break;
		default:
			break;
		}
		p = p->sibling;
	}

	p = r->child[2]; // 语法树的程序体节点
	if (p->nodeKind == StmLK) {
		Body(p);
	}
	cout << endl << endl;
	printSymbTable();
	if (level != -1) {
		DestroyTable();
		//level--; // 撤销
	}

}
void init() {
	intptr = intType();
	charptr = charType();
	boolptr = boolType();
	//for (int i = 0; i < maxn; i++)scope[i] = NULL;
}
// charPtr
typeIR* charType() {
	typeIR* t = (typeIR*)new typeIR();
	t->size = 1;
	t->type = charTy;
	return t;
}
// intPtr
typeIR* intType() {
	typeIR* t = (typeIR*)new typeIR();
	t->size = 1;
	t->type = intTy;
	return t;
}
// boolPtr
typeIR* boolType() {
	typeIR* t = (typeIR*)new typeIR();
	t->size = 1;
	t->type = boolTy;
	return t;
}

// 处理类型声明
void typeDecPart(grammarTreeNode* r) {
	int present = 0;
	AttributeIR attr;
	smtTable* entry = NULL;
	attr.mykind = typeKind;

	while (r != NULL) {
		present = Enter(r->name[0],&attr,&entry);
		if (present) {
			cout<<r->linenum<<" "<<r->name[0]<<" Repeat declaration" << endl;
			entry = NULL;
		}
		else {
			entry->attrIR.myType = typeProcess(r, r->kind.dec);
		}
		r = r->sibling;
	}
}
// 处理语法树当前节点
typeIR* typeProcess(grammarTreeNode* t, DecKind deckind) {
	typeIR* ptr = NULL;
	switch (deckind){
	case ArrayK:
		ptr = arrayType(t);
		break;
	case CharK:
		ptr = charptr;
		break;
	case IntegerK:
		ptr = intptr;
		break;
	case RecordK:
		ptr = recordType(t);
		break;
	case DecIdK:
		ptr = nameType(t);
		break;
	default:
		break;
	}
	return ptr;
}
// 检查下标是否合法, up >=low
typeIR* arrayType(grammarTreeNode* r) {
	typeIR* t = (typeIR*)new typeIR();
	if (r->Attr.arrayAttr.up < r->Attr.arrayAttr.low) {
		cout << r->linenum << " array: up is lower then low." << endl;
	}
	else {
		t->type = arrayTy;
		t->arrayAttr.up = r->Attr.arrayAttr.up;
		t->arrayAttr.low = r->Attr.arrayAttr.low;

		typeIR* lTy = typeProcess(r, IntegerK);
		t->arrayAttr.indexTy = lTy;
		typeIR* membTy = typeProcess(r, r->Attr.arrayAttr.arrayKind);
		t->arrayAttr.elemTy = membTy;
		// array[0..5] of integer  -> size:6
		t->size = (r->Attr.arrayAttr.up - r->Attr.arrayAttr.low+1) * (membTy->size);
	}
	return t;
}
// 记录属性 size recordTy body
typeIR* recordType(grammarTreeNode* r){
	typeIR* t = (typeIR*)new typeIR();
	r = r->child[0];
	//循环处理域变量
	fieldChain* ptr = NULL;
	fieldChain* pre = NULL;
	fieldChain* head = NULL;
	while (r != NULL) {
		for (int i = 0; i < r->nameNum; i++) {
			ptr =(fieldChain*) new fieldChain();
			if (head == NULL)head = ptr, pre = ptr;
			ptr->name = r->name[i];
			ptr->unitTy = typeProcess(r, r->kind.dec);
			ptr->next = NULL;

			if (ptr != pre) {
				ptr->off = pre->off + ptr->unitTy->size;
				pre->next = ptr;
				pre = ptr;
			}
		}
		r = r->sibling;
	}
	t->size = ptr->off + ptr->unitTy->size;
	t->body = head;
	return t;

}
// 寻找已定义类型名
typeIR* nameType(grammarTreeNode* r) {
	smtTable* entry = NULL;
	int present = FindEntry(r->typeName, &entry);
	typeIR* Eptr = NULL;
	if (present) {
		if (entry->attrIR.mykind == typeKind) {
			Eptr = entry->attrIR.myType;
		}
		else {
			cout<< r->linenum << " " << r->typeName << " this name is used." << endl;
		}
	}
	else {
		cout << r->linenum << " " << r->typeName << " type name without declaration." << endl;
	}
	return Eptr;
}

// 处理变量声明
void varDecList(grammarTreeNode* r) {
	int present = 0;
	AttributeIR attr;
	smtTable* entry = NULL;
	while (r != NULL) {
		attr.mykind = varKind;
		for (int i = 0; i < (r->nameNum); i++) {
			attr.myType = typeProcess(r, r->kind.dec);
			if (r->Attr.paramAttr.paramType == varparamType) { //变参 偏移+1
				attr.Attr.varAttr.access = indir;
				attr.Attr.varAttr.level = level;
				attr.Attr.varAttr.off = noff; //
				attr.offset = offset;
				offset += 1;

			}
			else { //值参或变量 偏移+size
				attr.Attr.varAttr.access = dir;
				attr.Attr.varAttr.level = level;
				if (attr.myType != NULL) {
					attr.offset = offset;
					offset += attr.myType->size;
				}
			}
			present = Enter(r->name[i], &attr, &entry);
			if (present) {
				cout << r->linenum << " " << r->name[i] << " Repeat declaration" << endl;
			}
			else {
				entry->level = level;
				scope_vec.push_back(entry);
				r->table[i] = entry;
			}
		}
		r = r->sibling;
	}
	if (level == 1) {

	}
	else {
		noff = offset;
	}
}

// 处理过程声明
void procDecPart(grammarTreeNode* r) {
	smtTable* entry = HeadProcess(r);

	grammarTreeNode* p = r->child[1];
	while (p != NULL) {
		switch (p->nodeKind){
		case TypeK:
			typeDecPart(p->child[0]);
			break;
		case VarK:
			varDecList(p->child[0]);
			break;
		case ProcDecK:
			break;
		default:
			cout << p->linenum << " " << "without kind" << endl;
			break;
		}
		if (p->nodeKind == ProcDecK)break;
		else p = p->sibling;
	}

	entry->attrIR.Attr.procAttr.off = noff;
	// entry->attrIR.Attr.procAttr.off = entry->attrIR.Attr.procAttr.off + entry->attrIR.Attr.procAttr.level + 1;
	while (p!=NULL){ //ProcDecK
		procDecPart(p);
		p = p->sibling;
	}
	
	Body(r->child[2]);

	// 删除本函数本层符号表
	if (level > 0)DestroyTable();
}
// 处理函数头，往当前层填写函数信息，在新的一层填写函数内部的信息
smtTable*  HeadProcess(grammarTreeNode* r) {
	int present = 0;
	AttributeIR attr;
	smtTable* entry = NULL;

	attr.mykind = procKind;
	attr.myType = NULL; // 返回值类型
	attr.offset = offset;
	attr.Attr.procAttr.level = level + 1;
	// 登记符号表项
	if (r != NULL) {
		present = Enter(r->name[0], &attr, &entry);
		entry->level = level;
		scope_vec.push_back(entry);
		r->table[0] = entry;
	}
	entry->attrIR.Attr.procAttr.param = paramDecList(r);
	return entry;
}
// 形参处理函数
paramTable* paramDecList(grammarTreeNode* r) {
	grammarTreeNode* p = NULL;
	paramTable* head = NULL;
	if (r == NULL)return head;
	if (r->child[0] != NULL)p = r->child[0];
	createTable();

	varDecList(p);

	smtTable* ptr = scope[level];
	paramTable* paramP = NULL;
	paramTable* pre = NULL;
	while (ptr != NULL) {
		paramP = (paramTable*)new paramTable();
		// 第一项
		if (head == NULL)head = paramP, pre = paramP;
		paramP->entry = ptr;
		paramP->next = NULL;
		if (paramP != pre)pre->next = paramP, pre = paramP;

		// 继续看下一个形参
		ptr = ptr->next;
	}
	noff = offset; // proc AR size
	return head;
}
// 处理程序体部分
void Body(grammarTreeNode* r) {
	if (r == NULL)return;
	grammarTreeNode* p;
	if (r->nodeKind == StmLK) {
		p = r->child[0];
		while (p != NULL) {
			statement(p); //处理语句节点
			p = p->sibling;
		}
	}
}
void statement(grammarTreeNode* r) { 
	switch (r->kind.stmt){
	case IfK:ifStatement(r); break;
	case WhileK:whileStatement(r); break;
	case AssignK:assignStatement(r); break;
	case ReadK:readStatement(r); break;
	case WriteK:writeStatement(r); break;
	case CallK:callStatement(r); break;
	case ReturnK:returnStatement(r); break;
	default:
		cout << r->linenum << " without statement" << endl;
		break;
	}
}
// 检查运算分量是否相容
typeIR* Expr(grammarTreeNode* r, AccessKind* Ekind) {
	typeIR* Eptr = NULL;

	switch (r->kind.exp){
	case ConstK:
	{
		Eptr = typeProcess(r, IntegerK);
		Eptr->type = intTy;
		if (Ekind != NULL)(*Ekind) = dir;
		break;
	}
	case ExpIdK: // 处理变量
	{
		if (r->child[0] == NULL) { // var = id
			smtTable* entry = NULL;
			int present = FindEntry(r->name[0], &entry);
			if (present) {
				if (entry->attrIR.mykind == varKind) {
					Eptr = entry->attrIR.myType;
					if(Ekind!=NULL)(*Ekind) = dir;
				}
				else {// 不是一个varkind
					cout << r->linenum << " " << r->name[0] << " is not a varkind." << endl;
				}
			}
			else { // 未声明
				cout << r->linenum << " " << r->name[0] << " is undefined." << endl;
			}
		}
		else {
			if (r->Attr.expAttr.varKind == ArrayMembV) {
				Eptr = arrayVar(r);
			}
			else if(r->Attr.expAttr.varKind == FieldMembV){ // 域成员
				Eptr = recordVar(r);
			}
		}
		break;
	}
	case OpK: // 处理操作符
	{
		typeIR* child0 = Expr(r->child[0], NULL);
		typeIR* child1 = Expr(r->child[1], NULL);
		if (child0 == NULL || child1 == NULL)return NULL;
		if (child0 == child1) { // 判断内部产生的指针值是否相同
			switch (r->Attr.expAttr.op){
			case LT:Eptr = boolptr; break;
			case EQ:Eptr = boolptr; break;
			case PLUS:Eptr = intptr; break;
			case MINUS:Eptr = intptr; break;
			case TIMES:Eptr = intptr; break;
			case OVER:Eptr = intptr; break;
			default:
				cout << r->linenum << " unknown operator." << endl;
				break;
			}
			if (Ekind != NULL)(*Ekind) = dir;
		}
		else {
			cout<< r->linenum << " operator is uncompat." << endl;
		}
		break;
	}
	}
	return Eptr;
}
// if 语句
void ifStatement(grammarTreeNode* r) {
	typeIR* flag = Expr(r->child[0], NULL);
	if (flag->type != boolTy) {
		cout << r->linenum << " condition expression type is not boolTy" << endl;
	}
	else { // 处理then else
		grammarTreeNode* p = r->child[1];
		while (p != NULL) {
			statement(p);
			p = p->sibling;
		}
		p = r->child[2];
		while (p != NULL) {
			statement(p);
			p = p->sibling;
		}
	}
}
// while语句
void whileStatement(grammarTreeNode* r) {
	typeIR* flag = Expr(r->child[0], NULL);
	if (flag == NULL)return;
	if(flag->type!=boolTy) {
		cout << r->linenum << " condition expression type is not boolTy" << endl;
	}
	else { // 处理循环体部分
		grammarTreeNode* p = r->child[1];
		while (p != NULL) {
			statement(p);
			p = p->sibling;
		}
	}
}
// assign语句 检查左右两端分量是否相容
void assignStatement(grammarTreeNode* r) {
	grammarTreeNode* child1 = r->child[0];
	grammarTreeNode* child2 = r->child[1];
	smtTable* entry=NULL;
	typeIR* Eptr = NULL;
	if (child1->child[0] == NULL) {
		int present = FindEntry(child1->name[0], &entry);
		if (present) {
			if (entry->attrIR.mykind == varKind) {
				Eptr = entry->attrIR.myType;
				child1->table[0] = entry;
			}
			else {
				cout << child1->linenum <<" " << child1->name[0] << " is not a varKind." << endl;
			}
		}
		else {
			cout << child1->linenum<<" " << child1->name[0] << " id without declaration." << endl;
		}
	}
	else {
		if (child1->Attr.expAttr.varKind == ArrayMembV)Eptr = arrayVar(child1);
		else if(child1->Attr.expAttr.varKind == FieldMembV)Eptr = recordVar(child1);
	}
	// 检查赋值号两侧类型相容
	if (Eptr != NULL) {
		typeIR* ptr = Expr(child2, NULL);
		if (ptr != Eptr) {
			cout << r->linenum << " type not match in assign expression." << endl;
		}
	}
}
// 是不是数组变量，下标是否类型匹配
typeIR* arrayVar(grammarTreeNode* r) {
	smtTable* entry = NULL;
	int present = FindEntry(r->name[0], &entry);
	r->table[0] = entry;

	typeIR* Eptr = NULL;
	if (!present) {
		cout << r->linenum << " " << r->name[0] << " without declaration." << endl;
	}
	else {
		if (entry->attrIR.myType->type != arrayTy) {
			cout<< r->linenum << " " << r->name[0] << " is not an array." << endl;
		}
		else {
			typeIR* rightTy = entry->attrIR.myType->arrayAttr.indexTy;
			typeIR* nowTy = Expr(r->child[0], NULL);
			if (rightTy != nowTy) {
				cout << r->linenum << " array member type is not match." << endl;
			}
			else {
				Eptr = entry->attrIR.myType->arrayAttr.elemTy;
			}
		}
	}
	return Eptr;
}
// 是不是记录变量，域成员是否类型匹配
typeIR* recordVar(grammarTreeNode* r) {
	smtTable* entry = NULL;
	int present = FindEntry(r->name[0], &entry);
	r->table[0] = entry;

	typeIR* Eptr = NULL;
	if (!present) {
		cout << r->linenum << " " << r->name[0] << " without declaration." << endl;
	}
	else {
		if (entry->attrIR.myType->type != recordTy) {
			cout << r->linenum << " " << r->name[0] << " is not an record." << endl;
		}
		else {
			typeIR* rightTy = entry->attrIR.myType;
			fieldChain* curP = rightTy->body;
			int flag = 1;
			while (curP != NULL) {
				if (r->child[0]->name[0] == curP->name) {
					curP = curP->next;
				}
				else {
					Eptr = curP->unitTy;
					flag = 0;
					break;
				}
			}
			if (curP == NULL) {
				if (flag) {
					cout << r->child[0]->linenum << " " << r->child[0]->name[0] << " is not a field type." << endl;
				}
			}
			/*
			else if (r->child[0]->child[0] != NULL) {

			}*/
		}
	}
	return Eptr;
}

// read语句 检查要读入的变量有无声明
void readStatement(grammarTreeNode* r) {
	smtTable* entry = NULL;
	int present = FindEntry(r->name[0], &entry);
	r->table[0] = entry;
	if (present) {
		if (entry->attrIR.mykind != varKind) {
			cout<<r->linenum<<" "<<r->name[0]<<" is not a varKind." << endl;
		}
	}
	else {
		cout<< r->linenum << " " << r->name[0] << " id without declaration." << endl;
	}
}
// write语句 分析表达式是否合法
void writeStatement(grammarTreeNode* r) {
	typeIR* Eptr = Expr(r->child[0], NULL);
	if (Eptr->type == boolTy) {
		cout << r->linenum  << " wrong expression type in write." << endl;
	}
}
// call语句 检形参与实参之间的对应关系
void callStatement(grammarTreeNode* r) {
	smtTable* entry = NULL;
	int present = FindEntry(r->child[0]->name[0], &entry);
	r->child[0]->table[0] = entry;

	if (!present) {
		cout << r->linenum << " " << r->child[0]->name[0] << " functon without declaration." << endl;
	}
	else {
		if (entry->attrIR.mykind != procKind) {
			cout<< r->linenum << " " << r->name[0] << " is not a function." << endl;
		}
		else {
			// 检查实参形参是否类型匹配
			grammarTreeNode* p = r->child[1];
			paramTable* paramP = entry->attrIR.Attr.procAttr.param;
			AccessKind Ekind;
			while (p != NULL && paramP != NULL) {
				smtTable* pEntry = paramP->entry;
				typeIR* Ety = Expr(p, &Ekind);
				if (pEntry->attrIR.Attr.varAttr.access == indir && Ekind == dir) {
					cout << p->linenum << " param kind is not match." << endl;
				}
				else if (pEntry->attrIR.myType != Ety) {
					cout<< p->linenum << " param type is not match." << endl;
				}
				p = p->sibling;
				paramP = paramP->next;
			}
			if (p != NULL || paramP != NULL) {
				cout << r->child[1]->linenum << " param number is not match." << endl;
			}
		}
	}
}
// return语句 返回语句不允许出现在主程序中
void returnStatement(grammarTreeNode* r) {
	if (level == 0) {
		cout << r->linenum << " wrong place of return." << endl;
	}
}


// 遇到新的无声明的标识符时创建一个空的符号表
smtTable* newTable() {
	smtTable* t = (smtTable*)new smtTable();
	if (t == NULL) {
		cout << "Error! " << "Out of memory." << endl;
	}
	return t;
}

// 进入一个新的局部化单位时创建空符号表 
void createTable() {
	level++;
	offset = initOFF;
	scope[level] = NULL;
}
// 退出一个局部化区时,撤销当前符号表
void DestroyTable(){
	level--;
}

// 查找当前id是否已声明
int FindEntry(string id, smtTable** entry) {
	int tmp = level;
	while (tmp > 0) {
		int present = searchOneTable(id,tmp,entry);
		if (present) {
			return 1;
		}
		else {
			tmp--;
		}
	}
	//*entry = NULL;
	return 0;
}
// 查找id是否已在当前层声明
int searchOneTable(string id, int now, smtTable** entry) {
	smtTable* tmp = scope[now];
	while (tmp != NULL) {
		if (tmp->name == id) { 
			*entry = tmp;
			return 1; 
		}
		else {
			tmp = tmp->next;
		}
	}
	return 0;
}

// 查找域名
bool findField(string id, fieldChain* head, fieldChain** entry) {
	fieldChain* tmp = head;
	while (tmp != NULL) {
		if (tmp->name == id) {
			*entry = tmp;
			return true;
		}
		else {
			tmp = tmp->next;
		}
	}
	*entry = NULL;
	return false;
}

// 登记标识符和属性
bool Enter(string id, AttributeIR* attribP, smtTable** entry) {
	int present = searchOneTable(id,level,entry); // 检查在本层是否已声明
	smtTable *cur=NULL, *pre=NULL;
	if (present) {
		// 标识符重复声明
		cout << "Error! " << id << " Repeat declaration" << endl;
		*entry = NULL;
	}
	else {
		cur = newTable(); // 建立一个新表项

		pre = scope[level];
		if (scope[level] == NULL) {
			scope[level] = cur;
		}
		else {
			while (pre->next != NULL)pre = pre->next;
			pre->next = cur;
		}
	}
	cur->name = id;
	cur->attrIR.myType = attribP->myType;
	cur->attrIR.mykind = attribP->mykind;
	cur->attrIR.offset = attribP->offset; // ...
	switch (attribP->mykind) {
	case typeKind:
		break;
	case varKind:
		cur->attrIR.Attr.varAttr.access = attribP->Attr.varAttr.access;
		cur->attrIR.Attr.varAttr.level = attribP->Attr.varAttr.level;
		cur->attrIR.Attr.varAttr.off = attribP->Attr.varAttr.off;
		break;
	case procKind:
		cur->attrIR.Attr.procAttr.level = attribP->Attr.procAttr.level;
		cur->attrIR.Attr.procAttr.param = attribP->Attr.procAttr.param;
		break;
	default:
		break;
	}
	*entry = cur;
	return present;
}



// 打印符号表 ----TO DO!----
void printSymbTable() {
	int lev = 1;
	cout << "level\t" << "name\t" << "type\t" << "kind\t" << "offset\t" << "dir\t" << "noff\t" << endl;
	for(auto now : scope_vec) {
		cout << now->level << "\t" << now->name << "\t";
		// type
		if (now->attrIR.myType == NULL) {
			cout << "void\t";
		}
		else {
			switch (now->attrIR.myType->type) {
			case intTy:
				cout << "intTy\t";
				break;
			case charTy:
				cout << "charTy\t";
				break;
			case arrayTy:
				cout << "arrayTy\t";
				break;
			case recordTy:
				cout << "recordTy\t";
				break;
			default:
				cout << now->name << " type error.";
				break;
			}
		}
		// kind
		switch (now->attrIR.mykind) {
		case typeKind:
			cout << "typeKind\t";
			break;
		case varKind:
			cout << "varKind\t" << now->attrIR.offset << "\t";
			switch (now->attrIR.Attr.varAttr.access) {
			case dir:
				cout << "dir\t";
				break;
			case indir:
				cout << "indir\t";
				break;
			default:
				cout << now->name << " access error.";
				break;
			}
			break;
		case procKind:
			cout << "procKind" << "\t\t" << now->attrIR.Attr.procAttr.off << "\t";
			break;
		default:
			cout << now->name << " kind error.";
			break;
		}

		cout << endl;
		
	}
}

void printSymbTable_old() {
	int lev = 1;
	smtTable* now;
	cout << "level\t" << "name\t" << "type\t" << "kind\t" << "offset\t" << "dir\t" << "noff\t" << endl;
	while (scope[lev] != NULL) {
		now = scope[lev];
		while (now != NULL) {
			cout << lev << "\t" << now->name << "\t";
			// type
			if (now->attrIR.myType == NULL) {
				cout << "void\t";
			}
			else {
				switch (now->attrIR.myType->type) {
				case intTy:
					cout << "intTy\t";
					break;
				case charTy:
					cout << "charTy\t";
					break;
				case arrayTy:
					cout << "arrayTy\t";
					break;
				case recordTy:
					cout << "recordTy\t";
					break;
				default:
					cout << now->name << " type error.";
					break;
				}
			}
			// kind
			switch (now->attrIR.mykind) {
			case typeKind:
				cout << "typeKind\t";
				break;
			case varKind:
				cout << "varKind\t" << now->attrIR.offset << "\t";
				switch (now->attrIR.Attr.varAttr.access) {
				case dir:
					cout << "dir\t";
					break;
				case indir:
					cout << "indir\t";
					break;
				default:
					cout << now->name << " access error.";
					break;
				}
				break;
			case procKind:
				cout << "procKind" << "\t\t" << now->attrIR.Attr.procAttr.off << "\t";
				break;
			default:
				cout << now->name << " kind error.";
				break;
			}

			cout << endl;
			now = now->next;
		}
		lev++;
	}
}

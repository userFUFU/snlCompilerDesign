#include<bits/stdc++.h>
#include "semantemeDesign.h"
using namespace std;
#define maxn 1010
#define initOFF 7

smtTable* scope[maxn];
vector<smtTable*> scope_vec;

int level = 0;
int offset = 0; // һ���ƫ��
int mOFF = 0; // �������ƫ��
int noff = 0; // ���̻��¼��С��¼
typeIR* boolptr = NULL;
typeIR* intptr = NULL;
typeIR* charptr = NULL;


void smtMain(grammarTreeNode* r) {
	createTable(); //������
	init();

	grammarTreeNode* p = r->child[1]; //�﷨���������ڵ�
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

	p = r->child[2]; // �﷨���ĳ�����ڵ�
	if (p->nodeKind == StmLK) {
		Body(p);
	}
	cout << endl << endl;
	printSymbTable();
	if (level != -1) {
		DestroyTable();
		//level--; // ����
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

// ������������
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
// �����﷨����ǰ�ڵ�
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
// ����±��Ƿ�Ϸ�, up >=low
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
// ��¼���� size recordTy body
typeIR* recordType(grammarTreeNode* r){
	typeIR* t = (typeIR*)new typeIR();
	r = r->child[0];
	//ѭ�����������
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
// Ѱ���Ѷ���������
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

// �����������
void varDecList(grammarTreeNode* r) {
	int present = 0;
	AttributeIR attr;
	smtTable* entry = NULL;
	while (r != NULL) {
		attr.mykind = varKind;
		for (int i = 0; i < (r->nameNum); i++) {
			attr.myType = typeProcess(r, r->kind.dec);
			if (r->Attr.paramAttr.paramType == varparamType) { //��� ƫ��+1
				attr.Attr.varAttr.access = indir;
				attr.Attr.varAttr.level = level;
				attr.Attr.varAttr.off = noff; //
				attr.offset = offset;
				offset += 1;

			}
			else { //ֵ�λ���� ƫ��+size
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

// �����������
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

	// ɾ��������������ű�
	if (level > 0)DestroyTable();
}
// ������ͷ������ǰ����д������Ϣ�����µ�һ����д�����ڲ�����Ϣ
smtTable*  HeadProcess(grammarTreeNode* r) {
	int present = 0;
	AttributeIR attr;
	smtTable* entry = NULL;

	attr.mykind = procKind;
	attr.myType = NULL; // ����ֵ����
	attr.offset = offset;
	attr.Attr.procAttr.level = level + 1;
	// �ǼǷ��ű���
	if (r != NULL) {
		present = Enter(r->name[0], &attr, &entry);
		entry->level = level;
		scope_vec.push_back(entry);
		r->table[0] = entry;
	}
	entry->attrIR.Attr.procAttr.param = paramDecList(r);
	return entry;
}
// �βδ�����
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
		// ��һ��
		if (head == NULL)head = paramP, pre = paramP;
		paramP->entry = ptr;
		paramP->next = NULL;
		if (paramP != pre)pre->next = paramP, pre = paramP;

		// ��������һ���β�
		ptr = ptr->next;
	}
	noff = offset; // proc AR size
	return head;
}
// ��������岿��
void Body(grammarTreeNode* r) {
	if (r == NULL)return;
	grammarTreeNode* p;
	if (r->nodeKind == StmLK) {
		p = r->child[0];
		while (p != NULL) {
			statement(p); //�������ڵ�
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
// �����������Ƿ�����
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
	case ExpIdK: // �������
	{
		if (r->child[0] == NULL) { // var = id
			smtTable* entry = NULL;
			int present = FindEntry(r->name[0], &entry);
			if (present) {
				if (entry->attrIR.mykind == varKind) {
					Eptr = entry->attrIR.myType;
					if(Ekind!=NULL)(*Ekind) = dir;
				}
				else {// ����һ��varkind
					cout << r->linenum << " " << r->name[0] << " is not a varkind." << endl;
				}
			}
			else { // δ����
				cout << r->linenum << " " << r->name[0] << " is undefined." << endl;
			}
		}
		else {
			if (r->Attr.expAttr.varKind == ArrayMembV) {
				Eptr = arrayVar(r);
			}
			else if(r->Attr.expAttr.varKind == FieldMembV){ // ���Ա
				Eptr = recordVar(r);
			}
		}
		break;
	}
	case OpK: // ���������
	{
		typeIR* child0 = Expr(r->child[0], NULL);
		typeIR* child1 = Expr(r->child[1], NULL);
		if (child0 == NULL || child1 == NULL)return NULL;
		if (child0 == child1) { // �ж��ڲ�������ָ��ֵ�Ƿ���ͬ
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
// if ���
void ifStatement(grammarTreeNode* r) {
	typeIR* flag = Expr(r->child[0], NULL);
	if (flag->type != boolTy) {
		cout << r->linenum << " condition expression type is not boolTy" << endl;
	}
	else { // ����then else
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
// while���
void whileStatement(grammarTreeNode* r) {
	typeIR* flag = Expr(r->child[0], NULL);
	if (flag == NULL)return;
	if(flag->type!=boolTy) {
		cout << r->linenum << " condition expression type is not boolTy" << endl;
	}
	else { // ����ѭ���岿��
		grammarTreeNode* p = r->child[1];
		while (p != NULL) {
			statement(p);
			p = p->sibling;
		}
	}
}
// assign��� ����������˷����Ƿ�����
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
	// ��鸳ֵ��������������
	if (Eptr != NULL) {
		typeIR* ptr = Expr(child2, NULL);
		if (ptr != Eptr) {
			cout << r->linenum << " type not match in assign expression." << endl;
		}
	}
}
// �ǲ�������������±��Ƿ�����ƥ��
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
// �ǲ��Ǽ�¼���������Ա�Ƿ�����ƥ��
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

// read��� ���Ҫ����ı�����������
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
// write��� �������ʽ�Ƿ�Ϸ�
void writeStatement(grammarTreeNode* r) {
	typeIR* Eptr = Expr(r->child[0], NULL);
	if (Eptr->type == boolTy) {
		cout << r->linenum  << " wrong expression type in write." << endl;
	}
}
// call��� ���β���ʵ��֮��Ķ�Ӧ��ϵ
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
			// ���ʵ���β��Ƿ�����ƥ��
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
// return��� ������䲻�����������������
void returnStatement(grammarTreeNode* r) {
	if (level == 0) {
		cout << r->linenum << " wrong place of return." << endl;
	}
}


// �����µ��������ı�ʶ��ʱ����һ���յķ��ű�
smtTable* newTable() {
	smtTable* t = (smtTable*)new smtTable();
	if (t == NULL) {
		cout << "Error! " << "Out of memory." << endl;
	}
	return t;
}

// ����һ���µľֲ�����λʱ�����շ��ű� 
void createTable() {
	level++;
	offset = initOFF;
	scope[level] = NULL;
}
// �˳�һ���ֲ�����ʱ,������ǰ���ű�
void DestroyTable(){
	level--;
}

// ���ҵ�ǰid�Ƿ�������
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
// ����id�Ƿ����ڵ�ǰ������
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

// ��������
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

// �ǼǱ�ʶ��������
bool Enter(string id, AttributeIR* attribP, smtTable** entry) {
	int present = searchOneTable(id,level,entry); // ����ڱ����Ƿ�������
	smtTable *cur=NULL, *pre=NULL;
	if (present) {
		// ��ʶ���ظ�����
		cout << "Error! " << id << " Repeat declaration" << endl;
		*entry = NULL;
	}
	else {
		cur = newTable(); // ����һ���±���

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



// ��ӡ���ű� ----TO DO!----
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

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <algorithm> 
using namespace std;

typedef enum { BLACK, RED } node_c;

typedef struct Node_ {
	struct Node_ *_left;        // ����� ������
	struct Node_ *_right;       // ������ �������
	struct Node_ *_parent;      // ��������
	node_c _color;              // ������
	int _data;                  // ����
} Node;

#define NIL &leaf                      // ������
Node leaf = { NIL, NIL, 0, BLACK, 0 }; 

class RBT {
private:
	Node *_root; // ������
	int _count;  // ���-�� ���������(�����), �������� ������
public:

	/*����������� ��� ���������*/
	RBT(){
		this->_root = NIL;
		this->_count = 0;
	}

	/*�������� ������ ������*/
	Node* GetRoot() { return _root; }

	/*�������� ���������� ���������*/
	int GetCount() { return _count; }

	/*����� �������*/
	void RotateLeft(Node *x) {

		Node *y = x->_right;

		x->_right = y->_left;
		if (y->_left != NIL)
			y->_left->_parent = x;

		if (y != NIL)
			y->_parent = x->_parent;
		if (x->_parent) {
			if (x == x->_parent->_left)
				x->_parent->_left = y;
			else
				x->_parent->_right = y;
		}
		else {
			_root = y;
		}

		y->_left = x;
		if (x != NIL) x->_parent = y;
	}

	/*������ �������*/
	void RotateRight(Node *x) {
		Node *y = x->_left;
		x->_left = y->_right;
		if (y->_right != NIL) y->_right->_parent = x;
		if (y != NIL) y->_parent = x->_parent;
		if (x->_parent) {
			if (x == x->_parent->_right)
				x->_parent->_right = y;
			else
				x->_parent->_left = y;
		}
		else {
			_root = y;
		}

		y->_right = x;
		if (x != NIL) x->_parent = y;
	}

	/*������������ ����� �������*/
	void Balancing(Node *x) {

		// �������� �������
		while (x != _root && x->_parent->_color == RED) {

			//  ����������� ������� � ����� ���������
			if (x->_parent == x->_parent->_parent->_left) {
				Node *y = x->_parent->_parent->_right;
				if (y->_color == RED) {
					// #1 ������ 
					// ������� ����
					x->_parent->_color = BLACK;
					y->_color = BLACK;
					x->_parent->_parent->_color = RED;
					x = x->_parent->_parent;
				}
				else {
					// #2 ������
					// ���� ������, x - ����� ��� ������ ����
					if (x == x->_parent->_right) {
						x = x->_parent;
						RotateLeft(x);
					}

					//#3 ������
					// ���� ������, � - ����� ��� ������ ����
					x->_parent->_color = BLACK;
					x->_parent->_parent->_color = RED;
					RotateRight(x->_parent->_parent);
				}
			}
			else { // � - � ������ ���������

				Node *y = x->_parent->_parent->_left;
				if (y->_color == RED) {
					//#4 ������ 
					// ������� ����
					x->_parent->_color = BLACK;
					y->_color = BLACK;
					x->_parent->_parent->_color = RED;
					x = x->_parent->_parent;
				}
				else {
					// #5 ������
					// ������ ����, � - ����� ��� ������ ����
					if (x == x->_parent->_left) {
						x = x->_parent;
						RotateRight(x);
					}
					//#6 ������
					// ������ ����, � - ������ ��� ������ ����
					x->_parent->_color = BLACK;
					x->_parent->_parent->_color = RED;
					RotateLeft(x->_parent->_parent);
				}
			}
		}
		_root->_color = BLACK;
	}


	/*������� ��������*/
	Node *Paste(int _data) {
		Node *current, *_parent, *x;

		// ���� ����� ��� �������
		current = _root;
		_parent = 0;

		while (current != NIL) {
			if (_data == current->_data) return current;
			_parent = current;
			current = _data  <  current->_data ?
				current->_left : current->_right;
		}

		//�������� ������ ��������
		x = new Node;
		x->_data = _data;
		x->_parent = _parent;
		x->_left = NIL;
		x->_right = NIL;
		x->_color = RED;
		_count++;

		//������� ��������
		if (_parent) {
			if (_data  < _parent->_data) // ������ �������� - �����, ����� - ������
				_parent->_left = x;
			else
				_parent->_right = x;
		}
		else {
			_root = x; // ������� � ������
		}

		Balancing(x); // ����������� �����������
		return(x);
	}


	/*����� ������*/
	void Print(Node *x, FILE *ptr) {
		if (x == NIL) return;
		Print(x->_left, ptr);
		if (x->_parent != NULL){
			fprintf(ptr, " %d -> %d\n", x->_parent->_data, x->_data);
		}
		Print(x->_right, ptr);
	}

	/*�������������� �������� � dot ������*/
	void DotNode(FILE* ptr, Node* x) {
		if (x == NIL) return;

		DotNode(ptr, x->_left);

		if (x->_color == RED)
			fprintf(ptr, " %d[style=\"filled\",fillcolor=\"red\", ", x->_data);
		else
			fprintf(ptr, " %d[style=\"filled\",fillcolor=\"black\", ", x->_data);

		fprintf(ptr, "label=%d]\n", x->_data);

		DotNode(ptr, x->_right);
	}

	/*������������ dot �����*/
	void DotTree(Node* x, int i) {
		string s;
		s = "g";
		s += (i == 1) ? "1" : ((i == 2) ? "2" : "3" );
		s += ".dot";
		FILE* ptr = fopen(s.c_str(),"w");
		s = "digraph graph";
		s += (i == 1) ? "One" : ((i == 2) ? "Two" :"Result" );
		s += "{\n";
		fprintf(ptr, s.c_str());
		fprintf(ptr, " node[fontcolor=""white"",fontsize=%d,shape=""rectangle""];\n", 20);
		Print(x, ptr);
		DotNode(ptr, x);
		fprintf(ptr, "}\n");
		fclose(ptr);
		s = "dot -Tpng g";
		s += (i == 1) ? "1" : ((i == 2) ? "2" : "3");
		s += ".dot -o ";
		s += (i == 1) ? "firstGraph" : ((i == 2) ? "secondGraph" : "ResultGraph");
		s += ".png";
		system(s.c_str());
	}

	/*����� �������� � ������� ������*/
	Node *FindNode(int data, Node *current) {

		while (current != NIL)
			if (data == current->_data)
				return current;
			else
				current = data < current->_data ?
				current->_left : current->_right;
		return NULL;
	}

	/*����� ������������ �����*/
	int MinKey(Node *current) {
		if (current->_left != NIL)
			return MinKey(current->_left);
		else 
			return current->_data;
	}

};

/*����� ������������ �������� �� ��������� � ������� ������*/
int PasteKey(vector<int> &f, vector<int> &s) {
	for (int i = 0; i < f.size(); i++) 
		for (int j = 0; j < s.size(); j++)
			if (f[i] == s[j])
				s.erase(s.begin() + j);

	sort(s.begin(), s.end());

	if(s.size() > 0)
		return s[0];
	else {
		sort(f.begin(), f.end());
		return f[0];
	}
}

/*�������� �������*/
void main(int argc, char **argv) {
	setlocale(LC_ALL, "rus");

	remove("firstGraph.png");
	remove("secondGraph.png");
	remove("ResultGraph.png");

	RBT *FIRST = new RBT(); // ������ �����
	RBT *SECOND = new RBT(); // ������ �����
	ifstream ifs;

	int tmp__data;
	vector<int> key_first;
	vector<int> key_second;

	ifs.open("f1.txt");
	//��������� ������
	for (int i = 0; !ifs.eof(); i++)
		if (ifs >> tmp__data){
			FIRST->Paste(tmp__data);
			key_first.push_back(tmp__data);
		}
	ifs.close();
	

	ifs.open("f2.txt");
	//��������� ������
	for (int i = 0; !ifs.eof(); i++)
		if (ifs >> tmp__data) {
			SECOND->Paste(tmp__data);
			key_second.push_back(tmp__data);
		}
	ifs.close();


	if (FIRST->GetCount() != 0 && SECOND->GetCount() != 0) {
		FIRST->DotTree(FIRST->GetRoot(), 1);
		SECOND->DotTree(SECOND->GetRoot(), 2);
	}
	else
		if (FIRST->GetCount() != 0 && SECOND->GetCount() == 0) {
			FIRST->DotTree(FIRST->GetRoot(), 1);
			FIRST->DotTree(FIRST->GetRoot(), 3);
			return;
		}
		else
			if (FIRST->GetCount() == 0 && SECOND->GetCount() != 0) {
				SECOND->DotTree(SECOND->GetRoot(), 2);
				SECOND->DotTree(SECOND->GetRoot(), 3);
				return;
			}
			else {
				cout << "��������� ����������! ������� �� �������� ���������!" << endl;
				system("pause");
				return;
			}

	//������� �������� � ������� ������
	if (FIRST->GetCount() > SECOND->GetCount()) {
		FIRST->Paste(PasteKey(key_first, key_second));
		FIRST->DotTree(FIRST->GetRoot(), 3);
	}
	else {
		SECOND->Paste(PasteKey(key_second, key_first));
		SECOND->DotTree(SECOND->GetRoot(), 3);
	}

	remove("g1.dot");
	remove("g2.dot");
	remove("g3.dot");

	system("pause");
}

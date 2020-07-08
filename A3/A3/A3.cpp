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
	struct Node_ *_left;        // левый ребёнок
	struct Node_ *_right;       // правый ребенок
	struct Node_ *_parent;      // родитель
	node_c _color;              // краска
	int _data;                  // ключ
} Node;

#define NIL &leaf                      // листок
Node leaf = { NIL, NIL, 0, BLACK, 0 }; 

class RBT {
private:
	Node *_root; // корень
	int _count;  // кол-во элементов(узлов), исключая листья
public:

	/*конструктор без параметра*/
	RBT(){
		this->_root = NIL;
		this->_count = 0;
	}

	/*получить корень дерева*/
	Node* GetRoot() { return _root; }

	/*получить количество элементов*/
	int GetCount() { return _count; }

	/*левый поворот*/
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

	/*правый поворот*/
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

	/*балансировка полсе вставки*/
	void Balancing(Node *x) {

		// проверка свойств
		while (x != _root && x->_parent->_color == RED) {

			//  добавляемый элемент в левом поддереве
			if (x->_parent == x->_parent->_parent->_left) {
				Node *y = x->_parent->_parent->_right;
				if (y->_color == RED) {
					// #1 СЛУЧАЙ 
					// красный дядя
					x->_parent->_color = BLACK;
					y->_color = BLACK;
					x->_parent->_parent->_color = RED;
					x = x->_parent->_parent;
				}
				else {
					// #2 СЛУЧАЙ
					// дядя черный, x - правй сын своего отца
					if (x == x->_parent->_right) {
						x = x->_parent;
						RotateLeft(x);
					}

					//#3 СЛУЧАЙ
					// дядя чёрный, х - левый сын своего отца
					x->_parent->_color = BLACK;
					x->_parent->_parent->_color = RED;
					RotateRight(x->_parent->_parent);
				}
			}
			else { // х - в правом поддереве

				Node *y = x->_parent->_parent->_left;
				if (y->_color == RED) {
					//#4 СЛУЧАЙ 
					// красный дядя
					x->_parent->_color = BLACK;
					y->_color = BLACK;
					x->_parent->_parent->_color = RED;
					x = x->_parent->_parent;
				}
				else {
					// #5 СЛУЧАЙ
					// чёрный дядя, х - левый сын своего отца
					if (x == x->_parent->_left) {
						x = x->_parent;
						RotateRight(x);
					}
					//#6 СЛУЧАЙ
					// чёрный дядя, х - правый сын своего отца
					x->_parent->_color = BLACK;
					x->_parent->_parent->_color = RED;
					RotateLeft(x->_parent->_parent);
				}
			}
		}
		_root->_color = BLACK;
	}


	/*вставка эелмента*/
	Node *Paste(int _data) {
		Node *current, *_parent, *x;

		// ищем место для вставки
		current = _root;
		_parent = 0;

		while (current != NIL) {
			if (_data == current->_data) return current;
			_parent = current;
			current = _data  <  current->_data ?
				current->_left : current->_right;
		}

		//создание нового элемента
		x = new Node;
		x->_data = _data;
		x->_parent = _parent;
		x->_left = NIL;
		x->_right = NIL;
		x->_color = RED;
		_count++;

		//вставка элемента
		if (_parent) {
			if (_data  < _parent->_data) // меньше родителя - влево, иначе - вправо
				_parent->_left = x;
			else
				_parent->_right = x;
		}
		else {
			_root = x; // вставка в корень
		}

		Balancing(x); // обязательно балансируем
		return(x);
	}


	/*вывод дерева*/
	void Print(Node *x, FILE *ptr) {
		if (x == NIL) return;
		Print(x->_left, ptr);
		if (x->_parent != NULL){
			fprintf(ptr, " %d -> %d\n", x->_parent->_data, x->_data);
		}
		Print(x->_right, ptr);
	}

	/*преобразование элемента в dot запись*/
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

	/*формирование dot файла*/
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

	/*поиск элемента с заданым ключом*/
	Node *FindNode(int data, Node *current) {

		while (current != NIL)
			if (data == current->_data)
				return current;
			else
				current = data < current->_data ?
				current->_left : current->_right;
		return NULL;
	}

	/*поиск минимального ключа*/
	int MinKey(Node *current) {
		if (current->_left != NIL)
			return MinKey(current->_left);
		else 
			return current->_data;
	}

};

/*поиск минимального значения не входящего в большое дерево*/
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

/*основная фукнция*/
void main(int argc, char **argv) {
	setlocale(LC_ALL, "rus");

	remove("firstGraph.png");
	remove("secondGraph.png");
	remove("ResultGraph.png");

	RBT *FIRST = new RBT(); // первое древо
	RBT *SECOND = new RBT(); // второе древо
	ifstream ifs;

	int tmp__data;
	vector<int> key_first;
	vector<int> key_second;

	ifs.open("f1.txt");
	//формируем дерево
	for (int i = 0; !ifs.eof(); i++)
		if (ifs >> tmp__data){
			FIRST->Paste(tmp__data);
			key_first.push_back(tmp__data);
		}
	ifs.close();
	

	ifs.open("f2.txt");
	//формируем дерево
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
				cout << "Обработка невозможна! Дерьвья не содержат элементов!" << endl;
				system("pause");
				return;
			}

	//вставка элемента в большое дерево
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

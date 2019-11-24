/*Дано число N и N строк. Каждая строка содержащит команду добавления или удаления натуральных чисел,
а также запрос на получение k-ой порядковой статистики.
Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
Запрос на получение k-ой порядковой статистики задается числом k. Требуемая скорость выполнения запроса - O(log n).*/
#include <cassert>
#include <iostream>
using namespace std;

//template<typename T, class C>
struct AVLNode {
	AVLNode* left, * right;
	int data;//данные
	int height;//высота дерева с корнем в данном узле
	int des;//количество потомков(то есть, количество узлов в поддереве исходного узла + сам узел)

	AVLNode(int const& _data) : data(_data) {
		left = right = nullptr;
		height = 1;
		des = 1;//сам узел считается
	}

	~AVLNode() {
		delete left;
		delete right;
	}

	int GetDeltaH() const {
		int r = right == nullptr ? 0 : right->height;
		int l = left == nullptr ? 0 : left->height;
		return r - l;
	}

	static int GetDes(AVLNode* p) {
		return p != nullptr ? p->des : 0;
	}

	void FixDes() {
		des = GetDes(left) + GetDes(right) + 1;
	}

	void FixHeight() {
		int r = right == nullptr ? 0 : right->height;
		int l = left == nullptr ? 0 : left->height;
		height = (r > l ? r : l) + 1;
	}

	AVLNode* Insert(int const& _data) {
		if (_data < data)
			left = left == nullptr ? new AVLNode(_data) : left->Insert(_data);//ребенок должен существовать
		else
			right = right == nullptr ? new AVLNode(_data) : right->Insert(_data);
		return Balance();
	}

	AVLNode* FindMin() {
		return left != nullptr ? left->FindMin() : this;
	}

	AVLNode* RemMin() {
		if (left == nullptr) return right;
		left = left->RemMin();
		return Balance();
	}

	static AVLNode* Remove(AVLNode* p, int const& _data) {
		if (p == nullptr) return nullptr;
		if (_data < p->data) {
			p->left = Remove(p->left, _data);
			return p->Balance();
		}
		else if (_data > p->data) {
			p->right = Remove(p->right, _data);
			return p->Balance();
		}
		else {
			AVLNode* p_l = p->left;//указатели на детей удаляемого
			AVLNode* p_r = p->right;
			p->left = nullptr;
			p->right = nullptr;
			delete p;

			if (p_r == nullptr) return p_l;
			AVLNode* min = p_r->FindMin();//указатель на самый маленький элемент правого поддерева
			min->right = p_r->RemMin();
			min->left = p_l;//теперь на месте удаляемого стоит правильный элемент
			return min->Balance();
		}
	}

	AVLNode* Balance() {
		FixHeight();
		FixDes();

		assert(GetDeltaH() >= -2 && GetDeltaH() <= 2);
		switch (GetDeltaH()) {
		case -2:
			if (left->GetDeltaH() > 0) {
				left = left->SmallLeftRotation();
			}
			return SmallRightRotation();
		case 2:
			if (right->GetDeltaH() < 0) {
				right = right->SmallRightRotation();
			}
			return SmallLeftRotation();
		case -1:
		case 1:
		case 0:
			return this;
		default:
			//return this;
			cout << "ERROR";
		}
	}

	AVLNode* SmallRightRotation() {
		auto result = left;
		left = result->right;
		result->right = this;
		FixHeight();
		FixDes();
		result->FixHeight();
		result->FixDes();
		return result;
	}

	AVLNode* SmallLeftRotation() {
		auto result = right;
		right = result->left;
		result->left = this;
		FixHeight();
		FixDes();
		result->FixHeight();
		result->FixDes();
		return result;
	}

	void Print(int level = 0) {
		if (this == nullptr) return;
		right->Print(level + 1);
		for (int i = 0; i < level; i++) cout << "_ ";
		cout << data << endl;
		left->Print(level + 1);
	}

	static AVLNode* GetKStat(AVLNode* p, int const& k) {
		assert(p->des > k);
		auto curr = p;
		int pos = GetDes(curr->left);//
		bool run = true;

		while (run) {
			if (pos == k)
				return curr;
			else if (pos > k) {//если мы находимся правее нужной нам позиции
				assert(curr->left != 0);
				curr = curr->left;
				pos = pos - 1 - GetDes(curr->right);
			}
			else {//левее
				assert(curr->right != 0);
				curr = curr->right;
				pos = pos + 1 + GetDes(curr->left);
			}
		}
		//return curr;
	}
};

struct AVLTree {
	AVLNode* root;

	AVLTree() {
		root = nullptr;
	}

	~AVLTree() {
		delete root;
	}

	void print() const {
		if (root != nullptr) root->Print();
	}

	bool insert(int const& _data) {
		if (root == nullptr) root = new AVLNode(_data);
		else root = root->Insert(_data);
		return true;
	}

	bool remove(int const& _data) {
		root = AVLNode::Remove(root, _data);
		return true;
	}

	void stat(int const& k) {
		cout << AVLNode::GetKStat(root, k)->data << " ";
	}
};


int main() {
	AVLTree t;
	/*int comand;
	int num = 0, k = 0;
	bool run = true;

	while (run) {
		cout << "1 - insert, 2 - print, 3 - delete, 4 - exit" << "\n";
		cin >> comand;
		switch (comand) {
		case 1:
			cin >> num;
			t.insert(num);
			break;
		case 2:
			t.print();
			cout << "\n";
			break;
		case 3:
			cin >> num;
			t.remove(num);
			break;
		case 4:
			int k; cin >> k;
			t.stat(k);
			break;
		default:
			break;
		}
	}*/

	int n; cin >> n;//количество запросов

	for (int i = 0; i < n; i++) {
		int num, k; cin >> num >> k;
		if (num < 0)
			t.remove(-num);
		else
			t.insert(num);
		t.stat(k);
	}

	return 0;
}
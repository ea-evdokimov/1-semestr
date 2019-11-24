/*Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево поиска, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Выведите элементы в порядке level-order (по слоям, “в ширину”).*/
#include <iostream>
#include <queue>

using namespace std;

template<typename T, class C>
class BST {
private:
	struct node {
		T data;
		node* left = nullptr;
		node* right = nullptr;
		explicit node(T const& dat) :data(dat) {}
		~node(){}
	} *root = nullptr;
	C comp;

public:
	BST(C comp) : comp(comp) {}

	~BST() {
		queue <node*> q;
		q.push(root);

		while (!q.empty()) {
			auto curr = q.front();
			q.pop();
					
			if (curr->left != nullptr)
				q.push(curr->left);

			if (curr->right != nullptr)
				q.push(curr->right);
			delete curr;
		}
	}

	bool empty() const {
		return root == nullptr;
	}

	bool insert(T const& dat) {
		if (empty()) {
			root = new node(dat);
			return true;
		}
		auto curr = root;
		auto par = root;
		bool isleft = true;
		while (curr != nullptr) {
			par = curr;
			if (comp(dat, curr->data)) {
				curr = curr->left;
				isleft = true;
			}
			else if (comp(curr->data, dat)) {
				curr = curr->right;
				isleft = false;
			}
			else {
				return false;
			}
		}
		if (isleft) par->left = new node(dat);
		else        par->right = new node(dat);
		return true;
	}

	void level_order() {
		queue <node*> q;
		q.push(root);

		while (!q.empty()) {
			auto curr = q.front();
			q.pop();
			cout << curr->data << " ";

			if (curr->left != nullptr)
				q.push(curr->left);
			if (curr->right != nullptr)
				q.push(curr->right);
		}
	}
};

class int_comparer {
public:
	bool operator()(int x, int y) {
		return x < y;
	}
};

int main() {
	int_comparer icomp;
	BST<int, int_comparer> tree(icomp);

	int comand; cin >> comand;
	for (int i = 0; i < comand; i++) {
		int x; cin >> x;
		tree.insert(x);
	}
	tree.level_order();
	return 0;
}
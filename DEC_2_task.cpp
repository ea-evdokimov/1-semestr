/*Дано число N < 106 и последовательность пар целых чисел из [-231, 231] длиной N.
Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi).
Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
Вычислить разницу глубин наивного дерева поиска и декартового дерева.
Разница может быть отрицательна, необходимо вывести модуль разности.*/
#include <iostream>
#include <cassert>
#include <queue>
using namespace std;

template<typename T, class C>
class BST {
private:
	struct node {
		T data;
		int h;//переменная для подсчета конечной высоты
		node* left = nullptr;
		node* right = nullptr;

		node(T const& dat) :data(dat), h(0) {}
		~node() {
		}
		void print(int level) const {
			if (this == nullptr) return;
			right->print(level + 1);
			for (int i = 0; i < level; i++) cout << "  ";
			cout << data << std::endl;
			left->print(level + 1);
		}
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

	bool insert(T const& new_data) {
		if (empty()) {
			root = new node(new_data);
			return true;
		}
		auto curr = root;
		auto par = root;
		bool isleft = true;
		while (curr != nullptr) {
			par = curr;
			if (comp(new_data, curr->data)) {
				curr = curr->left;
				isleft = true;
			}
			else if (comp(curr->data, new_data)) {
				curr = curr->right;
				isleft = false;
			}
			else {
				return false;
			}
		}
		if (isleft) par->left = new node(new_data);
		else        par->right = new node(new_data);
		return true;
	}

	int height() {
		if (empty())
			return 0;
		return find_height(root);
	}

	int find_height(node* root_) {
		int h_left = 0, h_right = 0;

		if (root_->left != nullptr)
			h_left = find_height(root_->left);
		if (root_->right != nullptr)
			h_right = find_height(root_->right);
		return h_left > h_right ? h_left + 1 : h_right + 1;
	}

	int no_rec_height() {
		assert(root != nullptr);
		queue <node*> q;
		root->h = 1;
		q.push(root);
		int max_h = 0;

		while (!q.empty()) {
			auto curr = q.front();
			q.pop();
			if (curr->h > max_h)
				max_h = curr->h;
			//cout << curr->data << " ";

			if (curr->left != nullptr) {
				q.push(curr->left);
				node* l = curr->left;
				l->h = curr->h + 1;
			}
			if (curr->right != nullptr) {
				q.push(curr->right);
				node* r = curr->right;
				r->h = curr->h + 1;
			}
		}
		return max_h;
	}

	void print() {
		root->print(0);
	}
};

class int_comparer {
public:
	bool operator()(int x, int y) {
		return x < y;
	}
};

template<typename T>
class DECART {
private:
	struct node {
		T data;
		int prior;
		int h;
		node* left = nullptr;
		node* right = nullptr;
		node(T const& new_data, int const& pr) :data(new_data), prior(pr), h(0) {}
		~node() {}

		void print_int(int level) const {
			if (this == nullptr) return;
			right->print(level + 1);
			for (int i = 0; i < level; i++) cout << "  ";
			cout << data << "-" << prior << endl;
			left->print(level + 1);
		}
	} *root = nullptr;

public:
	~DECART() {
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

	pair<node*, node*> split(node* root_, T const& new_data) {
		if (root_ == nullptr)
			return { nullptr , nullptr };

		if (root_->data < new_data) {
			pair< node*, node*> spl = split(root_->right, new_data);
			root_->right = spl.first;
			return { root_, spl.second };
		}
		else {
			pair<node*, node*> spl = split(root_->left, new_data);
			root_->left = spl.second;
			return { spl.first, root_ };
		}
	}

	node* merge(node* left, node* right) {
		if (left == nullptr || right == nullptr)
			return right == nullptr ? left : right;
		if (left->prior > right->prior) {
			left->right = merge(left->right, right);
			return left;
		}
		else {
			right->left = merge(left, right->left);
			return right;
		}
	}

	void insert(T const& new_data, int const& pr) {
		node* new_node = new node(new_data, pr);
		node* curr = root, * prev = nullptr;

		if (curr == nullptr)
			root = new_node;
		else {
			while (curr != nullptr) {
				if (pr <= curr->prior) {
					prev = curr;
					if (new_data <= curr->data) curr = curr->left;
					else                        curr = curr->right;
				}
				else
					break;
			}

			if (curr == nullptr) {
				curr = new_node;
				if (prev->data > new_data)
					prev->left = new_node;
				else
					prev->right = new_node;
				new_node->left = nullptr;
				new_node->right = nullptr;
			}
			else {
				pair<node*, node*> p = split(curr, new_data);
				if (prev == nullptr)
					root = new_node;
				else if (prev->data > new_data)
					prev->left = new_node;
				else
					prev->right = new_node;

				new_node->left = p.first;
				new_node->right = p.second;
			}
		}
	}

	int no_rec_height() {
		assert(root != nullptr);
		queue <node*> q;
		root->h = 1;
		q.push(root);
		int max_h = 0;

		while (!q.empty()) {
			auto curr = q.front();
			q.pop();
			if (curr->h > max_h)
				max_h = curr->h;
			//cout << curr->data << " ";

			if (curr->left != nullptr) {
				q.push(curr->left);
				node* l = curr->left;
				l->h = curr->h + 1;
			}
			if (curr->right != nullptr) {
				q.push(curr->right);
				node* r = curr->right;
				r->h = curr->h + 1;
			}
		}
		return max_h;
	}

	int height() {
		if (empty())
			return 0;
		return find_height(root);
	}

	int find_height(node* root_) {
		int h_left = 0, h_right = 0;

		if (root_->left != nullptr)
			h_left = find_height(root_->left);
		if (root_->right != nullptr)
			h_right = find_height(root_->right);
		return h_left > h_right ? h_left + 1 : h_right + 1;
	}

	void print() {
		root->print_int(0);
	}
};

int main() {
	DECART<int> d;
	int_comparer icomp;
	BST<int, int_comparer> tree(icomp);

	int k; cin >> k;
	int num, prior;
	for (int i = 0; i < k; i++) {
		cin >> num >> prior;
		d.insert(num, prior);
		tree.insert(num);
	}
	int res = tree.no_rec_height() - d.no_rec_height();
	if (res < 0) res *= (-1);
	cout << res;

	return 0;
}
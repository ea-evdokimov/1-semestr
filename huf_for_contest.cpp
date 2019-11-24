#include <iostream>
#include <string>
#include <utility>
#include <queue>
#include "Huffman.h"

using namespace std;
typedef unsigned char byte;

class node {
private:
	int frequency;
	byte symbol;
	bool help;//флаг, является ли узел вспомогательным
	string code;//строка для кода символа
	node* left = nullptr;
	node* right = nullptr;

public:
	node(int freq, byte symb, int h) : frequency(freq), symbol(symb), help(h) {}

	~node() {
		delete left;
		delete right;
	}

	void SetChildren(node* r, node *l) {
		right = r;
		left = l;
	}

	bool IsHelp() {
		return help;
	}

	int GetFreq() {
		return frequency;
	}
	
	int GetSymbol() {
		return (int)symbol;
	}

	node* GetLeftChild() {return left;}

	node* GetRightChild() {	return right;}

	void SetCode(string res){
		code = res;
	}

	void print(int level) const {
		if (this == nullptr) return;
		right->print(level + 1);
		for (int i = 0; i < level; i++) cout << "_ ";
		cout << "(" << symbol << "-" << frequency << "-" << code << ")" << endl;
		left->print(level + 1);
	}
};

struct Tree {
	node* root;

	Tree(node* root_) {
		root = root_;
	}

	~Tree() {
		delete root;
	}

	bool empty() const {
		return root == nullptr;
	}
	
	void SetCodes(node* node, int bit, string res, string *codes) {
		if (node == root) {
			if (node->GetRightChild())
				SetCodes(node->GetRightChild(), 1, res, codes);
			if (node->GetLeftChild())
				SetCodes(node->GetLeftChild(), 0, res, codes);
		}

		else if (node->IsHelp()) {//если node вспомогательный
			if (node->GetLeftChild()) {
				if (bit == 1)
					SetCodes(node->GetLeftChild(), 0, res + "1", codes);
				else
					SetCodes(node->GetLeftChild(), 0, res + "0", codes);
			}
			if (node->GetRightChild()) {
				if (bit == 1)
					SetCodes(node->GetRightChild(), 1, res + "1", codes);
				else
					SetCodes(node->GetRightChild(), 1, res + "0", codes);
			}
		}

		else {//node с нашим символом
			if (bit == 1) {
				node->SetCode(res + "1");
				codes[node->GetSymbol()] = res + "1";
			}
			else {
				node->SetCode(res + "0");
				codes[node->GetSymbol()] = res + "0";
			}
		}
	}

	void print() const {
		cout << "\n";
		root->print(0);
	}
};

class comp {
public:
	bool operator()(node *x, node* y) {
		return x->GetFreq() > y->GetFreq();
	}
};

struct bytein {
public:
	bool Read(byte &value) {
		value = getchar();
		if (value == 255 || value == '\n')
			return 0;
	}
};

string GetString(bytein& input) {
	byte value;
	string s;

	while (input.Read(value))	{
		s += value;
	}
	return s;
}

byte ToChars(string s, int start) {//перерводит часть строки из 0 и 1 в код символа..тупой метод
	int res = 0;
	int p = 1;
	for (int i = 0; i < 8; i++) {
		res += (s[start + 7 - i] == '1') * p;
		p = p << 1;
	}
	return (byte)res;
}

string ToBites(int n) {
	int mask = 255;
	string res = "    ";
	for (int i = 0; i < 4; i++) {
		res[3 - i] = (byte)n & mask;
		n = n >> 8;
	}
	return res;
}

int GetBit(byte c, int n) {//получение биты под номером n
	int mask = 1 << (7 - n);
	return ((c & mask) != 0);
}

void Decode(IOutputStream& compressed, IInputStream& original) {
	//string arch = GetString(compressed);	
	string result;
	byte value;

	while (compressed.Read(value)) {
		result += value;
	}

	int freq[256];

	for (int i = 0; i < 256; i++) {
		freq[i] = 0;
	}

	for (int i = 0; i < 256; i++) {
		int result = 0;
		for (int j = 0; j < 4; j++) {
			result <<= 8;
			result |= (byte)arch[i * 4 + j];
		}
		freq[i] = result;
	}//получил массив с частотами

	int length = 0;//длина исходной строки как сумма всех частот
	for (int i = 0; i < 256; i++) {
		length += freq[i];
	}

	//аналогичное построение дерева на частотах встречаемости элементов
	priority_queue<node*, vector<node*>, comp> q;

	for (int i = 0; i < 256; i++) {
		if (freq[i] > 0) {
			node* a = new node(freq[i], (byte)i, 0);
			q.push(a);
		}
	}
	
	while (q.size() > 1) {
		node* r = q.top();
		q.pop();
		node* l = q.top();
		q.pop();

		node* parent = new node(r->GetFreq() + l->GetFreq(), 'p', 1);
		parent->SetChildren(r, l);
		
		q.push(parent);
	}

	node* root_ = q.top();
	Tree* tree = new Tree(root_);
	auto curr = root_;
	
	int len_of_bytes = arch.size() - 1024;//длина символов, которые кодируют строку
	int k = 0;//количество считанных символов

	for (int i = 0; i < len_of_bytes; i++) {
		int j = 0;
		while (j < 8 && k < length) {
			if (curr->IsHelp() == 0) {
				result += (byte)curr->GetSymbol();//если дошли до листа, получаем
				k++;
				if (GetBit(arch[i + 1024], j) == 1)
					curr = root_->GetRightChild();
				else
					curr = root_->GetLeftChild();

			}
			else {
				if (GetBit(arch[i + 1024], j) == 1)
					curr = curr->GetRightChild();
				else
					curr = curr->GetLeftChild();
			}
			j++;
			}
	}
	//cout << "result:" << result;
	for (int i = 0; i < result.length(); i++)
		original.Write(result[i]);
}

void Encode(IInputStream& original, IOutputStream& compressed) {
	//string orig = GetString(original);//оригинальная строка	
	string orig;
	byte value;

	while (original.Read(value)) {
		result += value;
	}

	int freq[256];
	string codes[256];

	for (int i = 0; i < 256; i++)
		freq[i] = 0;

	for(int i = 0; i < orig.length(); i++) {
		freq[(byte)orig[i]]++;
	}//массив с частотами встречающихся элемнетов

	priority_queue<node*, vector<node*>, comp> q;
	
	for (int i = 0; i < 256; i++) {
		if (freq[i] > 0) {
			node* a = new node(freq[i], (byte)i, 0);
			q.push(a);
		}
	}//создана очередь из узлов с символами и их частотами 
	
	while (q.size() > 1) {
		node* r = q.top();
		q.pop();
		node* l = q.top();
		q.pop();

		node* parent = new node(r->GetFreq() + l->GetFreq(), '!', 1);
		parent->SetChildren(r, l);

		q.push(parent);
	}

	node* root_ = q.top();//указатель на корень дерева, постороенного на частотах
	q.pop();
	Tree* tree = new Tree(root_);
	string e_s = "";

	if (!tree->empty()) {
		//tree->print();
		tree->SetCodes(root_, 0, e_s, codes);
		tree->print();
	}
	////////////////////////////////////////////////////////////////////////
	string raw;
	for (int i = 0; i < orig.length(); i++)
		raw += codes[(byte)orig[i]];
	//cout << "raw:" << raw << endl;
	bool flag = 0;
	if (raw.length() % 8 == 0)
		flag = 1;
	while (raw.size() % 8 != 0)
		raw += "0";	

	string decode;	
	for (int i = 0; i < 256; i++) {
		decode += ToBites(freq[i]);
	}//в строку занесены частоты встречаемостей символов

	for (int i = 0; i < raw.length() / 8; i++) {
		decode += ToChars(raw, i * 8);
	}//после "алфавита" идут коды символов
	if (flag)
		decode += "0";

	//cout << flag << endl;
	Decode(decode);
	for(int i = 0; i < decode.length(); i++)
		compressed.Write(decode[i]);
}

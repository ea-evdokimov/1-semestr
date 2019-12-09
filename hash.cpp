/*Реализуйте структуру данных типа “множество строк” на основе динамической
хеш - таблицы с открытой адресацией.Хранимые строки непустые и состоят из строчных латинских букв.
Начальный размер таблицы должен быть равным 8 - ми.Перехеширование выполняйте при добавлении элементов в случае,
когда коэффициент заполнения таблицы достигает 3 / 4.
Хеш - функцию строки реализуйте с помощью вычисления значения многочлена методом Горнера.
Структура данных должна поддерживать операции добавления строки в множество,
удаления строки из множества и проверки принадлежности данной строки множеству.
1_1.Для разрешения коллизий используйте квадратичное пробирование.i - ая проба
g(k, i) = g(k, i - 1) + i(mod m).m - степень двойки.
!!! чисто математически вычисление i-ой пробы по функции из предыдущей строки будет точно такой же,
как (hash(key, 0) + i / 2 + i * i / 2) % data.size()!!!*/
#include <vector>
#include <iostream>
#include <string>

using namespace std;

int incr(int i) {
	return (i + i * i) / 2;
}

class hashtable {
public:
	hashtable(size_t init_size = 8) {
		data.resize(init_size);
	}

	~hashtable() {}

	bool insert(string const& key) {
		if (find(key))
			return false;
		if (inserted * 2 >= data.size()) {
			rehash();
		}

		int i = 0;
		for (auto h = hash(key, i); ; h = hash(key, i)) {
			if (data[h].tag == item::EMPTY || data[h].tag == item::DELETED) {
				data[h] = { key, item::BUSY };
				inserted++;
				return true;
			}

			if (data[h].tag == item::BUSY && data[h].key == key) return false;
			i++;
		}
		return true;
	}

	bool erase(string const& key) {
		int i = 0;
		int j = 0;
		for (auto h = hash(key, i); j < data.size(); h = hash(key, i)) {
			if (data[h].tag == item::EMPTY) return false;

			if (data[h].tag == item::BUSY && data[h].key == key) {
				data[h].tag = item::DELETED;
				inserted--;
				return true;
			}
			i++;
			j++;
		}
		return false;
	}

	bool find(string const& key) {
		int i = 0;
		int j = 0;
		for (auto h = hash(key, i); j < data.size(); h = hash(key, i)) {
			if (data[h].tag == item::EMPTY) return false;

			if (data[h].tag == item::BUSY && data[h].key == key) return true;
			i++;
			j++;
		}
		return false;
	}

	void rehash() {
		int old_size = data.size();
		vector<item> help;

		for (int i = 0; i < old_size; i++) {
			if (data[i].tag == item::BUSY)
				help.push_back(data[i].key);
		}

		data.clear();
		inserted = 0;

		data.resize(old_size * 2);

		for (int i = 0; i < help.size(); i++) {
			insert(help[i].key);
		}
	}

	long long hash(string const& key, int i) {
		unsigned int hash_const = 29;
		long long hash = 0;
		unsigned int pow = 1;

		for (int i = 0; i < key.length(); i++) {
			hash = (hash + (key[i] - 'a' + 1) * pow);
			pow = (pow * hash_const);
		}

		hash += incr(i);
		hash %= data.size();

		return hash;
	}

	void print() {
		for (int i = 0; i < data.size(); i++) {
			if (data[i].tag == item::BUSY)
				cout << i << "-" << data[i].key << endl;
		}
	}

private:
	struct item {
		item(string const& key = "", int tag = item::EMPTY) : key(key), tag(tag) {}
		string key;

		enum {
			EMPTY,
			BUSY,
			DELETED
		};
		int tag = EMPTY;
	};
	void make_ins_null() {
		inserted = 0;
	}

	vector<item> data;
	size_t inserted = 0;
};

int main() {
	hashtable ht;
	char comand;
	int run = 1;
	string s;

	while (run) {
		cout << "+ - insert, ? - find, - - delete, p - print" << endl;
		cin >> comand;
		switch (comand) {
		case '+':
			cin >> s;
			cout << ht.insert(s) << endl;
			break;
			cin >> s;
			cout << ht.find(s) << endl;
			break;
		case '-':
			cin >> s;
			cout << ht.erase(s) << endl;
			break;
		case 'p':
			ht.print();
			cout << endl;
			break;
		default:
			break;
		}
	}
	return 0;
}
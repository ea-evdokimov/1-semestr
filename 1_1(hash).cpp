/*Реализуйте структуру данных типа “множество строк” на основе динамической
хеш - таблицы с открытой адресацией.Хранимые строки непустые и состоят из строчных латинских букв.
Начальный размер таблицы должен быть равным 8 - ми.Перехеширование выполняйте при добавлении элементов в случае,
когда коэффициент заполнения таблицы достигает 3 / 4.
Хеш - функцию строки реализуйте с помощью вычисления значения многочлена методом Горнера.
Структура данных должна поддерживать операции добавления строки в множество,
удаления строки из множества и проверки принадлежности данной строки множеству.
1_1.Для разрешения коллизий используйте квадратичное пробирование.i - ая проба
g(k, i) = g(k, i - 1) + i(mod m).m - степень двойки.*/
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class hashtable {
public:
	hashtable(size_t init_size = 8) {
		data.resize(init_size);
	}

	~hashtable() {}

	bool insert(string const& key) {
		if (find(key))
			return false;

		if (inserted * 4 >= data.size() * 3) {
			rehash();
		}

		int i = 0;
		for (auto h = hash(key, i); ; h = (h + i) % data.size()) {
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
		for (auto h = hash(key, i); i < data.size(); h = (h + i) % data.size()) {
			if (data[h].tag == item::EMPTY) return false;

			if (data[h].tag == item::BUSY && data[h].key == key) {
				data[h].tag = item::DELETED;
				inserted--;
				return true;
			}
			i++;
		}
		return false;
	}

	bool find(string const& key) {
		int i = 0;
		for (auto h = hash(key, i); i < data.size(); h = (h + i) % data.size()) {
			if (data[h].tag == item::EMPTY) return false;
			if (data[h].tag == item::BUSY && data[h].key == key) return true;
			i++;
		}
		return false;
	}

	void rehash() {
		vector<item> help(data.size());

		for (int i = 0; i < data.size(); i++) {
			if (data[i].tag == item::BUSY)
				help.push_back(data[i].key);
		}

		data.clear();
		inserted = 0;
		data.resize(help.size() * 2);
		for (int i = 0; i < help.size(); i++) {
			insert(help[i].key);
		}
	}

	int hash(string const& key, int i) {
		int hash_const = 29;
		int hash = 0;
		
		for (int i = 0; i < key.length(); i++) {
			hash = (hash + (key[i] - 'a' + 1)) * hash_const;
		}

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

	char c;
	string s;
	while (getline(cin, s)) {
		if (s[0] == '+') {
			s.erase(0, 2);
			if (ht.insert(s))
				cout << "OK" << endl;
			else
				cout << "FAIL" << endl;
		}

		if (s[0] == '?') {
			s.erase(0, 2);
			if (ht.find(s))
				cout << "OK" << endl;
			else
				cout << "FAIL" << endl;
		}

		if (s[0] == '-') {
			s.erase(0, 2);
			if (ht.erase(s))
				cout << "OK" << endl;
			else
				cout << "FAIL" << endl;
		}
	}

	return 0;
}

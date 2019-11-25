#include <iostream>
#include <chrono>
#include <random>
#include <utility>
#include <vector>

using namespace std;
typedef long long ll;

bool isless_ll(const ll& a, const ll& b) {
	if (a < b)
		return 1;
	return 0;
}

template <typename T>
class stack {
private:
	vector<T> Data;
	int front, back;

public:
	stack() {
		front = 0;
		back = 0;
	}
	
	~stack() {
		Data.clear();
	}

	bool IsEmpty() {
		if (front == back)
			return 1;
		else
			return 0;
	}

	void push(T key) {
		Data.push_back(key);
		back++;
	}

	T pop() {
		if (IsEmpty())
			cout << "Stack is empty";
		else
		{
			T tmp = Data[back - 1];
			back = Data.size() - 1;
			Data.pop_back();
			return tmp;
		}
	}

};

template<class T>
void SortInsertion(T *mas, int n, bool(*isless)(const T&, const T&)) {
	for (int i = 1; i < n; i++) {
		int ind = i;
		T val = mas[i];

		while (ind >= 1 && isless(val, mas[ind - 1])) {
			mas[ind] = mas[ind - 1];
			ind--;
		}
		mas[ind] = val;
	}
}

///////////////////////////////////////////////

template<class T>
ll Partition(T* mas, ll l, ll r, bool(*isless)(const T&, const T&)) {//для среднего элемента
	T pivot;
	if (l == r)
		return l;
	pivot = mas[l + (r - l) / 2];
	ll i = l, j = r;
	
	while (true) {
		while (isless(mas[i], pivot))
			i++;
		while (isless(pivot, mas[j]))
			j--;
		if (i >= j)
			return j;
		swap(mas[i], mas[j]);
	}
}

template<class T>
void QuickSortOutRecursion(T* mas, ll l, ll r, bool(*isless)(const T&, const T&)) {
	stack<T> Stack; //заменяем рекурскивные вызовы на стек

	Stack.push(l); Stack.push(r);
	ll PosPiv;
	
	while (!Stack.IsEmpty()) {//пока есть, что сортироввть
		r = Stack.pop();
		l = Stack.pop();

		if (r - l < 10) {//место, где для избежания концевой рекурсии, необходимо 
			             //выйти из цикла и перейти на сортировку вставками
			continue;
		}

		else {
			PosPiv = Partition<T>(mas, l, r, isless);//получаем позицию опорного элемента(среднего)

			if (PosPiv - l > r - PosPiv) {
				ll r1 = PosPiv - 1;
				ll l1 = PosPiv + 1;
				Stack.push(l);//закидываем левую(большую половину) в стек
				Stack.push(r1);
				Stack.push(l1);//..правую
				Stack.push(r);
			}

			else {
				ll r1 = PosPiv - 1;
				ll l1 = PosPiv + 1;
				Stack.push(l1);//.. правую
				Stack.push(r);
				Stack.push(l);//.. левую
				Stack.push(r1);
			}
		}
	}
}


///////////////////////////////////////////////
int main() {
	ll n; cin >> n;
	ll* mas = new ll[n];
	cout << endl;

	srand(time(nullptr));

	for (ll i = 0; i < n; i++) {
		mas[i] = rand();
		//cin >> mas[i];
	}

	auto begin = chrono::high_resolution_clock::now();
	QuickSortOutRecursion<ll>(mas, 0, n - 1, isless_ll);
	SortInsertion(mas, n, isless_ll);//для избавления от концевой рекурсии последний куски сортируем вставками
	auto end = chrono::high_resolution_clock::now();

	for (ll i = 0; i < n; i++) {
		if (i % 20 == 0)
			cout << endl;
		cout << mas[i] << " ";
	}

	cout << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "mcs" << endl;

	delete[] mas;
	return 0;
}
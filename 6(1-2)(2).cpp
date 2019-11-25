#include <iostream>
#include <chrono>
#include <random>
#include <utility>

using namespace std;
typedef long long ll;

bool isless_ll(const ll& a, const ll& b) {
	if(a < b)
		return 1;
	return 0;
}

//реализация partition с начала - f, серединного - a, по трем медианам - m, рандомного - r

template<class T>
ll Partition(T *mas, ll l, ll r, char c, bool(*isless)(const T&, const T&)) {
	T pivot;
	if (l == r)
		return l;
	
	if (c == 'f') {
		pivot = mas[l];
		ll i = l, j = r;
		while (true) {
			while (isless(pivot, mas[j]))
				j--;
			while (isless(mas[i], pivot))
				i++;
			if (i < j)
				swap(mas[i], mas[j]);
			else
				return j;
		}
	}

	if (c == 'a') {
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

	if (c == 'm') {
		T piv[3] = { mas[l], mas[r], mas[(l + r) >> 1] };
		if (isless(piv[0], piv[1])) swap(piv[0], piv[1]);
		if (isless(piv[1], piv[2])) swap(piv[1], piv[2]);
		if (isless(piv[0], piv[1])) swap(piv[0], piv[1]);
		pivot = piv[1];

		ll i = l, j = r;
		
		while (true) {
			while (isless_ll(mas[i], pivot))
				i++;
			while (isless_ll(pivot, mas[j]))
				j--;
			if (i >= j)
				return j;
			swap(mas[i], mas[j]);
		}
	}
	
	if (c == 'r') {
		ll random = rand() % (r - l + 1);
		pivot = mas[l + random];

		ll i = l, j = r;

		while (true) {
			while (isless_ll(mas[i], pivot))
				i++;
			while (isless_ll(pivot, mas[j]))
				j--;
			if (i >= j)
				return j;
			swap(mas[i], mas[j]);
		}
	}

}

template<class T>
void QuickSort(T *a, ll l, ll r, char c, bool(*isless)(const T&, const T&)) {
	if (l < r) {
		ll m = Partition<T>(a, l, r, c, isless);
		QuickSort(a, l, m, c, isless);
		QuickSort(a, m + 1, r, c, isless);
	}
}


//p - продвинутый стек с разделением на >, < и = опорному

template<class T>
pair<ll, ll> PartitionP(T* mas, ll l, ll r, char c, bool(*isless)(const T&, const T&)) {
	if (r == l)
		return { r, l };
	if (c == 'p' && r > l) {
		T pivot = mas[r];//выбираем в качестве опорного последний элемент
		int i = l, j = r - 1;
		int end = l - 1, beg = r;//указатели на части(слева, справа) массива с равными опорному элементу
		
		while (i <= j) {
			while (isless(mas[i], pivot))
				i++;
			while (isless(pivot, mas[j]))
				j--;
			if (i >= j)
				break;
			swap(mas[i], mas[j]);
			
			if (mas[i] == pivot) {//если i указывает на равный опорному, меняем с p-ым
				end++;
				swap(mas[end], mas[i]);
			}
			i++;
			
			if (mas[j] == pivot) {//аналогично справа
				beg--;
				swap(mas[beg], mas[j]);
			}
			j--;
		}//когда указатели пересекаются, переписываем 
		
		swap(mas[i], mas[r]);//опорный элемент оказывается на своем месте
		j = i - 1;
		i++;
		for (int k = l; k <= end; k++, j--)
			swap(mas[k], mas[j]);//записываем все равные опорному элементу(слева) перед опорным, поставленным на предыдущем шаге
		for (int k = r - 1; k >= beg; k--, i++)
			swap(mas[k], mas[i]);//аналогично равные опорному (справа)
		
		return { j, i };
	}
}

template<class T>
void QuickSortP(T* a, ll l, ll r, char c, bool(*isless)(const T&, const T&)) {
	if (l < r) {
		pair<ll, ll> m = PartitionP<T>(a, l, r, c, isless);
		QuickSortP(a, l, m.first, c, isless);//не сортируем часть равных опорному
		QuickSortP(a, m.second, r, c, isless);
	}
}


///////////////////////////////////////////////////
int main() {
	ll n; cin >> n;
	char c;
	ll* mas = new ll[n];
	cin >> c;
	cout << endl;

	srand(time(nullptr));
	
	for (ll i = 0; i < n; i++) {
		//mas[i] = rand();
		cin >> mas[i];
	}

	/*for (ll i = 0; i < n; i++) {
		if (i % 20 == 0)
			cout << endl;
		cout << mas[i] << " ";
	}
	cout << endl;*/
	auto begin = chrono::high_resolution_clock::now();
	QuickSortP<ll>(mas, 0, n - 1, c, isless_ll);
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
	



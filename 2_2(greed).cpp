/*В большой IT-фирме есть только одна переговорная комната.
Желающие посовещаться заполняют заявки с желаемым временем начала и конца.
Ваша задача определить максимальное количество заявок, которое может быть удовлетворено. 
Число заявок ≤ 100000.*/
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

bool comp(const pair<int, int>& x, const pair<int, int>& y) {
	return x.second < y.second;
}

int function(vector<pair<int, int>>& v) {
	sort(v.begin(), v.end(), comp);
	int count = 0, last = 0;
	for (int i = 0; i < v.size(); i++) {
		if (v[i].first >= v[last].second) {
			count++;
			last = i;
		}
	}
	return count + 1;
}

int main() {
	int start, finish;
	vector<pair<int, int>> v;
	
	while (!cin.eof()) {
		cin >> start >> finish;
		v.push_back({ start, finish });
	}

	cout << function(v);
	return 0;
}

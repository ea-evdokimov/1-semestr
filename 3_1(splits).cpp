/*Дано N кубиков.Требуется определить, 
каким количеством способов можно выстроить из этих кубиков пирамиду.*/
#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

ll function(int n) {
	vector<vector<ll>> table(n + 1, vector<ll> (n + 1));//табличка размером n * n
	table[0][0] = 1;

	for (int i = 0; i <= n; i++)
		for (int j = 0; j <= n; j++) {
			if (j != 0) {
				if (j > i)
					table[i][j] = table[i][i];
				else
					table[i][j] = table[i][j - 1] + table[i - j][j];
			}
		}
	
	/*for (int i = 0; i <= n; i++)
		for (int j = 0; j <= n; j++) {
			cout << table[i][j] << " ";
			if (j == n)
				cout << endl;
	}*/

	return table[n][n];
}

int main() {
	int n; cin >> n;
	cout << function(n);
	return 0;
}

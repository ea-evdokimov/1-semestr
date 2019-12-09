/*Дано невзвешенное дерево. Расстоянием между двумя вершинами будем называть
количество ребер в пути, соединяющем эти две вершины.
Для каждой вершины определите сумму расстояний до всех остальных вершин. 
Время работы должно быть O(n).*/
#include <vector>
#include <iostream>

using namespace std;
typedef long long ll;

//void dfs(const vector<vector<int>>& graph, int n, vector<bool> checked) {			
//	checked[n] = true;
//	cout << n << ",";
//	
//	for (int i = 0; i < graph[n].size(); i++) {
//		int u = graph[n][i];
//		if (!checked[u])
//			dfs(graph, u, checked);
//	}
//}

ll function(int n, ll dist_par, int desc) {
	return (dist_par - desc - 1) + (n - desc + 1);
	//из суммы расстояний для родителя вычитаем количество вершин в поддереве с корнем в текущем элементе
	//( каждый раз считалось лишнее ребро + ребро между текущим и родителем)
	//+ сумма растояний для верхней части + 1(до родителя)
}

ll counting(const vector<vector<int>>& graph, int n, vector<int>& desc) {
	//подсчет потомков элементов и суммы расстояний для корня за один dfs
	ll res = 0;

	for (auto i = 0; i < graph[n].size(); i++) {
		int next = graph[n][i];//потомок

		res += counting(graph, next, desc) + desc[next];//до каждого потомка + еще одно ребро
														//соединяющее next и текущую вершину
		desc[n] += desc[next];
	}
	desc[n]++;//+сама вершина
	
	return res;
}

int main() {
	int n; cin >> n;
	int start, finish;
	//индексы вершин от 0 до n-1
	
	vector<vector<int>> graph(n);//показывает наличие ребер
								 //(i - номер вершины, в graph[i] ее дети	
	vector<int> parent(n);//parent[i] - номер родителя i вершины
						  //у 0 (корневого узла) родителя нет
	vector<int> desc(n);//число потомков в поддереве с корнем i	
	vector<ll> dist(n);//конечный вектор

	for (int i = 0; i < n - 1; i++) {
		cin >> start >> finish;
		
		if (finish > start) {
			graph[start].push_back(finish);
			parent[finish] = start;
		}

		else {
			graph[finish].push_back(start);
			parent[start] = finish;
		}
	}
	
	//заполнение конечного вектора
	for (int i = 0; i < n; i++) {
		if(i == 0)
			dist[0] = counting(graph, 0, desc);
		else {
			int par = parent[i];
			dist[i] = function(n, dist[par], desc[i]);
		}
		cout << dist[i] << "\n";
	}	

	return 0;
}

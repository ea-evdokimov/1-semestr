/*Дано невзвешенное дерево. Расстоянием между двумя вершинами будем называть
количество ребер в пути, соединяющем эти две вершины.
Для каждой вершины определите сумму расстояний до всех остальных вершин. 
Время работы должно быть O(n).*/
#include <vector>
#include <iostream>
#include <stack>

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

ll counting(const vector<vector<int>>& graph, vector<int>& desc, vector<ll> &dist) {
	int i = graph.size() - 1;//идем с наибольших(нижних) элементов(гарантировано посещаем раньше, чем их родителя)

	while (i >= 0) {
		desc[i]++;//сама вершина

		if (graph[i].size() != 0) {//если он не лист
			int k = 0;
			while(k < graph[i].size()) {//проебгаемся по его детям
				dist[i] += (dist[graph[i][k]] + desc[graph[i][k]]);//до каждого потомка + еще одно ребро
				desc[i] += desc[graph[i][k]];//инкрементируем потомков
				k++;
			}
		}
		i--;
	}
	//в итоге для корня сумма расстояний посчитана правильно
	return dist[0];
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
	counting(graph, desc, dist);

	for (int i = 0; i < n; i++) {
		if (i != 0){
			int par = parent[i];
			dist[i] = function(n, dist[par], desc[i]);
		}
		cout << dist[i] << "\n";
	}

	return 0;
}

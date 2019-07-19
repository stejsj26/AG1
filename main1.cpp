#include <cstdlib>
#include <iostream>
#include <queue>
#include <list>
#include <utility>
#include <set>
#include <limits>
#include <map>

using namespace std;
const int MAX_TOWNS = numeric_limits<int>::max();

class Graph {
public:
	Graph(const int &, const int &);
	
	void addEdge(const int &, const int &);
	
	void addFood(const int &, const int &);
	
	void addFoodCheck(const int &, const int &);
	
	void bfs(const int &);
	
	void printOutput();
	
	void shortestPath(const int &, const int &, int *&, int &, int& cnt);

private:
	vector<list<int>> m_AdjList;
	map<int, set<int>> m_Food;
	vector<int> m_FoodCheck;
	vector<int> m_Distance;
	int m_VertexCount;
	int m_Variety;
	int m_TotalDistance;
};

Graph::Graph(const int &a, const int &b) : m_VertexCount(a), m_Variety(b), m_TotalDistance(0) {
	m_AdjList.resize((unsigned) a);
}

void Graph::addEdge(const int &u, const int &v) {
	m_AdjList[u].push_back(v);
	m_AdjList[v].push_back(u);
}

void Graph::addFood(const int &a, const int &b) {
	m_Food[a].insert(b);
}

void Graph::addFoodCheck(const int &a, const int &b) {
	auto it = m_FoodCheck.begin();
	m_FoodCheck.insert(it + a, b);
}
/**
 *	Finds shortest path from source to destination
 * @param [in] source 				starting node
 * @param [in] destination 			finish node
 * @param [in] parent 				array of predecessors
 * @param [in/out] distance_cnt		total distance needed for particular town
 * @param [in/out] cnt				counter, remembers recursion count
 */
void Graph::shortestPath(const int &source, const int &destination, int *&parent, int &distance_cnt, int& cnt) {
	if (source == destination) {
		distance_cnt += cnt;
		return;
	} else {
		++cnt;
		shortestPath(source, parent[destination], parent, distance_cnt, cnt);
	}
}
/**
 * Breadth First Search algorithm
 * Checks all neighbours; if their type of food is different, adds food and computes shortest path
 * @param [in] source 				starting node for BFS
 */
void Graph::bfs(const int &source) {
	auto distance = new int[m_VertexCount];
	auto parent = new int[m_VertexCount];
	for (int i = 0; i < m_VertexCount; ++i) {
		distance[i] = MAX_TOWNS;
		parent[i] = -1;
	}
	list<int> l;
	distance[source] = 0;
	int distance_cnt = 0;
	l.push_back(source);
	while (!l.empty() && (m_Food[source].size() != (unsigned) m_Variety)) {
		int node = l.front();
		l.pop_front();
		for (const auto &it : m_AdjList[node]) {
			if (distance[it] == MAX_TOWNS) {
				l.push_back(it);
				parent[it] = node;
				distance[it] = distance[node] + 1;
				auto ret = m_Food[source].insert(m_FoodCheck.at((unsigned) it));
				if (ret.second) {int cnt = 0; shortestPath(source, it, parent, distance_cnt, cnt); }
				if ((m_Food[source].size() == (unsigned) m_Variety)) {
					delete[] distance;
					delete[] parent;
					m_Distance.insert(m_Distance.begin() + source, distance_cnt);
					m_TotalDistance += distance_cnt;
					return;
				}
				
			}
		}
	}
	delete[] distance;
	delete[] parent;
}
/**
 * Print the total distance needed to satisfy all towns,
 * print the distance needed for town and food in town
 */
void Graph::printOutput() {
	cout << m_TotalDistance << endl;
	for (int i = 0; i < m_VertexCount; ++i) {
		cout << (m_Distance.empty() ? 0 : m_Distance[i]) << " ";
		for (const auto &z : m_Food[i])
			cout << z << " ";
		cout << endl;
	}
}

int main() {
	int towns = 0, roads = 0, food = 0, variety = 0;
	cin >> towns >> roads;
	cin >> food >> variety;
	
	Graph g(towns, variety);
	
	int food_in_town = 0;
	for (int i = 0; i < towns; ++i) {
		cin >> food_in_town;
		g.addFoodCheck(i, food_in_town);
		g.addFood(i, food_in_town);
	}
	
	int from = 0, to = 0;
	for (int i = 0; i < roads; ++i) {
		cin >> from >> to;
		g.addEdge(from, to);
	}
	
	int a = 0;
	while (a < towns) {
		g.bfs(a);
		++a;
	}
	g.printOutput();
	
	return 0;
}
#include <bits/stdc++.h>

#define MAX 1001
#define INF 1000000007
#define pb push_back
#define VI vector < int >
#define read(a) freopen(a, "r", stdin)
#define write(a) freopen(a, "w", stdtout)

using namespace std;
#define VP vector < Pair >

map < int, int > M;// map value starts from 1, not 0 (for distinguishing)
VI G[MAX];// vector that defines the graph
bool active[MAX];// true if a function (variable) node is active
bool flag[MAX][MAX];// flag[u][v] is true if u has sent data to v
int comp[MAX][MAX], trans[MAX][MAX];// asymetric matrix containing costs
char str[MAX];// input machines
int nodes[MAX];// nodes by index
int val;// the number of nodes+1 (after mapping)
bool type[MAX];// function or variable
char status[] = {'x', 'f'};
VI Machines[MAX];// vector of machines, each vector contains the nodes
int num_mac;// total number of machines
int cost[MAX][MAX];// cost[u][v] is the total cost from u to v
int idx_mac[MAX];// which machine a node belong to

// structure for an edge from u to v
struct Edge {
	int u, v, c;// sender u, receiver v
	
	Edge (){}
	Edge (int a, int b){
		u = a, v = b;
		c = cost[u][v];
	}
	// compare function for priority_queue
	bool operator < (Edge e) const {
		// sort by cost first. if tied, then by machine number
		//return ((cost[u][v] < cost[e.u][e.v]) || (cost[u][v] == cost[e.u][e.v] && idx_mac[u] < 
											//		idx_mac[e.u]));
		return ((c > e.c) || (c == e.c && idx_mac[u] > idx_mac[e.u]))
		;
	}
	// true if this edge is active
	bool is_active(){
		int sz = G[u].size();
		// take all edges
		for (int i = 0; i < sz; i++){
			int w = G[u][i];
			//cout << u << " " << v << " " << w << " " << flag[w][u] << endl;
			
			if (v != w){
				// has to receive message from every node other than v
				if (!flag[w][u])	return false;
			}
		}
		return true;
	}
	
	void print(){
		cout << u << " " << v << " " << cost[u][v] << endl;
	}
};

//input
void process_inputs(){
	int m, n, k;// take inputs
	cin >> m >> n >> k;
	// nodes and costs
	while (k--){
		int func, var, cost_cf, cost_cv, cost_tf, cost_tv;
		cin >> func >> var;
		if (!M[func])	M[func] = val++;
		int u = M[func];
		nodes[u] = func;
		type[u] = true;
		//cout << func << " " << u << " "  << val << " " << nodes[u] << endl;
		if (!M[var])	M[var] = val++;
		int v = M[var];
		nodes[v] = var;
		//cout << var << " " << v << " "  << val << " " << nodes[v] << endl;
		G[u].pb(v);
		G[v].pb(u);
		cin >> cost_cf >> cost_tf;
		comp[u][v] = cost_cf;
		trans[u][v] = cost_tf;
		cost[u][v] = cost_cf+cost_tf;
		cin >> cost_cv >> cost_tv;
		comp[v][u] = cost_cv;
		trans[v][u] = cost_tv;
		cost[v][u] = cost_cv+cost_tv;
	}
	gets(str);// clearing buffer
	// machines
	while (gets(str)){
		char *p = strtok(str, " ,");
		VI tmp;
		tmp.clear();
		while (p){
			int node = atoi(p);
			tmp.pb(node);
			p = strtok(0, " ,");
			idx_mac[node] = num_mac;// this node belongs to the current machine number
		}
		sort(tmp.begin(), tmp.end());
		Machines[num_mac] = tmp;
		num_mac++;
	}
}

// debug prints
void print_graph(){
	for (int i = 1; i < val; i++){
		//int u = nodes[i];
		int sz = (int) G[i].size();
		for (int j = 0; j < sz; j++){
			int v = G[i][j];
			cout << i << " " << v << endl;
		}
	}
}

void print_nodes(){
	cout << val << endl;
	for (int i = 1; i < val; i++){
		//cout << nodes[i] << " " << i << endl;
	}
}

void print_machines(){
	for (int i = 0; i < num_mac; i++){
		cout << "Machine " << i+1<< ":";
		for (int j = 0; j < Machines[i].size(); j++){
			cout << " " << Machines[i][j];
		}
		cout << endl;
	}
}

void print_status(VI inits){
	for (int i = 0; i < inits.size(); i++){
		cout << inits[i] << " " << nodes[inits[i]] << endl;
	}
	for (int i = 1; i < val; i++){
		cout << i << " " << status[type[i]] << nodes[i] << " " << type[i] << endl;
	}
}

void test_queue(){
	priority_queue < Edge > Q;
	while (!Q.empty())	Q.pop();
	Q.push(Edge(1, 2));
	Q.push(Edge(2, 3));
	Q.push(Edge(1, 3));
	Q.push(Edge(2, 1));
	Edge e = Q.top();
	e.print();
	Q.pop();
	e = Q.top();
	e.print();
	Q.pop();
	e = Q.top();
	Q.pop();
	e.print();
	cout << e.is_active() << endl;
}

// true if u and v are in the same machine
bool same_mac(int u, int v){
	return (idx_mac[u] == idx_mac[v]);
}

void reset(){
	memset(flag, 0, MAX);
}

int num_act = 0;

void propagate(){
	int cnt = 1;
	priority_queue < Edge > Q;
	while (!Q.empty())	Q.pop();
	//while (cnt <= 1){
		for (int u = 1; u < val; u++){
			int sz = G[u].size();
			for (int i = 0; i < sz; i++){
				int v = G[u][i];
				Edge e = Edge(u, v);
				bool fl = e.is_active();
				//cout << u << " " << v << " " << fl << endl;
				if (fl)	Q.push(e);
			}
		}
		Edge e = Q.top();
		e.print();
		
		//cnt++;
	//}
	
}


int main(){
	read("input.txt");
	val = 1;
	process_inputs();
	propagate();
	//test_queue();
	
	return 0;
}
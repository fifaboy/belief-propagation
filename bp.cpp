#include <bits/stdc++.h>

#define MAX 1001
#define INF 1000000007
#define pb push_back
#define VI vector < int >
#define read(a) freopen(a, "r", stdin)
#define write(a) freopen(a, "w", stdtout)
#define clr(a) memset(a, 0, sizeof(a))
#define VE vector < Edge >

using namespace std;
#define VP vector < Pair >

map < int, int > M;// map value starts from 1, not 0 (for distinguishing)
VI G[MAX];// vector that defines the graph
bool active[MAX];// true if a function (variable) node is active
bool flag[MAX][MAX];// flag[u][v] is true if u has sent data to v already i.e. edge u to v
					// has been active
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
		cout << "data sending from "<< u << " to " << v << ". computation cost " << comp[u][v] 
		<< " transmission cost " << trans[u][v]<< endl;
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

int total_cost(Edge e){
	// may be updated because of the simulation, so calculate every time
	return (comp[e.u][e.v]+trans[e.u][e.v]);
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
	clr(flag);
}

int num_act = 0;
int mac_vis[MAX];// for each iteration, it is reset. if mac_vis[u] is true,
				 // then this machine has been visited already.

VE tmp;
int total = 0;

void propagate(){
	clr(flag);
	// first test for just one iteration
	while (true){
		clr(mac_vis);
		tmp.clear();
		cout << "Time " << total << ": " << endl;
		for (int u = 1; u < val; u++){
			int sz = G[u].size();
			bool is_vis = mac_vis[idx_mac[u]];
			//cout << "In node " << u << endl;
			// parent machine of node u. if visited, ignore all the edges that go from u
			//if (is_vis)	continue;
			
			for (int i = 0; i < sz; i++){
				int v = G[u][i];
				Edge e = Edge(u, v);
				// if this edge is visited already, then true
				if (flag[u][v])	continue;
				//cout << u << " " << v << " " << idx_mac[u] << " " << flag[u][v] << endl;
				bool fl = e.is_active();
				if (fl){
					flag[u][v] = true;
					mac_vis[idx_mac[u]] = true;
					tmp.pb(e);
				}
			}
		}
		// no more active edge to be found
		if (tmp.size() == 0){
			cout << "Propagation finished" << endl;
			break;
		}
		int min_cost = INF, idx = 0, i_val = 0;
		// idx is the machine id with minimum total cost
		// find the minimum total cost
		// change to some other cost in total_cost function if necessary
		// iterate through all active nodes
		for (int i = 0; i < tmp.size(); i++){
			//tmp[i].print();
			//cout << "node "<< tmp[i].u << " sends data to " << tmp[i].v << endl;
			// total cost of the current active node
			int val = total_cost(tmp[i]);
			if (min_cost > val){
				min_cost = val;
				i_val = i;
				idx = idx_mac[tmp[i].u];
			}
		}
		total += min_cost;
		for (int i = 0; i < tmp.size(); i++){
			if (i_val == i){
				cout << "Minimum cost: Data sent from "<< tmp[i].u << " to " << tmp[i].v << endl;
				continue;// the node with min_cost. nothing to do
			}
			int u = tmp[i].u, v = tmp[i].v;
			// if on the same machine, reduce the computation time but don't transmit
			if (idx_mac[u]  == idx){
				comp[u][v] = max(0, comp[u][v]-min_cost);
				cout << "Data computed for edge "<< u << " to " << v << ", costs: " << comp[u][v]<< 
				" "<< trans[u][v] <<endl;
			}
			// if on different machine, reduce comp cost first, then transmit since it can be sent
			else {
				// if comp cost alone is bigger than min_cost, transmit cost can not be reduced
				if (comp[u][v] >= min_cost){
					comp[u][v] -= min_cost;
				}
				// otherwise, both can be reduced, and comp cost is now 0
				else {
					int val = min_cost-comp[u][v];
					comp[u][v] = 0;
					trans[u][v] -= val;
					trans[u][v] = max(0, trans[u][v]);// in case it is less than 0
				}
				cout << "Data sent from "<< u << " to " << v << ", costs: " << comp[u][v] << " "<< 
				trans[u][v] << endl;
			}
		}
		cout <<  endl;
	}
	
}


int main(){
	read("input2.txt");
	val = 1;
	process_inputs();
	propagate();
	//cout << total << endl;
	//test_queue();
	
	return 0;
}
#include <bits/stdc++.h>

#define MAX 10001
#define pb push_back
#define VI vector < int >
#define read(a) freopen(a, "r", stdin)
#define write(a) freopen(a, "w", stdtout)

using namespace std;
#define VP vector < Pair >

map < int, int > M;// map value starts from 1, not 0 (for distinguishing)
VI G[MAX];// vector that defines the graph
bool active_left[MAX], active_right[MAX];// true if a function (variable) node is active
int comp[MAX][MAX], trans[MAX][MAX];// asymetric matrix containing costs
char str[MAX];// input machines
int nodes[MAX];
int val;
bool type[MAX];// function or variable
char status[] = {'x', 'f'};
VI Machines[MAX];
int num_mac;
queue < int > Q;
int idx_mac[MAX];// which machine a node belong to

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
		cin >> cost_cv >> cost_tv;
		comp[v][u] = cost_cv;
		trans[v][u] = cost_tv;
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
		cout << "Machine " << i+1;
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

// finds nodes that are initially active
VI initiators(){
	VI init;
	init.clear();
	for (int i = 1; i < val; i++){
		if (G[i].size() == 1){
			init.pb(i);
		}
	}
	return init;
}

int main(){
	read("input.txt");
	val = 1;
	process_inputs();
	//print_nodes();
	//print_graph();
	VI inits = initiators();
	//print_status();
	//print_machines();
	
	return 0;
}
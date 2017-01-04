#include <bits/stdc++.h>

using namespace std;

struct Edge{
	int u, v;
	Edge(){}
	Edge (int a, int b){
		u = a, v = b;
	}
	
	bool operator < (Edge e) const{
		return u > e.u || (u == e.u && v > e.v);
	}
	
	void print(){
		cout << u << " " << v << endl;
	}
};

priority_queue < Edge > Q;

int main(){
	while (!Q.empty())	Q.pop();
	Q.push(Edge(10, 12));
	Q.push(Edge(10, 14));
	Q.push(Edge(-1, 15));
	//Q.push();
	Edge e = Q.top();
	e.print();
	Q.pop();
	e = Q.top();
	e.print();
	Q.pop();
	
	return 0;
}
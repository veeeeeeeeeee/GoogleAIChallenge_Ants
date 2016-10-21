#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int a[10] = {10, 4, 9, 23, 7, 12, 2, 17, 9, 5};
int b[10] = {5, 9, 17, 2, 12, 7, 23, 9, 4, 10};

typedef pair<int, int> P;

struct cmp {
	bool operator()(P const& lhs, P const& rhs) const {
		return lhs.first > rhs.first;
	}
};

int main() {
	priority_queue< P, vector<P>, cmp > pq;
	for (int i=0; i<10; i++) {
		pq.push(P(a[i], b[i]));
		cout << pq.top().first << " " << pq.top().second << endl;
	}
	
	return 0;
}

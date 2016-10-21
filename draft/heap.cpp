#include <iostream>
#include <vector>

using namespace std;


int a[] = {-1, 32, 10, 24, 12, 8, 17, 4, 6, 42, 20, 5};
vector<int> v = vector<int>(a, a + sizeof(a)/sizeof(int));

void swap(int& a, int& b) {
	int tmp = b;
	b = a;
	a = tmp;
}

void heapify(vector<int>& v, int i) {
	int l = i*2;
	int r = i*2 + 1;
	int s = (int)v.size();
	int smallest = 1;
	
	if (l >= s)
		return;
	
	else {
		if (r < s) {
			if (v[i] < v[l] && v[i] < v[r])
				return;
		}
		else if (v[i] < v[l])
			return;
		
		if (v[i] > v[l])
			smallest = l;
		
		if (r < s)
			if (v[l] > v[r])
				smallest = r;
		
		if (smallest != 1) {
			swap(v[i], v[smallest]);
			heapify(v, smallest);
		}
	}
	return;
}

void build(vector<int>& v) {
	int s = (int)v.size();
	for (int i=s-1; i>=0; i--) {
		heapify(v, i);
	}
}


void add(vector<int>& v, int x) {
	v.push_back(x);
	
	int nx = (int)v.size()-1;
	int p = nx / 2;
	
	bool loop = true;
	while (loop) {
		if (nx <= 1)
			loop = false;
		
		else {
			int p = nx / 2;
			if (v[nx] < v[p]) {
				int tmp = v[nx];
				v[nx] = v[p];
				v[p] = tmp;
				nx = p;
			}
			else loop = false;
		}
	}
}

void delMin(vector<int>& v) {
	int nm = 1;
	if (v.size() == 1)
		return;

	if (v.size() == 2)
		v.clear();

	if (v.size() == 3) {
		int tmp = v[1];
		v[1] = v[2];
		v[2] = tmp;
		v.pop_back();
		return;
	}

	while (nm * 2 < (int)v.size()) {
		int toSwap = nm * 2;
		if (toSwap + 1 < (int)v.size())
			if (v[toSwap] > v[toSwap + 1])
				toSwap ++;
		
		if (v[toSwap] > v.back()) {
			int tmp = v[nm];
			v[nm] = v.back();
			v[(int)v.size() - 1] = tmp;

			break;
		}
		else {
			int tmp = v[nm];
			v[nm] = v[toSwap];
			v[toSwap] = tmp;
			nm = toSwap;
		}
	}
	if (nm == (int)v.size()-2) {
		int tmp = v[nm];
		v[nm] = v[(int)v.size()-1];
		v[(int)v.size()-1] = tmp;
	}
	v.pop_back();
}

void heapsort(vector<int>& v) {
	build(v);
	for (int i=0; i<(int)v.size(); i++) {
		cout << v[i] << " ";
	}
	cout << endl;

	vector<int> res;

	while ((int)v.size() >= 1) {
		res.push_back(v[1]);
		delMin(v);
	}
	v = res;
}

int main() {
	//heapsort(v);

	return 0;
}

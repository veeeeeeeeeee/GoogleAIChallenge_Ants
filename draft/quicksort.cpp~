#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

void selectSort(vector<int>& v, int l, int r) {
	/*
	 * sort a from (and including l and r)
	 */

	for (int i=l; i<=r; i++) {
		int min = v[i];
		int minInd = i;
		for (int j=i+1; j<=r; j++) {
			if (min > v[j]) {
				min = v[j];
				minInd = j;
			}
		}
		if (minInd != i) {
			swap(v[i], v[minInd]);
		}
	}
}

/*
 * General idea:
 * if S.size == 0 or 1 then return
 * else:
 * 	pick a pivot v
 * 	partition S - {v} into 2 groups
 * 		S1{x <= v} and S2{x >= v}
 * 		return quicksort(S1) + v + (S2)
 */

/*
 * use left, right, med to get pivot
 * swap pivot to the end
 * with 2 cursors: i = left, j = right - 1
 * repeat until i and j cross
 * 	repeat until s[i] < pivot && s[j] > pivot
 * 		i++, j++
 * 	afterwards:
 * 		swap s[i], s[j]
 * afterwards:
 * 	swap pivot with i
 */

/*
 * only useful with large arrays
 * thus:
 * quicksort(left, right) {
 * 	if left+10 < right
 * 		do n2 sort instead
 * 	else:
 * 		select pivot on left, right, med
 * 		i = left, j = right - 1
 * 		do {
 * 			while a[++i] < pivot
 * 			while [--j] > pivot
 *
 * 			swap a[i], a[j]
 * 		} while i<j
 * 		swap a[i], a[right-1] - restore pivot (at this stage, left of pivot < pivot < right of pivot)
 * 		
 * 		quicksort(a, left, i-1)
 * 		quicksort(a, i+1, right)
 * }
 */

void quicksort(vector<int>& a, int l, int r) {
	int pv;
	if (l+10 < r)
		selectSort(a, l, r);
	else {
		int med = (l+r) / 2;
		if (a[l] <= a[med]) {
			if (a[r] <= a[l])
				pv = l;
			if (a[l] <= a[r] && a[r] <= a[med])
				pv = r;
			if (a[med] <= a[r])
				pv = med;
		}
		else {
			if (a[r] <= a[med])
				pv = med;
			if (a[med] <= a[r] && a[r] <= a[l])
				pv = r;
			if (a[l] <= a[r])
				pv = l;
		}
		swap(a[pv], a[r]); pv = r;
		// done selecting pivot
		int i = l, j = r-1;
		do {
			while (a[++i] < a[pv]);
			while (a[--j] > a[pv]);

			swap(a[i], a[j]);
		} while (i < j);
		swap(a[i], a[r]);

		quicksort(a, l, i-1);
		quicksort(a, i+1, r);
	}
}

void swap(int& a, int& b) {
	int tmp = a;
	a = b;
	b = tmp;
}

int main() {
	int a[] = {5, 10, 3, 6, 14, 27, 12, 1, 4, 11, 15, 32, 13, 20, 9, 26, 75, 49, 37, 34, 18, 17, 2, 25, 40, 36};
	vector<int> v = vector<int>(a, a + sizeof(a)/sizeof(int));
/*
	selectSort(v, 0, (int)v.size()-1);
	quicksort(v, 0, (int)v.size()-1);
	for (int i=0; i<(int)v.size(); i++) {
		cout << v[i] << " ";
	}
	cout << endl;
*/
	return 0;
}

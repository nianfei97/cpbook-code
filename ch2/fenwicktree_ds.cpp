#include <bits/stdc++.h>
using namespace std;

#define LSOne(S) ((S) & -(S))

typedef vector<int> vi;

class FenwickTree {                    // remember that index 0 is not used
private: 
	vi ft;
	int n;

public: 
	FenwickTree() {

	}

	FenwickTree(int _n) : n(_n) { 
		ft.assign(n + 1, 0);
	}

	// Construct from frequency table
	FenwickTree(const vi& f) : n(f.size() - 1) {
		ft.assign(n + 1, 0);
		for (int i = 1; i <= n; i++) {
			ft[i] += f[i];
			if (i + LSOne(i) <= n)
				ft[i+LSOne(i)] += ft[i]; 
		} 
	}

	void init(int _n) {
		n = _n;
		ft.clear();
		ft.assign(n + 1, 0);
	}

	// [1, j]
	int rsq(int j) {
		int sum = 0; 
		for (; j; j -= LSOne(j)) 
			sum += ft[j];
		return sum; 
	}

	// [i, j]
	int rsq(int i, int j) {
		return rsq(j) - rsq(i - 1); 
	}

	
	void update(int i, int v) {
		for (; i <= n; i += LSOne(i)) 
			ft[i] += v; 
	}

	// Find index where cumulative frequency is k
	int select(int k) {
		int lo = 1, hi = n;
		for (int i = 0; i < 30; i++) {
			int mid = (lo + hi) / 2;
			(rsq(1, mid) < k) ? lo = mid : hi = mid; 
		}
		return hi; 
	}
};

class RUPQ : FenwickTree {
public:
	RUPQ(int n) : FenwickTree(n) {}

	int query(int i) { 
		return rsq(i); 
	}

	// [i, j] += v
	void update(int i, int j, int v) {
		FenwickTree::update(i, v);
		FenwickTree::update(j + 1, -v);
	}
};

class RURQ {
private:
	FenwickTree t1, t2;

public:
	RURQ(int n) {
		t1.init(n);
		t2.init(n);
	}

	void update(int l, int r, int v) {
		t1.update(l, v);
		t1.update(r + 1, -v);
		t2.update(l, (l - 1) * v);
		t2.update(r + 1, -r * v);
	}

	int rsq(int j) {
		return t1.rsq(j) * j - t2.rsq(j);
	}

	int rsq(int i, int j) {
		return rsq(j) - rsq(i - 1);
	}
};

int main() {
							 // idx   0 1 2 3 4 5 6 7  8 9 10, no index 0!
	FenwickTree ft1(10);     // ft = {-,0,0,0,0,0,0,0, 0,0,0}
	ft1.update(2, 1);        // ft = {-,0,1,0,1,0,0,0, 1,0,0}, idx 2,4,8 => +1
	ft1.update(4, 1);        // ft = {-,0,1,0,2,0,0,0, 2,0,0}, idx 4,8 => +1
	ft1.update(5, 2);        // ft = {-,0,1,0,2,2,2,0, 4,0,0}, idx 5,6,8 => +2
	ft1.update(6, 3);        // ft = {-,0,1,0,2,2,5,0, 7,0,0}, idx 6,8 => +3
	ft1.update(7, 2);        // ft = {-,0,1,0,2,2,5,2, 9,0,0}, idx 7,8 => +2
	ft1.update(8, 1);        // ft = {-,0,1,0,2,2,5,2,10,0,0}, idx 8 => +1
	ft1.update(9, 1);        // ft = {-,0,1,0,2,2,5,2,10,1,1}, idx 9,10 => +1
	printf("%d\n", ft1.rsq(1, 1));  // 0 => ft[1] = 0
	printf("%d\n", ft1.rsq(1, 2));  // 1 => ft[2] = 1
	printf("%d\n", ft1.rsq(1, 6));  // 7 => ft[6]+ft[4] = 5+2 = 7
	printf("%d\n", ft1.rsq(1, 10)); // 11 => ft[10]+ft[8] = 1+10 = 11
	printf("%d\n", ft1.rsq(3, 6));  // 6 => rsq(1, 6) - rsq(1, 2) = 7-1 = 6
	printf("%d\n", ft1.select(7));  // index 6, rsq(1, 6) == 7, which is >= 7
	printf("%d\n", ft1.select(8));  // index 7, rsq(1, 7) == 9, which is >= 8
	ft1.update(5, 2); // update demo
	printf("%d\n", ft1.rsq(1, 10)); // now 13

	printf("=====\n");

	vector<int> f = {0,0,1,0,1,2,3,2,1,1,0};  // index 0 is always 0 (unused)
	FenwickTree ft2(f);
	printf("%d\n", ft2.rsq(1, 1));  // 0 => ft[1] = 0
	printf("%d\n", ft2.rsq(1, 2));  // 1 => ft[2] = 1
	printf("%d\n", ft2.rsq(1, 6));  // 7 => ft[6]+ft[4] = 5+2 = 7
	printf("%d\n", ft2.rsq(1, 10)); // 11 => ft[10]+ft[8] = 1+10 = 11
	printf("%d\n", ft2.rsq(3, 6));  // 6 => rsq(1, 6) - rsq(1, 2) = 7-1 = 6
	printf("%d\n", ft2.select(7));  // index 6, rsq(1, 6) == 7, which is >= 7
	printf("%d\n", ft2.select(8));  // index 7, rsq(1, 7) == 9, which is >= 8
	ft2.update(5, 2); // update demo
	printf("%d\n", ft2.rsq(1, 10)); // now 13

	printf("=====\n");

	RUPQ rupq(10) ;                        // empty Fenwick Tree with 10 keys
	rupq.update(2, 9, 7);     // indices in [2, 3, .., 9] updated by +7
	rupq.update(6, 7, 3); // indices 6&7 are further updated by +3 (10)
	for (int i = 1; i <= 10; i++)
		printf("%d -> %d\n", i, rupq.query(i));

	printf("=====\n");

	RURQ rurq(18);
	printf("%d\n", rurq.rsq(14, 18));  // 0
	rurq.update(14, 18, 1);
	printf("%d\n", rurq.rsq(14, 18));  // 5
	rurq.update(12, 16, 1);
	printf("%d\n", rurq.rsq(16, 18));  // 4
	rurq.update(4, 7, 1);
	printf("%d\n", rurq.rsq(16, 18));  // 4
	rurq.update(7, 14, 1);
	printf("%d\n", rurq.rsq(11, 14));  // 8
	printf("%d\n", rurq.rsq(1, 18));   // 22
	
	return 0;
}

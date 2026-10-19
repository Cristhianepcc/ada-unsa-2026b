#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int particionar(vector<long long>& a, int l, int r) {
    int p = l + rand() % (r - l + 1);
    swap(a[p], a[r]);
    long long pivote = a[r];
    int i = l;
    for (int j = l; j < r; j++) {
        if (a[j] < pivote) { swap(a[i], a[j]); i++; }
    }
    swap(a[i], a[r]);
    return i;
}

long long quickSelect(vector<long long>& a, int l, int r, int idx) {
    while (true) {
        if (l == r) return a[l];
        int p = particionar(a, l, r);
        if (p == idx) return a[p];
        else if (idx < p) r = p - 1;
        else l = p + 1;
    }
}

int main() {
    srand((unsigned)time(nullptr));
    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    int idx = n - k;   // k-esimo mas grande = (n-k)-esimo mas chico (0-index, ascendente)
    cout << quickSelect(a, 0, n - 1, idx) << '\n';
    return 0;
}

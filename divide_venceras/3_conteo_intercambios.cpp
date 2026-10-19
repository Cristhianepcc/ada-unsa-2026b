#include <iostream>
#include <vector>
using namespace std;

vector<int> a, aux;

long long mergeContar(int l, int r) {
    if (r - l <= 1) return 0;
    int mid = (l + r) / 2;
    long long inv = mergeContar(l, mid) + mergeContar(mid, r);

    int i = l, j = mid, k = l;
    while (i < mid && j < r) {
        if (a[i] <= a[j]) {
            aux[k++] = a[i++];
        } else {
            aux[k++] = a[j++];
            inv += mid - i;      // a[i..mid-1] son todos > a[j]: inversiones cruzadas
        }
    }
    while (i < mid) aux[k++] = a[i++];
    while (j < r)   aux[k++] = a[j++];
    for (int t = l; t < r; t++) a[t] = aux[t];
    return inv;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        a.assign(n, 0);
        aux.assign(n, 0);
        for (int i = 0; i < n; i++) scanf("%d", &a[i]);
        printf("%lld\n", mergeContar(0, n));
    }
    return 0;
}

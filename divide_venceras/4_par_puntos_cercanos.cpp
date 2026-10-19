#include <iostream>
#include <vector>
#include <climits>
#include <cmath>

using namespace std;

struct P { long long x, y; };
vector<P> pts;
vector<P> aux;

long long dist2(P a, P b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return dx * dx + dy * dy;
}

void ordenarPorX(int l, int r) {
    if (r - l <= 1) return;
    int mid = (l + r) / 2;
    ordenarPorX(l, mid);
    ordenarPorX(mid, r);

    int i = l, j = mid, k = l;
    while (i < mid && j < r) {
        if (pts[i].x <= pts[j].x) { aux[k] = pts[i]; i++; k++; }
        else                      { aux[k] = pts[j]; j++; k++; }
    }
    while (i < mid) { aux[k] = pts[i]; i++; k++; }
    while (j < r)   { aux[k] = pts[j]; j++; k++; }
    for (int t = l; t < r; t++) pts[t] = aux[t];
}

void mezclarPorY(int l, int mid, int r) {
    int i = l, j = mid, k = l;
    while (i < mid && j < r) {
        if (pts[i].y <= pts[j].y) { aux[k] = pts[i]; i++; k++; }
        else                      { aux[k] = pts[j]; j++; k++; }
    }
    while (i < mid) { aux[k] = pts[i]; i++; k++; }
    while (j < r)   { aux[k] = pts[j]; j++; k++; }
    for (int t = l; t < r; t++) pts[t] = aux[t];
}

long long rec(int l, int r) {
    int n = r - l;

    if (n <= 3) {
        long long mejor = LLONG_MAX;
        for (int i = l; i < r; i++)
            for (int j = i + 1; j < r; j++)
                mejor = min(mejor, dist2(pts[i], pts[j]));
        for (int i = l + 1; i < r; i++) {
            P actual = pts[i];
            int j = i - 1;
            while (j >= l && pts[j].y > actual.y) {
                pts[j + 1] = pts[j];
                j--;
            }
            pts[j + 1] = actual;
        }
        return mejor;
    }

    int mid = (l + r) / 2;
    long long xLinea = pts[mid].x;

    long long dIzq = rec(l, mid);
    long long dDer = rec(mid, r);
    long long mejor = min(dIzq, dDer);

    mezclarPorY(l, mid, r);

    vector<P> franja;
    for (int i = l; i < r; i++) {
        long long dx = pts[i].x - xLinea;
        if (dx * dx < mejor) franja.push_back(pts[i]);
    }

    for (size_t i = 0; i < franja.size(); i++) {
        for (size_t j = i + 1; j < franja.size(); j++) {
            long long dy = franja[j].y - franja[i].y;
            if (dy * dy >= mejor) break;      // los demas estan mas arriba: no sirven
            mejor = min(mejor, dist2(franja[i], franja[j]));
        }
    }
    return mejor;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        pts.assign(n, {0, 0});
        aux.assign(n, {0, 0});
        for (int i = 0; i < n; i++) scanf("%lld %lld", &pts[i].x, &pts[i].y);

        if (n == 1) { printf("INFINITY\n"); continue; }

        ordenarPorX(0, n);
        long long d = rec(0, n);

        if (d >= 10000LL * 10000LL) printf("INFINITY\n");
        else printf("%.4f\n", sqrt((double)d));
    }
    return 0;
}

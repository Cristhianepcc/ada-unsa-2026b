#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<long long> resolver(const string &s, int l, int r) {
    vector<long long> res;
    bool tieneOp = false;

    for (int i = l; i < r; i++) {
        char c = s[i];
        if (c == '+' || c == '-' || c == '*') {
            tieneOp = true;
            vector<long long> izq = resolver(s, l, i);
            vector<long long> der = resolver(s, i + 1, r);
            for (long long a : izq)
                for (long long b : der) {
                    if (c == '+')      res.push_back(a + b);
                    else if (c == '-') res.push_back(a - b);
                    else               res.push_back(a * b);
                }
        }
    }

    if (!tieneOp) {
        long long num = 0;
        for (int i = l; i < r; i++) num = num * 10 + (s[i] - '0');
        res.push_back(num);
    }
    return res;
}

int main() {
    string linea;
    while (getline(cin, linea)) {
        if (linea.empty()) continue;
        vector<long long> res = resolver(linea, 0, (int)linea.size());
        sort(res.begin(), res.end());
        for (size_t i = 0; i < res.size(); i++) {
            if (i) cout << ' ';
            cout << res[i];
        }
        cout << '\n';
    }
    return 0;
}

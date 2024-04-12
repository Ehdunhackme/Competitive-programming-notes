#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

using ll = long long;
using vi = vector<int>;
int N;

vi operator+(const vi &l, const vi &r) {
    vi res = l;
    if (size(l) < size(r)) res.resize(size(r));
    for (int i = 0; i < size(r); ++i) res.at(i) += r.at(i);
    res.insert(begin(res), 0);
    int sum = accumulate(begin(res), end(res), 0);
    while (true) {
        assert(size(res));
        if (sum - res.back() < N) break;
        sum -= res.back();
        res.pop_back();
    }
    return res;
}

struct DP {
    int init;
    vi dp;
    void advance(int x) {  // push x strings into subtree
        for (int i = 0; i < size(dp); ++i) {
            int cnt = min(x, dp[i]);
            init += cnt * i;
            x -= cnt;
            dp[i] -= cnt;
        }
        assert(x == 0);
    }
};

DP empty_dp() {  // if no strings starting with p
    vi ans{1, 0};
    int s = 1;
    for (int i = 2; s < N; ++i) {
        int x = 1 << (i - 2);
        ans.push_back(x);
        s += x;
    }
    return DP{0, ans};
}

DP operator+(const DP &l, const DP &r) {
    return DP{l.init + r.init, l.dp + r.dp};
}

struct TrieNode {
    int cnt;
    array<int, 2> child;
    DP dp;
};

vector<TrieNode> nodes(2);
int nxt = 1;

void fill(int idx) {
    if (nodes.at(idx).child.at(0) == 0 && nodes.at(idx).child.at(1) == 0) {
        nodes.at(idx).dp = nodes.at(0).dp;
    } else {
        nodes.at(idx).dp = nodes.at(nodes.at(idx).child.at(0)).dp +
                           nodes.at(nodes.at(idx).child.at(1)).dp;
    }
    nodes.at(idx).dp.advance(nodes.at(idx).cnt);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> N;
    for (int i = 0; i < N; ++i) {
        string s;
        cin >> s;
        int cur = 1;
        for (char c : s) {
            if (!nodes.at(cur).child.at(c - '0')) {
                nodes.at(cur).child.at(c - '0') = ++nxt;
                nodes.emplace_back();
            }
            cur = nodes.at(cur).child.at(c - '0');
        }
        ++nodes.at(cur).cnt;
    }
    nodes.at(0).dp = empty_dp();
    for (int i = nxt; i >= 1; --i) fill(i);
    cout << nodes.at(1).dp.init << "\n";
}

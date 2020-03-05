//
// Created by vlad on 28.12.2019.
//

#include "SalesmanTotal.h"
SalesManTotal::SalesManTotal(const graph::Edges& edges, int n)
    : ans(std::numeric_limits<double>::max()),
      n(n),
      w(n, n),
      p(n, 0),
      used(n, false) {
    for (auto& e : edges) {
        w(e.u_, e.v_) = e.w_;
        w(e.v_, e.u_) = e.w_;
    }
}
void SalesManTotal::rec(int idx, double len) {
    if (len >= ans)
        return;
    if (idx == n) {
        ans = std::min(ans, len + w(p[idx - 1], 0));
        return;
    }
    for (int i = 0; i < n; i++) {
        if (used[i])
            continue;
        p[idx] = i;
        used[i] = true;
        rec(idx + 1, len + w(p[idx - 1], i));
        used[i] = false;
    }
}

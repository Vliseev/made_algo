#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> ZFunction(const std::string &s) {
    int n = (int)s.length();
    std::vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r)
            z[i] = std::min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] - 1 > r)
            l = i, r = i + z[i] - 1;
    }
    return z;
}

std::vector<int> FindPat(const std::string &s, const std::string &pat) {
    std::vector<int> pos{};
    std::string concat_pat;
    concat_pat.reserve(s.size() + 1 + pat.size());
    concat_pat += pat;
    concat_pat += (char)(0);
    concat_pat += s;

    auto z_val = ZFunction(concat_pat);

    for (int i = pat.size() + 1; i < z_val.size(); ++i) {
        if (z_val[i] == pat.size())
            pos.emplace_back(i - pat.size() - 1);
    }

    return pos;
}

int main() {

    std::string s,t;

    std::cin>>t>>s;
    for (auto pos : FindPat(s,t)) {
        std::cout << pos << " ";
    }

    return 0;
}

#include <fstream>
#include <iostream>
#include "MstSailman.h"
#include "SalesmanTotal.h"

int main() {
    auto [g, n] = graph::ReadEdges(std::cin);
    SalesManTotal smt(g, n);
    smt.rec(1, 0.0);

    std::cout << SailmanWeightMst(g, n) << "\n";
    std::cout << smt.ans << "\n";

    return 0;
}

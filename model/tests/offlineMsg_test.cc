#include <burger/db/DB.h>
#include "OfflineMsgManager.h"
#include <iostream>
#include <vector>

using namespace burger;
using namespace burger::db;

int main() {
    OfflineMsgManager off;
    off.add(3, "hello");
    off.add(3, " world");
    auto vec = off.query(3);
    for(auto v : vec) {
        std::cout<< v <<std::endl;
    }
    off.remove(3);
    auto vec1 = off.query(3);
    if(vec1.empty()) {
        std::cout << "clear all" << std::endl;
    }
}
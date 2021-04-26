#include <burger/db/DB.h>
#include "OfflineNotificationManager.h"
#include <iostream>
#include <vector>
#include <unistd.h>

using namespace burger;
using namespace burger::db;

int main() {
    OfflineNotificationManager off;

    uint64_t t1 = Timestamp::now().microSecondsSinceEpoch();
    off.add(3, 4, t1, "user", "hello");
    sleep(2);
    uint64_t t2 = Timestamp::now().microSecondsSinceEpoch();
    off.add(3, 5, t2, "user", "hello again");
    auto vec = off.query(3);
    for(auto& v : vec) {
        std::cout<< v <<std::endl;
    }
    off.remove(3, 4, t1, "user");
    auto vec1 = off.query(3);
    for(auto& v : vec1) {
        std::cout<< v <<std::endl;
    }

    off.remove(3, 5, t2, "user");

    auto vec3 = off.query(3);
    if(vec3.empty()) {
        std::cout << "clear all" << std::endl;
    }
}
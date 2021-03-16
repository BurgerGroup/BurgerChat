#include <burger/db/DB.h>
#include "user.h"
#include "userManager.h"
#include <iostream>

using namespace burger;
using namespace burger::db;

void test_add() {
    UserManager um;
    User u1("skMIT", "12333");
    User u2("lighao", "1312");
    um.add(u1);
    um.add(u2);
}

void test_query_and_state() {
    UserManager um;
    User user = um.query(1);
    std::cout << user.getName() << std::endl;
    std::cout << user.getState() << std::endl;

    // test updateState
    user.setState("online");
    um.updateState(user);
    User user1 = um.query(1);
    std::cout << user1.getState() << std::endl;
    // reset 
    um.resetState();
    user1 = um.query(1);
    std::cout << user1.getState() << std::endl;
}



int main() {
    // test_add();
    test_query_and_state();


}


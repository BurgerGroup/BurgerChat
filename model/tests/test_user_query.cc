#include <burger/db/DB.h>
#include "user.h"
#include "userManager.h"
#include <iostream>

using namespace burger;
using namespace burger::db;

int main() {
    UserManager um;
    User user = um.query(1);
    std::cout << user.getName() << std::endl;
}



#include <burger/db/DB.h>
#include "user.h"
#include "userManager.h"
#include <iostream>

using namespace burger;
using namespace burger::db;

int main() {
    UserManager um;
    User u1("skMIT", "12333");
    User u2("lighao", "1312");
    um.add(u1);
    um.add(u2);
}


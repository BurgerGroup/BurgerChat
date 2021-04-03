#include "../groupManager.h"
#include "../groupUserManager.h"
#include "../userManager.h"
#include <burger/db/DB.h>
#include <iostream>

using namespace burger;
using namespace burger::db;

UserManager um("127.0.0.1", "root", "123");
GroupManager gm("127.0.0.1", "root", "123");
GroupUserManager gum("127.0.0.1", "root", "123");

void create_users() {
    User u1("skMIT", "12333");
    User u2("lighao", "1312");
    User u3("ysy", "6666");
    um.add(u1);
    um.add(u2);
    um.add(u3);
}

void test_group_add() {
    Group g1("sk&ysy");
    Group g2("lgh&ysy");
    gm.add(g1);
    gm.add(g2);
}

void test_groupUser_add_and_query() {
    User user = um.query(1);
    std::cout << user.getName() << std::endl;
    std::cout << user.getState() << std::endl;
    std::cout << user.getId() << std::endl;

    Group group = gm.query(1);
    std::cout << group.getName() << std::endl;
    std::cout << group.getId() << std::endl;
    
    GroupUser gu1(group.getId(), user.getId());
    gum.add(gu1);

    GroupUser gu = gum.query(std::make_pair(group.getId(), user.getId()));
    std::cout << gu.getGroupId() << std::endl;
    std::cout << gu.getUserId() << std::endl;
}

int main() {
    std::cout <<  "create_users()" << std::endl;
    create_users();

    std::cout <<  "test_group_add()" << std::endl;
    test_group_add();

    std::cout <<  "test_groupUser_add_and_query()" << std::endl;
    test_groupUser_add_and_query();
    return 0;

}
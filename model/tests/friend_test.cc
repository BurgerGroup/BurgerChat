#include <burger/db/DB.h>
#include "user.h"
#include "userManager.h"
#include "friendManager.h"
#include <iostream>

using namespace burger;
using namespace burger::db;

UserManager um("127.0.0.1", "root", "123");
FriendManager fm("127.0.0.1", "root", "123");

void test_friend_add() {
    User u1 = um.query(1);
    User u2 = um.query(2);

    std::cout << u1.getName() << ", " << u2.getName() << std::endl;
    
    std::cout << "Now add friendship." << std::endl;
    fm.addFriendship(u2.getId(), u1.getId());

    if(fm.isFriend(u1.getId(), u2.getId()))
    {
        std::cout << u1.getName() << ", " << u2.getName() 
                << " now are friends." << std::endl;
    }
}

void test_friend_release() {
    User u1 = um.query(1);
    User u2 = um.query(2);

    std::cout << u1.getName() << ", " << u2.getName() << std::endl;
    
    std::cout << "Now release friendship." << std::endl;
    fm.releaseFriendship(u2.getId(), u1.getId());

    if(!fm.isFriend(u1.getId(), u2.getId()))
    {
        std::cout << u1.getName() << ", " << u2.getName() 
                << " now are NOT friends." << std::endl;
    }
}

int main() {
    test_friend_add();
    test_friend_release();
    return 0;
}
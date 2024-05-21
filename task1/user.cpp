#include "user.h"

std::ostream &operator<<(std::ostream &os, const User &user) {
    os << "User's id: " << user.getInfo().id << ", name: " << user.getInfo().name;
    std::cout << ", group id: " << user.getInfo().group.lock()->getInfo().id
              << std::endl;
    return os;
}

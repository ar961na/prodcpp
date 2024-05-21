#include "group.h"
#include "user.h"

std::ostream &operator<<(std::ostream &os, const Group &group) {
    os << "Group id: " << group.getInfo().id << '\n' << std::endl;
    for (int i = 0; i < group.getInfo().users.size(); ++i) {
        Group::Info info = group.getInfo();
        std::weak_ptr<User> weak_user = info.users[i];
        if (weak_user.expired())
            continue;
        std::cout << '\t' << *info.users[i].lock().get() << std::endl;
    }

    return os;
}

Group::Info::Info(unsigned int id, std::vector<std::weak_ptr<User>> users)
        : id(id), users{users} {}

Group::Info Group::getInfo() const { return info; }

void Group::addUser(std::shared_ptr<User> user) {
    info.users.push_back(std::weak_ptr<User>(user));
}
#ifndef TASK1_GROUP_H
#define TASK1_GROUP_H

#include <iostream>
#include <memory>
#include <vector>

class User;

class Group {

public:
    struct Info {
        unsigned int id;
        std::vector<std::weak_ptr<User>> users;

        Info(unsigned int id, std::vector<std::weak_ptr<User>> users =
        std::vector<std::weak_ptr<User>>());
    };

    Group(unsigned int id) : info(id) {}

    Info getInfo() const;

    void addUser(std::shared_ptr<User> user);

private:
    Info info;
};

std::ostream &operator<<(std::ostream &os, const Group &group);

#endif //TASK1_GROUP_H

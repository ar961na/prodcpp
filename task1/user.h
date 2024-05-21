#ifndef TASK1_USER_H
#define TASK1_USER_H

#include <iostream>
#include <memory>
#include <string>

#include "group.h"

class User {
public:
    struct Info {
        std::string name;
        unsigned int id;
        std::weak_ptr<Group> group;

        Info(unsigned int id, std::string name,
             std::shared_ptr<Group> group = nullptr)
                : id(id), name{name}, group{group} {}
    };

    User(unsigned int id, std::string name) : info(id, name) {}

    void addGroup(std::shared_ptr<Group> group) {
        this->info.group = group;
    }

    Info getInfo() const { return info; }

private:
    Info info;
};

std::ostream &operator<<(std::ostream &os, const User &user);

#endif //TASK1_USER_H

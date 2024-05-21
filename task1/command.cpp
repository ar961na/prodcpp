#include <iostream>

#include "command.h"

int findUser(unsigned int user_id, std::vector<std::shared_ptr<User>> &users) {
    for (size_t i = 0; i < users.size(); ++i)
        if (users.at(i)->getInfo().id == user_id)
            return i;
    std::cout << "No user with this id: " << user_id << std::endl;
    return -1;
}

int findGroup(unsigned int group_id,
              std::vector<std::shared_ptr<Group>> &groups) {
    for (size_t i = 0; i < groups.size(); ++i)
        if (groups.at(i)->getInfo().id == group_id)
            return i;
    std::cout << "No group with this id: " << group_id << std::endl;
    return -1;
}

std::string getComand() {
    bool word_started = false;
    std::string inp_comand = "";
    while (true) {
        char c = std::getchar();
        if (word_started == -1 && std::isspace(c) != 0) {
            continue;
        }
        if (word_started == -1 && std::isspace(c) == 0) {
            word_started = true;
            inp_comand += c;
        } else if (word_started != -1 && std::isspace(c) == 0) {
            inp_comand += c;
        } else if (word_started != -1 && std::isspace(c) != 0 || c == EOF) {
            return inp_comand;
        }
    }
}

void createUser(std::vector<std::shared_ptr<User>> &users,
                std::vector<std::shared_ptr<Group>> &groups) {
    unsigned int user_id;
    unsigned int group_id;
    std::string name;
    if (!(std::cin >> user_id >> group_id >> name).good())
        std::cout << "wrong argument" << std::endl;
    else {
        if (findUser(user_id, users) == -1 && findGroup(group_id, groups) != -1) {
            users.push_back(std::make_shared<User>(User(user_id, name)));
            users.back().get()->addGroup(groups.at(findGroup(group_id, groups)));
            groups.at(findGroup(group_id, groups))->addUser(users.back());
            std::cout << "User created successfuly" << std::endl;
        } else
            std::cout << "User exist or group doesn't exist" << std::endl;
    }
}

void deleteUser(std::vector<std::shared_ptr<User>> &users) {
    unsigned int user_id;
    if (!(std::cin >> user_id).good())
        std::cout << "wrong argument" << std::endl;
    else if (findUser(user_id, users) != -1) {
        users.erase(users.begin() + findUser(user_id, users));
        std::cout << "User deleted successfuly" << std::endl;
    } else
        std::cout << "User doesn't exist" << std::endl;
}

void allUsers(std::vector<std::shared_ptr<User>> &users) {
    for (size_t i = 0; i < users.size(); ++i)
        std::cout << *users.at(i);
}

void getUser(std::vector<std::shared_ptr<User>> &users) {
    unsigned int user_id;
    if (!(std::cin >> user_id).good())
        std::cout << "wrong argument" << std::endl;
    else if (findUser(user_id, users) != -1)
        std::cout << *users.at(findUser(user_id, users));
    else
        std::cout << "User doesn't exist" << std::endl;
}

void createGroup(std::vector<std::shared_ptr<Group>> &groups) {
    unsigned int group_id;
    if (!(std::cin >> group_id).good())
        std::cout << "wrong argument" << std::endl;
    else if (findGroup(group_id, groups) == -1) {
        groups.push_back(std::make_shared<Group>(Group(group_id)));
        std::cout << "Group created successfuly" << std::endl;
    } else
        std::cout << "Group exist" << std::endl;
}

void deleteGroup(std::vector<std::shared_ptr<Group>> &groups) {
    unsigned int group_id;
    if (!(std::cin >> group_id).good())
        std::cout << "wrong argument" << std::endl;
    else if (findGroup(group_id, groups) != -1) {
        groups.erase(groups.begin() + findGroup(group_id, groups));
        std::cout << "Group deleted successfuly" << std::endl;
    }
}

void allGroups(std::vector<std::shared_ptr<Group>> &groups) {
    for (auto group: groups) {
        std::cout << *group;
    }
}

void getGroup(std::vector<std::shared_ptr<Group>> &groups) {
    unsigned int group_id;
    if (!(std::cin >> group_id).good())
        std::cout << "wrong argument" << std::endl;
    else if (findGroup(group_id, groups) != -1) {
        std::cout << *groups.at(findGroup(group_id, groups));
    }
}
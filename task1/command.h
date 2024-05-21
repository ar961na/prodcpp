#ifndef TASK1_COMMAND_H
#define TASK1_COMMAND_H

#include <string>
#include <vector>

#include "group.h"
#include "user.h"

std::string getComand();

void createUser(std::vector<std::shared_ptr<User>> &users,
                std::vector<std::shared_ptr<Group>> &groups);
void deleteUser(std::vector<std::shared_ptr<User>> &users);
void allUsers(std::vector<std::shared_ptr<User>> &users);
void getUser(std::vector<std::shared_ptr<User>> &users);
void createGroup(std::vector<std::shared_ptr<Group>> &groups);
void deleteGroup(std::vector<std::shared_ptr<Group>> &groups);
void allGroups(std::vector<std::shared_ptr<Group>> &groups);
void getGroup(std::vector<std::shared_ptr<Group>> &groups);

#endif //TASK1_COMMAND_H

#ifndef TASK1_APP_H
#define TASK1_APP_H

#include "command.h"

class App {
private:
    std::vector<std::shared_ptr<User>> users;
    std::vector<std::shared_ptr<Group>> groups;
public:
    void play(){
        while (true) {
            std::cout << "Input command: ";
            std::string comand = getComand();
            if (comand == "createUser") {
                createUser(users, groups);
                continue;
            }
            if (comand == "deleteUser") {
                deleteUser(users);
                continue;
            }
            if (comand == "allUsers") {
                allUsers(users);
                continue;
            }
            if (comand == "getUser") {
                getUser(users);
                continue;
            }
            if (comand == "createGroup") {
                createGroup(groups);
                continue;
            }
            if (comand == "deleteGroup") {
                deleteGroup(groups);
                continue;
            }
            if (comand == "allGroups") {
                allGroups(groups);
                continue;
            }
            if (comand == "getGroup") {
                getGroup(groups);
                continue;
            }
            if (comand == "quit")
                break;
            if (comand == "")
                continue;
            std::cout << "unknown comand: " << comand << std::endl;
        }
    }
};

#endif //TASK1_APP_H
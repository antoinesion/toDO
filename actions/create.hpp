#ifndef CREATE_H
#define CREATE_H
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "../task.hpp"

void create_task (std::vector<Task*>* tasks, std::map<int, Task*>& id_to_ptr,
    int*next_id, int argc, char* argv []);

#endif

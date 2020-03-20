#ifndef CREATE_H
#define CREATE_H
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "task.hpp"

bool create_task (Task* new_task, std::map<int, Task*>& id_to_ptr, int argc, char* argv []);

#endif

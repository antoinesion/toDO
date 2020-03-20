#ifndef MOVE_H
#define MOVE_H
#include <map>
#include <string>
#include <iostream>
#include "task.hpp"

bool move_task (Task* task_to_move, int* where_id, int* positioning, 
    std::map<int, Task*>& id_to_ptr, int argc, char* argv []);

#endif

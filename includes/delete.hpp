#ifndef DELETE_H
#define DELETE_H
#include <map>
#include <vector>
#include <iostream>
#include "task.hpp"

void delete_tasks (std::map<int, Task*>& id_to_ptr, int argc, char* argv []);

#endif

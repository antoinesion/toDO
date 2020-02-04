#ifndef CLOSE_H
#define CLOSE_H
#include <map>
#include <vector>
#include <iostream>
#include "task.hpp"

void close_tasks (std::map<int, Task*>& id_to_ptr, int argc, char* argv []);

#endif

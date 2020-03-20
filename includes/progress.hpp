#ifndef PROGRESS_H
#define PROGRESS_H
#include <map>
#include <iostream>
#include "task.hpp"

void progress_task (std::map<int, Task*>& id_to_ptr, int argc, char* argv []);

#endif

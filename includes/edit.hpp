#ifndef EDIT_H
#define EDIT_H
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "task.hpp"

void edit_task (std::map<int, Task*>& id_to_ptr, int argc, char* argv []);

#endif

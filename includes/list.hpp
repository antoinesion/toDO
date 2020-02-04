#ifndef LIST_H
#define LIST_H
#include "task.hpp"
#include <vector>
#include <iostream>
#include <string>

bool taskcmp_id_incr(Task* t1, Task* t2);

bool taskcmp_priority_decr(Task* t1, Task* t2);

bool taskcmp_priority_incr(Task* t1, Task* t2);

bool taskcmp_date_incr(Task* t1, Task* t2);

bool taskcmp_date_decr(Task* t1, Task* t2);


void list_tasks (std::vector<Task*>& tasks, int argc, char* argv []);

#endif

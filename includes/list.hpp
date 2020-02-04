#ifndef LIST_H
#define LIST_H
#include "task.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <string>

bool taskcmp_id_incr(Task* t1, Task* t2);

bool taskcmp_priority_decr(Task* t1, Task* t2);

bool taskcmp_priority_incr(Task* t1, Task* t2);

bool taskcmp_date_incr(Task* t1, Task* t2);

bool taskcmp_date_decr(Task* t1, Task* t2);

bool priorityfilter (Task* t, char cmp, int priority);

bool statefilter_none(Task* t);

bool statefilter_not_started(Task* t);

bool statefilter_in_progress(Task* t);

bool statefilter_uncompleted(Task* t);

bool statefilter_done(Task* t);

void list_tasks (std::vector<Task*>& tasks, int argc, char* argv []);

#endif

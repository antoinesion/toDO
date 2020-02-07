#ifndef TASK_UTILS_H
#define TASK_UTILS_H
#include "task.hpp"

bool taskcmp_id_incr(Task* t1, Task* t2);

bool taskcmp_priority_decr(Task* t1, Task* t2);

bool taskcmp_priority_incr(Task* t1, Task* t2);

bool taskcmp_date_incr(Task* t1, Task* t2);

bool taskcmp_date_decr(Task* t1, Task* t2);

bool priorityfilter (Task* t, char cmp, int priority);

bool statefilter_none(int state);

bool statefilter_not_started(int state);

bool statefilter_in_progress(int state);

bool statefilter_uncompleted(int state);

bool statefilter_done(int state);

#endif

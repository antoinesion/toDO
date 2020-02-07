#include "../includes/task_utils.hpp"

bool taskcmp_id_incr(Task* t1, Task* t2) {
  return t1->get_id() < t2->get_id();
};

bool taskcmp_priority_decr(Task* t1, Task* t2) {
  return t1->get_priority () > t2->get_priority ();
};

bool taskcmp_priority_incr(Task* t1, Task* t2) {
  return t1->get_priority () < t2->get_priority ();
};

bool taskcmp_date_incr(Task* t1, Task* t2) {
  return t1->get_creation_date () < t2->get_creation_date ();
};

bool taskcmp_date_decr(Task* t1, Task* t2) {
  return t1->get_creation_date () > t2->get_creation_date ();
};

bool priorityfilter (Task* t, char cmp, int priority) {
  if (cmp == '-') {
    return t->get_priority() <= priority;
  } else if (cmp == '=') {
    return t->get_priority() == priority;
  } else {
    return t->get_priority() >= priority;
  }
};

bool statefilter_none(int state) {
  return state >= 0;
};

bool statefilter_not_started(int state) {
  return state == 0;
};

bool statefilter_in_progress(int state) {
  return state == 1;
};

bool statefilter_uncompleted(int state) {
  return state < 2;
};

bool statefilter_done(int state) {
  return state == 2;
};

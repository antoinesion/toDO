#ifndef TASK_H
#define TASK_H
#include <string>

class Task {
  private:
    std::string id;
    std::string title;
    std::string description;
    std::string creation_date;
    std::string closure_date;
    int status = 0;
    int priority = 0;
    std::string* comments;
    Task* subtasks;
  public:
    std::string get_id ();
    /* ... */
    void set_title ();
    /* ... */
    void add_comment (std::string comment);
    void update_status ();
    void add_subtask (Task subtask);
    
    void print ();

    void read (std::string writed_task);
    std::string write ();
}

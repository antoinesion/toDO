#ifndef TASK_H
#define TASK_H
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <ctime>
#include <iostream>
#include <string>

class Task {
  private:
    int id;
    std::string title;
    std::string description;
    int state = 0;
    time_t creation_date;
    time_t closure_date;
    int progression = 0;
    bool prgs_editable = true;
    int priority = 0;
    std::vector<std::string> comments;
    std::vector<int> subtasks_id;
    int subtask_of;
    std::map<int, Task*>* id_to_ptr;
    bool del = false;
  public:
    Task (std::map<int, Task*>* id_to_ptr);
    Task (std::map<int, Task*>* id_to_ptr, int id);

    int get_id ();
    int get_state ();
    int get_progression ();
    int get_priority ();
    time_t get_creation_date ();
    int get_subtask_of ();
    bool get_del ();

    void set_title (std::string& t);
    void set_description (std::string& d);
    void set_priority (int p);
    void set_subtask_of (int sto);
    /* ... */

    void add_comment (std::string& cmt);
    void update_progression ();
    void add_subtask (int subtask_id);
    int close ();

    int quickview (int sub);
    void print ();

    void read (std::string& stask);
    void write (std::ofstream& file);
};

#endif

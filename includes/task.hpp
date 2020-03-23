#ifndef TASK_H
#define TASK_H
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <ctime>
#include <iostream>
#include <string>
#include <tuple>
#include <functional>

#include <sys/ioctl.h>
#include <unistd.h>

#include "../libs/string_tools.hpp"

bool statefilter_default (int state);
void remove_special_chars(std::string* s);

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
    std::vector<std::tuple<std::string, time_t>> comments;
    std::vector<int> subtasks_id;
    int subtask_of = 0;
    double position = 0.;
    std::map<int, Task*>* id_to_ptr;
    bool to_delete = false;
  public:
    Task (std::map<int, Task*>* id_to_ptr);
    Task (std::map<int, Task*>* id_to_ptr, int id);

    int get_id ();
    std::string get_title ();
    std::string get_description ();
    int get_state ();
    int get_progression ();
    int get_priority ();
    time_t get_creation_date ();
    int get_subtask_of ();
    double get_position ();
    bool to_del ();
    int get_depth ();

    void set_title (std::string& t);
    void set_description (std::string& d);
    void set_priority (int p);
    void set_subtask_of (int sto);
    void set_position (double pos);
    void set_progression (int p);
    void add_comment (std::string& cmt);
    std::string del_comment (int cmt_i);
    
    void update_progression ();
    void update_subtasks_position ();

    void add_subtask (int subtask_id);
    void del_subtask (int subtask_id);
    bool has_subtask (int subtask_id);
    bool has_any_subtask ();
    int close ();
    int delete_task ();

    int quickview (int sub, std::function<bool(int)> statefilter = statefilter_default,
	bool last_sub = false, std::string pre_taskview = "");
    void print ();

    void read (std::string& stask);
    void write (std::ofstream& file);
};

bool tasksort_default(Task* t1, Task* t2);

#endif

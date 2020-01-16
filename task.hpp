#ifndef TASK_H
#define TASK_H
#include <string>
#include <vector>
#include <fstream>
#include <map>

class Task {
  private:
    int id;
    std::string title;
    std::string description = "No description";
    int status = 0;
    std::string creation_date = "-";
    std::string closure_date;
    int progression = 0;
    bool prgs_editable = true;
    int priority = 0;
    std::vector<std::string> comments;
    std::vector<int> subtasks_id;
    std::map<int, Task*>* id_to_ptr;
    bool del = false;
  public:
    Task (std::map<int, Task*>* id_to_ptr);
    Task (std::map<int, Task*>* id_to_ptr,
	int id, std::string title, std::string description = "", int priority = 0,
	int subtask_of = -1);
    int get_id ();
    int get_progression ();
    
    void set_title (std::string& ttl);
    /* ... */
    
    void add_comment (std::string& cmt);
    void update_progression ();
    void add_subtask (int subtask_id);
    
    void quickview (int sub);
    void print ();

    void read (std::string& stask);
    void write (std::ofstream& file);
};

#endif

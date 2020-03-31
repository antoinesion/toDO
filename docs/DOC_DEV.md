# toDO Dev Documentation

### General Information

Author: Antoine Sion<br/>
Mail: antoine.sion@mines-paristech.fr<br/>
Progamming language: **C++(11)**

### Structure
* `docs/` : the documentation
* `etc/` : config files
* `includes/` : all the include (`.hpp`) files
* `libs/` : libraries
* `media/` : image for the `README.md` file
* `src/` : all the source (`.cpp`) files
* `bin/` : (*created at* `config` *runtime*) `todo` executable

### Programming

#### Source and include files
Each action is coded by an *include file* and a *source file*. For instance, `src/create.cpp` and `includes/create.hpp` are used by the `create` action.

The main functionnalities called by these actions are however located in `src/task.cpp`. Here's the commented include file `includes/task.hpp` which reference all the functions.

```c++
// ...
bool statefilter_default (int state); // filter for the list action
void remove_special_chars(std::string* s); // to replace special characters ('é' by 'e' for example) to avoid some display issues

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
    std::map<int, Task*>* id_to_ptr; // useful to access to the other task pointers thanls to their id
    bool to_delete = false;
  public:
    // the constructors
    Task (std::map<int, Task*>* id_to_ptr);
    Task (std::map<int, Task*>* id_to_ptr, int id);

    // all the getter functions
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

    // all the setter functions
    void set_title (std::string& t);
    void set_description (std::string& d);
    void set_priority (int p);
    void set_subtask_of (int sto);
    void set_position (double pos);
    void set_progression (int p);
    void add_comment (std::string& cmt);
    std::string del_comment (int cmt_i);

    // update functions
    void update_progression ();
    void update_subtasks_position ();

    // functions dealing with subtasks
    void add_subtask (int subtask_id);
    void del_subtask (int subtask_id);
    bool has_subtask (int subtask_id);
    bool has_any_subtask ();
    
    // functions to close/delete
    int close ();
    int delete_task ();

    // display functions
    int quickview (int sub, std::function<bool(int)> statefilter = statefilter_default,
	bool last_sub = false, std::string pre_taskview = ""); // used by list action
    void print (); // used by show action

    // functions to read/write the task in the backup file etc/tasks.txt
    void read (std::string& stask);
    void write (std::ofstream& file);
};

bool tasksort_default(Task* t1, Task* t2); // sort for the list action
```

Moreover, the file `src/task_utils.cpp` gives some filter and sorting functions, useful for `list` action.

Finally, the `src/main.cpp` file achieve these 3 steps:
* It reads all the backup file to build a vector of the tasks as well as the map `id_to_ptr` seen in `includes/task.hpp`
* It deals with the user action by calling the right function (`create`, `list`, etc.)
* It writes in the backup file to save the potential changes

##### Libraries

The app also need the library `libs/string_tools.a` which offers 2 convenient functions `replace_all` (to replace all occurences of an expression in a string) and `smart_split` (used to display correctly long string in the terminal).

##### Config files
The config file `etc/config.hpp` is where the path to the backup file `etc/tasks.txt` is written (at`config` runtime).

#include "create.hpp"

void create_task (std::vector<Task*>* tasks, std::map<int, Task*>& id_to_ptr,
    int* next_id, int argc, char* argv []) {
  
  std::map<std::string, int> priority_map;
  priority_map["low"] = 1;
  priority_map["medium"] = 2;
  priority_map["high"] = 3;


  bool error = false;
  std::string title;
  std::string description;
  int priority;
  int subtask_of = 0;
  for (int i = 2 ; i < argc ; i += 2) {
    if (i+1 == argc) {
      if (!error) {
	std::cerr << "/!\\ error: no value specified after '" << argv[i] << "'. ";
	error = true;
      }
    }
    else if (std::strcmp(argv[i], "--title") == 0 || std::strcmp(argv[i], "-t") == 0) {
      title = argv[i+1];
      if (title.find("\n") != std::string::npos) {
	std::cerr << "/!\\ error: no multi-lined title allowed. ";
	error = true;
      }
    }
    else if (std::strcmp(argv[i], "--description") == 0 || std::strcmp(argv[i], "-d") == 0) {
      description = argv[i+1];
    }
    else if (std::strcmp(argv[i], "--priority") == 0 || std::strcmp(argv[i], "-p") == 0) {
      if (priority_map.find(argv[i+1]) != priority_map.end()) {
	priority = priority_map[argv[i+1]];
      } else {
	std::cerr << "/!\\ error: priority '" << argv[i+1] << "' unknowned.";
	error = true;
      }
    }
    else if (std::strcmp(argv[i], "--subtask_of") == 0 || std::strcmp(argv[i], "-st") == 0) {
      int subtask_id = std::stoi(argv[i+1]);
      if (id_to_ptr.find(subtask_id) == id_to_ptr.end()) {
	if (!error) {
	  std::cerr << "/!\\ error: no match for id '" << argv[i+1] << "'. ";
	  error = true;
	}
      } else {
	subtask_of = std::stoi(argv[i+1]);
      }
    }
    else if (!error) {
      std::cerr << "/!\\ error: no param named '" << argv[i] << "'. ";
      error = true;
    }
  }
  if (!error) {
    if (subtask_of > 0 && priority != 0) {
      std::cout << "warning: a subtask cannot have a priority." << std::endl << std::endl;
    }
    Task new_task (&id_to_ptr, *next_id, title, description, priority, subtask_of);
    tasks->push_back(&new_task);
    std::cout << "info: new task succesfully created (id:" << next_id << ").";
    ++*next_id;
  }
};

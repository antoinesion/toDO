#include "../includes/create.hpp"

bool create_task (Task* new_task, std::map<int, Task*>& id_to_ptr, int argc, char* argv []) {

  std::map<std::string, int> priority_map;
  priority_map["none"] = 0;
  priority_map["low"] = 1;
  priority_map["medium"] = 2;
  priority_map["high"] = 3;


  bool error = false;
  std::string title = "";
  std::string description = "";
  int priority = 0;
  int subtask_of = 0;
  for (int i = 2 ; i < argc ; i += 2) {
    if (i+1 == argc) {
      if (!error) {
	std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no value specified after '" << argv[i] << "'. ";
	error = true;
      }
    }
    else if (std::strcmp(argv[i], "--title") == 0 || std::strcmp(argv[i], "-t") == 0) {
      title = argv[i+1];
      if (title.find("\n") != std::string::npos) {
	std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no multi-lined title allowed. ";
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
	std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m priority '" << argv[i+1] << "' unknown.";
	error = true;
      }
    }
    else if (std::strcmp(argv[i], "--subtask_of") == 0 || std::strcmp(argv[i], "-sto") == 0) {
      int subtask_id = std::stoi(argv[i+1]);
      if (id_to_ptr.find(subtask_id) == id_to_ptr.end()) {
	if (!error) {
	  std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no match for id '" << argv[i+1] << "'. ";
	  error = true;
	}
      } else {
	subtask_of = std::stoi(argv[i+1]);
      }
    }
    else if (!error) {
      std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no param named '" << argv[i] << "'. ";
      error = true;
    }
  }
  if (title == "") {
      std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m title of a task cannot be empty.";
      error = true;
  }
  if (!error) {
    if (subtask_of > 0 && priority != 0) {
      std::cout << "\033[1;38;5;202mwarning:\033[0;38;5;202m a subtask cannot have a priority.\033[0m"
        << std::endl << std::endl;
    }
    new_task->set_title(title);
    new_task->set_description(description);
    new_task->set_priority(priority);
    new_task->set_subtask_of(subtask_of);
    std::cout << "\033[1minfo:\033[0m new task succesfully created (id:" << new_task->get_id () << ").";
    return true;
  }
  return false;
};

#include "../includes/edit.hpp"

void edit_task (std::map<int, Task*>& id_to_ptr, int argc, char* argv []) {
  
  std::map<std::string, int> priority_map;
  priority_map["low"] = 1;
  priority_map["medium"] = 2;
  priority_map["high"] = 3;


  bool error = false;
  if (argc == 2) {
    std::cerr << "/!\\ error: no id detected afer 'edit'.";
  } else {
    int id = std::stoi(argv[2]);
    if (id_to_ptr.find(id) == id_to_ptr.end()) {
      std::cerr << "/!\\ error: no match for id '" << argv[2] << "'. ";
      error = true;
    } else {
      for (int i = 3 ; i < argc ; i += 2) {
	if (i+1 == argc || error) {
	  if (!error) {
	    std::cerr << "/!\\ error: no value specified after '" << argv[i] << "'. ";
	    error = true;
	  }
	}
	else if (std::strcmp(argv[i], "--title") == 0 || std::strcmp(argv[i], "-t") == 0) {
	  std::string title (argv[i+1]);
	  if (title.find("\n") != std::string::npos) {
	    std::cerr << "/!\\ error: no multi-lined title allowed. ";
	    error = true;
	  } else {
	    id_to_ptr[id]->set_title(title);
	  }
	}
	else if (std::strcmp(argv[i], "--description") == 0 || std::strcmp(argv[i], "-d") == 0) {
	  std::string description (argv[i+1]);
	  id_to_ptr[id]->set_description(description);
	}
	else if (std::strcmp(argv[i], "--priority") == 0 || std::strcmp(argv[i], "-p") == 0) {
	  if (id_to_ptr[id]->get_subtask_of () != 0) {
	    std::cerr << "/!\\ warning: a subtask cannot have a priority." << std::endl;
	  }
	  else if (priority_map.find(argv[i+1]) != priority_map.end()) {
	    int priority = priority_map[argv[i+1]];
	    id_to_ptr[id]->set_priority(priority);
	  } else {
	    std::cerr << "/!\\ error: priority '" << argv[i+1] << "' unknowned.";
	    error = true;
	  }
	}
	else if (std::strcmp(argv[i], "--subtask_of") == 0 || std::strcmp(argv[i], "-sto") == 0) {
	  int subtask_id = std::stoi(argv[i+1]);
	  if (id_to_ptr.find(subtask_id) == id_to_ptr.end() && subtask_id != 0) {
	    if (!error) {
	      std::cerr << "/!\\ error: no match for id '" << argv[i+1] << "'. ";
	      error = true;
	    }
	  } else if (id_to_ptr[id]->has_subtask (subtask_id)) {
	    std::cerr << "/!\\ error: task (id:" << subtask_id << ") is already a subtask of "
	      << "id(" << id << ").";
	  } else {
	    id_to_ptr[id]->set_subtask_of(subtask_id);
	  }
	}
	else if (!error) {
	  std::cerr << "/!\\ error: no param named '" << argv[i] << "'. ";
	  error = true;
	}
      }
      if (!error) {
	std::cout << "info: task (id:" << id << ") successfuly edited.";
      }
    }
  }
};

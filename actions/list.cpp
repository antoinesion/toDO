#include "list.hpp"

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

void list_tasks (std::vector<Task*>& tasks, int argc, char* argv []) {
  
  std::map<std::string, int> priority_map;
  priority_map["low"] = 1;
  priority_map["medium"] = 2;
  priority_map["high"] = 3;

  std::function<bool(Task*, Task*)> sort_function = taskcmp_id_incr;
  std::map<char,bool> incr_map;
  incr_map['+'] = true;
  incr_map['-'] = false;
  bool priority_filter = false;
  int priority = 0;
  bool error = false;
  for (int i = 2 ; i < argc ; i += 2) {
    if (i+1 == argc) {
      if (!error) {
	std::cerr << "/!\\ error: no value specified after '" << argv[i] << "'. ";
	error = true;
      }
    }
    else if (std::strcmp(argv[i], "--sort") == 0 || std::strcmp(argv[i], "-s") == 0) {
      std::string arg (argv[i+1]);
      if (arg.find("priority") == 0) {
	sort_function = taskcmp_priority_decr;
	if (arg.length() == 9) {
	  if (incr_map.find(arg[8]) != incr_map.end()) {
	    if (incr_map[arg[8]]) {
	      sort_function = taskcmp_priority_incr;
	    }
	  } else {
	    std::cerr << "/!\\ error: cannot sort with value '" << arg << "'. ";
	    error = true;
	  }
	}
	else if (arg.length() > 9) {
	    std::cerr << "/!\\ error: cannot sort with value '" << arg << "'. ";
	    error = true;
	}
      }
      else if (arg.find("date") == 0) {
	sort_function = taskcmp_date_incr;
	if (arg.length() == 5) {
	  if (incr_map.find(arg[4]) != incr_map.end()) {
	    if (!incr_map[arg[4]]) {
	      sort_function = taskcmp_date_decr;
	    }
	  } else {
	    std::cerr << "/!\\ error: cannot sort with value '" << arg << "'. ";
	    error = true;
	  }
	}
	else if (arg.length() > 5) {
	    std::cerr << "/!\\ error: cannot sort with value '" << arg << "'. ";
	    error = true;
	}
      }
    }
    else if (std::strcmp(argv[i], "--priority") == 0 || std::strcmp(argv[i], "-p") == 0) {
      if (priority_map.find(argv[i+1]) != priority_map.end()) {
	priority_filter = true;
	priority = priority_map[argv[i+1]];
      }
      else {
	std::cerr << "/!\\ error: priority '" << argv[i+1] << "' unknowned.";
	error = true;
      }
    }
    else if (!error) {
      std::cerr << "/!\\ error: no param named '" << argv[i] << "'. ";
      error = true;
    }
  }
  if (!error) {
    std::sort (tasks.begin(), tasks.end(), sort_function);
    if (tasks.size() > 0) {
      int nb_tasks = 0;
      std::cout << "list:" << std::endl;
      for (Task* tsk : tasks) {
	if (tsk->get_subtask_of () == 0) {
	  if (priority_filter && tsk->get_priority () == priority) {
	    tsk->quickview(0);
	    nb_tasks++;
	  }
	  else if (!priority_filter) {
	    tsk->quickview(0);
	    nb_tasks++;
	  }
	}
      }
      std::cout << nb_tasks << " task(s).";
    } else {
      std::cout << "info: you have not created any task.";
    }
  }
};

#include "../includes/list.hpp"

void list_tasks (std::vector<Task*>& tasks, int argc, char* argv []) {
  
  std::map<std::string, std::function<bool(int)>> statefilter_map;
  std::function<bool(int)> statefilter = static_cast<bool(*)(int)> (statefilter_none);
  statefilter_map["not_started"] = static_cast<bool(*)(int)> (statefilter_not_started);
  statefilter_map["in_progress"] = static_cast<bool(*)(int)> (statefilter_in_progress);
  statefilter_map["uncompleted"] = static_cast<bool(*)(int)> (statefilter_uncompleted);
  statefilter_map["done"] = static_cast<bool(*)(int)> (statefilter_done);

  std::function<bool(Task*, Task*)> sort_function = taskcmp_id_incr;

  std::map<char,bool> incr_map;
  incr_map['+'] = true;
  incr_map['-'] = false;

  int priority = 0;
  char priority_cmp = '+';

  bool error = false;
  for (int i = 2 ; i < argc ; i += 2) {
    if (i+1 == argc) {
      if (!error) {
	std::cerr << "/!\\ error: no value specified after '" << argv[i] << "'. ";
	error = true;
      }
    }
    else if (std::strcmp(argv[i], "--sort") == 0 || std::strcmp(argv[i], "-so") == 0) {
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
      std::string arg (argv[i+1]);
      if (arg.find("+") == 0) {
	priority_cmp = '+';
      } else if (arg.find("-") == 0) {
	priority_cmp = '-';
      } else {
	priority_cmp = '=';
      }
      if (arg.find("low") != std::string::npos) {
	priority = 1;
      } else if (arg.find("medium") != std::string::npos) {
	priority = 2;
      } else if (arg.find("high") != std::string::npos) {
	priority = 3;
      } else {
	std::cerr << "/!\\ error: priority '" << argv[i+1] << "' unknowned.";
	error = true;
      }
    }
    else if (std::strcmp(argv[i], "--state") == 0 || std::strcmp(argv[i], "-st") == 0) {
      if (statefilter_map.find(argv[i+1]) != statefilter_map.end()) {
	statefilter = statefilter_map[argv[i+1]];
      } else {
	std::cerr << "/!\\ error: state '" << argv[i+1] << "' unknowned.";
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
	  if (priorityfilter (tsk, priority_cmp, priority)) {
	    nb_tasks += tsk->quickview(0, statefilter);
	  }
	}
      }
      std::cout << nb_tasks << " task(s).";
    } else {
      std::cout << "info: you have not created any task.";
    }
  }
};

#include "task.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>

std::map<int,Task*> id_to_ptr;

std::vector<Task*> read_tasks (std::ifstream& file) {
  std::vector<Task*> tasks;
  std::string line;
  std::string stask;
  while (std::getline(file, line)) {
    if (line[line.length()-1] != '"') {
	stask = stask + line + "\n";
    }
    else {
      stask = stask + line;
      Task* task = new Task (&id_to_ptr);
      task->read(stask);
      tasks.push_back(task);
      stask = "";
    }
  }
  return tasks;
};

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

int main (int argc, char* argv []) {
  /* --- READING TASKS --- */
  std::ifstream ifile ("tasks_backup.txt");
  std::vector<Task*> tasks;
  
  int next_id = 1;
  if (ifile) {
    /* readind 'next_id' */
    std::string snext_id;
    std::getline(ifile, snext_id);
    next_id = std::stoi(snext_id);
    
    /* reading tasks */
    tasks = read_tasks (ifile);
  }
  ifile.close();

  /* --- DEALING WITH ACTION --- */
  std::map<std::string, int> priority_map;
  priority_map["low"] = 1;
  priority_map["medium"] = 2;
  priority_map["high"] = 3;

  if (argc < 2) {
    std::cerr << "/!\\ error: no action passed in argument.";
    /* show help */
  }
  else {
    /* ACTION */
    if (std::strcmp(argv[1], "create") == 0) {
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
	Task new_task (&id_to_ptr, next_id, title, description, priority, subtask_of);
	tasks.push_back(&new_task);
	next_id++;
	std::cout << "info: new task succesfully created.";
	/* print task */
      }
    }
    /* LIST */
    else if (std::strcmp(argv[1], "list") == 0) {
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
    }
    /* SHOW */
    else if (std::strcmp(argv[1], "show") == 0) {
      if (argc == 2) {
	std::cerr << "/!\\ error: no id detected afer 'show'.";
      }
      else if (argc > 3) {
	std::cerr << "/!\\ error: too many arguements after 'show'.";
      }
      else {
	int id = std::stoi(argv[2]);
	if (id_to_ptr.find(id) == id_to_ptr.end()) {
	  std::cerr << "/!\\ error: no match for id '" << argv[2] << "'. ";
	} else {
	  id_to_ptr[id]->print();
	}
      }
    }
    else if (std::strcmp(argv[1], "close") == 0) {
      /* ... */
    }
    else if (std::strcmp(argv[1], "delete") == 0) {
      /* ... */
    }
    else if (std::strcmp(argv[1], "comment") == 0) { /* pas de retour à la ligne */
      /* ... */
    }
    else if (std::strcmp(argv[1], "edit") == 0) {
      /* ... */ /* si c'est un sub verifier qu'il y a deja pas un sub dans l'autre sens */
    }
    else if (std::strcmp(argv[1], "help") == 0) {

    }
  }
  std::cout << std::endl << std::endl;

  /* --- SAVING TASKS --- */
  std::ofstream ofile ("tasks_backup.txt");
  ofile << next_id << std::endl;
  for (Task* tsk : tasks) {
    tsk->write (ofile);
  }
  ofile.close();
  return 0;
}

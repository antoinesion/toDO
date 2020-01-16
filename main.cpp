#include "task.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

bool taskcmp(Task* t1, Task* t2) {
  return t1->get_priority () > t2->get_priority ();
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
    if (std::strcmp(argv[1], "create") == 0) {
      bool error = false;
      std::string title;
      std::string description;
      int priority;
      int subtask_of = 0;
      for (int i = 2 ; i < argc ; i += 2) {
	if (i+1 == argc) {
	  if (!error) {
	    std::cerr << "/!\\ error: no value specified after '" << argv[i] << "'.";
	    error = true;
	  }
	}
	else if (std::strcmp(argv[i], "--title") == 0 || std::strcmp(argv[i], "-t") == 0) {
	  title = argv[i+1]; /* TODO: title peut pas avoir \n */
	}
	else if (std::strcmp(argv[i], "--description") == 0 || std::strcmp(argv[i], "-d") == 0) {
	  description = argv[i+1];
	}
	else if (std::strcmp(argv[i], "--priority") == 0 || std::strcmp(argv[i], "-p") == 0) {
	  priority = priority_map[argv[i+1]];
	}
	else if (std::strcmp(argv[i], "--subtask_of") == 0 || std::strcmp(argv[i], "-st") == 0) {
	  int subtask_id = std::stoi(argv[i+1]);
	  if (id_to_ptr.find(subtask_id) == id_to_ptr.end()) {
	    if (!error) {
	      std::cerr << "/!\\ error: no match for id '" << argv[i+1] << "'.";
	      error = true;
	    }
	  } else {
	    subtask_of = std::stoi(argv[i+1]);
	  }
	}
	else if (!error) {
	  std::cerr << "/!\\ error: no match for param '" << argv[i] << "'.";
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
    else if (std::strcmp(argv[1], "list") == 0) {
      if (tasks.size() > 0) {
	std::cout << "list:" << std::endl;
	for (Task* tsk : tasks) {
	  tsk->quickview(0);
	}
	std::cout << tasks.size() << " task(s).";
      } else {
	std::cout << "info: you have not created any task.";
      }
    }
    else if (std::strcmp(argv[1], "show") == 0) {
      /* ... */
    }
    else if (std::strcmp(argv[1], "close") == 0) {
      /* ... */
    }
    else if (std::strcmp(argv[1], "delete") == 0) {
      /* ... */
    }
    else if (std::strcmp(argv[1], "comment") == 0) {
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
  std::sort (tasks.begin(), tasks.end(), taskcmp);
  std::ofstream ofile ("tasks_backup.txt");
  ofile << next_id << std::endl;
  for (Task* tsk : tasks) {
    tsk->write (ofile);
  }
  ofile.close();
  return 0;
}

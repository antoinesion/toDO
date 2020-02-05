#include "../includes/task.hpp"
#include "../includes/create.hpp"
#include "../includes/list.hpp"
#include "../includes/show.hpp"
#include "../includes/close.hpp"
#include "../includes/delete.hpp"
#include "../includes/comment.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>

std::map<int, Task*> id_to_ptr;

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

const std::string file_path = "../etc/tasks.txt";

int main (int argc, char* argv []) {
  /* --- READING TASKS --- */
  std::ifstream ifile (file_path);
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
  if (argc < 2) {
    std::cerr << "/!\\ error: no action passed in argument.";
    /* show help */
  }
  else {
    if (std::strcmp(argv[1], "create") == 0) {
      Task new_task = Task(&id_to_ptr, next_id);
      if (create_task (&new_task, id_to_ptr, argc, argv)) {
	tasks.push_back(&new_task);
	next_id++;
      }
    }
    else if (std::strcmp(argv[1], "list") == 0) {
      list_tasks (tasks, argc, argv);
    }
    else if (std::strcmp(argv[1], "show") == 0) {
      show_task (id_to_ptr, argc, argv); 
    }
    else if (std::strcmp(argv[1], "close") == 0) {
      close_tasks (id_to_ptr, argc, argv);
    }
    else if (std::strcmp(argv[1], "delete") == 0) {
      delete_tasks (id_to_ptr, argc, argv);
    }
    else if (std::strcmp(argv[1], "comment") == 0) {
      comment_task (id_to_ptr, argc, argv);
    }
    else if (std::strcmp(argv[1], "edit") == 0) {
      /* ... */ /* si c'est un sub verifier qu'il y a deja pas un sub dans l'autre sens */
    }
    else if (std::strcmp(argv[1], "help") == 0) {

    }
  }
  std::cout << std::endl << std::endl;

  /* --- SAVING TASKS --- */
  std::ofstream ofile (file_path);
  ofile << next_id << std::endl;
  for (Task* tsk : tasks) {
    tsk->write (ofile);
  }
  ofile.close();
  return 0;
}

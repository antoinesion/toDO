#include "../includes/task.hpp"
#include "../includes/create.hpp"
#include "../includes/list.hpp"
#include "../includes/show.hpp"
#include "../includes/close.hpp"
#include "../includes/delete.hpp"
#include "../includes/comment.hpp"
#include "../includes/edit.hpp"
#include "../includes/progress.hpp"
#include "../includes/move.hpp"
#include "../includes/help.hpp"
#include "../etc/config.hpp"
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

int main (int argc, char* argv []) {
  /* --- READING TASKS --- */
  std::ifstream ifile (path_to_backup_file);
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
  Task task_to_move = Task(&id_to_ptr);
  int where_id;
  int positioning;
  bool task_moved = false;

  if (argc < 2) {
    std::cerr << "\033[1;38;5;9m/!\\ error:\033[1;38;5;9m no action passed in argument." << std::endl
      << "\033[1minfo:\033[0m use 'help' to ask for assistance.";
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
      edit_task (id_to_ptr, argc, argv);
    }
    else if (std::strcmp(argv[1], "progress") == 0) {
      progress_task (id_to_ptr, argc, argv);
    }
    else if (std::strcmp(argv[1], "move") == 0) {
      task_moved = move_task (&task_to_move, &where_id, &positioning, id_to_ptr, argc, argv);
    }
    else if (std::strcmp(argv[1], "help") == 0) {
      help ();
    } else {
    std::cerr << "\033[1;38;5;9m/!\\ error:\033[1;38;5;9m action '" << argv[1] << "' unkown.\033[0m" << std::endl;
    std::cout << "\033[1minfo:\033[0m use action 'help' to ask for assistance.";
    }
  }
  std::cout << "\033[0m" << std::endl << std::endl;

  /* --- SAVING TASKS --- */
  std::ofstream ofile (path_to_backup_file);
  ofile << next_id << std::endl;
  if (!task_moved) {
    for (Task* tsk : tasks) {
      tsk->write (ofile);
    }
  } else {
    for (Task* tsk : tasks) {
      if (tsk->get_id () == where_id && positioning == -1) {
	task_to_move.write (ofile);
      }
      if (tsk->get_id () != task_to_move.get_id ()) {
	tsk->write (ofile);
      }
      if (tsk->get_id () == where_id && positioning == 1) {
	task_to_move.write (ofile);
      }
    }
  }
  ofile.close();
  return 0;
}

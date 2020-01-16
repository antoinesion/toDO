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
    std::cout << "line:" << line << std::endl;
    if (line[line.length()-1] != '"') {
	stask = stask + line + "\n";
    }
    else {
      stask = stask + line;
      std::cout << std::endl << "task read: " << stask << std::endl;
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
  std::ifstream ifile ("tasks_backup.txt");
  std::vector<Task*> tasks;
  
  int next_id = 0;
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
    std::cerr << "/!\\ error: no action passed in argument." << std::endl;
    /* show help */
  }
  else {
    if (std::strcmp(argv[1], "create") == 0) {
      /* ... */ /* test : title sans \n et subtask id correct */
    }
    else if (std::strcmp(argv[1], "list") == 0) {
      /* ... */
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
      /* ... */
    }
  }

  /* --- SAVING TASKS --- */
  std::ofstream ofile ("tasks_backup.txt");
  ofile << next_id << std::endl;
  for (Task* tsk : tasks) {
    tsk->write (ofile);
  }
  ofile.close();
  return 0;
}

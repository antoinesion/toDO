#include "../includes/close.hpp"

bool statefilter (Task* t) {
  return t->get_state () < 2;
};

void close_tasks (std::map<int, Task*>& id_to_ptr, int argc, char* argv []) {

  if (argc == 2) {
    std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no id detected afer 'close'.";
  }
  else {
    bool force = false;
    int start = 2;
    if (std::strcmp(argv[2], "--force") == 0 || std::strcmp(argv[2], "-f") == 0) {
      force = true;
      start++;
    }
    std::vector<int> id_to_close;
    for (int i = start ; i < argc ; i++) {
      int id = std::stoi(argv[i]);
      if (id_to_ptr.find(id) == id_to_ptr.end()) {
	std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no match for id '" << argv[i] << "'. " << std::endl;
      } else {
	id_to_close.push_back(id);
      }
    }

    std::vector<Task*> tsk_to_close;
    for (int id : id_to_close) {
      int id_sub = id;
      bool pop = false;
      while (id_to_ptr[id_sub]->get_depth () > 0 && !pop) {
	id_sub = id_to_ptr[id_sub]->get_subtask_of ();
	if (std::find(id_to_close.begin(), id_to_close.end(), id_sub) != id_to_close.end()) {
	  pop = true;
	}
      }
      if (!pop) {
	tsk_to_close.push_back(id_to_ptr[id]);
      }
    }

    if (!force) {
      std::cout << "It will close the following tasks:" << std::endl;
      for (Task* tsk : tsk_to_close) {
	if (statefilter (tsk)) {
	  tsk->quickview(0);
	}
      }
      std::cout << "Do you want to proceed ? [y/n] ";
      char choice;
      std::cin >> choice;
      std::cout << std::endl;
      if (choice == 'y') {
	force = true;
      } else {
	std::cout << "\033[1;38;5;202mwarning:\033[0;38;5;202m action abort.\033[0m";
      }
    }

    if (force) {
      int nb_tasks_closed = 0;
      for (Task* tsk : tsk_to_close) {
	nb_tasks_closed += tsk->close();
      }
      std::cout << "\033[1minfo:\033[0m " << nb_tasks_closed << " task(s) closed.";
    }
  }
};

#include "../includes/delete.hpp"

void delete_tasks (std::map<int, Task*>& id_to_ptr, int argc, char* argv []) {

  if (argc == 2) {
    std::cerr << "/!\\ error: no id detected afer 'delete'.";
  }
  else {
    bool force = false;
    int start = 2;
    if (std::strcmp(argv[2], "--force") == 0 || std::strcmp(argv[2], "-f") == 0) {
      force = true;
      start++;
    }
    std::vector<int> id_to_delete;
    for (int i = start ; i < argc ; i++) {
      int id = std::stoi(argv[i]);
      if (id_to_ptr.find(id) == id_to_ptr.end()) {
	std::cerr << "/!\\ error: no match for id '" << argv[i] << "'. " << std::endl;
      } else {
	id_to_delete.push_back(id);
      }
    }

    std::vector<Task*> tsk_to_delete;
    for (int id : id_to_delete) {
      int id_sub = id;
      bool pop = false;
      while (id_to_ptr[id_sub]->get_depth () > 0 && !pop) {
	id_sub = id_to_ptr[id_sub]->get_subtask_of ();
	if (std::find(id_to_delete.begin(), id_to_delete.end(), id_sub) != id_to_delete.end()) {
	  pop = true;
	}
      }
      if (!pop) {
	tsk_to_delete.push_back(id_to_ptr[id]);
      }
    }

    if (!force) {
      std::cout << "It will delete the following tasks:" << std::endl;
      for (Task* tsk : tsk_to_delete) {
	if (!tsk->to_del ()) { // TODO: inutile
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
	std::cout << "Action abort.";
      }
    }

    if (force) {
      int nb_tasks_deleted = 0;
      for (Task* tsk : tsk_to_delete) {
	nb_tasks_deleted += tsk->delete_task ();
      }
      std::cout << nb_tasks_deleted << " task(s) deleted.";
    }
  }
};

#include "../includes/move.hpp"

bool move_task (Task* task_to_move, int* where_id, int* positioning, 
    std::map<int, Task*>& id_to_ptr, int argc, char* argv []) {
  
  if (argc == 2) {
    std::cerr << "\033[1;31m/!\\ error:\033[0;31m no id detected afer 'move'.";
  } else {
    int id = std::stoi(argv[2]);
    if (id_to_ptr.find(id) == id_to_ptr.end()) {
      std::cerr << "\033[1;31m/!\\ error:\033[0;31m no match for id '" << argv[2] << "'. ";
    } else if (argc == 3) {
      std::cerr << "\033[1;31m/!\\ error:\033[0;31m no positioning argument detected after task id.";
    } else {
      bool error = false;
      *task_to_move = *(id_to_ptr[id]);
      if (std::strcmp(argv[3], "above") == 0) {
	*positioning = -1;
      } else if (std::strcmp(argv[3], "under") == 0) {
	*positioning = 1;
      } else {
	std::cerr << "\033[1;31m/!\\ error:\033[0;31m positioning argument must be 'above' or 'under'.";
	error = true;
      }
      if (!error) {
	if (argc == 4) {
	  std::cerr << "\033[1;31m/!\\ error:\033[0;31m no id detected afer '" << argv[3] << "'.";
	} else {
	  *where_id = std::stoi(argv[4]);
	  if (id_to_ptr.find(*where_id) == id_to_ptr.end()) {
	    std::cerr << "\033[1;31m/!\\ error:\033[0;31m no match for id '" << argv[4] << "'. ";
	  } else if (*where_id == id) {
	    std::cerr << "\033[1;31m/!\\ error:\033[0;31m task to move and positioning argument cannot be the same.";
	  } else {
	    if (id_to_ptr[*where_id]->get_depth () == task_to_move->get_depth ()) {
	      if (id_to_ptr[*where_id]->get_subtask_of () == task_to_move->get_subtask_of ()) {
		std::cout << "\033[1minfo:\033[0m task (id:" << id << ") successfully moved.";
		if (task_to_move->get_depth () == 0) {
		  return true;
		} else {
		  double pos = id_to_ptr[*where_id]->get_position () + *positioning * 0.5;
		  id_to_ptr[id]->set_position (pos);
		  id_to_ptr[task_to_move->get_subtask_of ()]->update_subtasks_position ();
		  return false;
		}
	      }
	      else {
		std::cerr << "\033[1;31m/!\\ error:\033[0;31m task (id:" << id << ") and task (id:" << argv[4]
		  << ") aren't under the same task.";
	      }
	    } else {
	      std::cerr << "\033[1;31m/!\\ error:\033[0;31m task (id:" << id
		<< ") hasn't the same depth than task (id:" << argv[4] << ").";
	    }
	  }
	}
      }
    }
  }
  return false;
};

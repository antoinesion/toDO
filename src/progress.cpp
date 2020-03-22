#include "../includes/progress.hpp"

void progress_task (std::map<int, Task*>& id_to_ptr, int argc, char* argv []) {
  
  if (argc == 2) {
    std::cerr << "/!\\ error: no id detected afer 'progress'.";
  }
  else if (argc > 4) {
    std::cerr << "/!\\ error: too many arguements after 'progress'.";
  }
  else {
    int id = std::stoi(argv[2]);
    if (id_to_ptr.find(id) == id_to_ptr.end()) {
      std::cerr << "/!\\ error: no match for id '" << argv[2] << "'. ";
    } else {
      if (id_to_ptr[id]->has_any_subtask ()) {
	std::cerr << "/!\\ error: task (id:" << id << ") has subtask(s), therefore its progression is automatically set";
      }
      else if (argc == 3) {
	std::cerr << "/!\\ error: no progression detected.";
      }
      else {
	std::string sprogress (argv[3]);
	int progress = std::stoi(argv[3]);
	if (sprogress.find("+") == std::string::npos && sprogress.find("-") == std::string::npos) {
	  if (0 <= progress && progress <= 100) {
	    id_to_ptr[id]->set_progression(progress);
	    std::cout << "info: progression of task (id:" << id << ") succesfully upddated.";
	  }
	  else {
	    std::cerr << "/!\\ error: progression number is a % (must be between 0 and 100).";
	  }
	}
	else {
	  int progression = id_to_ptr[id]->get_progression () + progress;
	  if (0 <= progression && progression <= 100) {
	    id_to_ptr[id]->set_progression(progression);
	  }
	  else if (progression < 0) {
	    id_to_ptr[id]->set_progression(0);
	  }
	  else {
	    id_to_ptr[id]->set_progression(100);
	  }
	  std::cout << "info: progression of task (id:" << id << ") succesfully upddated.";
	}
      }
    }
  }
};

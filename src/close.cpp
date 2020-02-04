#include "../includes/close.hpp"

void close_tasks (std::map<int, Task*>& id_to_ptr, int argc, char* argv []) {
  
  if (argc == 2) {
    std::cerr << "/!\\ error: no id detected afer 'show'.";
  }
  else {
    bool force = false;
    std::vector<int> id_to_close;
    int start = 2;
    if (std::strcmp(argv[2], "--force") == 0 || std::strcmp(argv[2], "-f") == 0) {
      force = true;
      start++;
    }
    int nb_tasks_closed = 0;
    for (int i = start ; i < argc ; i++) {
      int id = std::stoi(argv[i]);
      if (id_to_ptr.find(id) == id_to_ptr.end()) {
	std::cerr << "/!\\ error: no match for id '" << argv[i] << "'. ";
      } else {
	nb_tasks_closed += id_to_ptr[id]->close(force);
      }
    }
    std::cout << nb_tasks_closed << " task(s) closed.";
  }
};

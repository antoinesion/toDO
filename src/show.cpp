#include "../includes/show.hpp"

void show_task (std::map<int, Task*>& id_to_ptr, int argc, char* argv []) {

  if (argc == 2) {
    std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no id detected afer 'show'.";
  }
  else if (argc > 3) {
    std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m too many arguements after 'show'.";
  }
  else {
    int id = std::stoi(argv[2]);
    if (id_to_ptr.find(id) == id_to_ptr.end()) {
      std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no match for id '" << argv[2] << "'. ";
    } else {
      id_to_ptr[id]->print();
    }
  }
};

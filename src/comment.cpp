#include "../includes/comment.hpp"

void comment_task (std::map<int, Task*>& id_to_ptr, int argc, char* argv []) {

  if (argc == 2) {
    std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no id detected afer 'comment'.";
  }
  else if (argc > 5) {
    std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m too many arguements after 'comment'.";
  }
  else {
    int id = std::stoi(argv[2]);
    if (id_to_ptr.find(id) == id_to_ptr.end()) {
      std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no match for id '" << argv[2] << "'. ";
    } else if (argc == 3) {
      std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m need an action (new/del) after id.";
    } else {
      if (std::strcmp(argv[3], "new") == 0) {
	if (argc == 4) {
	  std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no comment detected.";
	} else {
	  std::string cmt (argv[4]);
	  if (cmt.find("\n") != std::string::npos) {
	    std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no multi-lined comment allowed.";
	  }
	  else {
	    id_to_ptr[id]->add_comment (cmt);
	    std::cout << "\033[1minfo:\033[0m new comment successfully added to task (id:" << id << ").";
	  }
	}
      }
      else if (std::strcmp(argv[3], "del") == 0) {
	if (argc == 4) {
	  std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m no comment position detected.";
	} else {
	  int cmt_i = std::stoi(argv[4]);
	  std::string cmt = id_to_ptr[id]->del_comment (cmt_i);
	  std::cout << "\033[1minfo:\033[0m comment '" << cmt << "' successfuly deleted.";
	}
      }
      else {
	std::cerr << "\033[1;38;5;9m/!\\ error:\033[0;38;5;9m action must be 'new' or 'del'.";
      }
    }
  }
};

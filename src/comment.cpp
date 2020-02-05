#include "../includes/comment.hpp"

void comment_task (std::map<int, Task*>& id_to_ptr, int argc, char* argv []) {
  
  if (argc == 2) {
    std::cerr << "/!\\ error: no id detected afer 'comment'.";
  }
  else if (argc > 4) {
    std::cerr << "/!\\ error: too many arguements after 'comment'.";
  }
  else {
    int id = std::stoi(argv[2]);
    if (id_to_ptr.find(id) == id_to_ptr.end()) {
      std::cerr << "/!\\ error: no match for id '" << argv[2] << "'. ";
    } else if (argc == 3) {
      std::cerr << "/!\\ error: need a comment after id.";
    } else {
      std::string cmt (argv[3]);
      if (cmt.find("\n") != std::string::npos) {
	std::cerr << "/!\\ error: no multi-lined comment allowed.";
      }
      else {
	id_to_ptr[id]->add_comment (cmt);
	std::cout << "info: new comment successfuly added to '"
	  << id_to_ptr[id]->get_title () << "'.";
      }
    }
  }
};

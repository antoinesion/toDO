#include "../includes/help.hpp"

void help () {
  std::cout << "\033[1mhelp:\033[0m" << std::endl;
  std::cout << "  * create [-t|-d|-p|-sto] (values) \033[38;5;240m-> create a new task\033[0m" << std::endl;
  std::cout << "  * list [-p|-so] (values) \033[38;5;240m-> list all the tasks created\033[0m" << std::endl;
  std::cout << "  * move (id) (above/under) (id) \033[38;5;240m-> move a task in the list\033[0m"
    << std::endl;
  std::cout << "  * show (id) \033[38;5;240m-> show a particular task\033[0m" << std::endl;
  std::cout << "  * edit (id) [-t|-d|-p|-sto] (values) \033[38;5;240m-> edit a task\033[0m" << std::endl;
  std::cout << "  * comment (id) (new/del) (comment) \033[38;5;240m-> add a comment to a task\033[0m"
    << std::endl;
  std::cout << "  * progress (id) [+|-](%) \033[38;5;240m-> change the progression of a task\033[0m"
    << std::endl;
  std::cout << "  * close (ids) \033[38;5;240m-> close some tasks\033[0m" << std::endl;
  std::cout << "  * delete (ids) \033[38;5;240m-> delete some tasks\033[0m" << std::endl;
  std::cout << "  * help \033[38;5;240m-> show this\033[0m" << std::endl << std::endl;
  std::cout << "See the docs on \033[4mhttps://github.com/antoinesion/toDO\033[0m"
    << " for more information.";
}

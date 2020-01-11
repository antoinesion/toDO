#include <iostream>

int main (int argc, char* argv []) {
  /* --- READING TASKS --- */


  /* --- DEALING WITH ACTION --- */
  if (argc < 2) {
    std::cout << "/!\\ error: no action passed in argument." << std::endl;
    /* show help */
  }
  else {
    if (argv[1] == "create") {
	/* call create function */
    }
    if (argv[1] == "list") {
	/* call list function */
    }
    if (argv[1] == "close") {
	/* call close function */
    }
    if (argv[1] == "delete") {
	/* call delete function */
    }
    if (argv[1] == "comment") {
	/* call comment function */
    }
    if (argv[1] == "edit") {
	/* call edit function */
    }
  }

  /* --- SAVING TASKS --- */
  return 0;
}

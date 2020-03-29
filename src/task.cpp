#include "../includes/task.hpp"

bool statefilter_default (int state) {
  return state >= 0;
}

void remove_special_chars(std::string* s) {
  replace_all(s, "é", "e");
  replace_all(s, "è", "e");
  replace_all(s, "ê", "e");
  replace_all(s, "ô", "o");
  replace_all(s, "à", "a");
  replace_all(s, "î", "i");
  replace_all(s, "ù", "u");
  replace_all(s, "û", "u");
  replace_all(s, "ç", "c");
  replace_all(s, "É", "E");
  replace_all(s, "È", "E");
  replace_all(s, "Ê", "E");
  replace_all(s, "Ô", "O");
  replace_all(s, "À", "A");
  replace_all(s, "Î", "I");
  replace_all(s, "Ù", "U");
  replace_all(s, "Û", "U");
  replace_all(s, "Ç", "C");
}

bool tasksort_default(Task* t1, Task* t2) {
  return t1->get_position () < t2->get_position ();
}

Task::Task (std::map<int, Task*>* id_to_ptr) : id_to_ptr(id_to_ptr) {}

Task::Task (std::map<int, Task*>* id_to_ptr, int id) : id_to_ptr(id_to_ptr), id(id) {
    (*id_to_ptr)[id] = this;
    time_t now = time(0);
    creation_date = now;
}

int Task::get_id () {return id;}
std::string Task::get_title () {return title;}
std::string Task::get_description () {return description;}
int Task::get_state () {return state;}
int Task::get_progression () {return progression;}
int Task::get_priority () {return priority;}
time_t Task::get_creation_date () {return creation_date;}
int Task::get_subtask_of () {return subtask_of;}
double Task::get_position () {return position;}
bool Task::to_del () {return to_delete;}

int Task::get_depth () {
  if (subtask_of == 0) {
    return 0;
  } else {
    return 1 + (*id_to_ptr)[subtask_of]->get_depth ();
  }
}

void Task::set_title(std::string& t) {
  remove_special_chars(&t);
  title = t;
}
void Task::set_description(std::string& d) {
  remove_special_chars(&d);
  description = d;
}
void Task::set_priority (int p) {priority = p;}
void Task::set_subtask_of (int sto) {
  if (subtask_of > 0) {
    (*id_to_ptr)[subtask_of]->del_subtask(id);
  }
  subtask_of = sto;
  position = 0.;
  if (subtask_of > 0) {
    (*id_to_ptr)[subtask_of]->add_subtask(id);
    priority = 0;
  }
}
void Task::set_position (double pos) {position = pos;}
void Task::set_progression (int p) {
  progression = p;
  if (progression == 100 && state < 2) {
    this->close ();
  }
  else if (progression == 0) {
    state = 0;
  } else if (progression < 100) {
    state = 1;
  }
  if (subtask_of > 0) {
    (*id_to_ptr)[subtask_of]->update_progression();
  }
}

void Task::add_comment (std::string& cmt) {
  time_t now = time(0);
  std::tuple<std::string, time_t> comment (cmt, now);
  comments.push_back(comment);
}

std::string Task::del_comment (int cmt_i) {
  std::string cmt = std::get<0> (comments[cmt_i-1]);
  comments.erase(comments.begin() + cmt_i - 1);
  return cmt;
}

void Task::update_progression () {
  if (subtasks_id.size() > 0) {
    int sum = 0;
    for (int subtask_id : subtasks_id) {
      sum += (*id_to_ptr)[subtask_id]->get_progression();
    }
    progression = sum / subtasks_id.size();
    if (progression == 100 && state < 2) {
      this->close ();
    }
    else if (progression == 0) {
      state = 0;
    } else {
      state = 1;
    }
    if (subtask_of > 0) {
      (*id_to_ptr)[subtask_of]->update_progression();
    }
  }
}

void Task::update_subtasks_position () {
  if (subtasks_id.size() > 0) {
    std::vector<Task*> subtasks;
    for (int subtask_id : subtasks_id) {
      subtasks.push_back((*id_to_ptr)[subtask_id]);
    }
    std::sort (subtasks.begin(), subtasks.end(), tasksort_default);
    double pos = 1.;
    for (Task* tsk : subtasks) {
      tsk->set_position (pos);
      pos++;
    }
  }
}

void Task::add_subtask (int subtask_id) {
  subtasks_id.push_back(subtask_id);
  (*id_to_ptr)[subtask_id]->set_position ((double) subtasks_id.size());
  this->update_progression ();
}

void Task::del_subtask (int subtask_id) {
  std::vector<int>::iterator it = std::find(subtasks_id.begin(), subtasks_id.end(), subtask_id);
  int i = std::distance (subtasks_id.begin(),it);
  subtasks_id.erase(subtasks_id.begin() + i);
  this->update_progression ();
  this->update_subtasks_position ();
}

bool Task::has_subtask (int subtask_id) {
  return (std::find (subtasks_id.begin(), subtasks_id.end(), subtask_id) != subtasks_id.end());
}

bool Task::has_any_subtask () {
  return (subtasks_id.size() > 0);
}

int Task::close () {
  if (state != 2) {
    state = 2;
    progression = 100;
    time_t now = time(0);
    closure_date = now;
    int nb_tasks_closed = 1;
    for (int subtask_id : subtasks_id) {
      nb_tasks_closed += (*id_to_ptr)[subtask_id]->close();
    }
    return nb_tasks_closed;
  }
  return 0;
}

int Task::delete_task () {
  if (!to_delete) {
    to_delete = true;
    if (subtask_of > 0) {
      (*id_to_ptr)[subtask_of]->del_subtask(id);
    }
    int nb_tasks_deleted = 1;
    for (int subtask_id : subtasks_id) {
      nb_tasks_deleted += (*id_to_ptr)[subtask_id]->delete_task ();
    }
    return nb_tasks_deleted;
  }
  return 0;
}

int Task::quickview (int sub, std::function<bool(int)> statefilter, bool last_sub,
    std::string pre_taskview) {
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

  if (statefilter(state) || sub != 0) {
    std::string pt;
    std::string pt_;
    if (sub == 0) {
      pt = "- ";
      pt_ = "  ";
    } else {
      pt_ = "│ ";
      if (!last_sub) {
	pt = "├ ";
      } else {
	pt = "└ ";
      }
    }
    std::string sprogr1;
    std::string sprogr2;
    std::string color;
    if (state == 0) {
      color = "38;5;55";
      sprogr1 = "Not Started ";
    } else if (state == 1) {
      color = "38;5;220";
      sprogr1 = "In Progress ";
      sprogr2 = "(" + std::to_string(progression) + "%) ";
    } else {
      color = "38;5;34";
      std::string scl_dt (ctime(&closure_date));
      sprogr1 = "Done ";
      sprogr2 = "(" + scl_dt.substr(0,scl_dt.length()-1) + ") ";
    }
    std::string spriority (priority, '!');
    if (priority == 0) {
      spriority = "";
    }
    else {
      spriority = "[" + spriority + "]";
    }
    std::string taskview = "(id:" + std::to_string(id) + ") " + title + ": ";
    int max_len = size.ws_col - pre_taskview.length() - 4;
    std::vector<std::string> split_taskview = smart_split(taskview, max_len);
    if (split_taskview.back().length() + sprogr1.length() <= max_len) {
      if (split_taskview.back().length() + sprogr1.length() + sprogr2.length() <= max_len) {
	if (split_taskview.back().length() + sprogr1.length() + sprogr2.length()
	    + spriority.length() <= max_len) {
	  split_taskview.back() += "\033[1;" + color + "m" + sprogr1
	    + "\033[0;" + color + "m" + sprogr2
	    + "\033[1;38;5;9m" + spriority + "\033[0m";
	} else {
	  split_taskview.back() += "\033[1;" + color + "m" + sprogr1
	    + "\033[0;" + color + "m" + sprogr2 + "\033[0m";
	  split_taskview.push_back("\033[1;38;5;9m" + spriority + "\033[0m");
	}
      } else {
	split_taskview.back() += "\033[1;" + color + "m" + sprogr1 + "\033[0m";
	split_taskview.push_back("\033[0;" + color + "m" + sprogr2
	  + "\033[1;38;5;9m" + spriority + "\033[0m");
      }
    } else {
      split_taskview.push_back("\033[1;" + color + "m" + sprogr1
	+ "\033[0;" + color + "m" + sprogr2
	+ "\033[1;38;5;9m" + spriority + "\033[0m");
    }
    for (int i = 0; i < split_taskview.size() ; i++) {
      std::cout << "  " + pre_taskview;
      if (i == 0) {
	std::cout << pt;
      } else {
	std::cout << pt_;
      }
      std::cout << split_taskview[i] << std::endl;
    }
    int nb_tasks = 1;
    int n = subtasks_id.size();
    std::vector<Task*> subtasks;
    for (int subtask_id : subtasks_id) {
      subtasks.push_back((*id_to_ptr)[subtask_id]);
    }
    std::sort (subtasks.begin(), subtasks.end(), tasksort_default);
    for (int i = 0 ; i < n; i++) {
      if (!last_sub && sub > 0) {
	nb_tasks += subtasks[i]->quickview(sub+1, statefilter, (i == n - 1), pre_taskview + "│ ");
      } else {
	nb_tasks += subtasks[i]->quickview(sub+1, statefilter, (i == n - 1), pre_taskview + "  ");
      }
    }
    return nb_tasks;
  }
  else {
    int nb_tasks = 0;
    for (int subtask_id : subtasks_id) {
      nb_tasks += (*id_to_ptr)[subtask_id]->quickview(sub, statefilter);
    }
    return nb_tasks;
  }
}

void Task::print () {
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

  std::string scr_dt = ctime(&creation_date);
    std::string sprogr1;
    std::string sprogr2;
    std::string color;
    if (state == 0) {
      color = "38;5;55";
      sprogr1 = "Not Started ";
    } else if (state == 1) {
      color = "38;5;220";
      sprogr1 = "In Progress ";
      sprogr2 = "(" + std::to_string(progression) + "%) ";
    } else {
      color = "38;5;34";
      std::string scl_dt (ctime(&closure_date));
      sprogr1 = "Done ";
      sprogr2 = "(" + scl_dt.substr(0,scl_dt.length()-1) + ") ";
    }
    std::string spriority (priority, '!');
    if (priority == 0) {
      spriority = "";
    }
    else {
      spriority = "[" + spriority + "]";
    }
  std::string space (size.ws_col - scr_dt.length() + 1
      - sprogr1.length() - sprogr2.length() - spriority.length(), ' ');
  std::cout << std::endl << scr_dt.substr(0,scr_dt.length()-1) << space
    << "\033[1;" << color << "m" << sprogr1
    << "\033[0;" << color << "m" << sprogr2
    << "\033[1;38;5;9m" << spriority << "\033[0m" << std::endl;
  int nsep = (size.ws_col - 2 - title.length()) / 2;
  std::string sepL (nsep, '-');
  std::string sepR (nsep, '-');
  if ((size.ws_col - title.length())%2 == 1) {
    sepR = sepR + "-";
  }
  std::cout << "\033[1m" << sepL << ' ' << title << ' ' <<  sepR << "\033[0m" <<std::endl;
  if (description != "") {
    std::vector<std::string> split_description = smart_split(description, size.ws_col);
    for (int i = 0; i < split_description.size(); i++) {
      std::cout << split_description[i] << std::endl;
    }
    std::cout << std::endl;
  }
  else {
    std::cout << "no description." << std::endl << std::endl;
  }
  if (comments.size() != 0) {
    std::cout << "comment(s):" << std::endl;
    for (std::tuple<std::string, time_t> comment : comments) {
      std::string cmt = std::get<0> (comment);
      time_t date = std::get<1> (comment);
      std::string sdate = ctime(&date);
      std::string cmtview = cmt + " (" + sdate.substr(0,sdate.length()-1) + ")";
      std::vector<std::string> split_cmtview = smart_split(cmtview,
	  size.ws_col - 3);
      for (int i = 0; i < split_cmtview.size(); i++) {
	if (i == 0) {
	  std::cout << " - ";
	}
	else {
	  std::cout << "   ";
	}
	std::cout << split_cmtview[i] << std::endl;
      }
    }
    std::cout << std::endl;
  }
  if (subtasks_id.size() != 0) {
    std::cout << "subtask(s):" << std::endl;
    std::vector<Task*> subtasks;
    for (int subtask_id : subtasks_id) {
      subtasks.push_back((*id_to_ptr)[subtask_id]);
    }
    std::sort (subtasks.begin(), subtasks.end(), tasksort_default);
    for (Task* subtask : subtasks) {
      subtask->quickview(0);
    }
  }
}

void Task::read (std::string& stask) {
  int srt = 0; int end = 0;
  /* id */
  while (stask[end] != ' ') {
    end++;
  }
  id = std::stoi(stask.substr(srt,end-srt));
  (*id_to_ptr)[id] = this;
  /* title */
  end += 2; srt = end;
  while (stask[end] != '"') {
    end++;
  }
  title = stask.substr(srt,end-srt);
  /* description */
  end += 3; srt = end;
  while (stask[end] != '"') {
    end++;
  }
  description = stask.substr(srt,end-srt);
  /* state */
  end += 3; srt = end-1;
  state = std::stoi(stask.substr(srt,end-srt));
  /* creation_date */
  end += 1; srt = end;
  while (stask[end] != ' ') {
    end++;
  }
  creation_date = std::stoi(stask.substr(srt,end-srt));
  /* closure_date */
  if (state == 2) {
    end += 1; srt = end;
    while (stask[end] != ' ') {
      end++;
    }
    closure_date = std::stoi(stask.substr(srt,end-srt));
  }
  /* progression */
  end += 1; srt = end;
  while (stask[end] != ' ') {
    end++;
  }
  progression = std::stoi(stask.substr(srt,end-srt));
  /* priority */
  end += 2; srt = end-1;
  priority = std::stoi(stask.substr(srt,end-srt));
  /* comments */
  int n;
  end += 1; srt = end;
  while (stask[end] != ' ') {
    end++;
  }
  n = std::stoi(stask.substr(srt,end-srt));
  for (int i = 0; i < n ; i++) {
    end += 2; srt = end;
    while (stask[end] != '"') {
      end++;
    }
    std::string cmt = stask.substr(srt,end-srt);
    end += 2; srt = end;
    while (stask[end] != ' ') {
      end++;
    }
    time_t date = std::stoi(stask.substr(srt,end-srt));
    std::tuple<std::string, time_t> comment (cmt, date);
    comments.push_back(comment);
  }
  /* subtasks */
  end += 1; srt = end;
  while (stask[end] != ' ') {
    end++;
  }
  n = std::stoi(stask.substr(srt,end-srt));
  for (int i = 0; i < n ; i++) {
    end += 1; srt = end;
    while (stask[end] != ' ') {
      end++;
    }
    subtasks_id.push_back(std::stoi(stask.substr(srt,end-srt)));
  }
  if (n > 0) {
    prgs_editable = false;
  }
  /* subtask_of */
  end += 1; srt = end;
  while (stask[end] != ' ') {
    end++;
  }
  subtask_of = std::stoi(stask.substr(srt,end-srt));
  /* position */
  end += 1; srt = end;
  while (stask[end] != ' ') {
    end++;
  }
  position = std::stoi(stask.substr(srt,end-srt));
}

void Task::write (std::ofstream& file) {
  if (!to_delete) {
    file << id << ' ';
    file << '"' << title << '"' << ' ';
    file << '"' << description << '"' << ' ';
    file << state << ' ';
    file  << creation_date << ' ';
    if (state == 2) {
      file << closure_date << ' ';
    }
    file << progression << ' ';
    file << priority << ' ';
    int n = comments.size();
    file << n << ' ';
    for (int i = 0 ; i < n ; i++) {
      std::string cmt = std::get<0> (comments[i]);
      time_t date = std::get<1> (comments[i]);
      file << '"' << cmt << '"' << ' ' << date << ' ';
    }
    n = subtasks_id.size();
    file << n << ' ';
    for (int i = 0 ; i < n ; i++) {
      file << subtasks_id[i] << ' ';
    }
    file << subtask_of << ' ';
    file << position << ' ';
    file << '"' << std::endl;
  }
}

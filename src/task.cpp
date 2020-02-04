#include "../includes/task.hpp"

Task::Task (std::map<int, Task*>* id_to_ptr) : id_to_ptr(id_to_ptr) {}

Task::Task (std::map<int, Task*>* id_to_ptr, int id) : id_to_ptr(id_to_ptr), id(id) {
    (*id_to_ptr)[id] = this;
    time_t now = time(0);
    creation_date = now;
}

int Task::get_id () {return id;}
int Task::get_state () {return state;}
int Task::get_progression () {return progression;}
int Task::get_priority () {return priority;}
time_t Task::get_creation_date () {return creation_date;}
int Task::get_subtask_of () {return subtask_of;}
bool Task::get_del () {return del;}

void Task::set_title(std::string& t) {title = t;}
void Task::set_description(std::string& d) {description = d;}
void Task::set_priority (int p) {priority = p;}
void Task::set_subtask_of (int sto) {
  subtask_of = sto;
  if (subtask_of > 0) {
    (*id_to_ptr)[subtask_of]->add_subtask(id);
    priority = 0;
  }
}

void Task::update_progression () {
  int sum = 0;
  for (int subtask_id : subtasks_id) {
    sum += (*id_to_ptr)[subtask_id]->get_progression();
  }
  progression = sum / (subtasks_id.size()*100);
  if (progression == 0) {
    state = 0;
  }
  else if (progression == 100) {
    state = 2;
    closure_date = time(0);
  } else {
    state = 1;
  }
}

void Task::add_subtask (int subtask_id) {
  subtasks_id.push_back(subtask_id);
  this->update_progression();
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

int Task::quickview (int sub) {
  std::string pt;
  if (sub == 0) {
    pt = "-";
  } else {
    pt = "└";
  }
  std::string sprogr;
  if (state == 0) {
    sprogr = "Not Started";
  } else if (state == 1) {
    sprogr = "In Progress (" + std::to_string(progression) + "%)";
  } else {
    std::string scl_dt (ctime(&closure_date));
    sprogr = "Done (" + scl_dt.substr(0,scl_dt.length()-1) + ")";
  }
  std::string spriority (priority, '!');
  if (priority == 0) {
    spriority = "";
  }
  else {
    spriority = " [" + spriority + "]";
  }
  std::cout << std::string((sub+1), ' ') << pt << " (id:" << id << ") "
    << title << ": " << sprogr << ' ' << spriority << std::endl;

  int nb_tasks = 1;
  for (int subtask_id : subtasks_id) {
    nb_tasks += (*id_to_ptr)[subtask_id]->quickview(sub+1);
  }
  return nb_tasks;
}

void Task::print () {
  std::string sprogr;
  if (state == 0) {
    sprogr = "Not Started";
  } else if (state == 1) {
    sprogr = "In Progress (" + std::to_string(progression) + "%)";
  } else {
    std::string scl_dt (ctime(&closure_date));
    sprogr = "Done (" + scl_dt.substr(0,scl_dt.length()-1) + ")";
  }
  std::string spriority (priority, '!');
  if (priority == 0) {
    spriority = "";
  }
  else {
    spriority = " [" + spriority + "]";
  }
  std::string space (56 - sprogr.length() - (priority > 0) - spriority.length(), ' ');
  if (title.length()%2 == 1) {
    space = space.substr(0,space.length()-1);
  }
  std::string scr_dt = ctime(&creation_date);
  std::cout << std::endl << scr_dt.substr(0,scr_dt.length()-1)
    << space << sprogr << ' ' << spriority << std::endl;
  int nsep = (78 - title.length()) / 2;
  std::string sep (nsep, '*');
  std::cout << sep << ' ' << title << ' ' <<  sep << std::endl;
  if (description != "") {
    std::cout << description << std::endl << std::endl;
  }
  else {
    std::cout << "no description." << std::endl << std::endl;
  }
  if (comments.size() != 0) {
    std::cout << "comment(s):" << std::endl;
    for (std::string com : comments) {
      std::cout << " - " << com << std::endl;
    }
    std::cout << std::endl;
  }
  else {
    std::cout << "no comment." << std::endl << std::endl;
  }
  if (subtasks_id.size() != 0) {
    std::cout << "subtask(s):" << std::endl;
    for (int subtask_id : subtasks_id) {
      (*id_to_ptr)[subtask_id]->quickview(0);
    }
  }
  else {
    std::cout << "no subtask." << std::endl;
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
  end += 2; srt += 2;
  n = std::stoi(stask.substr(srt,end-srt));
  end--;
  for (int i = 0; i < n ; i++) {
    end += 3; srt = end;
    while (stask[end] != '"') {
      end++;
    }
    comments.push_back(stask.substr(srt,end-srt));
  }
  /* subtasks */
  end += 3; srt = end-1;
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
  end += 2; srt = end-1;
  subtask_of = std::stoi(stask.substr(srt,end-srt));
}

void Task::write (std::ofstream& file) {
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
    file << '"' << comments[i] << '"' << ' ';
  }
  n = subtasks_id.size();
  file << n << ' ';
  for (int i = 0 ; i < n ; i++) {
    file << subtasks_id[i] << ' ';
  }
  file << subtask_of << ' ';
  file << '"' << std::endl;
}

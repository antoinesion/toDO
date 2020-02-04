#include "../includes/task.hpp"

Task::Task (std::map<int, Task*>* id_to_ptr) : id_to_ptr(id_to_ptr) {}

Task::Task (std::map<int, Task*>* id_to_ptr,
    int id, std::string t, std::string d, int p, int st) :
  id_to_ptr(id_to_ptr), id(id), title(t), description(d), priority(p), subtask_of(st) {
    (*id_to_ptr)[id] = this;
    time_t now = time(0);
    creation_date = now;
    if (subtask_of > 0) {
      (*id_to_ptr)[subtask_of]->add_subtask(id);
      priority = 0;
    }
}

int Task::get_id () {return id;}
int Task::get_progression () {return progression;}
int Task::get_priority () {return priority;}
time_t Task::get_creation_date () {return creation_date;}
int Task::get_subtask_of () {return subtask_of;}
bool Task::get_del () {return del;}

void Task::update_progression () {
  int sum = 0;
  for (int subtask_id : subtasks_id) {
    sum += (*id_to_ptr)[subtask_id]->get_progression();
  }
  progression = sum / (subtasks_id.size()*100);
  if (progression == 0) {
    status = 0;
  }
  else if (progression == 100) {
    status = 2;
    closure_date = time(0);
  } else {
    status = 1;
  }
}

void Task::add_subtask (int subtask_id) {
  subtasks_id.push_back(subtask_id);
  this->update_progression();
}

int Task::close (bool force) {
  if (subtasks_id.size () == 0 && status != 2) {
    status = 2;
    progression = 100;
    time_t now = time(0);
    closure_date = now;
    return 1;
  }
  else if (subtasks_id.size () > 0 && status != 2) {
    if (!force) {
      std::cout << "(id:" << id << ") " << title << " has subtask(s)." << std::endl
	<< "Closing it will also close all those subtask(s)." << std::endl
	<< "Do you still want to proceed? [y/n] ";
      char choice;
      std::cin >> choice;
      if (choice == 'y') { force = true; }
    }
    if (force) {
      int nb_tasks_closed = 0;
      for (int subtask_id : subtasks_id) {
	nb_tasks_closed += (*id_to_ptr)[subtask_id]->close(true);
      }
      status = 2;
      progression = 100;
      time_t now = time(0);
      closure_date = now;
      return nb_tasks_closed + 1;
    }
  }
  return 0;
}

void Task::quickview (int sub) {
  std::string pt;
  if (sub == 0) {
    pt = "-";
  } else {
    pt = "└";
  }
  std::string sprogr;
  if (status == 0) {
    sprogr = "Not Started";
  } else if (status == 1) {
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

  for (int subtask_id : subtasks_id) {
    (*id_to_ptr)[subtask_id]->quickview(sub+1);
  }
}

void Task::print () {
  std::string sprogr;
  if (status == 0) {
    sprogr = "Not Started";
  } else if (status == 1) {
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
  /* status */
  end += 3; srt = end-1;
  status = std::stoi(stask.substr(srt,end-srt));
  /* creation_date */
  end += 1; srt = end; 
  while (stask[end] != ' ') {
    end++;
  }
  creation_date = std::stoi(stask.substr(srt,end-srt));
  /* closure_date */
  if (status == 2) {
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
  file << status << ' ';
  file  << creation_date << ' ';
  if (status == 2) {
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

#include "task.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <ctime>

Task::Task (std::map<int, Task*>* id_to_ptr) : id_to_ptr(id_to_ptr) {}

Task::Task (std::map<int, Task*>* id_to_ptr,
    int id, std::string t, std::string d, int p, int st) :
  id_to_ptr(id_to_ptr), id(id), title(t), description(d), priority(p), subtask_of(st) {
    (*id_to_ptr)[id] = this;
    time_t now = time(0);
    creation_date = ctime(&now);
    creation_date = creation_date.substr(0,creation_date.length()-1);
    if (subtask_of > 0) {
      (*id_to_ptr)[subtask_of]->add_subtask(id);
      priority = 0;
    }
}

int Task::get_id () {return id;}
int Task::get_progression () {return progression;}
int Task::get_priority () {return priority;}

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
  } else {
    status = 1;
  }
}

void Task::add_subtask (int subtask_id) {
  subtasks_id.push_back(subtask_id);
  this->update_progression();
}

void Task::quickview (int sub) {
  if (printed == false) {
    std::string pt;
    if (sub == 0) {
      pt = "-";
    } else {
      pt = "└";
    }
    std::string sstatus;
    if (status == 0) {
      sstatus = "Not Started";
    } else if (status == 1) {
      sstatus = "In Progress";
    } else {
      sstatus = "Done";
    }
    std::string spriority (priority, '!');
    if (priority == 0) {
      spriority = "";
    }
    else {
      spriority = " [" + spriority + "]";
    }
    std::cout << std::string((sub+1), ' ') << pt << " (id:" << id << ") "
      << title << ": " << sstatus << " (" << std::to_string(progression) << "%)"
      << spriority << std::endl;

    printed = true;

    for (int subtask_id : subtasks_id) {
      (*id_to_ptr)[subtask_id]->quickview(sub+1);
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
  /* status */
  end += 3; srt = end-1;
  status = std::stoi(stask.substr(srt,end-srt));
  /* creation_date */
  end += 2; srt = end; 
  while (stask[end] != '"') {
    end++;
  }
  creation_date = stask.substr(srt,end-srt);
  /* closure_date */
  if (status == 2) {
    end += 3; srt = end;
    while (stask[end] != '"') {
      end++;
    }
    closure_date = stask.substr(srt,end-srt);
  }
  /* progression */
  end += 2; srt = end;
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
  file << '"' << creation_date << '"' << ' ';
  if (status == 2) {
    file << '"' << closure_date << '"' << ' ';
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

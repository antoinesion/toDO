# toDO User Documentation

The `todo`command always works with an action, the list of those is described below:
* `create [-t|-d|-p|-sto] (values)` -> create a new task
* `list [-p|-so] (values)` -> list all the tasks created
* `move (id) (above/under) (id)` -> move a task in the list
* `show (id)` -> show a particular task
* `edit (id) [-t|-d|-p|-sto] (values)` -> edit a task
* `comment (id) (new/del) (comment)` -> add a comment to a task
* `progress (id) [+|-](%)` -> change the progression of a task
* `close (ids)` -> close some tasks
* `delete (ids)` -> delete some tasks
* `help` -> show this

All these actions are more precisely explained below.

### create
`create [-t|-d|-p|-sto] (values)` -> create a new task

##### Parameters
* `--title` or `-t`<br/>
Set a title to your task. ***This is a mandatory parameter.***
* `--description` or `-d`<br/>
Set a description to your task. It can be multi-lined. *This is an optional parameter.*
* `--priority` or `-p`<br/>Set a priority to you task. **Warning: a subtask cannot have a priority.** *This is an optional parameter.*<br/>
The possible values are:
	* `none` (*default*)
	* `low`
	* `medium`
	* `high`
* `--subtask_of` or `-sto`<br/>
Make your task a subtask of another one. The value here is the id of a task (*the default value is 0*). *This is an optional parameter.*

##### Examples
<pre>
> todo create -t "Project" -d "Description of the project" -p high
<b>info:</b> new task succesfully created (id:1).

> todo create -t "First step" -sto 1
<b>info:</b> new task sucessfully created (id:2).

> todo create -t "Second project" -p low
<b>info:</b> new task sucessfully created (id:3).

> todo list
<b>list:</b>
  - (id:1) Project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!!!]</span></b>
    └ (id:2) First step: <b><span style="color: #5F00af">Not Started</span></b>
  - (id:3) Second project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!]</span></b>
3 task(s).
</pre>

### list
`list [-p|-so] (values)` -> list all the tasks created

##### Parameters
* `--priority` or `-p`<br/>
Filter by a certain priority. *This is an optional parameter.*<br/>
The values are formed this way: `(low/medium/high)(+/-/[=])` where
	* `+` means "greater or equal"
	* `-` means "lower or equal"
	* `=` means "equal" (*default*)
* `--state` or `-st`<br/>
Filter by a certain state. *This is an optional parameter.*<br/>
The values are:
	* `not_started` meaning "the tasks not started yet"
	* `in_progress` meaning "the tasks which are already started but not done"
	* `uncompleted` meaning "the tasks which are not done"
	* `done` meaning "the tasks which are done"
* `--sort` or `-so`<br/>
Sort the tasks. *This is an optional parameter.*<br/>
The possible values are:
	* `priority` or `-priority` meaning "from the most important to the less important task"
	* `+priority` meaning "from the less important to the most important task"
	* `date` or `+date` meaning "from the oldest to the most recent task"
	* `-date` meaning "from the most recent to the oldest task"

##### Examples
<pre>
> todo list
<b>list:</b>
  - (id:1) Project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!!!]</span></b>
    └ (id:2) First step: <b><span style="color: #5F00af">Not Started</span></b>
  - (id:3) Second project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!]</span></b>
3 task(s).

> todo list -p high
<b>list:</b>
  - (id:1) Project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!!!]</span></b>
    └ (id:2) First step: <b><span style="color: #5F00af">Not Started</span></b>
2 task(s).

> todo list -p +low -so date-
<b>list:</b>
  - (id:3) Second project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!]</span></b>
  - (id:1) Project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!!!]</span></b>
    └ (id:2) First step: <b><span style="color: #5F00af">Not Started</span></b>
3 task(s).
</pre>

### move
`move (id) (above/under) (id)` -> move a task in the list
##### Example
<pre>
> todo list
<b>list:</b>
  - (id:1) Project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!!!]</span></b>
    └ (id:2) First step: <b><span style="color: #5F00af">Not Started</span></b>
  - (id:3) Second project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!]</span></b>
3 task(s).

> todo move 3 above 1
<b>info:</b> task (id:3) successfully moved.

> todo list
<b>list:</b>
  - (id:3) Second project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!]</span></b>
  - (id:1) Project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!!!]</span></b>
    └ (id:2) First step: <b><span style="color: #5F00af">Not Started</span></b>
3 task(s).
</pre>

### show
`show (id)` -> show a particular task

##### Example
<pre>
> todo show 1

Sun Mar 29 17:17:25 2020                                                    <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!!!]</span></b>
<b>------------------------------------------- Project -----------------------------------------</b>
Description of the project

subtask(s):
  - (id:2) First step: <b><span style="color: #5F00af">Not Started</span></b>
</pre>

### edit
`edit (id) [-t|-d|-p|-sto] (values)` -> edit a task

##### Parameters
* `--title` or `-t`<br/>
Change the title of your task. *This is an optional parameter.*
* `--description` or `-d`<br/>
Change the description of your task. It can be multi-lined. **Warning: it erases the former description.** *This is an optional parameter.*
* `-+description` ir `+d`<br/>
Add description to your task. *This is an optional parameter.*
* `--priority` or `-p`<br/>Set a priority to you task. **Warning: a subtask cannot have a priority.** *This is an optional parameter.*<br/>
The possible values are:
	* `none`
	* `low`
	* `medium`
	* `high`
* `--subtask_of` or `-sto`<br/>
Make your task a subtask of another one. The value here is the id of a task (*the default value is 0*). *This is an optional parameter.*

##### Example
<pre>
> todo show 1

Sun Mar 29 17:17:25 2020                                                    <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!!!]</span></b>
<b>------------------------------------------- Project -----------------------------------------</b>
Description of the project

subtask(s):
  - (id:2) First step: <b><span style="color: #5F00af">Not Started</span></b>


> todo edit 1 -t "Project ALPHA" +d "New line of description"
<b>info:</b> task (id:3) successfully edited.

> todo show 1

Sun Mar 29 17:17:25 2020                                                    <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!!!]</span></b>
<b>---------------------------------------- Project ALPHA --------------------------------------</b>
Description of the project
New line of description

subtask(s):
  - (id:2) First step: <b><span style="color: #5F00af">Not Started</span></b>
</pre>

### comment
`comment (id) (new/del) (comment)` -> add a comment to a task

##### Explanation
* To add a comment, simply write `todo comment (id) new "your comment"` replacing `(id)` by the id of your task
* To delete a comment, simply write `todo comment (id) del (position)` replacing `(id)` by the id of the task and `(position)` by the position of the comment you want to delete (like 1, 2, 3, etc.)

##### Examples
<pre>
> todo comment 1 new "I am adding a comment"
<b>info:</b> new comment successfully added to task (id:1).

> todo show 1

Sun Mar 29 17:17:25 2020                                                    <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!!!]</span></b>
<b>---------------------------------------- Project ALPHA --------------------------------------</b>
Description of the project
New line of description

comment(s):
 - I am adding a comment (Sun Mar 29 18:37:10 2020)

subtask(s):
  - (id:2) First step: <b><span style="color: #5F00af">Not Started</span></b>


> todo comment 1 del 1
<b>info:</b> comment 'I am adding a comment' successfuly deleted.
</pre>

### progress
`progress (id) [+|-](%)` -> change the progression of a task

##### Explanation
* Either use `progress (id) (percentage)` to set directly the percentage of progression of your task
* Or use `progress (id) +(percentage)` to increase the current progression of your task
* Or use `progress (id) -(percentage)` to decrease the current progression of your task

**Warning: you cannot change the progression a task that has subtask(s).** 

##### Example
<pre>
> todo progress 1 60
<b>info:</b> progression of task (id:2) succesfully upddated.

> todo list
<b>list:</b>
  - (id:3) Second project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!]</span></b>
  - (id:1) Project ALPHA: <span style="color: #ffd700"><b>In Progress</b> (60%)</span> <b><span style="color: red;">[!!!]</span></b>
    └ (id:2) First step: <span style="color: #ffd700"><b>In Progress</b> (60%)</span>
3 task(s).

> todo progress 1 -20
<b>info:</b> progression of task (id:2) succesfully upddated.

> todo list
<b>list:</b>
  - (id:3) Second project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!]</span></b>
  - (id:1) Project ALPHA: <span style="color: #ffd700"><b>In Progress</b> (40%)</span> <b><span style="color: red;">[!!!]</span></b>
    └ (id:2) First step: <span style="color: #ffd700"><b>In Progress</b> (40%)</span>
3 task(s).
</pre>

### close
`close (ids)` -> close some tasks

##### Explanation
You can close multiple tasks at one: `todo close (id1) (id2) [...]`

##### Example
<pre>
> todo close 2
It will close the following tasks:
  - (id:2) First step: <span style="color: #ffd700"><b>In Progress</b> (40%)</span>
Do you want to proceed ? [y/n] y

<b>info:</b> 1 task(s) closed.

> todo list
<b>list:</b>
  - (id:3) Second project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!]</span></b>
  - (id:1) Project ALPHA: <span style="color: #00af00"><b>Done</b> (Sun Mar 29 18:59:17 2020)</span> <b><span style="color: red;">[!!!]</span></b>
    └ (id:2) First step: <span style="color: #00af00"><b>Done</b> (Sun Mar 29 18:59:17 2020)</span>
3 task(s).
</pre>

### delete
`delete (ids)` -> delete some tasks

##### Explanation
You can delete multiple tasks at one: `todo delete (id1) (id2) [...]`

##### Example
<pre>
> todo delete 1
It will delete the following tasks:
  - (id:1) Project ALPHA: <span style="color: #00af00"><b>Done</b> (Sun Mar 29 18:59:17 2020)</span> <b><span style="color: red;">[!!!]</span></b>
    └ (id:2) First step: <span style="color: #00af00"><b>Done</b> (Sun Mar 29 18:59:17 2020)</span>
Do you want to proceed ? [y/n] y

<b>info:</b> 2 task(s) deleted.

> todo list
<b>list:</b>
  - (id:3) Second project: <b><span style="color: #5F00af">Not Started</span> <span style="color: red;">[!]</span></b>
1 task(s).
</pre>

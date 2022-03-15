# CyShell

## Introduction

This is an implementation of a Linux shell written in C language Using Sys-Calls

░█████╗░██╗░░░██╗░██████╗██╗░░██╗███████╗██╗░░░░░██╗░░░░░
██╔══██╗╚██╗░██╔╝██╔════╝██║░░██║██╔════╝██║░░░░░██║░░░░░
██║░░╚═╝░╚████╔╝░╚█████╗░███████║█████╗░░██║░░░░░██║░░░░░
██║░░██╗░░╚██╔╝░░░╚═══██╗██╔══██║██╔══╝░░██║░░░░░██║░░░░░
╚█████╔╝░░░██║░░░██████╔╝██║░░██║███████╗███████╗███████╗
░╚════╝░░░░╚═╝░░░╚═════╝░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝


## Run the shell

1. Clone this directory and `cd` into it.
2. Run the command `make`.
3. Run `./Shell` to get a prompt of the form `username@system_name:path`.
4. Run any command in the shell. It can entail as many number of tabs and spaces, the shell accounts for those.
5. In order to exit, run `quit` or `exit`.

## Features

### Built-in commands implementation

These commands have been defined by me and are contained within the shell itself.

1. `pwd`

   - Displays the name of the working directory.
   - Implemented in [pwd.c](pwd.c)
2. `ls [-l -a] [directory]`

   - Lists all the files and directories in the specified directory in alphabetical order.
   - Variations such as `ls, ls . , ls ..` also work.
   - Also handles multiple directories as arguments. eg. `ls -l dir1 dir2 dir3`.
   - Throws error if you try to `ls` on anything except a directory.
   - Also highlights directories in blue and executable files in green.
   - Implemented in [ls.c](ls.c)
3. `cd [file]`

   - Changes directory to the directory specified, throws an error if the directory does not exist.
   - Implemented in [cd.c](cd.c)
4. `echo [arguments]`

   - Displays whatever is specified in [arguments]
   - Implemented in [echo.c](echo.c)
5. `history [num]`

   - Prints the last arguement number of commands used in the shell (upto 20).
   - If no arguement is passed, by default it prints last 10 commands.
   - Retains the `history` even upon Shell exit - uses [history.txt](history.text).
   - Implemented in [history.c](history.c)
6. `pinfo [PID]`

   - Prints the information about the id of the process passed as argument.
   - Information about `PID` ,`Name` , `Memory` , `State` and `Executable Path` are displayed.
   - By default argument value is set to id of the shell process.
7. `jobs [-s -r]`

   - Prints a list of all currently running and stopped jobs along with their pid in alphabetical order.
   - Gives the state of the job – Running or Stopped.
   - Jobs with status corresponding to the passed flag are displayed.
   - Implemented in [jobs.c](jobs.c)
8. `fg <jobNumber>`

   - Brings a running or a stopped background job with given job number to foreground.
   - Implemented in [fg.c](fg.c)
9. `bg <jobNumber>`

   - Changes a stopped background job to a running background job.
   - Implemented in [bg.c](bg.c)
10. `sig [arguments]`

    - Passes the provided signal to the given background process.
    - Implemented in [signal.c](signal.c)
11. `repeat [arguemnts]`

    - Repeats the passed command given number of times.
    - Implemented in [pipe.c](pipe.c)
12. `replay [arguments]`

    - Executes a particular command in fixed time interval for a certain period.
    - Implemented in [pipe.c](pipe.c)
13. `baywatch [arguements]`

    - Interrupt: Print the number of times the CPU(s) has(ve) been interrupted by the keyboard controller.
    - Newborn: Prints the list of PIDs of active background processes in order of creation.
    - Dirty: Print the size of the part of the memory which is dirty .
    - Implemented in [baywatch.c](baywatch.c)

### Foreground and Background Processes

- To run a process in the background, follow the command with a '&' symbol. Eg. `gedit &`.
- Upon termination of a background process, the shell prints its PID and exit status.
- Handles `&` no matter where it is in the end. eg. `gedit& , gedit         &, ls -l&`.

## Additional Features

1. `CTRL-Z`

   - Changes the status of currently running job to stop, and pushes it to the background.
2. `CTRL-C`

   - Sends SIGINT signal to the current foreground job of the shell.
   - If there is no foreground job, then the signal does not have any effect.
3. `CTRL-D`

   - Logs you out of the shell without having any impact on the terminal.
4. Input-Output Redirection & Piping

   - Handles `<`, `>`, `>>`, and `|` operators appropriately, wherever they are in the command
   - Throws error if syntax is incorrect

### Coding style

The code is completely modular with different `.c` files for each command, and a `main.c` binding them all together. `Headers.h` entails all the necessary header files, and global variables.


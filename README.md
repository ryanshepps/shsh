# A1
## Features:
#### Set 1:
- [x] Internal shell command "exit" which terminates the shell
- [x] A command with no arguments
- [x] A command with arguments
- [x] A command, with or without arguments, executed in the background using &
#### Set 2: 
- [x] A command, with or without arguments, whose output is redirected to a file
- [x] A command, with or without arguments, whose input is redirected from a file
- [ ] A command, with or without arguments whose output is piped to the input of another command
#### Set 3: 
- [x] Limited shell environment variables
- [x] Reading in the profile file on initialization of the shell and executing any commands inside
  - [x] $PATH
  - [x] $HISTFILE
  - [x] $HOME
- [x] Implementing the buiiltin functions:
  - [x] export
  - [x] history
  - [x] cd

## Limitations
Redirecting
- Cannot be done with custom commands

Background Processes
- Cannot be done with custom commands

Piping
- Non-existent
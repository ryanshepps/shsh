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
- [ ] Limited shell environment variables
- [ ] Reading in the profile file on initialization of the shell and executing any commands inside
  - [ ] $PATH
  - [ ] $HISTFILE
  - [ ] $HOME
- [ ] Implementing the buiiltin functions:
  - [ ] export
  - [ ] history
  - [x] cd
## Functions
### int **new_custom_process**(char\* command, char\* parameters[])
**Parameters:**
 - **char\* command**: The custom command to execute
 - **char\* parameters[]**: The NULL terminated list of parameters to execute with the command

**Returns:**
 - **1** on success
 - **0** on failure

**Notes:**
 - This function returns failure if the command cannot be found. This is so that the new_process function can let the user know that they have not executed a recognized command, and exit the process normally. See the new_process function for more information.
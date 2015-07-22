// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"

#include <error.h>

// Added Header Files
#include "stdlib.h"

int
command_status (command_t c)
{
  return c->status;
}

void
execute_command (command_t c, int time_travel)
{
  // We assumed every command was a simple command
  // Get the command and run it
  system(*(c->u.word));
}

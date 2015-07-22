// UCLA CS 111 Lab 1 command reading
#include <error.h>

#include "command.h"
#include "command-internals.h"

// Added Header Files
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "alloc.h"

struct command_stream
{
	char *buffer; // Stores the characters as they are read
	command_t c; // command_t is a type
				 // it is a pointer to the command data structure
				 // The buffer will eventually need to placed in here
				 // Along with information about the type of command, input, output, etc.

	struct command_stream *next;
	struct command_stream *prev;
	struct command_stream *ix;
};

command_stream_t make_command_stream (int (*get_next_byte) (void *), void *get_next_byte_argument)
{

  struct command_stream *root;
  root =  (struct command_stream *) checked_malloc(sizeof(struct command_stream));
  root->next = NULL;
  root->prev = NULL;
  struct command_stream *curr = root;

  for (;;) // Outer loop = Adding nodes
  {
	int cursize = 20;
	int count = 0;
	char c;
	bool exit = false;

	curr->buffer = (char *) checked_malloc(sizeof(char) * cursize);
	curr->next = NULL;

	do // Inner loop = Adding characters to buffer
	{
		if(count > cursize)
		{
			curr->buffer = (char *) checked_realloc (curr->buffer, sizeof(char) * 2* cursize);
			cursize = 2 * cursize;
		}

		c = get_next_byte(get_next_byte_argument);

		if (c == EOF)
		{
			exit = true;
			break;
		}

		curr->buffer[count] = c;
		count = count + 1;

	} while (c != '\n');

	if (exit == true)
	{
	    curr->next = NULL;
		break;
    }

	// There's still more commands, so add another node
	curr->next = (struct command_stream *) checked_malloc(sizeof(struct command_stream));
	struct command_stream *temp = curr;
	curr = curr->next;
	curr->prev = (struct command_stream *) checked_malloc(sizeof(struct command_stream));
	curr->prev = temp;
  }

  // Set up the command data structure
  struct command_stream *travy = root; // Travy for traversing, get it?
  while(travy != NULL) // Traverse the whole list
  {
	travy->c = (command_t) checked_malloc(sizeof(struct command));
	// For now we'll just assume that everything's a simple command
	travy->c->type = SIMPLE_COMMAND;
	// Put whatever's in the buffer into the char **word in the command data structure
    travy->c->u.word = (char **) checked_malloc(sizeof(char *));
	*(travy->c->u.word) = (char *) travy->buffer;
	// Input means whatever is to the right of < in a command
	travy->c->input = NULL;
	// Output means whatever is to the right of > in a command
	travy->c->output = NULL;
	travy = travy->next;
  }

  return root;
}

command_t
read_command_stream (command_stream_t s)
{
   if(s->prev == NULL)
   {
        s->prev = s;
				s->ix = s; // Remember the current position when this function gets called again
				return s->c;
   }
   else if(s->ix->next == NULL)
        return NULL;
   else
   {
        s->ix = s->ix->next;
				return s->ix->c;

	}
}

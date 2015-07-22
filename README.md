Time Travel Shell
=================

INTRODUCTION
------------
Many shell scripts have command sequences that look like the following:

    sort < a | cat b - | tr A-Z a-z > c
    sort -k2 d - < a | uniq -c > e
    diff a c > f

In this example, the standard POSIX shell executes the code serially: it waits for the command in the first line to finish before starting the command in the second line. The goal of this project is to speed this up by running commands in parallel when it is safe to do so. In this example, it is safe to start running the second command before the first has finished, because the second command does not read any file that the first command writes. However, the third command cannot be started until the first command finishes, because the first command writes a file c that the third command reads.

SHELL SYNTAX SUBSET
-------------------
The implementation of the shell needs to support the following small subset of the standard POSIX shell grammar:

* Words, consisting of a maximal sequence of one or more adjacent characters that are ASCII letters (either upper or lower case), digits, or any of: ! % + , - . / : @ ^ _
* The following eight special tokens: ; | && || ( ) < >
* Simple commands, which are sequences of one or more words. The first word is the file to be executed.
* Subshells, which are complete commands surrounded by ( ).
* Commands, which are simple commands or subshells followed by I/O redirections. An I/O redirection is possibly empty, or < followed by a word, or > followed by a word, or < followed by a word followed by > followed by a word.
* Pipelines, which are one or more commands separated by |.
* And-ors, which are one or more pipelines separated by && or ||. The && and || operators are left-associative and have the same operator precedence.
* Complete commands, which are one or more and-ors each separated by a semicolon or newline, and which are optionally followed by a semicolon. An entire shell script is a complete command.
Comments, each consisting of a # that is not immediately preceded by an ordinary token, followed by characters up to (but not including) the next newline.
* White space consisting of space, tab, and newline. Newline is special: as described above, it can substitute for semicolon. Also, although white space can ordinarily appear before and after any token, the only tokens that newlines can appear before are (, ), and the first words of simple commands. Newlines may follow any special token other than < and >.

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

//checks for duplicates in from string, returns 1 if duplicate found, 0 otherwise
int checkDuplicates(const char* str)
{
  for ( ; *str != 0; str++)
    {
      for (const char* temp = str + 1; *temp != 0; temp++)
	{
	  if (*str == *temp)
	    return 1;
	}
    }
  return 0;
}

//find matching char to transpose to
char transpose(const char* from, const char* to, char letter)
{
  for (int i = 0; i < strlen(from); i++)
    {
      if (from[i] == letter)
	return to[i];
    }
  //if from doesn't contain letter, letter shouldn't be transposed 
  return letter;
}

//function to report and display error, called with specific error
void errOutput(const char* message)
{
  fprintf(stderr, "%s\nThe error number is: %d\n", message, errno);
  exit(1);
}

//tr2u function for main to call
void tr2u(const char* from, const char* to)
{
  //argument error checking
  if (!strlen(from))
    errOutput("Error: missing argument");
  if (strlen(from) != strlen(to))
    errOutput("Error: from and to are unequal length");
  if (checkDuplicates(from))
    errOutput("Error: from has duplicate bytes");

  //return value for read, write functions
  ssize_t eof;
  //initialize buffer with space for one char
  char buff[] = " ";

  //break condition is eof
  while (1)
    {
      eof = read(STDIN_FILENO, buff, 1);
      //read returns 0 if eof
      if (!eof)
	break;
      //returns negative number if error
      if (eof < 0)
	errOutput("Error: read unsuccessful");

      //store transposed char back in buff
      buff[0] = transpose(from, to, buff[0]);
      eof = write(STDOUT_FILENO, buff, 1);
      if (eof < 0)
	errOutput("Error: write unsuccessful");
    }
}

int main(int argc, const char* argv[])
{
  //first argument is name of program
  if (argc != 3)
    errOutput("Error: number of arguments");

  //from stored in argv[1], to in argv[2]
  tr2u(argv[1], argv[2]);
}

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

//checks for duplicates in from string
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

//checks for IO errors 
void IOErr(FILE* file)
{
  if (ferror(file))
    errOutput("I/O error");
}

//tr2b function for main to call
void tr2b(const char* from, const char* to)
{
  //argument error checking
  if (!strlen(from))
    errOutput("Error: missing argument");
  if (strlen(from) != strlen(to))
    errOutput("Error: from and to are unequal length");
  if (checkDuplicates(from))
    errOutput("Error: from has duplicate bytes");

  char fromChar;
  char toChar;

  //until end of file is reached
  while (!feof(stdin))
    {
      fromChar = getchar();
      IOErr(stdin);

      //find transposed character (ie. if char is in from str)
      toChar = transpose(from, to, fromChar);
      putchar(toChar);
      IOErr(stdout);
    }
}

int main(int argc, const char* argv[])
{
  //first argument is name of program
  if (argc != 3)
    errOutput("Error: number of arguments");

  //from stored in argv[1], to in argv[2]
  tr2b(argv[1], argv[2]);
}

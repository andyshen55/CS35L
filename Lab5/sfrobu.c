/*
Name: Thant Zin Oo
UID: 705172144

sfrobu.c - reads frobnicated text lines from standard input, and writes a
 sorted version to standard output in frobnicated form, without using library
 I/O functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>

bool f_opt = false;

int frobcmp(char const* a, char const* b)
{
  while(*a != ' ' && *b != ' ')
  {
    //accounting for null bytes
    while (*a == '\0')
      a++;
      
    while ( *b == '\0')
      b++;
    
    //unfrobnicates by xor-ing with 42 (0x2a in hex)
    char a_frob = *a ^ 0x2a;
    char b_frob = *b ^ 0x2a;

    //if -f is provided, toupper unfrobnicated a,b
    if (f_opt)
      {
	a_frob = toupper((unsigned char) a_frob);
	b_frob = toupper((unsigned char) b_frob);
      }
    
    //return -1 if a_frob is lexicographically < b_frob, or if a is shorter
    if (a_frob < b_frob || *a == ' ')
      return -1;

    //return 1 if a_frob is lexicographically > b_frob, or if b is shorter
    else if (a_frob > b_frob || *b == ' ')
      return 1;
    
    a++;
    b++;
  }

  //if function got to this point, *a == *b until end of word
  return 0;
}

//function to pass into qsort
int frobcmpPtr(const void* a, const void* b)
{
  //typecast required because we have array of strings (char**)
  return frobcmp(*(char**)a, *(char**)b);
}

//function to report and display error, called with specific error
void errOutput(const char* message)
{
  fprintf(stderr, "%s\nThe error number is: %d\n", message, errno);
  exit(1);
}

//checks for IO errors
void IOErr(ssize_t ret)
{
  if (ret < 0)
    errOutput("I/O error");
}

//checks if ptr is null (for return values of malloc/realloc)
void memErr(void* ptr)
{
  if (!ptr)
    errOutput("Memory allocation error");
}

//print word passed to function to stdout
void printWord(const char* s)
{
  ssize_t writeStat;
  //exit condition is end of word (space char)
  while (true)
    {
      writeStat = write(STDOUT_FILENO, s, 1);
      IOErr(writeStat);

      if (*s == ' ')
	return;

      s++;
    }
}

int main(int argc, char* argv[])
{
  //error checking of user-supplied arguments
  if (argc > 2)
    errOutput("Too many arguments");
  else if (argc == 2)
    {
      if (argv[1][0] != '-' && argv[1][1] != 'f')
	errOutput("Invalid argument");
      f_opt = true;
    }
  
  //number of letters in current word
  int numLetters = 0;
  //number of words parsed through stdin
  int numWords = 0;
  //index of buffer (as its used from 0 -> bufferCapacity)
  int bufferSize = 0;
  //current character read in from stdin
  char curr;
  //buffer to pass to read/write
  char rwBuf[] = " ";

  //checks for whether or not EOF or space (word separator) has been encountered
  bool isEOF = false;
  bool isSpace;

  //variables used for file information
  struct stat fileBuf;
  ssize_t ret;
  ret = fstat(STDIN_FILENO, &fileBuf);
  IOErr(ret);

  //size of input buffer
  int bufferCapacity = fileBuf.st_size + 1;

  //buffer used to store input
  char* buffer = malloc(sizeof(char) * bufferCapacity);
  memErr(buffer);
  //temporary buffer used to merge realloc calls
  char* temp;
  
  //continues until EOF
  while (!isEOF)
    {
      //reads one byte from stdin
      ret = read(STDIN_FILENO, rwBuf, 1);
      IOErr(ret);
      isEOF = !ret;
      curr = rwBuf[0];
      isSpace = (curr == ' ');

      //repeated spaces should be ignored
      if (!numLetters && isSpace)
	continue;

      //if buffer has reached capacity, resize buffer by scaling factor 2
      if (bufferSize == bufferCapacity)
	{
	  bufferCapacity *= 2;
	  temp = realloc(buffer, sizeof(char) * bufferCapacity);
	  memErr(temp);
	  //assign buffer pointer to temp only if temp reallocation is successful
	  buffer = temp;
	}

      //if current character should be stored in buffer
      if (!isEOF)
	{
	  buffer[bufferSize] = curr;
	  bufferSize++;
	  numLetters++;

	  //word hasn't terminated
	  if (!isSpace)
	    continue;
	}

      //EOF cases
      else
	{
	  //empty file 
	  if (!bufferSize)
	    {
	      free(buffer);
	      return 0;
	    }

	  //EOF implies last word, and every non-last word has space appended to end
	  //appends space to last word to normalize all words
	  if (buffer[bufferSize - 1] != ' ')
	    {
	      buffer[bufferSize] = ' ';
	      bufferSize++;
	    }

	  //if 0-length word (not a word)
	  if (!numLetters)
	    break;
	}

      //reaches this point if space character, implying end of current word
      //resets word length counter and increments word count
      numLetters = 0;
      numWords++;
    }

  //array of strings (pointer to char*) to store all words distinctly
  char** words = malloc(sizeof(char*) * numWords);
  memErr(words);

  //similar effect to buffer.split() in python, parses buffer for words to store
  char* tempWord = buffer;
  for (int i = 0, word = 0; i < bufferSize; i++)
    {
      if (buffer[i] == ' ')
	{
	  words[word] = tempWord;
	  word++;
	  //pointer manipulation, buffer + i points to current space character, + 1 is
	  //start of next word
	  tempWord = buffer + i + 1;
	}
    }

  //sort based on frobcmp return value
  qsort(words, numWords, sizeof(char*), frobcmpPtr);

  //print every word in words
  for (int i = 0; i < numWords; i++)
    printWord(words[i]);
  
  free(words);
  free(buffer);
  return 0;
}

/*
Name: Thant Zin Oo
UID: 705172144

sfrob.c - reads frobnicated text lines from standard input, and writes a
 sorted version to standard output in frobnicated form
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define DEFAULT_LEN 20

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

//checks for IO errors (stdin)
void IOErr(FILE* file)
{
  if (ferror(file))
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
  while (true)
    {
      putchar(*s);
      IOErr(stdout);

      if (*s == ' ')
	return;

      s++;
    }
}

int main(void)
{
  //variables to keep track of:
  
  //number of letters in current word
  int numLetters = 0;
  //number of words parsed through stdin
  int numWords = 0;
  //size of input buffer
  int bufferCapacity = DEFAULT_LEN;
  //index of buffer (as its used from 0 -> bufferCapacity)
  int bufferSize = 0;
  //current character read in from stdin
  char curr;

  //checks for whether or not EOF or space (word separator) has been encountered
  bool isEOF = feof(stdin);
  bool isSpace;

  //buffer used to store input
  char* buffer = malloc(sizeof(char) * bufferCapacity);
  memErr(buffer);
  //temporary buffer used to merge realloc calls
  char* temp;

  //continues until EOF
  while (!isEOF)
    {
      curr = getchar();
      IOErr(stdin);
      isEOF = feof(stdin);
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

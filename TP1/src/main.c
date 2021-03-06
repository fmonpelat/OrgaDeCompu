
/* 
## Algoritmo de Euclides - 66.20 Organizacion de computadoras FIUBA
## Date: 12/10/2020
## Notes: This program will essentially perform Euclides Algorithm

# MIT License

# Copyright (c) 2020 
# Written by:
# 		Damian ganopolsky (dganopolsky@fi.uba.ar)
# 		Jorge Sandez (sandez.jorge@gmail.com)
# 		Facundo Monpelat (fmonpelat@fi.uba.ar)
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h> /* File open errors */
#include <limits.h>
#include "../includes/common.h"
#include "../includes/euclidean.h"
#include "../includes/asmEuclidean.h"

#define MIN_VALUE 2

enum Options {OUTPUT};     // filenameOptions

int main (int argc, char **argv)
{
  char * filenameOut = NULL;      /* filename output */
  int getOpts = 0;                /* getOpts variable that holds the return status of getopts parser */
  int nums[2];
  FILE * f;
  bool divisor = false;            /* Divisor flag that activates divisor mode */
  bool multiple = false;          /* Multiple flag that activates the multiple mode*/
  
  if( (getOpts = getOptsProcedure(argc,argv,&filenameOut,nums,&divisor,&multiple)) == 1 )
    return 1;
  else if( getOpts == -1)
    return 0;

  if( filenameOut==NULL || !strcmp(filenameOut,"-") )
  {
    f = stdout; /* compatibility with piping */
  }
  else
  {
    f = fopen (filenameOut,"w"); /* open the file for writing*/
    if(f == NULL)
    {
      fprintf(stderr,"Error al grabar %s, %s\n", filenameOut, strerror(errno)); 
      return(1);             
    }
  }

  if(divisor)
  {
    #ifdef MIPS
    // call divisor procedure to calculate in asm
    fprintf(f,"%d\n",asmMcd((unsigned int)nums[0],(unsigned int)nums[1]));
    #else
    // call divisor procedure to calculate in C
    fprintf(f,"%d\n",mcd((unsigned int)nums[0],(unsigned int)nums[1]));
    #endif
  }
  else if(multiple)
  {
    #ifdef MIPS
    // call multiple procedure to calculate in asm
    fprintf(f,"%d\n",asmMcm((unsigned int)nums[0],(unsigned int)nums[1]));
    #else
    // call multiple procedure to calculate in C
    fprintf(f,"%d\n",mcm((unsigned int)nums[0],(unsigned int)nums[1]));
    #endif
  }
  else
  {
    #ifdef MIPS
    fprintf(f,"%d\n",asmMcd((unsigned int)nums[0],(unsigned int)nums[1]));
    fprintf(f,"%d\n",asmMcm((unsigned int)nums[0],(unsigned int)nums[1]));
    #else
    fprintf(f,"%d\n",mcd((unsigned int)nums[0],(unsigned int)nums[1]));
    fprintf(f,"%d\n",mcm((unsigned int)nums[0],(unsigned int)nums[1]));
    #endif
  }

  fclose(f);
  /* SO return exit code
  */
  return 0;
}



/* Procedure Name: getOptsProcedure
*  Arguments: 
*             int argc
*             int ** argv
*             char * filename  : ouptut filename
*             bool * divisor : Divisor flag
*             bool * multiple: Multiple flag
*  Notes: GetOpts Procedure of main program.
*/
int getOptsProcedure(int argc,char ** argv,char ** filename,int nums[2],bool *divisor,bool *multiple)
{
  *divisor = false;
  *multiple = false;

  int c;
  while (1)
  {
      static struct option long_options[] =
        {
          /* These options set a flag. */
          // {"input", required_argument, 0, 'i'},
          {"output", required_argument, 0, 'o'},
          {"divisor", no_argument, 0, 'd'},
          {"multiple", no_argument, 0, 'm'},
          {"version", no_argument, 0, 'v'},
          {"help", no_argument, 0, 'h'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "o:vhdm", long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
      {
        case 'o':
          *filename = optarg;
          break;

        case 'v':
          show_version();
          return -1;
          break;

        case 'h':
          show_help();
          return -1;
          break;
        
        case 'd':
          *divisor = true;
          break;

        case 'm':
          *multiple = true;
          break;

        default:
            fprintf(stderr, "Invalid option. Please run again.\n");
            show_help();
            return 1;
           break;
        }
    }

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
  {
    int idx=0;
    long lnum;
    int num;
    char *end;
    errno = 0;
    while (optind < argc)
    {
      lnum = strtol(argv[optind], &end, 10);
      if (end == argv[optind])
      {
        fprintf(stderr, "ERROR: can't convert string to number\n");
        return 1;
      }
      //If sizeof(int) == sizeof(long), we have to explicitly check for overflows
      if ((lnum == ULONG_MAX || lnum == MIN_VALUE) && errno == ERANGE)  
      {
        fprintf(stderr, "ERROR: number out of range\n");
        return 1;
      }
      //Because strtol produces a long, check for overflow
      if ( (lnum > UINT_MAX) || (lnum < MIN_VALUE) )
      {
        fprintf(stderr, "ERROR: number out of range\n");
        return 1;
      }

      num = (int) lnum;

      if(idx>=2)
      {
        printf("ERROR: only two arguments can be processed\n");
        return 1;
      }
      nums[idx++] = num;
      optind++;
    }
    // fix first number must be less than second for mcd/mdm (swap nums[0] and nums[1])
    if(nums[0]>nums[1])
    {
      int aux = nums[0];
      nums[0] = nums[1];
      nums[0] = aux; 
    }
  }
  else
  {
    printf("No arguments provided\n");
    show_help();
    return 1;
  }
  return 0;
}


/* Function Name: show_help 
*
*  Notes: this function show the menu of help with all the options available
*/
void show_help(){
    printf("Usage:\n");
    printf("  tp1 256 192\n");
    printf("  tp1 -h\n");
    printf("  tp1 -v\n");
    printf("  tp1 -o out_file\n");
    printf("Options:\n");
    printf("  -v, --version Print version and quit.\n");
    printf("  -h, --help Print this information and quit.\n");
    printf("  -o, --output Specify output stream/file, '-' for stdout.\n");
    printf("  -d, --divisor Only show common divisor.\n");
    printf("  -m, --multiple Only show common multiple.\n");
    printf("Examples:\n");
    printf("  ./tp1 -d -o - 256 192\n");
    printf("  ./tp1 -m -o - 256 192\n");
    // printf("tp0 -i 'inputs/input.txt' -o 'output.txt' \n");
}

/* Function Name: show_version 
*
*  Notes: this function show the version number of the program
*/
void show_version(){
  printf("v0.0.1\n");
}

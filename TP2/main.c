
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
#include <limits.h>
#include <errno.h> 
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "Cache.h"

extern cache_t cache;
extern unsigned int accesses_number;
extern memory_ram_t memory_ram;
extern int block_size_global;

enum Options {INPUT, OUTPUT};     // filenameOptions
#define BUF_LINE 30
char *buf;

int main(int argc,char* argv[])
{
    char * filenameOptions[2] = {NULL, NULL};      /* filename options */
    int getOpts = 0;                /* getOpts variable that holds the return status of getopts parser */
    FILE *f_in, *f_out;
    size_t cacheSize, blockSize, ways;

    if( (getOpts = getOptsProcedure(argc,argv,&filenameOptions[INPUT],&filenameOptions[OUTPUT],&ways,&blockSize,&cacheSize)) == 1 )
        return 1;
    else if( getOpts == -1)
        return 0;

    // setting cache parameters from command line args
    cache.block_size = (int) blockSize;
    cache.cache_size = (int) cacheSize*1024; // all sizes in bytes
    cache.number_of_ways = (int) ways;

    // preparing input/output streams
    if( prepareStreams(filenameOptions[INPUT],filenameOptions[OUTPUT],&f_in,&f_out)!=true )
        return 1;

    ssize_t nread;
    size_t buf_size = BUF_LINE;
    bool init_set = false;

    while ( (nread = getline(&buf,&buf_size,f_in)) >=0 )
    {
	    parse_command_cache(buf,&init_set,f_out);
	}

    free(buf);
    fclose(f_in);
    fclose(f_out);
    return 0;
}

/* Function Name: parse_command_cache 
*
*  Notes: this function parses the file input for valid cache commands
*/
bool parse_command_cache(char * buf, bool *init_set ,FILE *f)
{
    char command[5];
    unsigned int arg1;
    unsigned int arg2;

    strstrip(buf); // take unwanted chars = \r,\n and \t
    if( !strcmp(buf,"init") )
    {   
        if(*init_set==true)
            fprintf(f,"init is set already...\n");
        else 
        {
            init();
            *init_set = true;
        }
    }
    else if(!strcmp(buf,"MR"))
        fprintf(f,"miss rate: %d \n",get_miss_rate());
    else
    {
        if(!init_set)
        {
            fprintf(f,"must set init first\n");
            return false;
        }
        switch (buf[0])
        {
            case 'R':
                sscanf(buf,"%c %u",command,&arg1);
                fprintf(f,"command: %s arg1: %u\n",command,arg1);
                // read cache
                break;
            case 'W':
                sscanf(buf,"%c %u,%u",command,&arg1,&arg2);
                fprintf(f,"command: %s arg1: %u arg2: %u\n",command,arg1,arg2);
                // write cache
                break;
            default:
                fprintf(f,"Not a valid command: %s\n",buf);
                return false;
        }
    }
    return true;
}

/* Function Name: show_help 
*
*  Notes: this function show the menu of help with all the options available
*/
void show_help()
{
    printf("Usage:\n");
    printf("  tp2\n");
    printf("  tp2 -h\n");
    printf("  tp2 -V\n");
    printf("  tp2 -o out_file\n");
    printf("Options:\n");
    printf("  -V, --version Print version and quit.\n");
    printf("  -h, --help Print this information and quit.\n");
    printf("  -o, --output Specify output stream/file, '-' for stdout.\n");
    printf("  -w, --ways Ways quantity.\n");
    printf("  -cs, --cachesize Size of cache in Kilobytes.\n");
    printf("  -bs, --blocksize Size of blocks in bytes.\n");

    printf("Examples:\n");
    printf("  ./tp2 -w 4 -cs 8 -bs 16 prueba1.mem\n");
}

/* Function Name: show_version 
*
*  Notes: this function show the version number of the program
*/
void show_version()
{
  printf("v0.0.1\n");
}


bool convertParamToNumber(char *num,size_t *converted)
{
    char *end;
    *converted = strtol(num, &end, 10);
    if ( end == num )
    {
        fprintf(stderr, "ERROR: can't convert string to number\n");
        return false;
    }
    //If sizeof(int) == sizeof(long), we have to explicitly check for overflows
    if ( *converted == ULONG_MAX && errno == ERANGE )  
    {
        fprintf(stderr, "ERROR: number out of range\n");
        return false;
    }
    //Because strtol produces a long, check for overflow
    if ( *converted > UINT_MAX )
    {
        fprintf(stderr, "ERROR: number out of range\n");
        return false;
    }
    return true;
}

/* Procedure Name: getOptsProcedure
*  Arguments: 
*             int argc
*             int ** argv
*             char * filename  : ouptut filename
*             size_t * ways : ways quantity
*             size_t * cachesize : size of cache in Kilobytes
*             size_t * blocksize : size of block in bytes
*  Notes: GetOpts Procedure of main program.
*/
int getOptsProcedure(int argc,char ** argv,char ** filenameInput,char ** filenameOutput,size_t *ways,size_t *blocksize,size_t *cachesize)
{
  int c;
  int prev = 0;
  while (1)
  {
      static struct option long_options[] =
        {
          /* These options set a flag. */
          {"output", required_argument, 0, 'o'},
          {"ways", required_argument, 0, 'w'},
          {"cachesize", required_argument, 0, 's'}, // cs not allowed defaulted to s
          {"blocksize", required_argument, 0, 's'}, // bs not allowed defaulted to s
          {"version", no_argument, 0, 'V'},
          {"help", no_argument, 0, 'h'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "o:w:cs:bs:Vh", long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;
      switch (c)
      {
        case 'o':
          *filenameOutput = optarg;
          break;

        case 'V':
          show_version();
          return -1;
          break;

        case 'h':
          show_help();
          return -1;
          break;
        
        case 'w':
            if( !convertParamToNumber(optarg,ways) )
                return 1;
            break;

        // dummy options defaulted to s instead on next char
        case 'b':
        case 'c':
            break;
        case 's':
            if( prev == (int)'c' || !strcmp(long_options[option_index].name,"cachesize") )
            {
                if( !convertParamToNumber(optarg,cachesize) )
                {
                    fprintf(stderr,"Invalid cache size. \n");
                    return 1;
                }
                break;
            }
            if( !convertParamToNumber(optarg,blocksize) )
            {
                fprintf(stderr,"Invalid block size. \n");
                return 1;
            }
            break;

        default:
            fprintf(stderr, "Invalid option. Please run again.\n");
            show_help();
            return 1;
           break;
        }
        prev = c; // previous argument option (hack for more than 1 char for short opts)
    }

   /* Print any remaining command line arguments (not options). */
  if (optind < argc)
  {
    *filenameInput = argv[optind];
    optind++;
    if(optind < argc)
    {
        fprintf(stderr,"Error: More arguments supplied than input filename.\n");
        return 1;
    }
  }
  return 0;
}

/* Procedure Name: prepareStreams
*  Arguments: 
*             char *filenameinput  : Filename input char *
*             char *filenameoutput : Filename output char *
*             FILE **f_in          : FILE * for input stream
*             FILE **f_out         : FILE * for output stream
*  Notes: n/a
*/
bool prepareStreams(char *filenameinput,char *filenameoutput,FILE **f_in,FILE **f_out)
{
  if( filenameinput==NULL || !strcmp(filenameinput,"-"))
    *f_in = stdin; /* compatibility with piping */
  else
    *f_in = fopen(filenameinput, "r"); /* open from file */
  if (*f_in==NULL)
  {
      fprintf(stderr,"Error opening filename '%s', %s\n", filenameinput, strerror(errno));
      return false;
  }

  if(filenameoutput == NULL || !strcmp(filenameoutput,"-"))
      *f_out = stdout; /* compatibility with piping */
  else
    *f_out = fopen (filenameoutput,"w"); /* open the file for writing*/
  if(*f_out == NULL)
  {
    fprintf(stderr,"Error writting on filename %s, %s\n", filenameoutput, strerror(errno)); 
    return false;    
  }
  return true;
}

/* Function Name: strstrip 
*
*  Notes: strip \n \r and \t from string
*/
void strstrip(char *s)
{
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n' && *s != '\r') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}

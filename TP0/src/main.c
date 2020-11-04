
/* 
## Base 64 - 66.20 Organizacion de computadoras FIUBA
## Date: 12/10/2020
## Notes: This program will essentially perform a base64 encoding and decoding

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
#include "../includes/common.h"
#include "../includes/encriptado_base64.h"

#define BUF_SIZE 3
#define BUF_BASE64 BUF_SIZE*2

enum Options {INPUT, OUTPUT}; // filenameOptions

int main (int argc, char **argv)
{

  char * filenameOptions[2]; /* array that contains filenames from input and output */
  filenameOptions[INPUT] = NULL;
  filenameOptions[OUTPUT] = NULL;
  bool decode = false;       /* Decode flag that activates decoding mode */
  int getOpts = 0;           /* getOpts variable that holds the return status of getopts parser */
  FILE *f = NULL;
  FILE *fout = NULL;
  char buf[BUF_SIZE+1]; // BUF_SIZE + 1 for '\n'
  size_t bufsize = BUF_SIZE; // Buffer size
  char bufBase64[BUF_BASE64]; // buffer for encoded/decoded string
  size_t nread; // readed chars from stream
  size_t len; // length of decoded/encoded string

  if( (getOpts = getOptsProcedure(argc,argv,filenameOptions,&decode)) == 1 )
    return 1;
  else if( getOpts == -1)
    return 0;

  if( prepareStreams(filenameOptions[INPUT],filenameOptions[OUTPUT],&f,&fout)!=true )
    return 1;

  while( (nread = fread(buf, sizeof(char), bufsize, f)) >=1 ) {
      if(decode==true)
        desencriptar_base64(buf,nread,bufBase64,&len);
      else
        encriptar_base64((const unsigned char *)buf,nread,bufBase64,&len);
      
      /* write numbers on the file stream*/
      fwrite(bufBase64,sizeof(char),len,fout);
  }
   /* close the file*/  
  fclose(f);
  fclose(fout);

  /* free of used filenames */
  if(filenameOptions[OUTPUT]) free(filenameOptions[OUTPUT]);
  if(filenameOptions[INPUT]) free(filenameOptions[INPUT]);

  /* SO return exit code
  */
  return 0;
}



/* Procedure Name: getOptsProcedure
*  Arguments: 
*             int argc
*             int ** argv
*             char * filenames[x] : Array of strings (char*) of size x
*             bool * decode : Decoding flag instead of encoding (initially false)
*  Notes: GetOpts Procedure of main program, please free up options array after using the procedure.
*/
int getOptsProcedure(int argc,char ** argv,char * filenames[2],bool *decode)
{
  char * input = NULL;
  char * output = NULL;
  // initially decode is false
  *decode = false;

  int c;
  while (1)
  {
      static struct option long_options[] =
        {
          /* These options set a flag. */
          {"input", required_argument, 0, 'i'},
          {"output", required_argument, 0, 'o'},
          {"decode", no_argument, 0, 'd'},
          {"version", no_argument, 0, 'v'},
          {"help", no_argument, 0, 'h'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "i:o:vhd", long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
      {
        case 'i':
          /* printf ("option -i with value `%s'\n", optarg); */
          if( !(input = (char*) malloc( sizeof(char) * strlen(optarg) + 1 )) )
          {
            printf("Pedido de memoria insatisfactorio, getopts 1\n");
            return 1;
          }
          strncpy (input, optarg, sizeof(char) * strlen(optarg) + 1);
          filenames[INPUT] = input;
          break;

        case 'o':
          /* printf ("option -o with value `%s'\n", optarg); */
          /* set output filename */
          if( !(output = (char*) malloc(sizeof(char)*strlen(optarg)+1)) )
          {
            printf("Pedido de memoria insatisfactorio, getopts 2\n");
            return 1;
          }
          strncpy (output, optarg, sizeof(char)*strlen(optarg)+1);
          filenames[OUTPUT] = output;
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
          *decode = true;
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
    printf ("Invalid option. Please add '-' before option: ");
    while (optind < argc)
      printf ("%s ", argv[optind++]);
    putchar ('\n');
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
      fprintf(stderr,"%s\n",filenameinput);
      fprintf(stderr,"Error al abrir el archivo '%s', %s\n", filenameinput, strerror(errno));
      return false;
  }

  if(filenameoutput == NULL || !strcmp(filenameoutput,"-"))
    *f_out = stdout; /* compatibility with piping */
  else
    *f_out = fopen (filenameoutput,"w"); /* open the file for writing*/
  if(*f_out == NULL)
  {
    fprintf(stderr,"Error al grabar %s, %s\n", filenameoutput, strerror(errno)); 
    return false;    
  }
  return true;
}

/* Function Name: show_help 
*
*  Notes: this function show the menu of help with all the options available
*/
void show_help(){
    printf("Usage:\n");
    printf("  tp0 -h\n");
    printf("  tp0 -v\n");
    printf("  tp0 -i in_file -o out_file\n");
    printf("Options:\n");
    printf("  -v, --version Print version and quit.\n");
    printf("  -h, --help Print this information and quit.\n");
    printf("  -i, --input Specify input stream/file, '-' for stdin.\n");
    printf("  -o, --output Specify output stream/file, '-' for stdout.\n");
    printf("  -d, --decode Decode from Base64 encoding.\n");
    printf("Examples:\n");
    printf("  tp0 < in.txt > out.txt\n");
    printf("  cat in.txt | tp1 -i - > out.txt\n");
    printf("tp0 -i 'inputs/input.txt' -o 'output.txt' \n");

}

/* Function Name: show_version 
*
*  Notes: this function show the version number of the program
*/
void show_version(){
  printf("v0.0.1\n");
}

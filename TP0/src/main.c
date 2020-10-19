
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


enum Options {INPUT, OUTPUT}; // filenameOptions

int main (int argc, char **argv)
{

  char * filenameOptions[2]; /* array that contains filenames from input and output */
  filenameOptions[INPUT] = NULL;
  filenameOptions[OUTPUT] = NULL;
  bool decode = false;       /* Decode flag that activates decoding mode */
  int getOpts = 0;           /* getOpts variable that holds the return status of getopts parser */
  int numberLines = 0;       /* Number of lines readed by process_file() */
  char **arraysStrings = NULL; /* Array of strings that holds what is read */

  if( (getOpts = getOptsProcedure(argc,argv,filenameOptions,&decode)) == 1 )
    return 1;
  else if( getOpts == -1)
    return 0;

  if( process_file(filenameOptions[INPUT],&arraysStrings,&numberLines) == 1 )
    return 1;
  
  char ** outStrings = (char **) malloc( sizeof(char*) * numberLines ); /* Initial malloc outStrings vector */

  for(int j = 0; j < numberLines; j++)
  {
    char * outString = (char *) malloc( sizeof(char) * (strlen(arraysStrings[j]+1)));

    if(decode==true)
    {
      outString = (char*) desencriptar_base64(arraysStrings[j],strlen(arraysStrings[j]));
    }
    else
    {
      outString = (char*) encriptar_base64((unsigned char*)arraysStrings[j],strlen(arraysStrings[j]));
    }
    outStrings[j] = outString;
  }
  

  /* if have filename set save to file */
  if(filenameOptions[OUTPUT]!=NULL)
  {
    if ( save_file(filenameOptions[OUTPUT],outStrings,numberLines) != 0 )
      return 1;
  }
  
  
  /* dynamic memory free 
  */
  for(int i = 0; i<numberLines;i++)
  {
    free(arraysStrings[i]);
    free(outStrings[i]);
  }
  
  free(arraysStrings);
  free(outStrings);

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

/* Function Name: process_file
*  Arguments: 
*             char * file     : Input filename
*             char ** lineas [] : char ** array of strings to read
*             int * numlines: Number of lines read
*             
*  Notes: this function process the incoming file and creates the array.
*/
int process_file(char* file, char ***lineas,int *numlines){

    /* Opening file pointer
    */
    FILE *f;
    if( file==NULL || !strcmp(file,"-"))
      f = stdin; /* compatibility with piping */
    else
      f = fopen(file, "r"); /* open from file */
    if (f==NULL)
    {
        fprintf(stderr,"Error al abrir el archivo '%s', %s\n", file, strerror(errno));
        return 1;
    }

    /* Buffer array of chars to read each line
    */
    char *line = NULL; /* Assign pointer of buffer */
    size_t linesize = 200; /* Buffer size is 200 */
    size_t nread; /* Number of characters read */
    
     /* malloc for the pointer for int *lineas */
    if( !(*lineas = (char **) malloc(sizeof(char *))) )
    {
      fprintf(stderr,"Pedido de memoria insatisfactorio, process file 2\n");
      return 1;
    }

    /* numlines is the variable that holds the count of lines read */
    *numlines = 0;

    /* Reading each line until '\n' or EOF appears
    */
    while ((nread = getline(&line, &linesize, f)) != -1){
        
        char * string = (char *) malloc( sizeof(char) * nread);
        if(string == NULL )
        {
          fprintf(stderr,"Pedido de memoria insatisfactorio, process file 4\n");
          return 1;
        }
        if( line[nread-1]=='\n') line[nread-1]='\0';
        memcpy(string, line, nread * sizeof(char));

        (*lineas)[*numlines] = string;
        // printf("strlen: %zu\n",strlen(string));
        (*numlines)++;
               
    }
    fclose(f);

    if(line) 
      free(line);
    return 0;
}

/* Function Name: save_file
*  Arguments: 
*             char * file     : Saving filename
*             int * lineas [] :
*             int * array_len :
*             
*  Notes: this function process the incoming file and creates the array.
*/
int save_file(char* file, char **lineas, size_t numLines){
    FILE * f;
    if(!strcmp(file,"-"))
      f = stdout; /* compatibility with piping */
    else
      f = fopen (file,"w"); /* open the file for writing*/
    if(f == NULL){
      fprintf(stderr,"Error al grabar %s, %s\n", file, strerror(errno)); 
      return(1);             
    }

    /* write numbers on the file stream*/
    for(int j = 0; j<numLines ; j++)
    {
      fprintf(f,"%s\n",lineas[j]);
    }
   /* close the file*/  
   fclose (f);
   return 0;
}

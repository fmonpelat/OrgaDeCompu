
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
#include <errno.h> /* File open errors */
#include "common.h"

int main (int argc, char **argv)
{

  /* The variable options has two strings that will be returned by the 
      calling getOptsProcedure() 
  */
  char * options[2];
  options[0] = NULL;
  options[1] = NULL;
  int **arrays_nros = NULL;
  int *arrays_len = NULL;

  if(getOptsProcedure(argc,argv,options) == 1)
    return 1;

  int numberLines;
  /* if input filename is not present cannot process => print help*/
  if( process_file(options[0],&arrays_nros,&arrays_len,&numberLines) == 1 )
    return 1;
  
   for(int j = 0; j < numberLines; j++) {
     /*
    if(arrays_nros[j] == 0 || arrays_len[j] == 0)
    {
      printf("%s\n","");
      continue;
    }
    
    // call for base64 encoding/decoding procedure
    printing if needed
    for(int h = 0; h < arrays_len[j]; h++)
      printf("%i ", arrays_nros[j][h]);
    printf("\n");
    fflush(stdout);
  */
  }

  /* if have filename set save to file */
  if(options[1]!=NULL)
  {
    if ( save_file(options[1],arrays_nros,arrays_len) != 0 )
      return 1;
  }
  
  
  /* dynamic memory free 
  */
  for(int i = 0; arrays_nros[i]==0;i++)
    free(arrays_nros[i]);
  free(arrays_len);
  free(arrays_nros);

  if(options[1]) free(options[1]);
  if(options[0]) free(options[0]);

  /* SO return exit code
  */
  return 0;
}



/* Procedure Name: getOptsProcedure
*  Arguments: 
*             int argc
*             int ** argv
*             char * options[x] : Array of strings (char*) of size x
*  Notes: GetOpts Procedure of main program, please free up options array after using the procedure.
*/
int getOptsProcedure(int argc,char ** argv,char * options[2])
{
  char * input = NULL;
  char * output = NULL;
  int c;
  while (1)
  {
      static struct option long_options[] =
        {
          /* These options set a flag. */
          {"input", required_argument, 0, 'i'},
          {"output", required_argument, 0, 'o'},
          {"version", no_argument, 0, 'v'},
          {"help", no_argument, 0, 'h'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "i:o:vh", long_options, &option_index);

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
          options[0] = input;
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
          options[1] = output;
          break;

        case 'v':
          show_version();
          return 1;
          break;

        case 'h':
          show_help();
          return 1;
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
*             int * lineas [] : int * array of numbers by lines read
*             int * array_len : Array length of each line in lineas array.
*             
*  Notes: this function process the incoming file and creates the array.
*/
int process_file(char* file, int ***lineas, int **array_len,int *numlines){

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
    size_t linesize = 32; /* Buffer size is 32 */
    size_t nread; /* Number of characters read */

    /* malloc always returns void * better cast it */
    if( !(line = (char *) malloc(linesize * sizeof(char))) )
    {
      fprintf(stderr,"Pedido de memoria insatisfactorio,process file 1\n");
      return 1;
    }
    
     /* malloc for the pointer for int *lineas */
    if( !(*lineas = (int **) malloc(sizeof(int *))) )
    {
      fprintf(stderr,"Pedido de memoria insatisfactorio, process file 2\n");
      return 1;
    }

    /* malloc for the pointer int * array_len */
    if( !(*array_len = (int *) malloc( sizeof(int) )) )
    {
      fprintf(stderr,"Pedido de memoria insatisfactorio, process file 3\n");
      return 1;
    }

    /* numlines is the variable that holds the count of lines read */
    *numlines = 0;

    /* Reading each line until '\n' or EOF appears
    */
    while ((nread = getline(&line, &linesize, f)) != -1){
        
        /* aux variable to store all numbers */
        int * array_num = (int *) malloc( sizeof(int) * nread);
        if(array_num == NULL )
        {
          fprintf(stderr,"Pedido de memoria insatisfactorio, process file 4\n");
          return 1;
        }
        /* idx is the index of array_num, after for finish we will have the len of array_num
        */
        int idx = 0;
            
        for(int i = 0; i < nread ; i++){
          int x;
          if(isdigit(line[i])) {   
            sscanf(&line[i], "%i", &x);
            /* printf("%i \n", x); */
            array_num[idx] = x;
            idx ++;
          }
          else if(line[i]=='-')
          {
            i++; /* scan the next char (if its not a digit its a filerror) */
            if(! isdigit(line[i]))
            {
              fprintf(stderr,"%s\n","File syntax error!");
              return 1;
            }
            sscanf(&line[i], "%i", &x);
            /* printf("%i \n", x); */
            array_num[idx] = (x * -1);
            idx ++;
          }
        }

        /* malloc to withold the data exactly */
        int * array_new = malloc( idx * sizeof(int) );
        if( array_new == NULL)
        {
          fprintf(stderr,"Pedido de memoria insatisfactorio, process file 5\n");
          return 1;
        }
        /* guardo valores en la memoria */
        memcpy(array_new, array_num, idx * sizeof(int));

        /* guardo direccion de memoria del array y longitud */
        (*lineas)[*numlines] = array_new;
        (*array_len)[*numlines] = idx;
        (*numlines)++;
        
        /* realloc to increase one size pointer for int *lineas */
        *lineas =  (int **) realloc( *lineas, ((size_t)(*numlines)+1) * sizeof(int *) );
        if(lineas == NULL)
        {
          fprintf(stderr,"Pedido de memoria insatisfactorio, process file 6\n");
          return 1;
        }
        /* realloc to increase one size pointer for int *lineas */
        *array_len =  (int *) realloc( *array_len, ((size_t)(*numlines)+1) * sizeof(int) );
        if(array_len == NULL)
        {
          fprintf(stderr,"Pedido de memoria insatisfactorio, process file 6\n");
          return 1;
        }
        free(array_num);
    }
    /* terminator line */
    (*lineas)[*numlines] = 0;
    (*array_len)[*numlines] = 0;
    fclose(f);
    if(line) free(line);
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
int save_file(char* file, int **lineas, int *array_len){
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
    for(int j = 0; lineas[j]!=0 ; j++)
    {
        for(int h = 0; h < array_len[j]; h++)
        {
                fprintf(f,"%i ", (lineas[j])[h]);
        }
        fprintf(f,"\n");
    }
   /* close the file*/  
   fclose (f);
   return 0;
}

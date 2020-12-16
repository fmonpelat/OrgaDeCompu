/*
* Header of C main
* Notes: 
* Date: 16/12/2020
*/

#ifndef __MAIN_H__
#define __MAIN_H__
#include <stdio.h>
#include <stdlib.h>

void show_help(void);

void show_version(void);

int getOptsProcedure(int argc,char ** argv,char ** filenameInput,char ** filenameOutput,size_t *ways,size_t *blocksize,size_t *cachesize);

bool convertParamToNumber(char *num,size_t *converted);

bool prepareStreams(char *filenameinput,char *filenameoutput,FILE **f_in,FILE **f_out);

#endif /* __MAIN_H__ */

#ifndef __COMMON_H__
#define __COMMON_H__

void show_help();

void show_version();

int process_file(char* file, int ***lineas, int **array_len,int *numlines);

int save_file(char* file, int **lineas, int *array_len);

int getOptsProcedure(int argc,char ** argv,char * options[2]);

#endif /* __COMMON_H__ */
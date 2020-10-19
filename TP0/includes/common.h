#ifndef __COMMON_H__
#define __COMMON_H__

void show_help();

void show_version();

int process_file(char* file, char ***lineas, int *numlines);

int save_file(char* file, char **lineas, size_t numLines);

int getOptsProcedure(int argc,char ** argv,char * options[2], bool * decode);

#endif /* __COMMON_H__ */
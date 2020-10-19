#ifndef __COMMON_H__
#define __COMMON_H__

void show_help();

void show_version();

int process_file(char* file, char ***lineas, size_t *numlines);

int save_file(char* file, char **lineas, size_t numLines, size_t *lineasLen);

int getOptsProcedure(int argc,char ** argv,char * options[2], bool * decode);

#endif /* __COMMON_H__ */
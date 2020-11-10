#include "../includes/euclidean.h"
#include <stdio.h>

int main(int argc, char** argv) {
    printf("mcd entre 10 y 15 tiene que ser 5: %s\n",(mcd(10,15) == 5) ? "true":"false");
    printf("mcd entre 15 y 10 tiene que ser 5: %s\n",(mcd(10,15) == 5) ? "true":"false");
    printf("mcm entre 10 y 15 tiene que ser 30: %s\n",(mcm(10,15) == 30) ? "true":"false");
    printf("mcd entre 10001 y 1 tiene que ser 1: %s\n",(mcd(10001,1) == 1) ? "true":"false");
    printf("mcm entre 10001 y 1 tiene que ser 10001: %s\n",(mcm(10001,1) == 10001) ? "true":"false");
    return 0;
}
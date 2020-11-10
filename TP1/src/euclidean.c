#include "../includes/euclidean.h"

unsigned int mcd(unsigned int m, unsigned int n) {
    if (m == 0) 
        return n; 
    return mcd(n % m, m);
}

unsigned int mcm(unsigned int m, unsigned int n) {
    return (m*n / mcd(m,n));
}

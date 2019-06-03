#include<math.h>

void sol_gauss(double **A, double *b, double *x, int n);
void direct_st(double **A, double *x, int n);
void transform(double **A, double *x, int i, int n);
void exchange(double **A, double *x, int first, int second, int n);
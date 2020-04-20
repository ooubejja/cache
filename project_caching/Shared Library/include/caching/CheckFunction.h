#ifndef CHECKFUNCTION_H
#define CHECKFUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace caching{

using namespace std;

void check_memory_allocation_1D(int *pt, string msg);
void check_memory_allocation_2D(int **pt, string msg);
void check_memory_allocation_3D(int ***pt, string msg);
void check_memory_double_allocation_1D(double *pt, string msg);
void check_memory_double_allocation_2D(double **pt, string msg);

}//end namespace caching

#endif // CHECKFUNCTION_H
#ifdef SWIG
%module example
%{
#include "a.h"
%}
#endif

#ifndef A_H
#define A_H

void callme (void);

#endif

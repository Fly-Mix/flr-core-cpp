/* File : flr.i */
%module flr

/* include STL/C++ Library */
%include "stl.i"

%{
#include "api-libs/flr-core/include/flr-core/flr.h"
%}

/* Let's just grab the original header file here */
%include "api-libs/flr-core/include/flr-core/flr.h"

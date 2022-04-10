/* File : flr.i */
%module flr

/* include STL/C++ Library */
%include "stl.i"

/* 注意：flr.h 依赖 flr_log_def.h 和 flr_ui_def.h，因此2者的声明需要在其前面 */
%{
#include "api-libs/flr-core/include/flr-core/flr_log_def.h"
#include "api-libs/flr-core/include/flr-core/flr_ui_def.h"
#include "api-libs/flr-core/include/flr-core/flr.h"
%}

/* Let's just grab the original header file here */
%include "api-libs/flr-core/include/flr-core/flr_log_def.h"
%include "api-libs/flr-core/include/flr-core/flr_ui_def.h"
%include "api-libs/flr-core/include/flr-core/flr.h"

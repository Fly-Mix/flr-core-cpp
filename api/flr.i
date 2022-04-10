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

/* Wrap a function taking a pointer to a function */
extern flr::register_log_message_callback(void(*op)(const char *, int, const char *, int, const char *));
extern flr::register_ui_message_callback(void(*op)(int, const char *));

/* Now install a bunch of "ops" as constants */
%constant void (*DEFAULT_LOG_MESSAGE_PRINTER)(const char *, int, const char *, int, const char *) = flr::default_log_message_printer;
%constant void (*DEFAULT_UI_MESSAGE_PRINTER)(int, const char *) = flr::default_ui_message_printer;

%module exjudger
%{
#define SWIG_PYTHON_EXTRA_NATIVE_CONTAINERS 
#include "../src/Exception/JudgerException.h"
#include "../src/ExtraJudger.h"
%}
%include "../src/ExtraJudger.h"
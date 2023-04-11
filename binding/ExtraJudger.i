%module exjudger
%{
#define SWIG_PYTHON_EXTRA_NATIVE_CONTAINERS 
#include "../src/Exception/JudgerException.h"
#include "../src/ExtraJudger.h"
#include "../src/util.h"
#include "../src/Judger.h"
#include "../src/JudgerFSM.h"
#include "../src/FormatJudger.h"
#include "../src/FunctionJudger.h"
#include "../src/MemoryJudger.h"
#include "../src/StyleJudger.h"
%}
%include "../src/ExtraJudger.h"
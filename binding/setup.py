from distutils.core import setup, Extension
 
mod_module = Extension('_exjudger',
                           sources=['ExtraJudger_wrap.cxx', 
                                    '../src/ExtraJudger.cpp',
                                    '../src/Exception/JudgerException.cpp',
                                    '../src/JudgerFSM.cpp',
                                    '../src/FormatJudger.cpp',
                                    '../src/FunctionJudger.cpp',
                                    '../src/MemoryJudger.cpp',
                                    '../src/StyleJudger.cpp'],
                           )
setup (name = 'exjudger',
       version = '3.0',
       author      = "beshar",
       description = """Simple swig example from docs""",
       ext_modules = [mod_module],
       py_modules = ["exjudger"],
       )
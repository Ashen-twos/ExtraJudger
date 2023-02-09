#! /bin/bash

set -xe

swig -c++ -python ExtraJudger.i
python3 setup.py build_ext --inplace
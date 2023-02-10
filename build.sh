#! /bin/bash

set -xe

cd src
git clone https://github.com/gabime/spdlog.git
cd spdlog && mkdir build && cd build
cmake .. && make -j 

cd ../../..
cd binding
bash build.sh

#! /bin/bash

set -xe

cp -rf include/spdlog /usr/include/
cd binding
bash build.sh

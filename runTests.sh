#!/bin/bash
#
BROWN='\033[0;33m'
NC='\033[0m' # No Color

set -e

if [ -z "$1" ]
then
  echo "Usage: [assignment number (e.g. 1)]"
  exit -1
fi


printf "${BROWN}############### Building ###############${NC}\n"

# Build Testystem
cd framework/testsystem
mkdir -p build
cd build
cmake .. >> /dev/null
cmake --build . --parallel >> /dev/null
cd ../../../

# Build server
mkdir -p build
cd build
cmake ..
cmake --build . --parallel

if [ $? -eq 0 ]; then
    echo "Server compiled"
  else
    echo "Failed to compile server"
fi

printf "\n${BROWN}############### Testing ###############${NC}\n"

../framework/testsystem/build/testsystem $1


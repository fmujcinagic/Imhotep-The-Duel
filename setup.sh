#!/bin/bash

script_full_path=$(dirname "$0")
cd $script_full_path

git clone git@gitlab.tugraz.at:oop1_ws/ws2022/framework2022.git framework
cd framework
git checkout ass1-3
git submodule update --init --recursive

echo "Setup completed!"

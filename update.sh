#!/bin/bash

script_full_path=$(dirname "$0")
cd $script_full_path

cd framework
git pull origin ass1-3
git submodule update --init --recursive

echo "Update completed!"

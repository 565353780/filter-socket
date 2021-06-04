#!/bin/bash

Target=$1
LibDir=$PWD"/lib_"$Target

lib_array=($(ldd $Target | grep -o "/.*" | grep -o "/.*/[^[:space:]]*"))

$(mkdir $LibDir)

for Variable in ${lib_array[@]}
do
    cp "$Variable" $LibDir
done

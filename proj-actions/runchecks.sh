#!/usr/bin/env bash


folders=("include" "includes" "src" "source" "sources" "test" "tests")
for folder in "${folders[@]}"
do
    echo ${folder}
    if [[ -d ${folder} ]]; then
        command="cppcheck --language=c++ ${folder}"
        echo ${command}
        if ${command} | grep "error"; then
            echo "::error ::Review your code"
#exit 1            
        fi
    fi
done
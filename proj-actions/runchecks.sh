#!/usr/bin/env bash
folder = "src"
echo ${folder}
if [[ -d ${folder} ]]; then
    command=$(cppcheck --language=c++ --error-exitcode=1 $folder 2>&1)  #"cppcheck --language=c++ --error-exitcode=1 ${folder}"
    echo ${command}
    #if ${command} | grep "error"; then
    if [[ $command == *"error"* ]]
    then
        echo "::error ::Review your code"
        exit 1            
    fi
fi


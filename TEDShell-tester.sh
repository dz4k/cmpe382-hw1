#! /bin/bash

if ! [[ -x TEDShell ]]; then
    echo "TEDShell executable does not exist"
    exit 1
fi
echo $*
tester/run-tests.sh $*


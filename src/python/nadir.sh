#!/bin/bash

#nadir point calculation
filename=$1
nadir=`basename $filename .front`

# setting 1 to Z, plus 20% to cost and 50% to hops counting
python nadir.py $filename > $nadir.'nadir'

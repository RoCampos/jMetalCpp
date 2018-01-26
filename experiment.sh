#!/bin/bash

algorithm=$1
shift 1
arguments=$@

instances=`ls n30/*`

echo $algorithm

# mkdir $algorithm.d/

for insta in ${instances}; do
    file=`basename $insta .brite`

    # mkdir -p $algorithm.d/$file
    # touch $algorithm.d/$file/fronts
    echo $insta
    for r in `seq 5`;
    do
        # running the algorithm
        ./${algorithm} "MMRP" $insta ${arguments} - - cross 
    done
done

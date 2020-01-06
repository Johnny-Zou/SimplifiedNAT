#!/bin/bash

get_test_results() {
	result=$(diff EXPECTED OUTPUT)
	if [ "$result" != "" ]; then
		printf "...failed\n"
		printf "############ DIFF ############\n"
		printf "$result\n"
		printf "########## END DIFF ##########\n\n"
	else
		printf "...pass\n"
	fi
}

printf "Testing NAT\n"

for dir in */ ; do
    printf "    Running $dir"
    cd $dir
    rm -f OUTPUT
    ../../build/simplified_NAT NAT FLOW
    get_test_results
    cd ..
done
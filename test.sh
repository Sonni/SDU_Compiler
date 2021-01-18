#!/bin/bash
echo "+-------------+"
echo "|    Make     |"
echo "+-------------+"
make
echo "+-------------+"
echo "|    Tests    |"
echo "+-------------+"
# for file in ./tests/*/*.src
# do
# 	echo "+--------------------"
# 	echo "| TEST: "$file
# 	echo "+--------------------"
# 	./compiler -p 1 $file
# 	echo " "
# done
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'
echo "+-------------+"
echo "|  Examples   |"
echo "+-------------+"
let "passed = 0"
let "files = 0"
# Create output folder
mkdir -p out

#Create error file
echo "error" > out/error

#Only tests the folder containing the templated tests
find test/template -type f -name '*.sk' -print0  | while read -d $'\0' file; do
	
	y=${file%.*}
	if [ -f $y.out ]; then
		let "files = files + 1"
		echo -e "${BLUE}+--------------------"
		echo -e "| EXAMPLE: "$file
		echo -e "+--------------------"

		outF=out/${y##*/}.s
		outE=out/${y##*/}.a

		./compiler < $file > $outF 2>/dev/null
		let "ret = $(echo $?)"

		if [ $ret = "0" ]; then
	
			gcc -no-pie $outF -o $outE
			./$outE > out/${y##*/}.output

			if diff /dev/null "$y.out" | tail -1 | grep '^\\ No newline' > /dev/null
			then
				echo >> "$y.out"
			fi
			diff out/${y##*/}.output $y.out
			let "eq = $(echo $?)"
			#echo "eq: $(echo $?)" 
			if [ $eq = "0" ]; then
				echo -e "${GREEN}+-----"
				echo -e "| " $file " has passed!"
				echo -e "+-----"
				let "passed=passed+1"
			else 
				echo -e "${RED}+-----"
				echo -e "| " $file " has NOT passed!"
				echo -e "+-----"
			fi
		
		else
			if diff /dev/null "$y.out" | tail -1 | grep '^\\ No newline' > /dev/null
			then
					echo >> "$y.out"
			fi
			diff out/error $y.out
			let "eq = $(echo $?)"
			if [ $eq = "0" ]; then
					echo -e "${GREEN}+-----"
					echo -e "| " $file " has passed!"
					echo -e "+-----"
					let "passed=passed+1"
				else 
					echo -e "${RED}+-----"
					echo -e "| " $file " has NOT passed!"
					echo -e "+-----"
				fi

		fi
		echo $passed > out/passes
		echo $files > out/files
	fi	

done 

passed=$(head -n 1 out/passes)
files=$(head -n 1 out/files)

echo $passed "/" $files " has passed."
echo "done"

rm out/passes
rm out/files

echo -e "${NC}"
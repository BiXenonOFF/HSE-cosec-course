#!/bin/sh

counter=0 # interrupt counter
keys=""
filenames=""

readArgs()
{
	read -p "keys: " keys
	read -p "filenames: " filenames
}




sigintHandler() # SIGING Signal handler
{
	echo '' # Beatify

	counter=`expr $counter + 1`

	if [ $((counter % 3)) -eq 0 ]; then
		if [ -z "$filenames" ]; then
			echo 'No filenames provided.'
		else
			wc -l $filenames
		fi
	else
		echo Interrupt
	fi
}



# main

trap 'sigintHandler' 2 # override sigint handler
readArgs
if [ -z "$filenames" ]; then 
	ls $keys $filenames
fi

while :
do
	echo 'Enter "exit" to stop or Ctrl+C 3 times to see number of lines'
	echo 'or enter "again" to continue with new args'
	read command
	if [ "$command" = "exit" ]; then
		echo $filenames
		exit 0
	else
		if test "$command" = "again" ; then
			readArgs
			if [ -z "$filenames" ]; then
				ls $keys $filenames
			fi
		fi
	fi
done

echo $filenames

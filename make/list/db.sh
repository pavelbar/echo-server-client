#!/bin/bash



if [ -f /home/ity/BIN/echoserver ] 
then
	#File exist
	statNew=$(stat -c%x echoserver)
	statOld=$(stat -c%x /home/ity/BIN/echoserver)
	if [ "$statNew" -lt "$statOld" ] #<=
	then
		echo
	else
		
		cp echoserver /home/ity/BIN/echoserver
	fi
else
	#File does not exist
	if [ -d /home/ity/BIN/ ] 
	then
		echo
	else
		mkdir BIN
	fi
	cp echoserver /home/ity/BIN/echoserver
fi





#!/bin/sh

if [ $# = "2" ]
then {
    echo -e "Ersetze ..."
    find ./ -iname '*.[ch]'|xargs perl -pi -e "s/[^:alpha:]$1[^:alpha:]/$2/eg"
    
    echo "Done."
    }
else
    {
	echo -e "Zu wenig Argumente!"
	echo -e "USAGE: umbenennen.sh zusuchen ersetzenmit"
    
    }
fi

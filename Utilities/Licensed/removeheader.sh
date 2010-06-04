#/bin/bash
for x in $*; do
 	echo "File: $x"
	sed -i "1,41d" $x
done



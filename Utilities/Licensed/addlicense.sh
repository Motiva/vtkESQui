licensed
#/bin/bash
for x in $*; do
 echo "File: $x"
 #head -$LICENSELEN $x | diff license.txt - || echo "diff"
 #cat license.txt $x  > /tmp/file; 
 #mv /tmp/file $x
done


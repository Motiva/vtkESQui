licensed
#export LICENSELEN='wc -l license.txt | sed 's/[^0-9]//g''
find ../../ -type f -name "*cxx" -print0 | xargs -0 ./addlicense.sh
find ../../ -type f -name "*h" -print0 | xargs -0 ./addlicense.sh

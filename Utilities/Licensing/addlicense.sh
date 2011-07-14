export LICENSELEN=`wc -l license.txt | sed 's/[^0-9]//g'`
ls ../../BMM/*.cxx | xargs -0 | xargs -0 ./addheader.sh
ls ../../BMM/*.h | xargs -0 | xargs -0 ./addheader.sh
find ../../Common/ -type f -name "*.cxx" -print0 | xargs -0 ./addheader.sh
find ../../Common/ -type f -name "*.h" -print0 | xargs -0 ./addheader.sh
find ../../Examples/ -type f -name "*.cxx" -print0 | xargs -0 ./addheader.sh
find ../../Examples/ -type f -name "*.h" -print0 | xargs -0 ./addheader.sh
find ../../Haptics/ -type f -name "*.cxx" -print0 | xargs -0 ./addheader.sh
find ../../Haptics/ -type f -name "*.h" -print0 | xargs -0 ./addheader.sh
find ../../Manager/ -type f -name "*.cxx" -print0 | xargs -0 ./addheader.sh
find ../../Manager/ -type f -name "*.h" -print0 | xargs -0 ./addheader.sh
find ../../Scenario/ -type f -name "*.cxx" -print0 | xargs -0 ./addheader.sh
find ../../Scenario/ -type f -name "*.h" -print0 | xargs -0 ./addheader.sh
find ../../Utilities/ -type f -name "*.cxx" -print0 | xargs -0 ./addheader.sh
find ../../Utilities/ -type f -name "*.h" -print0 | xargs -0 ./addheader.sh


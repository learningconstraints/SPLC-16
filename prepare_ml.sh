#!/bin/sh

make -C ml_utils
./ml_utils/bin/init_csv "ml_data.csv"

if [ $# -ne 2 ] ; then
	echo "You must provide a path to data and a matching pattern."
	exit
fi

list=$(ls $1 | grep -e "$2.cfg")

for conf in $list ; do
	elem="${conf%%.*}"
	elem="$elem.avi"
	./ml_utils/bin/fill_csv "$1/$conf" "$1/$elem" "ml_data.csv"
done

rm -f ml_data.arff
./ml_utils/bin/create_arff

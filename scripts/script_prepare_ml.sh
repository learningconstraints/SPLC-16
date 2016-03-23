#!/bin/sh

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ptemple/OpenCV/openCV-3.0.0_build/lib

./prepare_output_file "data_for_ml.csv"


list=$(ls /run/media/ptemple/Back_up/MOTIV_on_igrida/generated_videos/sequences/ | grep -e "cfg")

for element in $list ; do
	echo $element

	elem="${element%%.*}"
	elem="$elem.avi"
	#echo $elem

	#echo "/run/media/ptemple/Back_up/MOTIV_on_igrida/generated_videos/sequences/$element"
	#echo "./MOTIV_GT_Char_2_vehicles.avi"
	#echo "/run/media/ptemple/Back_up/MOTIV_on_igrida/generated_videos/sequences/$elem"

	./prepare_for_ml "/run/media/ptemple/Back_up/MOTIV_on_igrida/generated_videos/sequences/$element" "./MOTIV_GT_Char_2_vehicles.avi" "/run/media/ptemple/Back_up/MOTIV_on_igrida/generated_videos/sequences/$elem"

done

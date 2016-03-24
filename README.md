# SPLC-16
Materials to reproduce SPLC'16 experiments

The method we presented in the paper aim at infering constraints to specify a generator
to comply with a specific task. For instance, we would like to derive, from a variability model, linux kernels that compile.
To do so, we propose to use Machine Learning techniques to infer constraints.
Our method is composed of different parts including configuration sampling, oracle's definition and the choice of the Machine Learning algorithm.
Finally, we applied our method to videos.

NOTE : image processing parts are all performed with the OpenCV library so be sure to have downloaded and built it before.

You will find in this file some explanations about the materials available on this github repository.

## vm_reasoner
In vm_reasoner, there is the random generator materials allowing to draw some configurations randomly in the space of possible configurations. There is also the variability model we use to derive videos.

##oracle_bad_videos
The oracle_bad_videos folder contains the code of the oracle we used (coming from Image Quality Assessment).

## prepare_output_file
The code files present in the prepare_output_file folder aim at creating a csv file. The output csv file only contains columns labels which are features coming from the configuration files plus 1 extra column which is the decision from the oracle.

## prepare_for_ml
The prepare_for_ml folder contains code to fill in the rows of the previous csv file. One row corresponds to one configuration file and every features' values are copied to the corresponding column. Then, the decision of the oracle is also copied at the end of the row.

## create_arff_file
The ML part is done by using Weka which takes formatted input files. The format is called ARFF. This folder gathers the code to convert a csv file to an arff one.

## ml_model_parser
After the learning is done and the classification model is built, constraints still need to be extracted. This is the aim of the code contained in the ml_model_parser folder.

## scripts
In order to automate a bit the process, two scripts has been written : a first one (script_prepare_ml.sh) to run the process of labeling by the oracle and the creation and filling of the csv file. The second one (run_J48.sh) allows to run the Weka J48 Decision Tree algorithm on a csv file.


From this description, the total workflow should be as follow :
- generate configurations via the VM (and generate videos out of these files)
- use the script script_prepare_ml which will try to find configuration files and corresponding videos; create a csv file and create corresponding columns with features' name; run the desired oracle and gather features' value as well as oracle's decision to the csv file
- use the create_arff_file program to create a arff_file corresponding to the csv file
- run the run_J48.sh script to create a decision model
- run the ml_model_parser code to extract constraints out of the decision model into a text file


# SPLC'16
*Materials to reproduce SPLC'16 experiments*

The method we presented in the paper aim at inferring constraints to specify a generator to comply with a given task. For instance we would like to derive, from a variability model, all Linux kernels that compile. To do so, we propose to use Machine Learning techniques to infer constraints.

Our method is composed of different parts including configuration sampling, oracle's definition and the choice of the Machine Learning algorithm. We applied it to a video generator.

### vm_reasoner
This folder contains the random generator allowing to draw some configurations randomly in the space of possible configurations. It depends on another tool to parse and exploit a variability model, which can be found [here](https://github.com/ViViD-DiverSE/VM-Source) and should be placed next to the RandomGenerator directory.

### oracle_bad_videos & oracle_heavy_videos
The two oracles we used to run our experiments. The first is coming from Image Quality Assessment and needs [OpenCV](http://opencv.org/).

### prepare_output_file & prepare_for_ml
These programs respectively create and fill a CSV file containing labels for each of the configuration features, plus one for the oracle's decision.

### create_arff_file
The ML part is done by using [Weka](http://www.cs.waikato.ac.nz/ml/weka/) which takes input files in a format called ARFF. This folder gathers the code to convert a CSV file to an ARFF one.

### ml_model_parser
After the learning is done and the classification model is built, constraints still need to be extracted. This is the aim of the code contained in the ml_model_parser folder.

### scripts
In order to automate a bit the process, two scripts have been written. A first one (script_prepare_ml.sh) runs the process of labeling by the oracle and the creation and filling of the CSV file. The second one (run_J48.sh) allows to run the Weka J48 Decision Tree algorithm on a ARFF file.

From this description, the total workflow should be as follow :
- generate configurations via the VM (and generate videos out of these files)
- use script_prepare_ml.sh which will try to find configuration files and corresponding videos; create a CSV file with corresponding columns for features' names; run the desired oracle and gather features' values as well as oracle's decisiosn to the CSV file
- use create_arff_file to create a ARFF file corresponding to the CSV file
- run run_J48.sh to create a decision model
- run ml_model_parser to extract constraints out of the decision model into a text file

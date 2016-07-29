# SPLC'16
*Materials to reproduce SPLC'16 experiments*

The method we presented in the paper aim at inferring constraints to specify a generator to comply with a given task. For instance we would like to derive, from a variability model, all Linux kernels that compile. To do so, we propose to use Machine Learning techniques to infer constraints.

Our method is composed of different parts including configuration sampling, oracle's definition and the choice of the Machine Learning algorithm. We applied it to a video generator.

### vm_reasoner
This folder contains the random generator allowing to draw some configurations randomly in the space of possible configurations. It depends on another tool to parse and exploit a variability model, which can be found [here](https://github.com/ViViD-DiverSE/VM-Source) and should be placed next to the RandomGenerator directory.

### oracles
The two oracles we used to run our experiments. The first is coming from Image Quality Assessment and needs [OpenCV](http://opencv.org/). The second one labels videos depending on their size.

### ml_utils
This folder regroups various utilities needed to format data and results (notably, extracting and parsing constraints). The global Makefile in it is where an oracle can be choosed; don't forget to recompile after any change.

### scripts
In order to automate a bit the process, three scripts have been written. A first one (prepare_ml.sh) runs the process of labeling by the oracle and the creation and filling of the CSV file. Its arguments are the path to configurations and a matching pattern for the ones that should be used for training (based on the filenames, each one being identified with an unique number). The second one (run_J48.sh) allows to run the Weka J48 Decision Tree algorithm on the pre-processed data (don't forget to modify it to get Weka path relevant with your setup). The third script (clean.sh) just deletes intermediate data and trash files.

From this description, the total workflow should be as follow :
- generate configurations via the VM (and generate videos out of these files)
- use prepare_ml.sh which will try to find configuration files and corresponding videos; create a CSV file with corresponding columns for features' names; run the desired oracle and gather features' values as well as oracle's decisiosn to the CSV file
- run run_J48.sh to create a decision model and output constraints into a text file

#!/bin/sh
java -cp ../weka-3-8-0/weka.jar weka.classifiers.trees.J48 -t ./ml_data.arff -C 0.25 -M 2 -d j48.model -g > tree.txt
./ml_utils/bin/constraint_parser tree.txt

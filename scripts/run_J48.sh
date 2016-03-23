#!/bin/sh

java -cp /usr/share/java/weka.jar weka.classifiers.trees.J48 -t ./data_old.arff -C 0.25 -M 2 -d j48.model -g > output.txt
#java -cp /usr/share/java/weka.jar weka.classifiers.trees.J48 -t ./data_train.arff -C 0.25 -M 2 -d j48.model -g > output_train.txt

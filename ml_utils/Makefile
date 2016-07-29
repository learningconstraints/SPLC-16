# The 3 following variables allow to choose an oracle before building the utilitaries.
# OPATH should contain the path to the oracle, and ODEPS his dependencies flags.
# For "bad" videos:
#OPATH = ../oracles/bad
#ODEPS = `pkg-config --cflags --libs opencv`
OPATH=../oracles/heavy
ODEPS=

OOBJS=$(shell find $(OPATH) -type f -name "*.cpp")
INC = -Iinclude
FLAGS = 

all: init_csv fill_csv create_arff constraint_parser

init_csv: init_csv.cpp
	g++ $(FLAGS) -o bin/init_csv init_csv.cpp

fill_csv: fill_csv.cpp oracle build/conf_file_parser.o
	g++ $(FLAGS) -o bin/fill_csv $(ODEPS) -I$(OPATH) $(OOBJS) $(INC) build/conf_file_parser.o fill_csv.cpp
oracle: $(OOBJS)
	$(MAKE) -C $(OPATH) all

create_arff: create_arff.cpp build/csv_to_arff.o
	g++ $(FLAGS) -o bin/create_arff $(INC) build/csv_to_arff.o create_arff.cpp

constraint_parser: constraint_parser.cpp build/constraint_simplifier.o
	g++ $(FLAGS) -o bin/constraint_parser $(INC) build/constraint_simplifier.o constraint_parser.cpp

build/%.o: src/%.cpp
	@mkdir -p build
	g++ $(FLAGS) $(INC) -c -o $@ $<

clean:
	rm -r build/ bin/*
.PHONY: clean

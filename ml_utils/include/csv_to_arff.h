#ifndef CSV_TO_ARFF_H
#define CSV_TO_ARFF_H

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class csv_to_arff
{
    public:
        csv_to_arff();
        csv_to_arff(const string input_filename, const string output_filename);
        virtual ~csv_to_arff();
        void write();
        void close();

    protected:
    private:

        ifstream input_file;
        ofstream output_file;

        void write_title();
        void write_columns();
        void write_data();
};

#endif // CSV_TO_ARFF_H

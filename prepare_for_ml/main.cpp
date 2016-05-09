#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "oracle.h"
#include "oracle_contrasts.h"
#include "oracle_contrasts_with_ref.h"

#include "conf_file_parser.h"

using namespace std;

/**
* a utility function to tell how to use the program
*/
void help()
{
    cout << "This program will prepare some data (video files and associated configuration files) to be feed to a Machine Learning algorithm." << endl;
    cout << "It will first parse a configuration to retrieve values of features in order to put it in a csv file" << endl;
    cout << "Then, this program will compute the fact that a video sequence has uniform color and/or is too noisy or too blurred." << endl;
    cout << "Because of the cost of computing such things, a discretization will be performed to avoid computing histograms and edges at each frames (every 10 frames ?)" << endl;
    cout << "It takes three inputs : " << endl;
    cout << "\t - a configuration file (complete path, filename and extension)" << endl;
    cout << "\t - a reference video file (complete path, filename and extension)" << endl;
    cout << "\t - a video file corresponding to the configuration file and to be compared to the reference (complete path, filename and extension)" << endl;
}

void assign_label(const string filename_ref, const string filename_cur, const int computation_frequency, const string output);

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        help();
        exit(1);
    }

    ///constants definition
    const string filename_conf_file = argv[1];
    const string filename_ref = argv[2];
    const string filename = argv[3];

    const string output_filename = "data_for_ml.csv";

    //for the second part of the computations
    const int freq_compute = 50;

    //parser
    conf_file_parser parser(filename_conf_file,output_filename);
    cout << "Launching parsing..." << endl;
    parser.parse_file();
    cout << "done" << endl;

    assign_label(filename_ref,filename, freq_compute, output_filename);

}

void assign_label(const string filename_ref, const string filename_cur, const int computation_frequency, const string output)
{

    // These two lines are oracle-specific
    oracle_contrasts_with_ref oracle_with_ref = oracle_contrasts_with_ref(filename_cur,filename_ref,computation_frequency);
    bool result = oracle_with_ref.decide();
    // Try this one instead to use the oracle for heavy videos:
    //bool result = OracleHeavyVids(filename, threshold).decide();

    //cout << "Processing succeeded !" << endl;
    ofstream ofs;
    ofs.open(output.c_str(), ofstream::out | ofstream::app);
    if(!ofs.is_open())
    {
        cout << "something went wrong while trying to write the result" << endl;
    }
    cout << "result is ";
    if(result == true)
    {
        cout << "not computable";
        if(ofs.is_open())
        {
            ofs << "1";
        }
    }
    else
    {
        cout << "computable";
        if(ofs.is_open())
        {
            ofs << "0";
        }
    }
    cout << endl;
    if(ofs.is_open())
        {
            ofs << "\n";
            ofs.close();
        }
}

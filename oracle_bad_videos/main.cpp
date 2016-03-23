#include <iostream>
#include <stdlib.h>

#include "oracle.h"
#include "oracle_histograms.h"
#include "oracle_sharpness.h"
#include "oracle_contrasts.h"
#include "oracle_contrasts_with_ref.h"

using namespace std;

/**
* a utility function to tell how to use the program
*/
void help()
{
    cout << "This program will compute the histogram and/or the sharpness of edges in a video sequence." << endl;
    cout << "Because of the cost of computing such things, a discretization will be performed to avoid computing histograms and edges at each frames (every 10 frames ?)" << endl;
    cout << "It takes two inputs : " << endl;
    cout << "\t - the video file path and filename" << endl;
    cout << "\t - one of the following choice : \n \t \t + 1 : compute only the histogram of the video sequence \n \t \t + 2 : compute only the sharpness of edges \n \t \t + -1 : compute both" << endl;
}

int main(int argc, char** argv)
{
    cout << "Hello world!" << endl;

    //constant
    const int choice = atoi(argv[2]);
    const int freq_compute = 50;
    const string filename = argv[1];
    const string filename_ref = argv[3];


    //oracle_histograms oracle_h;
    oracle_contrasts oracle_c;
    oracle_contrasts_with_ref oracle_with_ref;

    switch(choice)
    {
        case 1 : oracle_c = oracle_contrasts(filename,freq_compute); break;
        //case 1 : oracle_h = oracle_histograms(filename,freq_compute); break;
        case 2 : oracle_with_ref = oracle_contrasts_with_ref(filename,filename_ref,freq_compute); break;
        //case -1 : oracle_h = oracle_histograms(filename,freq_compute); oracle_s = oracle_sharpness(filename,freq_compute); break;
        //case -1 : oracle_c = oracle_contrasts(filename,freq_compute); oracle_s = oracle_contrasts_with_ref(filename, filename_ref,freq_compute); break;
        default : help(); break;
    }

    ///TODO test to know which oracle are set (method to retrieved a bool is_set where is_set returns true if the constructor has been called)
    //oracle_h.decide();
    bool result = oracle_with_ref.decide();

    //cout << "Processing succeeded !" << endl;
    cout << "result is ";
    if(result == true)
    {
        cout << "not computable";
    }
    else
    {
        cout << "computable";
    }
    cout << endl;

    return 0;
}

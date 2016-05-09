#include <iostream>
#include <stdlib.h>

#include "oracle.h"

using namespace std;

void help() {
    cout << "syntax: heavy <file> <thresh>" << endl;
    exit(1);
}

int main(int argc, char** argv)
{
    if (argc != 3) help();
    const char* filename = argv[1];
    const int threshold = atoi(argv[2]);

    cout << "result is ";
    if(OracleHeavyVids(filename, threshold).decide())
        cout << "not computable";
    else
        cout << "computable";
    cout << endl;

    return 0;
}

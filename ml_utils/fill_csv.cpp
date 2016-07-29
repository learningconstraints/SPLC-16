#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "oracle.h"
#include "conf_file_parser.h"

using namespace std;

void help() {
    cout << "fill_csv <config_path> <video_path> <out>" << endl;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        help(); // If wrong parameters
        exit(1);
    }
    const string conf_path = argv[1]; // Conf file path
    const string vid_path = argv[2]; // Video path
    ofstream out_file; // CSV output
    out_file.open(argv[3], ofstream::out|ofstream::app);
    if (!out_file.is_open()) {
        cerr << "Can't write " << argv[3] << endl;
        exit(2);
    }
    cout << "Parsing and labeling " << conf_path << endl;
    // Parse configuration file to get feature values
    conf_file_parser cfg(conf_path, argv[3]);
    cfg.parse_file();
    // Label with the oracle selected in Makefile
    bool result = Oracle(vid_path.c_str()).decide();
    if (result) {
        cout << "Not computable";
        out_file << "1";
    } else {
        cout << "Computable";
        out_file << "0";
    }
    cout << endl;
    out_file << "\n";
    return 0;
}

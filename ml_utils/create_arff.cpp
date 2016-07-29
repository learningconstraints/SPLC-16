#include <iostream>

#include "csv_to_arff.h"

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		cerr << "You must specify the input file." << endl;
		exit(1);
	}
    //cout << "Converting CSV to ARFF..." << endl;
    csv_to_arff parser(argv[1], "ml_data.arff");
    parser.write();
    parser.close();
    //cout << "Done." << endl;
    return 0;
}

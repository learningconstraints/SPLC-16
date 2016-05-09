#include "oracle.h"

#include <iostream>
#include <fstream>

using namespace std;

OracleHeavyVids::OracleHeavyVids() { }

OracleHeavyVids::OracleHeavyVids(const char* fname, int threshold) {
    ifstream in(fname, ifstream::ate | ifstream::binary);
    heavy = in.tellg() > threshold; 
}

bool OracleHeavyVids::decide() {
	return heavy;
}

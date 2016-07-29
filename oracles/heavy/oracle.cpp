#include <iostream>
#include <fstream>

#include "oracle.h"

using namespace std;

Oracle::Oracle() { }

Oracle::Oracle(const char* video_path) {
    ifstream vid(video_path, ifstream::ate | ifstream::binary);
    heavy = vid.tellg() > THRESHOLD; // 1 if vid>thresh, 0 if not
}

bool Oracle::decide() {
	return heavy;
}

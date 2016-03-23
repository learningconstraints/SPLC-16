#include "oracle_sharpness.h"

oracle_sharpness::oracle_sharpness()
{
    //ctor
}

oracle_sharpness::oracle_sharpness(string fname, int f)
:oracle(fname, f)
{

}

oracle_sharpness::~oracle_sharpness()
{
    //dtor
}

bool oracle_sharpness::decide()
{
    cout << "METHOD TO WRITE" << endl;
    return true;
}

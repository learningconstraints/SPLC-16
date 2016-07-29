#include "abs_oracle.h"

/**
* constructors
*/

//default constructor : nothing to do
oracle::oracle()
{
    //ctor
}

/**
* constructor taking two parameters
* string name : a string representing the filename of the video to be computed
* int f : the "frequency" of images that will be computed -> number of frames between two frames computed
*/
oracle::oracle(string name, int f)
: filename(name), vid(filename)
{
    freq = f;
}

/**
* destructor
*/
oracle::~oracle()
{
    //dtor
}



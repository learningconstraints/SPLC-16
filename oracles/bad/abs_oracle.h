#ifndef ABS_ORACLE_H
#define ABS_ORACLE_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

/**
* a class representing a generic interface to
* tell if a video can be computed by a Computer Vision
* algorithm or not.
* Please note that attributes are protected in order to
* be used by classes that inherited from the class
*/
class oracle
{
    public:
        oracle();
        oracle(string name, int f=10);
        virtual ~oracle();
        virtual bool decide(double& val=-1) = 0;
    protected:

    string filename;
    VideoCapture vid;
    int freq;

    private:

};

#endif // ABS_ORACLE_H

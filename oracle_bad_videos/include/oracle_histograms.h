#ifndef ORACLE_HISTOGRAMS_H
#define ORACLE_HISTOGRAMS_H

#include <oracle.h>
#include <opencv/cv.h>


class oracle_histograms : public oracle
{
    public:
        oracle_histograms();
        oracle_histograms(string fname, int f=10);
        virtual ~oracle_histograms();
        bool decide();
    protected:
    private:

    void compute_histogram(Mat& img);
    void display_histogram(Mat& histo, int histSize);
    void display_histogram(int histo[], int histSize);
};

#endif // ORACLE_HISTOGRAMS_H

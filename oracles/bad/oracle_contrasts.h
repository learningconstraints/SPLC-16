#ifndef ORACLE_CONTRASTS_H
#define ORACLE_CONTRASTS_H

#include <abs_oracle.h>
#include <math.h>

/**
* A class that allow to decide whether a video is computable or not
* This class uses OpenCV algorithms and the decision algorithm
* is the one presented in the article :
* No reference noise and blur detection via fourier transform
* from Richard Diosselmann and Xue Dong Yang in 2012
*/


class oracle_contrasts : public oracle
{
    public:
        //constructors
        oracle_contrasts();
        oracle_contrasts(string fname,int f=10);
        //destructor
        virtual ~oracle_contrasts();
        //decide whether a video is not computable or it is
        bool decide();
    protected:
        //openCV DFT of an image
        bool compute_fourier_transform(Mat& img);
        //normalize and rearrange fourrier coefficient to display DFT image
        void rearrange_fourier_coeff(Mat& fourierImg);
        //compute rings from the article
        void compute_rings(const Mat& fourier_coeff, vector<double>& cumul_distrib, const int min=10, const int delta=1);
        //compute recursively the cumulative distribution function
        void compute_cumulative(const Mat& fourierImg, const int number_of_rings, int min_rings, Mat& fourierMask, vector<double>& cumul_distrib, double nb_freq);
        //normalize and assess that the image is not uniform
        bool normalize_distrib(vector<double>& cumul_distrib);
        //decide whether the image is blury or noisy
        bool decision(double val);


        //display methods
        void display_fourier_transform(Mat& fourierImg);
        //display the histogram
        bool display_cumulative(vector<double>& cumul_distrib);

        //compute delta between bins
        double prepare_decision(vector<double>& cumul_distrib);
        //compute delta of reference (diagonal of the representation)
        double compute_value_ref(vector<double>& cumul_distrib, int index);
    private:
};

#endif // ORACLE_CONTRASTS_H

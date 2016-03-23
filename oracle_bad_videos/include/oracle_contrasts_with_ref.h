#ifndef ORACLE_CONTRASTS_WITH_REF_H
#define ORACLE_CONTRASTS_WITH_REF_H

#include <oracle_contrasts.h>


class oracle_contrasts_with_ref : public oracle_contrasts
{
    public:
        oracle_contrasts_with_ref();
        oracle_contrasts_with_ref(string fname, string fname_ref, int f=10);

        virtual ~oracle_contrasts_with_ref();

        //decide whether a video is not computable or it is
        bool decide();
    protected:
    private:

    string filename_reference;
    VideoCapture vid_reference;

    //openCV DFT of an image
    Mat compute_fourier_transform(Mat& img);
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
    double prepare_decision(vector<double>& cumul_distrib, vector<double>& cumul_distrib_ref);
    //compute delta of reference (diagonal of the representation)
    double compute_value_ref(vector<double>& cumul_distrib, int index);
};

#endif // ORACLE_CONTRASTS_WITH_REF_H

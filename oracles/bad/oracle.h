#ifndef ORACLE_H
#define ORACLE_H

#include <oracle_contrasts.h>

#define FREQ 50
#define REF "/temp_dd/igrida-fs1/lnoelbar/dataset/sequences_gen_script_modified/MOTIV_GT_Char_00000001.avi"

class Oracle : public oracle_contrasts
{
    public:
        Oracle();
        Oracle(const char*);

        virtual ~Oracle();

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
    //compute recursively the cumulative distribution function
    void compute_cumulative(const Mat& fourierImg, const int number_of_rings, int min_rings, Mat& fourierMask, vector<double>& cumul_distrib, double nb_freq);
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

#endif // ORACLE_H

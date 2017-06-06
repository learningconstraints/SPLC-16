#include "oracle.h"

Oracle::Oracle() { }

Oracle::Oracle(const char* fname)
:oracle_contrasts(fname,FREQ),filename_reference(REF),vid_reference(filename_reference) { }

Oracle::~Oracle() { }

bool Oracle::decide(double& val) {
    unsigned int nb_frame=0;
    unsigned int nb_not_computable = 0;
    bool isNotComputable = false; //final result to decide
    Mat tmp, tmp_ref, img, img_ref;
    if(!vid.isOpened())
        vid.open(filename);
    if(!vid_reference.isOpened())
        vid_reference.open(filename_reference);
    vid >> img;
    vid_reference >> img_ref;
    while(img.data && img_ref.data) {
        //compute only some images according to the argument given in the constructor
        if(nb_frame%freq == 0) {
            ///begin the computation from the paper
            tmp = compute_fourier_transform(img);
            tmp_ref = compute_fourier_transform(img_ref);
            ///compute diff
            vector<double> vec_frequencies;
            vector<double> vec_frequencies_ref;
            compute_rings(tmp,vec_frequencies);
            compute_rings(tmp_ref,vec_frequencies_ref);
            bool isNotZero = normalize_distrib(vec_frequencies);
            normalize_distrib(vec_frequencies_ref);
            if(!isNotZero)
                nb_not_computable++;
            else {
                ///compute the deviation between the diagonal of the cumulated distribution function and the actual value
                double val = prepare_decision(vec_frequencies,vec_frequencies_ref);
                if(!decision(val))
                    nb_not_computable++;
            }
        }
        //next step in the loop
        vid >> img;
        vid_reference >> img_ref;
        nb_frame++;
    }
    vid.release();
    vid_reference.release();
    double nb_computed = ceil((double)(nb_frame)/freq);
    
    //to get a float value about "how well" the video is "computable"
    val  = nb_not_computable/nb_computed;
    
    if( val >= 0.5)
        isNotComputable = true;
    //return whether the video is computable or not
    return isNotComputable;
}

Mat Oracle::compute_fourier_transform(Mat& img) {
    Mat tmp_img;
    ///prepare size of fourier transform image
    int m = getOptimalDFTSize(img.rows);
    int n = getOptimalDFTSize(img.cols);
    copyMakeBorder(img,tmp_img,0, m - img.rows, 0, n - img.cols, BORDER_CONSTANT, Scalar::all(0));
    ///change from color to grayscale
    cvtColor(tmp_img,tmp_img,CV_BGR2GRAY);
    ///prepare 2 plans of the fourier transform: R + Img
    ///the encoding of the planes are float -> visible values are between 0 and 1
    Mat planes[2];
    tmp_img.convertTo(planes[0],CV_32FC1);
    planes[1] = Mat::zeros(tmp_img.size(),CV_32FC1);
    ///a complex image that merges the two planes
    Mat complexImg;
    merge(planes,2,complexImg);
    complexImg.convertTo(complexImg,CV_32FC1);
    ///openCV DFT
    dft(complexImg,complexImg);
    ///output fourier images and split in different plans
    //compute magnitude
    split(complexImg,planes);
    magnitude(planes[0],planes[1],planes[0]);
    Mat fourierImg = planes[0];
    return fourierImg;
}

/**
* a function to compute the final steps of the paper.
* It computes the delta between the diagonal of the cumulative distribution function
* and the actual line formed earlier
* the parameter is a vector representing the cumulated distribution function of coefficients normalized
* it returns the delta between the diagonal of the cumulative distribution function
* and the actual line formed earlier in a normalized fashion
*/
double Oracle::prepare_decision(vector<double>& cumul_distrib, vector<double>& cumul_distrib_ref)
{
    //the difference between diagonal and line
    double diff = 0;
    //the total number of coefficient in order to normalize
    double total = 0;
    int histSize = cumul_distrib.size();
    //window histogram size
    int hist_w = histSize;
    int hist_h = histSize;
    //width of a bin
    //creation of corresponding matrix (representing the final image)
    Mat histImage2(hist_h, hist_w, CV_8UC1, Scalar(0,0,0));
    Mat histImage2_ref(hist_h, hist_w, CV_8UC1, Scalar(0,0,0));
    //draw lines between bins
    ///for each bin, compute the delta between the diagonal and the line
    for(unsigned int index = 1; index < cumul_distrib.size(); index++) {
        //compute the inclination of the diagonal and the position of the diagonal w.r.t. the bin
        double inclination_ref = compute_value_ref(cumul_distrib_ref, index);
        //compute delta between the two measures
        //diff += (cumul_distrib[cumul_distrib.size() - index] - inclination_ref);
        diff += cumul_distrib[index] - inclination_ref;
        total += cumul_distrib[cumul_distrib.size() - index];
    }
    return diff/total;
}

/**
* a utility function to compute the inclination of the diagonal
*/
double Oracle::compute_value_ref(vector<double>& cumul_distrib, int index)
{
    //given the size of the image (size of the vector as image is squared)
    //compute, between two bins, what should be the diagonale of the image
    return cumul_distrib[index];

}

bool Oracle::decision(double val) {
    return -0.1 <= val && val <= 0.1;
}

#include "oracle_contrasts.h"

///constructors
//default constructor : nothing to do
oracle_contrasts::oracle_contrasts()
{
    //ctor
}

//default used
// take an input file (a video) and a frequency to perform computations
oracle_contrasts::oracle_contrasts(string name, int f)
:oracle(name,f)
{
    //ctor
}

oracle_contrasts::~oracle_contrasts()
{
    //dtor
}

/**
* the main method of the class
* tell if the given video is too blurred or too noisy
* it uses the functions from OpenCV library
* return true if the video is ok, false otherwise
* A video is considered non-computable if at least 1 frame of 10 is not computable (arbitrary threshold)
*/
bool oracle_contrasts::decide()
{

    //the number of frame retrieved (to know which ones are to be computed)
    unsigned int nb_frame=0;
    unsigned int nb_not_computable = 0;
    //final result to decide
    bool isNotComputable = false;
    //result of every computed frame
    bool tmp = false;

    //retrieve image from video file
    Mat img;
    if(!vid.isOpened())
    {
        vid.open(filename);
    }
    vid >> img;

    //while there are still images left in the video stream
    while(img.data)
    {
        /*//@test to comply with the paper
        Size size(256,256);
        resize(img,img,size);*/

        //compute only some images according to the argument given in the constructor
        if(nb_frame%freq == 0)
        {
            //@debug
            /*imshow("image before processing",img);
            cout << "about to compute histogram" << endl;*/

            ///begin the computation from the paper
            tmp = compute_fourier_transform(img);
            /*if(!tmp)
            {
                cout << "false" << "\t"<< false << endl;
            }
            else
            {
                cout << "true" << "\t"<< true << endl;
            }*/
            ///update the final result
            //isNotComputable = isNotComputable && tmp;
            if(!tmp)
            {
                nb_not_computable++;
            }

            //@debug
            //cout << "finish histogram" << endl;
        }
        //next step in the loop
        vid >> img;
        nb_frame++;
    }
    vid.release();

    double nb_computed = ceil((double)(nb_frame)/freq);
    cout << (nb_not_computable/ nb_computed) << endl;
    cout << nb_computed << "\t" << ceil((double)(nb_frame)/freq) << "\t" << nb_not_computable << endl;
    if( (nb_not_computable/nb_computed) >= 0.5)
    {
        isNotComputable = true;
    }
    //return whether the video is computable or not
    return isNotComputable;
}

/**
* a method to compute the DFT (as implemented in OpenCV) coefficient of an image
* from this transformation, the program has to be able to tell if the image from the video
* is computable or not
*/
bool oracle_contrasts::compute_fourier_transform(Mat& img)
{
    Mat tmp_img;

    ///prepare size of fourier transform image
    int m = getOptimalDFTSize(img.rows);
    int n = getOptimalDFTSize(img.cols);
    copyMakeBorder(img,tmp_img,0, m - img.rows, 0, n - img.cols, BORDER_CONSTANT, Scalar::all(0));

    /*//@debug
    cout << "preparing size img --------> Done" << endl;
    cout << "converting to grayscale" << endl;*/

    ///change from color to grayscale
    cvtColor(tmp_img,tmp_img,CV_BGR2GRAY);


    /*//@debug
    cout << "convertion done" << endl;
    cout << "try" << endl;*/

    ///prepare 2 plans of the fourier transform: R + Img
    ///the encoding of the planes are float -> visible values are between 0 and 1
    Mat planes[2];
    tmp_img.convertTo(planes[0],CV_32FC1);
    planes[1] = Mat::zeros(tmp_img.size(),CV_32FC1);

    /*//@debug
    cout << "done" << endl;
    cout << "planes created" << endl;*/

    ///a complex image that merges the two planes
    Mat complexImg;
    merge(planes,2,complexImg);
    complexImg.convertTo(complexImg,CV_32FC1);

    //@debug
    /*cout << "merging done" << endl;
    cout << "preparing planes img --------> Done" << endl;

    ///for debugging, in case one time the depth is not the same
    if(complexImg.type()==CV_32FC1)
    {
        cout << "true" << endl;
    }
    else
    {
        cout << "false" << endl;
        cout << complexImg.type() << endl;
        cout << CV_32FC1 << endl;
        cout << "expected 5 (depth()) or 29 (type())" << endl;
    }*/

    ///openCV DFT
    dft(complexImg,complexImg);

    /*//@debug
    cout << "compute DFT --------> Done" << endl;*/

    ///output fourier images and split in different plans
    //compute magnitude
    split(complexImg,planes);
    magnitude(planes[0],planes[1],planes[0]);
    Mat fourierImg = planes[0];

    /*//@debug
    imshow("DFT avant",fourierImg);
    waitKey(0);*/

    /*//@debug
    imshow("DFT log",fourierImg);
    waitKey(0);*/

    ///display
    display_fourier_transform(fourierImg);

    /*//@debug
    imshow("DFT2",fourierImg);
    waitKey(0);*/

    /*//@debug
    ///inverse DFT (to be sure that the DFT image is the right one)
    Mat inverseT;
    dft(complexImg,inverseT, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
    normalize(inverseT, inverseT, 0, 1, CV_MINMAX);
    imshow("iDFT", inverseT);
    waitKey(0);*/

    ///paper technical report 2012
    vector<double> vec_frequencies;
    compute_rings(fourierImg,vec_frequencies);

    bool isNotZero = normalize_distrib(vec_frequencies);

    ///compute the deviation between the diagonal of the cumulated distribution function and the actual value
    double val = prepare_decision(vec_frequencies);

    //cout << "returned value : " << val << endl;

    bool isComputable = decision(val);

    return isComputable && isNotZero;

}

/**
* a function to prepare and display the fourier transform image.
* the argument is an fourier image
* the function rearrange the different quadrants of the image
*/
void oracle_contrasts::display_fourier_transform(Mat& fourierImg)
{

    //crop the spectrum if it has an odd number of rows or columns
    fourierImg = fourierImg(Rect(0,0,fourierImg.cols & -2, fourierImg.rows & -2));

    //move different quadrants
    rearrange_fourier_coeff(fourierImg);

    //switch to logarithmic scale
    fourierImg += Scalar::all(1);
    log(fourierImg,fourierImg);

    //cout << fourierImg << endl;

    //normalize into a viewable image
    //since the image is encoding using floats, values that can be visible are between 0 and 1
    normalize(fourierImg,fourierImg,0,1,CV_MINMAX);

    //@debug
    ///the resulting images seems to be a bit bright...
    imshow("DFT",fourierImg);
    //waitKey(0);


}

/**
* a utility function to rearrange fourier coefficients of an image
* due to implicit symmetry, fourier coefficient of an image must be
* exchanged in order to have low frequencies at the center of the image.
* input : an matrix representing an image of fourier coefficients
* the image given is a reference, so it will be modified
*/
void oracle_contrasts::rearrange_fourier_coeff(Mat& fourierImg)
{
    ///rearrange quadrants so that the origin is at the center of the image

    //define image center
    int cx = fourierImg.cols/2;
    int cy = fourierImg.rows/2;

    //define quadrants
    Mat q0(fourierImg,Rect(0, 0, cx, cy));
    Mat q1(fourierImg,Rect(cx, 0, cx, cy));
    Mat q2(fourierImg,Rect(0, cy, cx, cy));
    Mat q3(fourierImg,Rect(cx, cy, cx, cy));

    //swap quadrants
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

/**
* the first method allowing to reproduce the proposed method of the paper :
* No reference noise and blur detection via fourier transform
* from Richard Diosselmann and Xue Dong Yang in 2012
* it will first compute successive rings over the fourier coefficient image
* then it will compute a distribution function over those rings in order to see
* how the coefficient are spread when going away from the center of the image
* the first parameter is a matrix representing fourier coefficient of an image;
* the second is a vector representing the succession of the cumulative distribution function
* the third is the distance (in pixels) where rings begin to be computed
* the last is an integer representing the distance (in pixels) between two rings
*/
void oracle_contrasts::compute_rings(const Mat& fourier_coeff, vector<double>& cumul_distrib, const int min_rings, const int delta)
{

    //retrieve minimum between height and width
    int height = fourier_coeff.rows;
    int width = fourier_coeff.cols;

    //maximum number of rings that can be done
    int number_of_rings;
    if(height < width)
    {
        number_of_rings = height/2;
    }
    else
    {
        number_of_rings = width/2;
    }

    //cout << "entering recursivity" << endl;
    Mat test(fourier_coeff.size(),fourier_coeff.type());
    //be sure that the matrix is well-initialized to zero
    test.setTo(Scalar(0,0,0));

    //number of non-zero frequency in the fourier coefficient image
    //int nb_freq = 0;
    double nb_freq = 0;

    //compute the cumulative function (cumulate rings)
    compute_cumulative(fourier_coeff, number_of_rings, min_rings, test, cumul_distrib, nb_freq);

}


/**
* a function to compute the cumulative distribution function based on rings
* the function is recursive since the process is the same only the ring changes
* the first parameter is an image of the fourier coefficient;
* the second is the maximum number of rings to compute;
* the third is the actual number of rings;
* another image representing the fourier coefficient masked with the current ring;
* the vector cumulating values
* the number of non-zero frequencies for the whole process (use to normalize)
*/
void oracle_contrasts::compute_cumulative(const Mat& fourierImg, const int number_of_rings, int nb_rings, Mat& fourierMasked, vector<double>& cumul_distrib, double nb_freq)
{
    /*//@debug
    cout << number_of_rings << "\t" << nb_rings << endl;*/

    ///loop while the current number of rings is less than the total number of rings to compute
    if(nb_rings < number_of_rings)
    {

        ///compute a mask image where 0 is the background and 255 (white) is the frequencies that are considered
        Mat fourierMask(fourierImg.size(),CV_8UC1);
        fourierMask.setTo(Scalar(0,0,0));

        ///compute mask thanks to a circle (an ellipse could be better when dealing with rectangular images)
        circle (fourierMask,Point(fourierImg.cols/2,fourierImg.rows/2),nb_rings,Scalar(255,255,255),1,CV_AA,0);
        //ellipse(fourierMask,Point(fourierImg.cols/2,fourierImg.rows/2), Size(fourierImg.cols/2,fourierImg.rows/2),360,0,360,Scalar(255,255,255),1,CV_AA,0);

        ///copy foreground on mask
        fourierImg.copyTo(fourierMasked,fourierMask);

        /*//@debug
        imshow("Test mask", fourierMask);
        waitKey(0);*/

        /*imshow("Test recursivity", fourierMasked);
        waitKey(0);*/

        ///add the cumulative frequencies
        SparseMat sparse(fourierMasked);
        SparseMatConstIterator_<float> end = sparse.end<float>();

        for (SparseMatConstIterator_<float> it = sparse.begin<float>(); it != end; ++it)
        {
            nb_freq += it.value<float>();
            //nb_freq += *it;
        }


        /*//@debug
        imshow("Test recursivity", fourierMasked);
        waitKey(0);*/

        ///compute the number of frequency
        ///add number of frequency to the vector
        //nb_freq += countNonZero(fourierMasked);
        cumul_distrib.push_back(nb_freq);

        //recursivity
        compute_cumulative(fourierImg, number_of_rings, ++nb_rings, fourierMask, cumul_distrib,nb_freq);
    }

}

bool oracle_contrasts::normalize_distrib(vector<double>& cumul_distrib)
{
    ///normalize after verifying in the paper
    double cumul = 0;
    for(unsigned int i = 0; i < cumul_distrib.size(); i++)
    {
        cumul += cumul_distrib[i];
    }
    for(unsigned int i = 0; i < cumul_distrib.size(); i++)
    {
        cumul_distrib[i] /= cumul;
    }

    ///verify the evolution of rings and there are coefficient responses (not totally uniform images)
    bool isNotZero = display_cumulative(cumul_distrib);
    return isNotZero;
}

/**
* a function to display the cumulative function computed earlier
* the parameter is the vector represented the cumulated distribution
* it returns whether the image was uniform or not
*/
bool oracle_contrasts::display_cumulative(vector<double>& cumul_distrib)
{
    ///compute histogram size
    int histSize = cumul_distrib.size();

    ///normalize values to fit in window
    double min = cumul_distrib[0];
    double max = cumul_distrib[cumul_distrib.size()-1] - min;

    //if max == 0 then there were no frequencies and thus the image was uniform
    if(max == 0)
    {
        return false;
    }

    //normalization (between [0;histSize]) over the vector
    for(int i =0; i< histSize; i++)
    {
        cumul_distrib[i] -= min;
        cumul_distrib[i] /= max;
        cumul_distrib[i] *= histSize;
        //cout << cumul_distrib[i] << endl;
    }

    ///display by begining to iterate over the vector in reverse order
    //@debug
    //cout << histSize << endl;

    //window histogram size
    int hist_w = histSize;
    int hist_h = histSize;
    //width of a bin
    int bin_w = cvRound( (double) hist_w/histSize);

    //creation of corresponding matrix (representing the final image)
    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0,0,0));
    //draw lines between bins
    for(int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w*(i-1), hist_h - cvRound(cumul_distrib[histSize - i])), Point(bin_w*(i), hist_h - cvRound(cumul_distrib[histSize - i-1])), Scalar(255,255,255), 1, 8, 0);
    }

    /*//@debug
    //dislpay
    namedWindow("histogram result",CV_WINDOW_AUTOSIZE);
    imshow("histogram result", histImage);
    waitKey(0);
    */
    //cvDestroyWindow("histogram result");
    return true;
}

/**
* a function to compute the final steps of the paper.
* It computes the delta between the diagonal of the cumulative distribution function
* and the actual line formed earlier
* the parameter is a vector representing the cumulated distribution function of coefficients normalized
* it returns the delta between the diagonal of the cumulative distribution function
* and the actual line formed earlier in a normalized fashion
*/
double oracle_contrasts::prepare_decision(vector<double>& cumul_distrib)
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
    int bin_w = cvRound( (double) hist_w/histSize);

    //creation of corresponding matrix (representing the final image)
    Mat histImage2(hist_h, hist_w, CV_8UC1, Scalar(0,0,0));
    //draw lines between bins

    double inclination_prec =0;
    ///for each bin, compute the delta between the diagonal and the line
    for(unsigned int index = 1; index < cumul_distrib.size(); index++)
    {
        //compute the inclination of the diagonal and the position of the diagonal w.r.t. the bin
        double inclination_ref = compute_value_ref(cumul_distrib, index);

        //compute delta between the two measures
        //diff += (cumul_distrib[cumul_distrib.size() - index] - inclination_ref);
        diff += cumul_distrib[index] - inclination_ref;
        total += cumul_distrib[cumul_distrib.size() - index];

        line(histImage2, Point(bin_w*(index-1), cvRound(inclination_prec)), Point(bin_w*(index), cvRound(inclination_ref)), Scalar(255,255,255), 1, 8, 0);
        /*cout <<bin_w*(index-1) << "\t " <<  cvRound(inclination_prec) << "\t " <<  bin_w*(index) << "\t " <<  cvRound(inclination_ref) << endl;
        cout << cumul_distrib[index] << "\t" << inclination_ref << "\t" << cumul_distrib[index] - inclination_ref << endl;*/
        inclination_prec = inclination_ref;
    }

    /*//@debug
    //dislpay
    namedWindow("histogram diag",CV_WINDOW_AUTOSIZE);
    imshow("histogram diag", histImage2);
    //waitKey(0);*/

    return diff/total;
}

/**
* a utility function to compute the inclination of the diagonal
*/
double oracle_contrasts::compute_value_ref(vector<double>& cumul_distrib, int index)
{
    //given the size of the image (size of the vector as image is squared)
    //compute, between two bins, what should be the diagonale of the image
    return ( ( ( ( cumul_distrib[cumul_distrib.size()-1] - cumul_distrib[0]) / cumul_distrib.size() ) * index) );

}

bool oracle_contrasts::decision(double val)
{
    ///decide whether computable or not
    bool isNotBlurredNorNoisy = (val <= -0.375&& val >= -0.385)? true:false;
    return isNotBlurredNorNoisy;
}

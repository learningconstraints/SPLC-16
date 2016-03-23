#include "oracle_histograms.h"

oracle_histograms::oracle_histograms()
{
    //ctor
}

oracle_histograms::oracle_histograms(string fname, int f)
:oracle(fname, f)
{
}

oracle_histograms::~oracle_histograms()
{
    //dtor
}

bool oracle_histograms::decide()
{
    Mat img;
    if(!vid.isOpened())
    {
        vid.open(filename);
    }

    unsigned int nb_frame=1;

    vid >> img;
    while(img.data)
    {
        if(nb_frame%freq == 0)
        {
            cout << "about to compute histogram" << endl;
            compute_histogram(img);
            cout << "finish histogram" << endl;
        }
        vid >> img;
        nb_frame++;
    }
    vid.release();

    return true;
}

/**
* a method to compute the histogram of an image.
* The histogram will be compute on the grayscale version of the image.
*/
void oracle_histograms::compute_histogram(Mat& img)
{
    Mat img_gray;
    cvtColor(img,img_gray,CV_BGR2GRAY);

    imshow("img grayscale",img_gray);
    waitKey(0);
    cvDestroyWindow("img grayscale");

    //array representing the histogram
    int histogram[256];

    //initialize to 0 every bin of the histogram
    for(int i=0; i< 256; i++)
    {
        histogram[i] = 0;
    }

    //compute the histogram
    for(int i =0; i< img_gray.rows; i++)
    {
        for(int j=0; j< img_gray.cols; j++)
        {
            histogram[(int) img_gray.at<uchar>(i,j)]++;
        }
    }

    for(int i = 0; i< 256; i++)
    {
        cout << histogram[i] << " ";
    }
    cout << endl;


    display_histogram(histogram,256);

}

void oracle_histograms::display_histogram(int histo[], int histSize)
{
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound((double) hist_w/histSize);

    Mat img_hist(hist_h,hist_w, CV_8UC3, Scalar(0,0,0));

    //find max intensity
    int max = histo[0];
    for(int i =1; i< histSize; i++)
    {
        if(max < histo[i])
        {
            max = histo[i];
        }
    }

    //normalize
    for(int i=0; i< histSize; i++)
    {
        histo[i] = ((double) histo[i]/max)*img_hist.rows;
    }

    cout << "after normalization" << endl;
    for(int i = 0; i< 256; i++)
    {
        cout << histo[i] << " ";
    }
    cout << endl;

    //draw the histogram
    for(int i = 0; i< histSize; i++)
    {
        line(img_hist, Point(bin_w*i, hist_h), Point(bin_w*i, hist_h-histo[i]), Scalar(0,0,255), 1, 8, 0);
    }

    namedWindow("histogram",CV_WINDOW_AUTOSIZE);
    imshow("histogram",img_hist);
    waitKey(0);

}

void oracle_histograms::display_histogram(Mat& histo, int histSize)
{

    cout << "HIST SIZE !!!!! ------> " << histSize << endl;
    //draw the histogram for RGB
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize);

    Mat hist_img(hist_h, hist_w, CV_8UC3, Scalar(0,0,0) );
    Mat cumul_img(hist_h, hist_w, CV_8UC3, Scalar(0,0,0) );

    //normalize the result to [0, histImg.rows]
    normalize(histo, histo, 0, hist_img.rows, NORM_MINMAX, -1, Mat() );

    double cumul_prev = 0, cumul_cur = histo.at<float>(0);

    cout << histo.at<float>(0) << endl;
    cout << cvRound(histo.at<float>(0)) << endl;

    //draw a line
    for(int i=1; i < histSize; i++)
    {
        cout << histo.at<float>(i) << endl;
        cout << cvRound(histo.at<float>(i)) << endl;

        line( hist_img, Point(bin_w * (i-1), cvRound(histo.at<float>(i-1)) ) , Point( bin_w * i, cvRound(histo.at<float>(i)) ), Scalar(0,0,255), 1, 8, 0);
        line( cumul_img, Point(bin_w * (i-1), cumul_prev), Point( bin_w *i, cumul_cur), Scalar(0,255,0), 1, 8 ,0);
        cumul_prev = cumul_cur;
        cumul_cur += histo.at<float>(i);

    }

    for(int i =1; i< histSize; i++)
    {

        cout << histo.at<float>(i) << endl;
        cout << cvRound(histo.at<float>(i)) << endl;

        line(hist_img, Point(bin_w * (i-1),hist_h - cvRound(histo.at<float>(i-1))), Point(bin_w*i,hist_h - cvRound(histo.at<float>(i))), Scalar(0,255,0),1,8,0);
    }

    namedWindow("histo",CV_WINDOW_AUTOSIZE);
    namedWindow("cumul",CV_WINDOW_AUTOSIZE);

    imshow("histo",hist_img);
    imshow("cumul",cumul_img);

    waitKey(0);
    cvDestroyAllWindows();
}

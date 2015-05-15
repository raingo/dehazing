#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

#include "include/dark_channel.hpp"
#include "include/estimate_AL.hpp"
#include "include/estimate_transmission.hpp"
#include "include/soft_matting.hpp"
#include "include/dehazing.hpp"
#include "guided-filter/guidedfilter.h"

int main(int argc, char *argv[])
{
    Mat haze_img;

    if (argc == 2) {
        haze_img = imread(argv[1], CV_LOAD_IMAGE_COLOR);
        ////namedWindow("haze image", CV_WINDOW_AUTOSIZE);
        ////imshow("haze image", haze_img);
    } else {
        cout << "Usage: ./dehaze filename" << endl;
        return -1;
    }

    Mat dark_channel = Mat::zeros(haze_img.size(), CV_8UC1);
    Mat transmission = Mat::zeros(haze_img.size(), CV_8UC1);
    Mat op_transmission = Mat::zeros(haze_img.size(), CV_8UC1);
    Mat dehaze_img = Mat::zeros(haze_img.size(), haze_img.type());

    // get dark channel of hazy image
    get_dark_channel(haze_img, dark_channel);

    // atmospheric light A
    vector<Mat> layers;
    split(haze_img, layers);
#if 1
    Scalar A = estimate_AL(layers[0], layers[1], layers[2]);
#else
    Scalar A = estimate_AL_v2(layers[0], layers[1], layers[2], dark_channel);
#endif

    // get transmission diagram
    estimate_transmission(dark_channel, transmission, A);

    int r = 60;
    double eps = 1e-6;
    eps *= 255 * 255;

    // refine tansmission image
    op_transmission = guidedFilter(haze_img, transmission, r, eps);
    //solve_laplacian_matrix(haze_img, transmission, op_transmission, 3);

    // get haze free image
    dehazing(haze_img, op_transmission, dehaze_img, A);

    ////namedWindow("haze image", CV_WINDOW_AUTOSIZE);
    ////imshow("haze image", haze_img);

    ////namedWindow("dark channel", CV_WINDOW_AUTOSIZE);
    //imshow("dark channel", dark_channel);
    //imwrite("result/dark_channel.png", dark_channel);

    //namedWindow("transmission image", CV_WINDOW_AUTOSIZE);
    //imshow("transmission image", transmission);
    imwrite("result/t.png", transmission);

    //namedWindow("refine transmission image", CV_WINDOW_AUTOSIZE);
    //imshow("refine transmission image", op_transmission);
    imwrite("result/refine_t.png", op_transmission);

    //namedWindow("haze free image", CV_WINDOW_AUTOSIZE);
    //imshow("haze free image", dehaze_img);
    imwrite("result/dehaze.png", dehaze_img);

    //waitKey();

    return 0;
}

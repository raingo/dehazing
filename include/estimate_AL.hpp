#ifndef ESTIMATE_AL_HPP
#define ESTIMATE_AL_HPP

/*
 * estimating the atmospheric light of each channel
 */

Scalar estimate_AL(Mat &img_b, Mat &img_g, Mat &img_r, const Mat &mask = Mat())
{
    double min_pixel = 0;
    double AL_B = 0, AL_G = 0, AL_R = 0;

    minMaxLoc(img_b, &min_pixel, &AL_B, NULL, NULL, mask);
    minMaxLoc(img_g, &min_pixel, &AL_G, NULL, NULL, mask);
    minMaxLoc(img_r, &min_pixel, &AL_R, NULL, NULL, mask);

    return Scalar(AL_B, AL_G, AL_R);
}

uchar get_top_pixels(const Mat &input, float ratio)
{
    float thre = input.rows * input.cols * ratio;
    Mat b_hist;
    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    calcHist(&input, 1, NULL, Mat(), b_hist, 1, &histSize, &histRange, true, false);
    int i;
    float *p = b_hist.ptr<float>(0);
    for (i = 1; i < b_hist.rows; i++) {
        p[i] = p[i] + p[i-1];
        if (p[i] > thre)
            break;
    }
    return uchar(i);
}

Scalar estimate_AL_v2(Mat &img_b, Mat &img_g, Mat &img_r, Mat &dark_channel)
{
    // get top 0.1% dark_channel
    uchar top_pixel = get_top_pixels(dark_channel, 1 - 0.001);
    Mat mask;
    threshold(dark_channel, mask, top_pixel, 1,0);
    return estimate_AL(img_b, img_g, img_r, mask);
}

#endif

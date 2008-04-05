 #include "image.h"
#include "bmp.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>


/**
 * Image
 **/
Image::Image (int width_, int height_)
{
    assert(width_ > 0);
    assert(height_ > 0);

    width           = width_;
    height          = height_;
    num_pixels      = width * height;
    pixels          = new Pixel[num_pixels];
    sampling_method = IMAGE_SAMPLING_POINT;

    assert(pixels != NULL);
}


Image::Image (const Image& src)
{
    width           = src.width;
    height          = src.height;
    num_pixels      = width * height;
    pixels          = new Pixel[num_pixels];
    sampling_method = IMAGE_SAMPLING_POINT;

    assert(pixels != NULL);
    memcpy(pixels, src.pixels, src.width * src.height * sizeof(Pixel));
}


Image::~Image ()
{
    delete [] pixels;
    pixels = NULL;
}


void Image::AddNoise (double factor)
{
    /** WORK HERE **/
}


void Image::Brighten (double factor)
{
    /** WORK HERE **/
}


void Image::ChangeContrast (double factor)
{
    /** WORK HERE **/
}


void Image::ChangeSaturation(double factor)
{
    /** WORK HERE **/
}


Image* Image::Crop(int x, int y, int w, int h)
{
    /** WORK HERE **/
    return NULL;
}


void Image::ExtractChannel(int channel)
{
    /** WORK HERE **/
}


void Image::Quantize (int nbits)
{
    /** WORK HERE **/
}


void Image::RandomDither (int nbits)
{
    /** WORK HERE **/
}


void ImageComposite(Image *bottom, Image *top, Image *result)
{
    /** WORK HERE **/
}


void Image::Blur(int n)
{
    /** WORK HERE **/
}


void Image::EdgeDetect()
{
    /** WORK HERE **/
}


/* Matrix for Bayer's 4x4 pattern dither. */
static int Bayer4[4][4] =
{
    {15, 7, 13, 5},
    {3, 11, 1, 9},
    {12, 4, 14, 6},
    {0, 8, 2, 10}
};

void Image::OrderedDither(int nbits)
{
    /** WORK HERE **/
}


/* Error-diffusion parameters */
const double
    ALPHA = 7.0 / 16.0,
    BETA  = 3.0 / 16.0,
    GAMMA = 5.0 / 16.0,
    DELTA = 1.0 / 16.0;

void Image::FloydSteinbergDither(int nbits)
{
    /** WORK HERE **/
}


Image* Image::Scale(double sx, double sy)
{
    /** WORK HERE **/
    return NULL;
}


Image* Image::Rotate(double angle)
{
    /** WORK HERE **/
    return NULL;
}


void Image::Fun()
{
    /** WORK HERE **/
}


Image* ImageMorph (Image* I0, Image* I1, int numLines, Line* L0, Line* L1, double t)
{
    /** WORK HERE **/
    return NULL;
}


/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method)
{
    assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
    sampling_method = method;
}

Pixel Image::Sample (double u, double v)
{
    /** WORK HERE **/
    return Pixel();
}


#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

#include <stdio.h>

void debug_print_image_header(IplImage *image)
{
    printf("IplImage: \n"
           "\tnSize: %d\n"
           "\tID: %d\n"
           "\tnChannels: %d\n"
           "\talphaChannel: %d\n"
           "\tdepth: %d\n"
           "\tdataOrder: %d\n"
           "\torigin: %d\n"
           "\talign: %d\n"
           "\twidth: %d\n"
           "\theight: %d\n"
           "\timageSize: %d\n"
           "\twidthStep: %d\n",
           image->nSize, image->ID, image->nChannels, image->alphaChannel,
           image->depth, image->dataOrder, image->origin, image->align,
           image->width, image->height, image->imageSize, image->widthStep);
}

/****************************************************************************/

void release_capture(CvCapture *capture)
{
    CvCapture *temp = capture;
    cvReleaseCapture(&temp);
}

/****************************************************************************/

void num_to_name(int num, char *name, int length)
{
    snprintf(name, length, "win-%d", num);
}

void new_window(int num, int flags)
{
    char name[100];
    num_to_name(num, name, sizeof(name));
    cvNamedWindow(name, flags);
}

void del_window(int num)
{
    char name[100];
    num_to_name(num, name, sizeof(name));
    cvDestroyWindow(name);
}

void show_image(int num, IplImage *image)
{
    char name[100];
    num_to_name(num, name, sizeof(name));
    cvShowImage(name, image);
}

/**********************************************************/

/* Haskell's FFI doesn't know how to pass structs by value, I think. */
IplImage *create_image(int width, int height, int depth, int channels)
{
    return cvCreateImage(cvSize(width,height), depth, channels);
}

void release_image(IplImage *image)
{
    IplImage *temp = image;
    return cvReleaseImage(&temp);
}

/* Notice that according to opencv docs, cvGetSize returns the ROI for images, not the image.width and .height */
void get_size(const CvArr *arr, CvSize *size)
{
    *size = cvGetSize(arr);
}

/* TODO instead of get_depth, get_nChannels maybe we should allow serialization of the IplImage struct itself, as we do
   for CvSize? */
int get_depth(const IplImage *image)
{
    return image->depth;
}

int get_nChannels(const IplImage *image)
{
    return image->nChannels;
}

/*void dilate(IplImage *src, int iterations, IplImage *dest)
{
    cvDilate(src, dest, NULL, iterations);
    }*/
/**********************************************************/

void release_mem_storage(CvMemStorage *mem_store)
{
    CvMemStorage *temp = mem_store;
    cvReleaseMemStorage(&temp);
}

#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

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
           "\timageData: %x\n"
           "\twidthStep: %d\n"
           "\timageDataOrigin: %x\n",
           image->nSize, image->ID, image->nChannels, image->alphaChannel,
           image->depth, image->dataOrder, image->origin, image->align,
           image->width, image->height, image->imageSize, (int)image->imageData, 
           image->widthStep, (int)image->imageDataOrigin);
}

/****************************************************************************/

void release_capture(CvCapture *capture)
{
    CvCapture *temp = capture;
    cvReleaseCapture(&temp);
}

void release_video_writer(CvVideoWriter *writer)
{
    CvVideoWriter *temp = writer;
    cvReleaseVideoWriter(&temp);
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

void dilate(const CvArr *src, CvArr *dest, int iterations)
{
    cvDilate(src, dest, NULL, iterations);
}

/**********************************************************/

void release_mem_storage(CvMemStorage *mem_store)
{
    CvMemStorage *temp = mem_store;
    cvReleaseMemStorage(&temp);
}

void cv_free(void *obj)
{
    void *temp = obj;
    cvFree(&temp);
}

/****************************************************************************/

int seq_total(const CvSeq *seq) {
    return seq->total;
}

/* Commonly used case of CV_GET_SEQ_ELEM is CvRect-typed elements.
   The macro CV_GET_SEQ_ELEM is supposed to be faster in some cases
   than the function cvGetSeqElem. */
/*CvRect *c_rect_cvGetSeqElem(const CvSeq *seq, int index) {
    return CV_GET_SEQ_ELEM( CvRect, seq, index);
    }*/


void c_cvRectangle(CvArr *img, int x, int y, int width, int height)
{
    cvRectangle(img, cvPoint(x,y),
                     cvPoint((x+width),
                             (y+height)),
                CV_RGB(255,0,0), 3 , 8, 0);
}

void c_cvLine(CvArr *img, int x1, int y1, int x2, int y2, double r, double g, 
              double b, int thickness, int lineType, int shift)
{
    cvLine(img, cvPoint(x1,y1), cvPoint(x2,y2), CV_RGB(r,g,b), thickness, 
           lineType, shift);
}

CvFont defaultFont;
unsigned char defaultFontInitialized = 0;

void c_cvPutText(CvArr *img, const char* msg, int x, int y, 
                 double r, double g, double b)
{
  if(!defaultFontInitialized)
  {
    cvInitFont(&defaultFont, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0.0, 1, 8);
    defaultFontInitialized = 1;
  }
  cvPutText(img, msg, cvPoint(x,y), &defaultFont, CV_RGB(r,g,b));
}

int c_cvFindContours(CvArr *img, CvMemStorage *storage, CvSeq** first_contour, 
                     int header_size, int mode, int method, int offset_x, 
                     int offset_y)
{
    return cvFindContours(img, storage, first_contour, header_size, mode, 
                          method, cvPoint(offset_x,offset_y));
}

void c_cvSetRoi(IplImage* img, int x, int y, int width, int height)
{
  cvSetImageROI(img, cvRect(x,y,width,height));
}


/****************************************************************************/
/*
CvSeq *c_cvHaarDetectObjects( const CvArr* image,
                              CvHaarClassifierCascade* cascade,
                              CvMemStorage* storage, double scale_factor,
                              int min_neighbors , int flags,
                              int min_width, int min_height,
                              int max_width, int max_height)
{
    return cvHaarDetectObjects(image, cascade, storage, scale_factor, min_neighbors, flags, cvSize(min_width, min_height), cvSize(max_width, max_height));
}
*/

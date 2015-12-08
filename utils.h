#ifndef UTILS
#define UTILS
#include <QImage>
#include <math.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

//image size
#define LEN 256
#define WID 256

//quantization opition
#define QUANTIZATION_LAYER 128

struct dynamicParameter
{
    double al;
    double bright;
    double dark;
    double vl;
    int ve;
    double beta;
    double ae;
};

class imageUtils
{
public:
    QImage RGBtoGray(QImage image);
    double** QImagetoArray(QImage image);
    QImage arraytoQImage(double** array);

    double caculateStd2(double **image);
    int caculateGraythresh(double** image);
    dynamicParameter caculatePCNNParameters(double** image);
    void caculateConv2(double image[][WID], double filter[][3]);

    double** allocateHeap(int rows, int cols);
    void releaseHeap(double** A, int rows);

    double** PCNNQuantize(double** image);
};

#endif


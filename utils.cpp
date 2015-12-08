#include "utils.h"

QImage imageUtils::RGBtoGray(QImage image)
{
    int height = image.height();
    int width = image.width();
    QImage ret(width, height, QImage::Format_Indexed8);

    for(int i=0; i<256; i++)
    {
        ret.setColor(i, qRgb(i, i, i));
    }

    switch(image.format())
    {
    case QImage::Format_Indexed8:
        for(int i=0; i<height; i++)
        {
            const uchar *pSrc = (uchar *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);
            memcpy(pDest, pSrc, width);
        }
        break;

    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        for(int i=0; i<height; i++)
        {
            const QRgb* pSrc = (QRgb*)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);

            for (int j=0; j<width; j++)
            {
                pDest[j] = qGray(pSrc[j]);
            }
        }
        break;
    }
    return ret;
}

// QImage Format to array in C++
double** imageUtils::QImagetoArray(QImage image)
{
    double** outArray = allocateHeap(LEN, WID);
    for(int i=0; i<LEN; i++)
    {
        const uchar *pSrc = (uchar *)image.constScanLine(i);
        for(int j=0; j<WID; j++)
        {
            outArray[i][j] = pSrc[j] / 255.0;
        }
    }

    return outArray;
}

QImage imageUtils::arraytoQImage(double** array)
{
    QImage outQImage(LEN, WID, QImage::Format_Indexed8);
    for(int i=0; i<LEN; i++)
    {
        for(int j=0; j<WID; j++)
        {
            //outQImage.setPixel(i, j, array[i][j]);
            outQImage.bits()[i*LEN +j] = array[i][j];
        }
    }

    return outQImage;
}


double imageUtils::caculateStd2(double** image)
{
    double grayValueSum;
    double stdValueSum;
    for(int i=0; i<LEN; i++)
    {
        for(int j=0; j<WID; j++)
        {
            grayValueSum += image[i][j];
        }
    }

    double mean = grayValueSum / ( LEN * WID);

    for(int i=0; i<LEN; i++)
    {
        for(int j=0; j<WID; j++)
        {
            stdValueSum += pow((image[i][j] - mean), 2);
        }
    }

    double std2 = sqrt(stdValueSum / (double)(LEN * WID));
    return std2;
}


int imageUtils::caculateGraythresh(double** image)
{
    int grayLayerCount[256];
    float grayLayerRatio[256];
    int threshold = 0;

    // 初始化为 0
    for(int i=0; i<LEN; i++)
    {
        grayLayerCount[i] = 0;
        grayLayerRatio[i] = 0;
    }

    // 统计每个灰度在图像中的个数
    for(int i=0; i<LEN; i++)
    {
        for(int j=0; j<WID; j++)
        {
            grayLayerCount[(int)(255*image[i][j])]++;
        }
    }

    //计算每个像素在整幅图像中的比例
    for(int i=0; i<LEN; i++)
    {
        grayLayerRatio[i] = (float)(grayLayerCount[i]) / (float)(LEN * WID);
    }

    //经典ostu算法，得到前景和背景的分割
    //遍历灰度级[0,255],计算出方差最大的灰度值为最佳阈值
    float w0, w1, u0tmp, u1tmp, u0, u1, u,deltaTmp, deltaMax = 0;
    for(int i = 0; i < 256; i++)
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

        for(int j = 0; j < 256; j++)
        {
            if(j <= i) //背景部分
            {
                //以i为阈值分类，第一类总的概率
                w0 += grayLayerRatio[j];
                u0tmp += j * grayLayerRatio[j];
            }
            else       //前景部分
            {
                //以i为阈值分类，第二类总的概率
                w1 += grayLayerRatio[j];
                u1tmp += j * grayLayerRatio[j];
            }
        }

        u0 = u0tmp / w0;		//第一类的平均灰度
        u1 = u1tmp / w1;		//第二类的平均灰度
        u = u0tmp + u1tmp;		//整幅图像的平均灰度
        //计算类间方差
        deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
        //找出最大类间方差以及对应的阈值
        if(deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = i;
        }
    }

    return threshold;
}


dynamicParameter imageUtils::caculatePCNNParameters(double** image)
{
    dynamicParameter parameterSet;
    parameterSet.al = log10(1 / caculateStd2(image));

    double brightestPixel = 0;
    double darkestPixel = 1;
    for(int i=0; i<LEN; i++)
    {
        for(int j=0; j<WID; j++)
        {
            if (image[i][j] > brightestPixel)
                brightestPixel = image[i][j];
            if (image[i][j] < darkestPixel)
                darkestPixel = image[i][j];
        }
    }

    parameterSet.bright = brightestPixel;
    parameterSet.dark = darkestPixel;
    parameterSet.vl = 1.00;
    parameterSet.ve = 200;
    parameterSet.beta = ((parameterSet.bright / ((caculateGraythresh(image)) / (double)255)) - 1) / (double)(6 * parameterSet.vl);
    parameterSet.ae = (log10(parameterSet.bright + 0.0001) - log10(parameterSet.dark + 0.0001)) / (double)(QUANTIZATION_LAYER - 1);
    return parameterSet;
}


void imageUtils::caculateConv2(double image[][WID], double filter[][3])
{
    for(int i=1; i<LEN-1; i++)
    {
        for(int j=1; j<WID-1; j++)
        {
            double sum = 0;

            for(int x=0; x<3; x++)
            {
                for(int y=0; y<3; y++)
                {

                    sum = image[i-1][j-1]*filter[0][0] + image[i-1][j]*filter[0][1] + image[i-1][j+1]*filter[0][2]
                        + image[i][j-1]*filter[1][0] + image[i][j]*filter[1][1] + image[i][j+1]*filter[1][2]
                        + image[i+1][j-1]*filter[2][0] + image[i+1][j]*filter[2][1] + image[i+1][j+1]*filter[2][2];
                }
            }
            image[i][j] = sum;
        }
    }
}


double** imageUtils::allocateHeap(int rows, int cols)
{
    double **A;
    A = new double* [rows];
    for (int i=0; i<rows; i++)
    {
        A[i] = new double [cols];
    }

    return A;
}


void imageUtils::releaseHeap(double** A, int rows)
{
    for (int i=0; i<rows; i++)
    {
        delete[] A[i];
    }
    delete[] A;
}


//return gray value (0~QUANTIZATION_LAYER)
double** imageUtils::PCNNQuantize(double** image)
{
    int np = QUANTIZATION_LAYER;
    double QuiImage[LEN][WID], L[LEN][WID], U[LEN][WID], Y[LEN][WID], Y0[LEN][WID], Y_bak[LEN][WID], K[LEN][WID], E[LEN][WID];
    double F[LEN][WID];

    dynamicParameter paraSet;
    paraSet = caculatePCNNParameters(image);

    double** out = allocateHeap(LEN, WID);

    for(int i=0; i<LEN; i++)
    {
        for(int j=0; j<WID; j++)
        {
            QuiImage[i][j] = 0;
            L[i][j] = 0;
            U[i][j] = 0;
            Y[i][j] = 0;
            Y0[i][j] = 0;
            Y_bak[i][j] = 0;
            K[i][j] = 0;

            F[i][j] = image[i][j];
            E[i][j] = paraSet.bright;
        }
    }

    double W[3][3] =
    {
        {0.1091, 0.1409, 0.1091},
        {0.1409, 0     , 0.1409},
        {0.1091, 0.1409, 0.1091}
    };

    for(int loop=0; loop<np; loop++)
    {
        for(int i=0; i<LEN; i++)
        {
            for(int j=0; j<WID; j++)
            {
                Y_bak[i][j] = Y[i][j];
            }
        }

        caculateConv2(Y, W);

        for(int i=0; i<LEN; i++)
        {
            for(int j=0; j<WID; j++)
            {
                K[i][j] = Y[i][j];
            }
        }


        for(int i=0; i<LEN; i++) {
            for(int j=0; j<WID; j++) {
                L[i][j] = exp(-paraSet.al) * L[i][j] + paraSet.vl * K[i][j];
        }}

        for(int i=0; i<LEN; i++) {
            for(int j=0; j<WID; j++) {
                E[i][j] = exp(-paraSet.ae) * E[i][j] + paraSet.ve * Y_bak[i][j];
        }}

        for(int i=0; i<LEN; i++) {
            for(int j=0; j<WID; j++) {
                U[i][j] = F[i][j] * (1 + paraSet.beta * L[i][j]);
        }}

        for(int i=0; i<LEN; i++) {
            for(int j=0; j<WID; j++) {
                Y[i][j] = U[i][j] > E[i][j] ? 1 : 0;
        }}


        for(int i=0; i<LEN; i++) {
            for(int j=0; j<WID; j++) {
                if( Y0[i][j] == 0 && Y[i][j] != 0)
                {
                    Y0[i][j] = 1;
                    out[i][j] = loop;
                }
        }}

        int nonzeroCount = 0;
        for(int i=0; i<LEN; i++) {
            for(int j=0; j<WID; j++) {
                if (Y[i][j] == 0)
                    nonzeroCount++;
                if (nonzeroCount == LEN * WID)
                    break;
        }}

        // reverse Time Matrix
        for(int i=0; i<LEN; i++) {
            for(int j=0; j<WID; j++) {
                out[i][j] = np - out[i][j];
        }}
    }

    return out;
}

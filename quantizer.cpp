#include "quantizer.h"
#include "utils.h"
#include "ui_quantizer.h"
#include <iostream>
using namespace std;

Quantizer::Quantizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Quantizer)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags);
    setFixedSize(547,359);

    this->image = new QImage();

    //the format on_XXXX_clicked() do not need to connect
    // connect(ui->openButton, SIGNAL(clicked()), this, SLOT(on_openButton_clicked()));
    // connect(ui->quantizeButton, SIGNAL(clicked()), this, SLOT(on_quantizeButton_clicked()));
}

Quantizer::~Quantizer()
{
    delete image;
    delete ui;
}

void Quantizer::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "open image file",
                                                ".",
                                                "Image files (*.bmp *.jpg)");
    if (fileName != "")
    {
        if (image->load(fileName))
        {
            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(QPixmap::fromImage(*image));
            ui->leftView->setScene(scene);
            ui->leftView->show();
        }
    }
}

void Quantizer::on_quantizeButton_clicked()
{
    imageUtils utils;
    QImage grayImage = utils.RGBtoGray(*this->image);

    double** imageArray = utils.QImagetoArray(grayImage);
    double** quantized_image = utils.PCNNQuantize(imageArray);

    // normalized to (0~255)
    for(int i=0; i<LEN; i++)
    {
        for(int j=0; j<WID; j++)
        {
            quantized_image[i][j] = uchar(quantized_image[i][j] / QUANTIZATION_LAYER * 255);
        }
    }

    QImage outImage = utils.arraytoQImage(quantized_image);

    utils.releaseHeap(imageArray, LEN);
    utils.releaseHeap(quantized_image, LEN);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(outImage));
    ui->rightView->setScene(scene);
    ui->rightView->show();
}

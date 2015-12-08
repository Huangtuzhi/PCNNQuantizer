#ifndef QUANTIZER_H
#define QUANTIZER_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFileDialog>

namespace Ui {
class Quantizer;
}

class Quantizer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Quantizer(QWidget *parent = 0);
    ~Quantizer();

private slots:
    void on_openButton_clicked();

    void on_quantizeButton_clicked();

private:
    Ui::Quantizer *ui;
    QImage* image;
};

#endif // QUANTIZER_H

#ifndef CUTIMAGE_H
#define CUTIMAGE_H
#include "cameraconnection.h"

class cutimage : public QObject
{
    Q_OBJECT

public:

    cv::Mat templ;//шаблонное изображение
    cv::Mat mask;//рамка
    cv::Mat result;//результат выполнения функции match template
    int matchMethod = 2;//метод сопоставления шаблона
    bool useMask;
    double threshold;


    cutimage();

    void setThreshold(double value){
        threshold = value;
    }

    void setTemplate(QString path){
        templ = cv::imread(path.toStdString(), cv::IMREAD_GRAYSCALE);
    }
    //void templateWork();

public slots:
    void templateWork(cv::Mat);

signals:
    void imageChanged(cv::Mat);
};

#endif // CUTIMAGE_H

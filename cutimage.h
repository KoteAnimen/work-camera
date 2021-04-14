#ifndef CUTIMAGE_H
#define CUTIMAGE_H
#include "cameraconnection.h"

class cutimage : public QObject
{
    Q_OBJECT

public:

    cv::Mat source;
    cv::Mat templ;//шаблонное изображение
    cv::Mat mask;//рамка
    cv::Mat result;//результат выполнения функции match template
    int matchMethod = 2;//метод сопоставления шаблона
    double threshold;
    bool useMask;
    cutimage();
    void setTemplate(QString path){
        templ = cv::imread(path.toStdString(), cv::IMREAD_GRAYSCALE);
    }
    void setThreashold(double value){
        threshold = value;
    }

public slots:
    void templateWork(cv::Mat);    
signals:
    void imageChanged(cv::Mat);
};

#endif // CUTIMAGE_H

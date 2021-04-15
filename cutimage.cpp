#include "cutimage.h"
#include "QDebug"



cutimage::cutimage()
{
   int metatype_id = qRegisterMetaType<cv::Mat>("cv::Mat");


}

void cutimage::templateWork(cv::Mat source){

    if(isWork == true)
    {
        cv::Mat dst=source.clone();
        int result_cols =  source.cols - templ.cols + 1;
        int result_rows = source.rows - templ.rows + 1;
        result.create( result_rows, result_cols, cv::IMREAD_GRAYSCALE );
        bool method_accepts_mask = (cv::TM_SQDIFF == matchMethod || matchMethod == cv::TM_CCORR_NORMED);
        if (useMask && method_accepts_mask) {
            matchTemplate( source, templ, result, matchMethod, mask);
        }
        else
        {
            matchTemplate( source, templ, result, matchMethod);
        }
        //normalize( result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
        double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
        cv::Point matchLoc;
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
        if( matchMethod  == cv::TM_SQDIFF || matchMethod == cv::TM_SQDIFF_NORMED )
        {
            matchLoc = minLoc;
        }
        else
        {
            matchLoc = maxLoc;
        }
        cv::cvtColor(dst, dst, cv::COLOR_RGB2RGBA);
        //qDebug() << minVal;

        //если совпадение будет максимально хорошим, то рисуем прямоугольник
        if(minVal < threshold * pow(10, 8)){
            rectangle( dst, matchLoc, cv::Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cv::Scalar(0, 255, 0), 3, 8, 0 );
        }
        emit imageChanged(dst);
    }
    else
    {
       emit finished();
    }

}



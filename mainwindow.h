#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cameraconnection.h"
#include "cutimage.h"
#include <QThread>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    cutimage *cut;
    QString path;

private:
    Ui::MainWindow *ui;
    CameraConnection *camera;
    QThread *thread_cam;
    QThread *thread_cut;


public slots:
    void Paint(cv::Mat);    
    void DrawFrame(cv::Mat src);

signals:
    void getFrame();
    void sendImage(cv::Mat);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_loadTemplate_clicked();

private:
    cv::Mat templ;//шаблонное изображение
};
#endif // MAINWINDOW_H
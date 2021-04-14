#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"

//https://docs.opencv.org/3.4/de/da9/tutorial_template_matching.html - здесь все что нужно про match template


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int meta_type_id = qRegisterMetaType<cv::Mat>("cv::Mat");

    camera=new CameraConnection();
    cut = new cutimage();

    connect(camera,&CameraConnection::FrameReady,this,&MainWindow::Paint);
    connect(this,&MainWindow::getFrame,camera,&CameraConnection::Grab);
    thread_cam = new QThread();
    thread_cut = new QThread();
    cut->moveToThread(thread_cut);
    camera->moveToThread(thread_cam);    
    connect(thread_cam, &QThread::started,camera, &CameraConnection::Grab);
    connect(thread_cut, &QThread::started, this, &MainWindow::on_pushButton_clicked);
    thread_cam->start();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Paint(cv::Mat src)
{
    QImage *CamImg = new QImage(src.data, src.cols, src.rows, src.step,QImage::Format_Grayscale8);
    ui->label->setPixmap(QPixmap::fromImage(*CamImg).scaled(ui->label->size()));
    ui->label->update();
    emit getFrame();
    emit sendImage(src);
    delete CamImg;
}

void MainWindow::DrawFrame(cv::Mat src)
{
QImage *CamImg = new QImage(src.data, src.cols, src.rows, src.step,QImage::Format_Grayscale8);
ui->label_2->setPixmap(QPixmap::fromImage(*CamImg).scaled(ui->label_2->size()));
ui->label_2->update();
delete CamImg;
}

void MainWindow::on_loadTemplate_clicked()
{
    path = QFileDialog::getOpenFileName(this, QString::fromUtf8("Открыть файл"), QDir::currentPath(),"Images (*.png *.xpm *.bmp);;All files (*.*)");
}

void MainWindow::on_pushButton_clicked()
{
    if(path == "")
    {
        QMessageBox box;
        box.setWindowTitle("Ошибка");
        box.setText("Загрузите шаблон");
        box.exec();
        return;
    }
    else
    {
        cut->setTemplate(path);
        connect(this, &MainWindow::sendImage, cut, &cutimage::templateWork);
        thread_cam->start();
        connect(cut, &cutimage::imageChanged, this, &MainWindow::DrawFrame);
        thread_cut->start();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    thread_cam->quit();
    thread_cut->quit();
    disconnect(this, &MainWindow::sendImage, cut, &cutimage::templateWork);
    disconnect(cut, &cutimage::imageChanged, this, &MainWindow::DrawFrame);
}



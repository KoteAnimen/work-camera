#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QFile"
#include "QDataStream"
#include "QMessageBox"

//https://docs.opencv.org/3.4/de/da9/tutorial_template_matching.html - здесь все что нужно про match template


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int meta_type_id = qRegisterMetaType<cv::Mat>("cv::Mat");
    camera=new CameraConnection();
    QSettings settings("settings.ini", QSettings::IniFormat);
    path = settings.value("path").toString();
    connect(camera,&CameraConnection::FrameReady,this,&MainWindow::Paint);
    connect(this,&MainWindow::getFrame,camera,&CameraConnection::Grab);
    thread_cam = new QThread();
    camera->moveToThread(thread_cam);
    connect(thread_cam, &QThread::started, camera, &CameraConnection::Grab);
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
    delete CamImg;
}

void MainWindow::DrawFrame(cv::Mat src)
{
    QImage *CamImg = new QImage(src.data, src.cols, src.rows, src.step,QImage::Format_RGB32);
    ui->label_2->setPixmap(QPixmap::fromImage(*CamImg).scaled(ui->label_2->size()));
    ui->label_2->update();
    delete CamImg;
}

void MainWindow::on_loadTemplate_clicked()
{
    path = QFileDialog::getOpenFileName(this, QString::fromUtf8("Открыть файл"), QDir::currentPath(),"Images (*.png *.xpm *.bmp);;All files (*.*)");
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("path", path);
}

void MainWindow::on_pushButton_clicked()
{
    if(cut == NULL)
    {
        cut = new cutimage();
        thread_cut = new QThread();
        cut->moveToThread(thread_cut);
        connect(thread_cut, &QThread::started, camera, &CameraConnection::Grab);

    }
    //check if path exists and if yes: Is it a file and no directory?
    bool fileExists = QFileInfo::exists(path) && QFileInfo(path).isFile();
    if(path == "" || fileExists == false)
    {
        QMessageBox box;
        box.setWindowTitle("Ошибка");
        box.setText("Загрузите шаблон");
        box.exec();
        return;
    }
    else
    {
        QMainWindow::resize(620, 558);
        cut->setTemplate(path);
        cut->setWork(true);
        cut->setThreshold((double)ui->slider->value());
        connect(camera, &CameraConnection::FrameReady, cut,&cutimage::templateWork);
        connect(cut, &cutimage::imageChanged, this, &MainWindow::DrawFrame);        
        thread_cut->start();        
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(cut != NULL)
    {
        disconnect(camera, &CameraConnection::FrameReady, cut,&cutimage::templateWork);
        disconnect(cut, &cutimage::imageChanged, this, &MainWindow::DrawFrame);
        cut->setWork(false);
        connect(cut, &cutimage::finished, thread_cut, &QThread::deleteLater);
        delete cut;
        cut = NULL;
        ui->label_2->clear();// как по мне то это не удобно        
        QMainWindow::resize(QMainWindow::minimumWidth(), QMainWindow::minimumHeight());
        QMainWindow::updateGeometry();
    }

}



void MainWindow::on_slider_valueChanged(int value)
{
    if(cut != NULL)
    {
        cut->setThreshold((double)value);
    }
}

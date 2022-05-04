#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_testt.h"
#include <QMouseEvent>

#include <vtkAutoInit.h>

#include <QFileDialog>
#include <vtkRenderWindow.h>
#include <pcl/io/pcd_io.h>    //檔案輸入輸出
#include <pcl/point_types.h>  //點型別相關定義
#include <pcl/visualization/pcl_visualizer.h>  //點型別相關定義



class testt : public QMainWindow
{
    Q_OBJECT

public:
    testt(QWidget *parent = Q_NULLPTR);

protected:
    unsigned red;
    unsigned green;
    unsigned blue;
    double size;

private slots:
    void butt_open();
    void rgbSliderReleased();
    void pSliderValueChangeed(int value);
    void redSliderValueChangeed(int value);
    void greenSliderValueChangeed(int value);
    void blueSliderValueChangeed(int value);
    
private:

    static void pp_callback(const pcl::visualization::PointPickingEvent& event, void* args);//螢幕選點
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr clicked_points_3d;

    Ui::testtClass ui;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
    void initialVtkWidget();
};

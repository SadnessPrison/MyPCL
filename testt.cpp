#include "testt.h"
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include<QFileDialog>

testt::testt(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    initialVtkWidget();
}

void testt::initialVtkWidget()
{

    //初始化
    red = 255;
    green = 255;
    blue = 255;
    size = 1.0;
    //加載
    cloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
    viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
    //viewer->addPointCloud(cloud, "cloud");
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(cloud, red, green, blue);//自定義
    viewer->addPointCloud<pcl::PointXYZ>(cloud, single_color, "cloud");

    viewer->registerPointPickingCallback(pp_callback, this);//回調函數
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr clicked_points_3d_(new pcl::PointCloud<pcl::PointXYZRGB>);
    this->clicked_points_3d = clicked_points_3d_;//對其初始化

    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, size, "cloud");//設置單點大小
    ui.qvtkWidget->SetRenderWindow(viewer->getRenderWindow());
    viewer->setupInteractor(ui.qvtkWidget->GetInteractor(), ui.qvtkWidget->GetRenderWindow());
    ui.qvtkWidget->update();
}

void testt::butt_open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open PointCloud", ".", "Open PCD files(*.pcd)");
    if (!fileName.isEmpty()) {

        std::string file_name = fileName.toStdString();

        pcl::PCLPointCloud2 cloud2;

        Eigen::Vector4f origin;
        Eigen::Quaternionf orientation;
        int pcd_version;
        int data_type;
        unsigned int data_idx;
        int offset = 0;
        pcl::PCDReader rd;
        rd.readHeader(file_name, cloud2, origin, orientation, pcd_version, data_type, data_idx);
        if (data_type == 0)
        {
            pcl::io::loadPCDFile(fileName.toStdString(), *cloud);
        }
        else if (data_type == 2)
        {
            pcl::PCDReader reader;
            reader.read<pcl::PointXYZ>(fileName.toStdString(), *cloud);
        }

        //re加載
        //viewer->updatePointCloud(cloud, "cloud");
        pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(cloud, red, green, blue);//自定義
        viewer->updatePointCloud<pcl::PointXYZ>(cloud, single_color, "cloud");
        viewer->resetCamera();
        ui.qvtkWidget->update();
    }

}

void testt::rgbSliderReleased()
{
    //更新點雲顏色
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(cloud, red, green, blue);
    viewer->updatePointCloud<pcl::PointXYZ>(cloud, single_color, "cloud");
    ui.qvtkWidget->update();
}

void testt::redSliderValueChangeed(int value)
{
    red = value;
}

void testt::greenSliderValueChangeed(int value)
{
    green = value;
}

void testt::blueSliderValueChangeed(int value)
{
    blue = value;
}

void testt::pSliderValueChangeed(int value)
{
    size = double(10 + value) / 10;
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, size, "cloud");
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>single_color(cloud, red, green, blue);
    viewer->updatePointCloud < pcl::PointXYZ>(cloud, single_color, "cloud");
    ui.qvtkWidget->update();
}

void testt::pp_callback(const pcl::visualization::PointPickingEvent& event, void* args)
{
    testt* p = (testt*)args;
    if (event.getPointIndex() == -1)
        return;
    pcl::PointXYZRGB current_point;
    event.getPoint(current_point.x, current_point.y, current_point.z);
    p->clicked_points_3d->points.push_back(current_point);
    //将选取的点设为红色
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZRGB> red(p->clicked_points_3d, 255, 0, 0);
    p->viewer->removePointCloud("clicked_points");//delete point
    p->viewer->addPointCloud(p->clicked_points_3d, red, "clicked_points");
    p->viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 10, "clicked_points");//addpoint
    cout << current_point.x << "," << current_point.y << "," << current_point.z << endl;
}

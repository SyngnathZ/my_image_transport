#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>s

image_transport::Subscriber sub;
image_transport::Publisher pub;

void imageCallback(const sensor_msgs::ImageConstPtr &msg) {
    try {
        cv::Mat img, gray;
        img = cv_bridge::toCvShare(msg, "bgr8")->image;
        cv::cvtColor(img, gray, CV_BGR2GRAY);

        ros::NodeHandle nh;
        image_transport::ImageTransport it(nh);
        sensor_msgs::ImagePtr msg2 = cv_bridge::CvImage(std_msgs::Header(), "mono8", gray).toImageMsg();
        pub.publish(msg2);

        cv::imshow("view", img);
    }
    catch (cv_bridge::Exception &e) {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "image_listener1");
    ros::NodeHandle nh;
    cv::namedWindow("view");
    cv::startWindowThread();
    image_transport::ImageTransport it(nh);
    sub = it.subscribe("camera/image", 1, imageCallback);
    pub = it.advertise("camera/image_gray", 1);

    ros::spin();
    cv::destroyWindow("view");
}
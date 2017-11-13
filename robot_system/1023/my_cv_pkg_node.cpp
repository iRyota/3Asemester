#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

class MyCvPkg
{
	image_transport::Subscriber img_sub_;
	image_transport::ImageTransport it_;

	void imageCallback(const sensor_msgs::ImageConstPtr &msg) {
		ROS_INFO("Received image");
		cv::Mat in_img = cv_bridge::toCvCopy(msg, msg->encoding)->image;
		//write opencv code
		
		cv::Mat tmp_img = in_img.clone();
		cv::Mat mirror(tmp_img.size(),tmp_img.type(),cv::Scalar::all(127));
		int width = tmp_img.cols;
		int height = tmp_img.rows;
		// int step = tmp_img.step;
		int x;
		int y;
		for (y=0;y<height;y++) {
			for (x=0;x<width;x++) {
				mirror.data[y*width + x/* *step */ + 0] = tmp_img.data[y*width + (width-x)/* step */ + 0];
				// mirror.data[y*width + x*step + 1] = tmp_img.data[y*width + (width-x)*step + 1];
				// mirror.data[y*width + x*step + 2] = tmp_img.data[y*width + (width-x)*step + 2];
			}
		}

		cv::imshow("Mirror", mirror);
		cv::waitKey(1);



		//write opencv code
	}

public:
	MyCvPkg(ros::NodeHandle nh = ros::NodeHandle()) : it_(nh)
	{
		img_sub_ = it_.subscribe("image", 3, &MyCvPkg::imageCallback, this);
		cv::namedWindow("Fast", 1);
	}
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "my_cv_pkg_node");
	MyCvPkg mcp;
	ros::spin();
}
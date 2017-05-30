#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"

ros::Subscriber sub;
ros::Publisher pub;

void raceCallback(const sensor_msgs::LaserScan::ConstPtr& scan){
	geometry_msgs::Twist twist;
	geometry_msgs::Vector3 angular;
	geometry_msgs::Vector3 linear;

	angular.z = (scan->ranges[scan->ranges.size() - 1] - scan->ranges[0]);
	
	angular.z *= angular.z * angular.z;

	const float lim = 0.6;
	if (angular.z > lim) angular.z = lim;
	if (angular.z < -lim) angular.z = -lim;

	linear.x = 1;
	
	ROS_INFO("Intensities: %f, %f", scan->ranges[0], scan->ranges[scan->ranges.size() - 1]);	

	twist.angular = angular;
	twist.linear = linear;

	pub.publish(twist);
}

int main(int argc, char** argv){
	ros::init(argc, argv, "uml_race_client");

	ros::NodeHandle node;

	pub = node.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
	sub = node.subscribe("base_scan", 1000, raceCallback);
	
	ros::spin();
	
	return 0;
}

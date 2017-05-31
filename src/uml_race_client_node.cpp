#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"

ros::Subscriber sub;
ros::Publisher pub;

double p_ang = 0;

void raceCallback(const sensor_msgs::LaserScan::ConstPtr& scan){
	geometry_msgs::Twist twist;
	geometry_msgs::Vector3 angular;
	geometry_msgs::Vector3 linear;

	
	double d_term, p_term, speed;
	ros::param::get("/uml_race_solution/proportional_term", p_term);
	ros::param::get("/uml_race_solution/derivative_term", d_term);	
	ros::param::get("/uml_race_solution/speed", speed);
	
	double ang = (scan->ranges[scan->ranges.size() * 7 / 8] - scan->ranges[scan->ranges.size() / 8]);
	double d_ang = p_ang - ang;
	p_ang = ang;

	angular.z = p_term * ang - d_term * d_ang;
	angular.z /= (1.0 + 0.5 * scan->ranges[scan->ranges.size() / 2]);	
	//angular.z *= angular.z * angular.z;

	const double lim = 8.0;
	//if (angular.z > lim) angular.z = lim;
	//if (angular.z < -lim) angular.z = -lim;
	
	speed *= scan->ranges[scan->ranges.size() / 2];
	//speed += 0.1;	
	
	if (speed > 3.0) speed = 5-1e-10;

	linear.x = speed;
	
	//ROS_INFO("P: %f, D: %f", ang, d_ang);	

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

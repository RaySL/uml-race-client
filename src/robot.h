#pragma once

class Robot {
	Robot();
	~Robot();

	void laserScanCallback(sensor_msgs::LaserScan scan);

private:
	ros::Publisher m_twist_publisher;
	ros::Subscriber m_laser_subscriber;	

	double previous_angle;
}

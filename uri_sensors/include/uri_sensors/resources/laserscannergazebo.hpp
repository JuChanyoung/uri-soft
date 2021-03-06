
#include <string>
#include <thread>

#include <pluginlib/class_list_macros.h>
#include <uri_core/resource.hpp>

#include <uri_sensors/resources/laserscanner.hpp>

// #include <Eigen/Geometry>


#ifndef __LASERSCANNERGAZEBO_HPP__
#define __LASERSCANNERGAZEBO_HPP__


using namespace uri;

namespace uri_sensors {
	
	
/// @brief An example of Resource.
/// @details This Resource does nothing. 
/// By default, a Resource contains an OptionVector _options and a ros::NodeHandle n.
/// Options are automatically initalized to the value in the configfile after the execution of the constructor.
class LaserScannerGazebo: public LaserScanner{
		
		
		
		/// @brief Madatory initialization method.
		/// @details this method is called once after the instantiation is created. You can use it to initialize all your private variables
		void __init();
		
		// put here all your private variables
		
		ros::Subscriber _laser_sub;
		
		
		void scan_callback(const sensor_msgs::LaserScan::ConstPtr& msg);
		
		
		// if you need a thread, you can use the c++11 standard 
		// std::thread* getcharacters;
		
	public:
		
		/// @brief Constructor
		/// @details This method does not build anything.
		LaserScannerGazebo();
		
		
		/// @brief Set function.
		/// @details This function sets the scan within the specified timeout.
		/// This is a pure virtual method. Each derived class must implement this method
		/// @param[in] &scan the latest laser scan to be copied.
		/// @param[in] timeout maximum time in seconds to wait for setting the scan.
		/// @return \b true if the scan is set within the timeout, \b false otherwise.
// 		bool set(sensor_msgs::LaserScan &scan, double timeout);
		
};


} // end namespace uri_sensors





#endif







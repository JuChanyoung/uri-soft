#include <iostream>
#include <vector>
//#include <thread>

#include "ros/ros.h"
#include <pluginlib/class_loader.h>

#include <tinyxml.h>

#include <uri_core/utilities.hpp>
#include <uri_core/resource.hpp>
#include <uri_core/task.hpp>
#include <uri_core/behavior.hpp>
#include <uri_core/scheduler.hpp>











int main (int argc, char** argv){
	

	ros::init(argc, argv, "~");
	ros::NodeHandle n;
	
	std::string ns = ros::this_node::getNamespace();
	ROS_INFO("namespace is : %s", ns.c_str());

	std::string nm = ros::this_node::getName();
	ROS_INFO("name is : %s", nm.c_str());
	
	std::string config_file_name;
	uri::getStringParam(n,config_file_name,"configuration_file",nm);
	
	double scheduler_time;
	uri::getDoubleParam(n,scheduler_time,"scheduler_time",nm);
	
	
	uri::Scheduler scheduler(n, config_file_name, scheduler_time);
	

	
	
	while (ros::ok()){
		usleep(100);
		ros::spinOnce();
		
// 		scheduler.run();
// 		std::cout << behaviors.size() << " " << behaviors[0]->num_tasks() << std::endl;
// 		for(int i=0; i<behaviors[0]->num_tasks(); i++){
// 			behaviors[0]->task(i)->run();
// 		}
	}
	
	
}
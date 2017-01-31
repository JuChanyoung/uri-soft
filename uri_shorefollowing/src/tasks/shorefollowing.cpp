#include <iostream>
#include <vector>
//#include <thread>

#include "ros/ros.h"
#include <uri_shorefollowing/tasks/shorefollowing.hpp>

namespace uri_shorefollowing{

  
  
  

ShoreFollowing::ShoreFollowing():Task(){
	// You should keep this line on top and put the name of your task in it.
	// Not giving a name to your task will have an unpredictable behavior
	// and most likely will not work.
	_name = "uri_shorefollowing::ShoreFollowing";
	
	// The class Task contains a field _options which you can use freely.
	// You can add (and pass options through the configuration file) with the following lines.
	// [ format: _options.addTYPEOption(OPTION_NAME, DEFAULT_VALUE); ]
	//
	// _options.addIntOption("name_option_1",15);
	// _options.addDoubleOption("name_option_2",0.05);
	// _options.addBoolOption("name_option_3",false);
	// _options.addStringOption("name_option_2","default_value");
	_options.addDoubleOption("speed",2.0);
	_options.addDoubleOption("max_acc",0.5);
	//
	// Note that options are only of those types listed here.
	// The first parameter in the above lines is the name, while the second parameter is the default value.
	// If the option is specified in the config file, the value of the option is automatically updated.
	// If the option specfied is not in the config_file, the option will have the default value
	// note that the options are updated after the execution of this constructor, so any option in this
	// construction will have its default value.
	
	speed = 0.0;
}

TaskOutput ShoreFollowing::_run(){
	// do your cool code here!
	
	bool terminate = false;
	
	
	if (!ls->new_laser_available()){
		return uri::Continue;
	}
	sensor_msgs::LaserScan scan;
	if (!ls->get(scan, 0.001)){
		return uri::Continue;
	}
	
	std::cout << scan.header << std::endl;
	
	double elapsed = (ros::Time::now() - start_t).toSec();
	delta_t = elapsed - last_elapsed;
	
	// put here algorithm to decide heading direction
	
	if (elapsed<max_speed_time)
		speed = _options["max_acc"]->getDoubleValue()*elapsed;
	else
		speed = _options["speed"]->getDoubleValue();
	
	
// 	pos = pos + elapsed
	
	
	last_elapsed = elapsed;
	
	if (terminate){
		
		return uri::Terminate;
	}
	return uri::Continue;
	// how to get the value of an option - use the following syntax:
	// _options["OPTION_NAME"]->getTYPEValue()
	//
	// example:
	// _options["name_option_2"]->getDoubleValue();
	//
}

void ShoreFollowing::_activate(){
	// what do you need to do every time the task is activated?
	
	max_speed_time = _options["speed"]->getDoubleValue()/_options["max_acc"]->getDoubleValue();
	
	vel(0) = 0.0;
	vel(1) = 0.0;
	vel(2) = 0.0;
	acc(0) = 0.0;
	acc(1) = 0.0;
	acc(2) = 0.0;
	
	last_elapsed = 0.0;
	
	pos = uav->position();
	Eigen::Quaterniond ori = uav->orientation();
	uri_base::quaternion_to_yaw(ori, yaw);
	
	start_t = ros::Time::now();
	
}

void ShoreFollowing::_deactivate(){
	// what do you need to do every time the task is deactivated?
}

void ShoreFollowing::get_mandatory_resources(ResourceVector &res){
	
	// to get the resources needed in this task, use the following method:
	//
	// ResourceType res; this declaration should be in the class definition in the header file
	// std::string iint("uri::name_of_the_resource");
	// res = (ResourceType*)res.get_resource_ptr(iint);
	
	std::string iint("uri_uav::IrisInterface");
	uav = (uri_uav::IrisInterface*)res.get_resource_ptr(iint);
	
	
	std::string lint("uri_sensors::LaserScanner");
	ls = (uri_sensors::LaserScanner*)res.get_resource_ptr(lint);

	//
	// if you have put res in the header file, you'll be able to use it in any other method of this class (except fo the constructor, which is executed first)
}

};



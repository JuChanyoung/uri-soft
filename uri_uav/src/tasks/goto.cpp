#include <iostream>
#include <vector>
//#include <thread>

#include "ros/ros.h"

#include <uri_base/angle_conversion.hpp>

#include <uri_uav/tasks/goto.hpp>

namespace uri_uav{

  
  
  

GotoTask::GotoTask():Task(){
	// You should keep this line on top and put the name of your task in it.
	// Not giving a name to your task will have an unpredictable behavior
	// and most likely will not work.
	_name = "uri_uav::GotoTask";
	
	// The class Task contains a field _options which you can use freely.
	// You can add (and pass options through the configuration file) with the following lines.
	// [ format: _options.addTYPEOption(OPTION_NAME, DEFAULT_VALUE); ]
	//
	// _options.addIntOption("name_option_1",15);
	_options.addDoubleOption("max_acc", 0.25);
	_options.addDoubleOption("max_vel", 1.0);
	_options.addDoubleOption("max_yawrate", 0.25);
	_options.addDoubleOption("goal_x", 26.0);
	_options.addDoubleOption("goal_y", 26.0);
	_options.addDoubleOption("goal_z", 3.0);
	_options.addDoubleOption("goal_vx", 0.0);
	_options.addDoubleOption("goal_vy", 0.0);
	_options.addDoubleOption("goal_vz", 0.0);
	_options.addDoubleOption("goal_yaw", 0.0);
	_options.addBoolOption("relative_commands", false);
	// _options.addBoolOption("name_option_3",false);
	// _options.addStringOption("name_option_2","default_value");
	//
	// Note that options are only of those types listed here.
	// The first parameter in the above lines is the name, while the second parameter is the default value.
	// If the option is specified in the config file, the value of the option is automatically updated.
	// If the option specfied is not in the config_file, the option will have the default value
	// note that the options are updated after the execution of this constructor, so any option in this
	// construction will have its default value.
	
}


TaskOutput GotoTask::_run(){
	
	Eigen::Vector3d nextpos = _start;
	Eigen::Vector3d nextvel;
	Eigen::Vector3d nextacc;
	double next_yaw;
	
	if (t3 >0){
		switch (_stage) {
			
			case GOTO_START:
				if (uav->guided()){
					_stage = GOTO_ACCELERATION;
					_start_time = ros::Time::now();
				}
				break;
				
			case GOTO_ACCELERATION: {
	// 			std::cout << "GOTO_ACCELERATION" << std::endl;
				double t_elapsed = (ros::Time::now() - _start_time).toSec();
				
				nextpos(0) = _start(0) + _startvel(0)*t_elapsed + 0.5*max_x_acc*(t_elapsed*t_elapsed);
				nextpos(1) = _start(1) + _startvel(1)*t_elapsed + 0.5*max_y_acc*(t_elapsed*t_elapsed);
				nextpos(2) = _start(2) + _startvel(2)*t_elapsed + 0.5*max_z_acc*(t_elapsed*t_elapsed);
				
				nextvel(0) = _startvel(0) + max_x_acc*(t_elapsed);
				nextvel(1) = _startvel(1) + max_y_acc*(t_elapsed);
				nextvel(2) = _startvel(2) + max_z_acc*(t_elapsed);
				
				nextacc(0) = max_x_acc;
				nextacc(1) = max_y_acc;
				nextacc(2) = max_z_acc;
				
				next_yaw = _start_yaw + _yawrate*t_elapsed;
				
				if (t_elapsed>=t1) _stage=GOTO_CONSTANT_SPEED;
				break;
			}
			
			case GOTO_CONSTANT_SPEED: {
	// 			std::cout << "GOTO_CONSTANT_SPEED" << std::endl;
				double t_elapsed = (ros::Time::now() - _start_time).toSec();
				
				nextpos(0) = x1 + max_x_vel*(t_elapsed-t1);
				nextpos(1) = y1 + max_y_vel*(t_elapsed-t1);
				nextpos(2) = z1 + max_z_vel*(t_elapsed-t1);
				
				nextvel(0) = max_x_vel;
				nextvel(1) = max_y_vel;
				nextvel(2) = max_z_vel;
				
				nextacc(0) = 0;
				nextacc(1) = 0;
				nextacc(2) = 0;
				
				next_yaw = _start_yaw + _yawrate*t_elapsed;
				
				if (t_elapsed>=t2) _stage=GOTO_DECELERATION;
				break;
			}
				
			case GOTO_DECELERATION: {
	// 			std::cout << "GOTO_DECELERATION" << std::endl;
				double t_elapsed = (ros::Time::now() - _start_time).toSec();
				
				nextpos(0) = x2 + max_x_vel*(t_elapsed - t2) - 0.5*max_x_acc*(t_elapsed - t2)*(t_elapsed - t2);
				nextpos(1) = y2 + max_y_vel*(t_elapsed - t2) - 0.5*max_y_acc*(t_elapsed - t2)*(t_elapsed - t2);
				nextpos(2) = z2 + max_z_vel*(t_elapsed - t2) - 0.5*max_z_acc*(t_elapsed - t2)*(t_elapsed - t2);
				
				nextvel(0) = max_x_vel - max_x_acc*(t_elapsed - t2);
				nextvel(1) = max_y_vel - max_y_acc*(t_elapsed - t2);
				nextvel(2) = max_z_vel - max_z_acc*(t_elapsed - t2);
				
				nextacc(0) = - max_x_acc;
				nextacc(1) = - max_y_acc;
				nextacc(2) = - max_z_acc;
				
				next_yaw = _start_yaw + _yawrate*t_elapsed;
				
				if (t_elapsed>=t3) {
					_stage=GOTO_END;
					if (t_elapsed>=ty) return Terminate;
				}
				break;
			}
			
			case GOTO_END: {
				double t_elapsed = (ros::Time::now() - _start_time).toSec();
				
				next_yaw = _start_yaw + _yawrate*t_elapsed;
	// 			std::cout << "GOTO_END " << _yawrate << " " << next_yaw << std::endl;
				
				if (t_elapsed>=ty) return Terminate;
				break;
			}
				
			default:
				_stage = GOTO_START;
				break;
		}
	}
	
	else { // if t3 == 0 => yaw rotation only
				switch (_stage) {
			
			case GOTO_START:
				if (uav->guided()){
					_stage = GOTO_CONSTANT_SPEED;
					_start_time = ros::Time::now();
				}
				break;
				
			case GOTO_CONSTANT_SPEED: {
	// 			std::cout << "GOTO_ACCELERATION" << std::endl;
				double t_elapsed = (ros::Time::now() - _start_time).toSec();
				
				nextpos(0) = _start(0);
				nextpos(1) = _start(1);
				nextpos(2) = _start(2);
				
				nextvel(0) = _startvel(0);
				nextvel(1) = _startvel(1);
				nextvel(2) = _startvel(2);
				
				nextacc(0) = 0;
				nextacc(1) = 0;
				nextacc(2) = 0;
				
				next_yaw = _start_yaw + _yawrate*t_elapsed;
				
				if (t_elapsed>=t1) _stage=GOTO_END;
				break;
			}
			
			case GOTO_END: {
				double t_elapsed = (ros::Time::now() - _start_time).toSec();
				
				next_yaw = _start_yaw + _yawrate*t_elapsed;
				
				if (t_elapsed>=ty) return Terminate;
				break;
			}
				
			default:
				_stage = GOTO_START;
				break;
		}
	}
	
	
// 	std::cout << nextpos.transpose() << " " << nextvel.transpose() << " " << nextacc.transpose() << " " << next_yaw << " " << _yawrate << " " << t_elapsed << std::endl;
	
	uri_base::Trajectory traj;
	traj.pos = nextpos;
	traj.vel = nextvel;
	traj.acc = nextacc;
	
	traj.yaw = next_yaw;
// // 	std::cout << "y " <<  traj.yaw << std::endl;
	traj.yawrate = 0.0/*_yawrate*/;
	
	trajectory->set(traj, 0.01);
	
	return Continue;
}



void GotoTask::_activate(){
	
	_start = uav->position();
	if ( _options["relative_commands"]->getBoolValue() ){
		_goal(0) = _start(0) + _options["goal_x"]->getDoubleValue();
		_goal(1) = _start(1) + _options["goal_y"]->getDoubleValue();
		_goal(2) = _start(2) + _options["goal_z"]->getDoubleValue();
	}
	else {
		_goal(0) = _options["goal_x"]->getDoubleValue();
		_goal(1) = _options["goal_y"]->getDoubleValue();
		_goal(2) = _options["goal_z"]->getDoubleValue();
	}
	
	_start_yaw = uav->yaw();
	_goal_yaw = _options["goal_yaw"]->getDoubleValue();
	
	ROS_INFO("Current : (%f, %f, %f).", _start(0), _start(1), _start(2));
	ROS_INFO("New goal: (%f, %f, %f).", _goal(0), _goal(1), _goal(2));
	ROS_INFO("Current yaw: %f", _start_yaw);
	ROS_INFO("Goal yaw   : %f", _goal_yaw);
	
	_startvel << 0,0,0;
	_stage = GOTO_START;
	
	_direction = _goal-_start;
	
	if (_direction.norm() > 0.0){
		_direction = _direction/_direction.norm();
		
		max_speed = _options["max_vel"]->getDoubleValue();
		max_acc = _options["max_acc"]->getDoubleValue();
		bool good_trajectory = false;
		
		while(not good_trajectory){
			max_x_vel = max_speed*_direction(0);
			max_y_vel = max_speed*_direction(1);
			max_z_vel = max_speed*_direction(2);
			
			max_x_acc = max_acc*_direction(0);
			max_y_acc = max_acc*_direction(1);
			max_z_acc = max_acc*_direction(2);
			
// 			std::cout << "A " << max_speed << " " << _direction(0) << " " << _direction(1) << " " << _direction(2)  << std::endl; 
			if (std::abs<double>(_direction(0))>=std::abs<double>(_direction(1)) and std::abs<double>(_direction(0)) >= std::abs<double>(_direction(2))){
// 				std::cout << "A1" << std::endl; 
				t1 = (max_x_vel-_startvel(0))/max_x_acc;
				x1 = _start(0) + 0.5*t1*(_startvel(0) + max_x_vel);
				y1 = _start(1) + 0.5*t1*(_startvel(1) + max_y_vel);
				z1 = _start(2) + 0.5*t1*(_startvel(2) + max_z_vel);
				
				x2 = _goal(0) - (_goalvel(0)*_goalvel(0) - max_x_vel*max_x_vel)/(-2*max_x_acc);
				y2 = _start(1) *( _goal(0) - x2 ) / ( _goal(0)  - _start(0) ) + _goal(1) * ( x2 - _start(0) ) / ( _goal(0)  - _start(0) ) ;
				z2 = _start(2) *( _goal(0) - x2 ) / ( _goal(0)  - _start(0) ) + _goal(2) * ( x2 - _start(0) ) / ( _goal(0)  - _start(0) ) ;
				
				t2 = t1 + (x2 - x1)/max_x_vel;
				t3 = t2 + (_goalvel(0)-max_x_vel)/(-max_x_acc);
			}
			else if (std::abs<double>(_direction(1))>=std::abs<double>(_direction(0)) and std::abs<double>(_direction(1)) >= std::abs<double>(_direction(2))){
// 				std::cout << "A2" << std::endl; 
				t1 = (max_y_vel-_startvel(1))/max_y_acc;
				x1 = _start(0) + 0.5*t1*(_startvel(0) + max_x_vel);
				y1 = _start(1) + 0.5*t1*(_startvel(1) + max_y_vel);
				z1 = _start(2) + 0.5*t1*(_startvel(2) + max_z_vel);
				
				y2 = _goal(1) - (_goalvel(1)*_goalvel(1) - max_y_vel*max_y_vel)/(-2*max_y_acc);
				x2 = _start(0) *( _goal(1) - y2 ) / ( _goal(1)  - _start(1) ) + _goal(0) * ( y2 - _start(1) ) / ( _goal(1)  - _start(1) ) ;
				z2 = _start(2) *( _goal(1) - y2 ) / ( _goal(1)  - _start(1) ) + _goal(2) * ( y2 - _start(1) ) / ( _goal(1)  - _start(1) ) ;
				
				t2 = t1 + (y2 - y1)/max_y_vel;
				t3 = t2 + (_goalvel(1)-max_y_vel)/(-max_y_acc);
			}
			else if (std::abs<double>(_direction(2))>=std::abs<double>(_direction(0)) and std::abs<double>(_direction(2)) >= std::abs<double>(_direction(1))){
// 				std::cout << "A3" << std::endl; 
				t1 = (max_z_vel-_startvel(2))/max_z_acc;
				x1 = _start(0) + 0.5*t1*(_startvel(0) + max_x_vel);
				y1 = _start(1) + 0.5*t1*(_startvel(1) + max_y_vel);
				z1 = _start(2) + 0.5*t1*(_startvel(2) + max_z_vel);
				
				z2 = _goal(2) - (_goalvel(2)*_goalvel(2) - max_z_vel*max_z_vel)/(-2*max_z_acc);
				x2 = _start(0) *( _goal(2) - z2 ) / ( _goal(2)  - _start(2) ) + _goal(0) * ( z2 - _start(2) ) / ( _goal(2)  - _start(2) ) ;
				y2 = _start(1) *( _goal(2) - z2 ) / ( _goal(2)  - _start(2) ) + _goal(1) * ( z2 - _start(2) ) / ( _goal(2)  - _start(2) ) ;
				
				t2 = t1 + (z2 - z1)/max_z_vel;
				t3 = t2 + (_goalvel(2)-max_z_vel)/(-max_z_acc);
			}
		
			if (t3>t2 and t2>t1){
				good_trajectory = true;
			}
			else{
				max_speed = max_speed*0.95;
			}
// 			std::cout << "t1 " << t1  << " t2 " << t2 << " t3 " << t3 << std::endl; 
		}
		
		double yawdiff = _goal_yaw - _start_yaw;
		_yawrate = _options["max_yawrate"]->getDoubleValue();
		ty = std::abs<double>(yawdiff/_yawrate);
		if (ty<t3){
			_yawrate = yawdiff/t3;
		}
	}
	
	else {
		t1=0.0;
		t2=0.0;
		t3=0.0;
		double yawdiff = _goal_yaw - _start_yaw;
		_yawrate = _options["max_yawrate"]->getDoubleValue();
		ty = std::abs<double>(yawdiff/_yawrate);
		
		max_x_vel = 0.0;
		max_y_vel = 0.0;
		max_z_vel = 0.0;
		
		max_x_acc = 0.0;
		max_y_acc = 0.0;
		max_z_acc = 0.0;
	}
	
	std::cout << "t1 " << t1  << " t2 " << t2 << " t3 " << t3 << " ty " << ty << " x1 " << x1 << " x2 " << x2 << " x3 " << _goal(0) << std::endl; 
// 	
// 	if (t2<t1){
// 		std::cout << "GOTO task problem! t2<t1 " << std::endl;
// 	}
	// what do you need to do every time the task is activated?
}

void GotoTask::_deactivate(){
	// what do you need to do every time the task is deactivated?
}

void GotoTask::get_mandatory_resources(ResourceVector &res){
	
	// to get the resources needed in this task, use the following method:
	//
	// ResourceType res; this declaration should be in the class definition in the header file
	std::string iint("uri_uav_resources::IrisInterface");
	uav = (uri_uav_resources::IrisInterface*)res.get_resource_ptr(iint);
	//
	// if you have put res in the header file, you'll be able to use it in any other method of this class (except fo the constructor, which is executed first)
	iint = std::string("uri_base::SharedMemory<uri_base::Trajectory>");
	trajectory = (uri_base::SharedMemory<uri_base::Trajectory>*)res.get_resource_ptr(iint);
	
	
}

};



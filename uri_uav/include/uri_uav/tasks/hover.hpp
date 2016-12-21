
#include <pluginlib/class_list_macros.h>
#include <uri_core/task.hpp>

#include <uri_uav/iris_interface.hpp>
#include <uri_base/trajectory.hpp>

#ifndef __HOVER_HPP__
#define __HOVER_HPP__



namespace uri_uav{
	
	class Hover: public Task{
		
	private:
		
		IrisInterface* uav;
		uri_base::SharedMemory<uri_base::Trajectory>* trajectory;
		
		virtual TaskOutput _run();
		
		virtual void _initialize(){}
		
		virtual void _activate(){
		}

		virtual void _deactivate(){}
		
		bool _first_run;
		
		
		Eigen::Vector3d _pos;
		Eigen::Vector3d _vel;
		Eigen::Vector3d _acc;
		
		double _yaw;
		double _yawrate;
		
		
		
		Eigen::Vector3d _pos_s;
		double _yaw_s;
		ros::Time _time_s;
		
		
	public:
		
		Hover();
		
		void get_mandatory_resources(ResourceVector& res);
		
	};

  PLUGINLIB_EXPORT_CLASS(uri_uav::Hover, uri::Task)
	
	
};


#endif
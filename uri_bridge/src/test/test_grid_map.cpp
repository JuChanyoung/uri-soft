#include <uri_bridge/bridge_map.hpp>
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

#include <fstream>

// int main(void){
// 	
// 	uri_bridge::GridMap map;
// 	
// // 	map.print();
// 	std::cout << map;
// 	
// 	
// 	return 0;
// }




int main(int argc, char** argv){
	
	
   ros::init(argc, argv, "test");
	 ros::NodeHandle n;
	
	
	std::fstream fin("/home/paolos/Desktop/data_ridot/mapscan_polar.txt", std::fstream::in);
	
	

	unsigned int num_readings = 360;
	
	fin >> num_readings;
	
// 	std::cout << "num readings " << std::endl;
	
	double laser_frequency = 40;
	
	//populate the LaserScan message
	sensor_msgs::LaserScan scan;
	
	
// 	scan.header.stamp = scan_time;
	scan.header.frame_id = "laser_frame";
	scan.angle_min = -M_PI;
	scan.angle_max = M_PI;
	
	fin >> scan.angle_min;
	fin >> scan.angle_max;
	
	
	scan.angle_increment = 2*M_PI / num_readings;
	scan.time_increment = (1 / laser_frequency) / (num_readings);
	scan.range_min = 0.0;
	scan.range_max = 10.0;

	scan.ranges.resize(num_readings);
// 	scan.intensities.resize(num_readings);
	double angle = scan.angle_min;
	for(unsigned int i = 0; i < num_readings; ++i){
		
		fin >> scan.ranges[i];
// 		scan.ranges[i] = std::min(3.5 + 1.3*std::sin(4.5*angle), (double)scan.range_max);
// 		angle += scan.angle_increment;
	}
	
// 		std::cout << "a" << std::endl;
	ros::Time start = ros::Time::now();
	
	// create a node
	Eigen::Vector2d pos; pos(0) = 0; pos(1) = -0;
	uri_bridge::GridMapParams gmp(pos, 0.05, 0.05, 10.80, 10.80, 0.31, 0.5);
	uri_bridge::GridMap node(nullptr, gmp);
	
	// save pointer to current node
	uri_bridge::GridMap* current_node = node.ptr();
	
	// import the scan
	node.import_scan(scan);
	// output
	std::cout << ( ros::Time::now() - start ).toSec() << std::endl ;
	
	///////////////////////////////////////////////////////////
	// here we'll test the creation of a child node
	// selection of new waypoint (provided as cell in the gridmap)
	cv::Point2i new_wp_cell = node.select_next_waypoint();
	
	// transform the cell in world coordinates
	Eigen::Vector2d pos2;
	node.cell_to_cartesian_global(new_wp_cell.x, new_wp_cell.y, pos2(0), pos2(1));
	std::cout << " aaaa "<< pos2 << std::endl;
	
	
	uri_bridge::GridMapParams gmp2(pos2, 0.05, 0.05, 10.80, 10.80, 0.31, 0.5);
	std::cout << " b1"<< pos2 << std::endl;
	uri_bridge::GridMap* new_child = node.create_new_child(gmp2, scan);
	std::cout << " c1"<< pos2 << std::endl;
	node.update_node(new_child);
	std::cout << " d1"<< pos2 << std::endl;
	
	node.show_grid_map(2,0.1, "node1");
	std::cout << " e1"<< pos2 << std::endl;
	node.print_stats();
	std::cout << " f1"<< pos2 << std::endl;
	
	new_child->show_grid_map(2,0, "node2");
	std::cout << " g1"<< pos2 << std::endl;
	new_child->print_stats();
	std::cout << " h1"<< pos2 << std::endl;
	
// 	int a, b;
// 	node.cartesian_local_to_cell(0,0,a,b);
// 	std::cout << " 0  0  " << a << " " << b << std::endl;
// 	node.cartesian_local_to_cell(1,1,a,b);
// 	std::cout << " 1  1  " << a << " " << b << std::endl;
// 	node.cartesian_local_to_cell(4,4,a,b);
// 	std::cout << " 4  4  " << a << " " << b << std::endl;
// 	node.cartesian_local_to_cell(8,8,a,b);
// 	std::cout << " 8  8  " << a << " " << b << std::endl;
// 	node.cartesian_local_to_cell(10.8,10.8,a,b);
// 	std::cout << " 10.8  10.8  " << a << " " << b << std::endl;
// 	node.cartesian_local_to_cell(-5,-5,a,b);
// 	std::cout << " -5  -5  " << a << " " << b << std::endl;
// 	node.cartesian_local_to_cell(-10.8,-10.8,a,b);
// 	std::cout << " -10.8 -10.8  " << a << " " << b << std::endl;
	
}


#include "ros/ros.h"
#include "std_msgs/String.h"
#include <tf/transform_listener.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char **argv) {
  ros::init(argc, argv, "my_tf_listener");
  ros::NodeHandle node;
  tf::TransformListener listener;

  int file_idx = 1;
  string input_command = " ";
  string file_path = "/home/msjun-msi/catkin_ws/src/waypoint_publisher/waypoints/waypoint_txt" + to_string(file_idx) + ".txt";
  ofstream write_file;
  write_file.open(file_path, ios::app);
  ROS_INFO("For Robot%d", file_idx);

  ros::Rate rate(10.0);
  while (node.ok()){
    tf::StampedTransform transform;
    try{
      listener.lookupTransform("/map", "/base_footprint",
                                ros::Time(0), transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }

    cin >> input_command;
    if (input_command == "s") {
      // save at memo
      if (write_file.is_open()) {
        ROS_INFO("---");
        ROS_INFO("For Robot %d", file_idx);
        ROS_INFO("frame id : ", transform.frame_id_.c_str());
        ROS_INFO("chile frame id : ", transform.child_frame_id_.c_str());
        ROS_INFO("translastion");
        ROS_INFO("x : %lf", transform.getOrigin().x());
        ROS_INFO("y : %lf", transform.getOrigin().y());
        ROS_INFO("z : %lf", transform.getOrigin().z());
        ROS_INFO("");
        ROS_INFO("rotation");
        ROS_INFO("x : %lf", transform.getRotation().x());
        ROS_INFO("y : %lf", transform.getRotation().y());
        ROS_INFO("z : %lf", transform.getRotation().z());
        ROS_INFO("w : %lf", transform.getRotation().w());
        ROS_INFO("");
        
        write_file << to_string(transform.getOrigin().x()) << " ";
        write_file << to_string(transform.getOrigin().y()) << " ";
        write_file << to_string(transform.getOrigin().z()) << " ";

        write_file << to_string(transform.getRotation().x()) << " ";
        write_file << to_string(transform.getRotation().y()) << " ";
        write_file << to_string(transform.getRotation().z()) << " ";
        write_file << to_string(transform.getRotation().w()) << " ";

        write_file << "\n";
      }
    }
    else if (input_command == "n") {
      write_file.close();

      file_idx++;
      file_path = "/home/msjun-msi/catkin_ws/src/waypoint_publisher/waypoints/waypoint_txt" + to_string(file_idx) + ".txt";
      write_file.open(file_path, ios::app);
      ROS_INFO("For Robot%d", file_idx);
    }
    else if (input_command == "q") {
      break;
    }

    rate.sleep();
  }

  write_file.close();
  return 0;
}

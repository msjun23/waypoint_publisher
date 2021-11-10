#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct curr_point {
    float pose_x;
    float pose_y;
    float pose_z;

    float orien_x;
    float orien_y;
    float orien_z;
    float orien_w;
};

vector<curr_point> waypoints;

string reach_msg = "reach";
void ReachSub(const std_msgs::String::ConstPtr& msg) {
    reach_msg = msg->data.c_str();
    ROS_INFO("REACH SUBSCRIBE!!!");
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "robot_pose_pub");
    ros::NodeHandle sn;
    ros::Subscriber reached = sn.subscribe("/reach", 10, ReachSub);
    ros::NodeHandle pn("move_base_simple");
    ros::Publisher waypoint_pub = pn.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);

    int ns;
    if (ros::param::get("~/ns", ns));
    else {ns = 3;}

    string file_path = "/home/msjun-msi/catkin_ws/src/waypoint_publisher/waypoints/waypoint_txt" + to_string(ns) + ".txt";

    ifstream open_file(file_path.data());
    string str_buff = " ";
    // 처음 한 줄은 쓰레기
    getline(open_file, str_buff);
    while (getline(open_file, str_buff)) {
        istringstream ss(str_buff);
        string next_goal;
        
        curr_point temp;
        getline(ss, next_goal, ' ');
        temp.pose_x = stof(next_goal);
        getline(ss, next_goal, ' ');
        temp.pose_y = stof(next_goal);
        getline(ss, next_goal, ' ');
        temp.pose_z = stof(next_goal);

        getline(ss, next_goal, ' ');
        temp.orien_x = stof(next_goal);
        getline(ss, next_goal, ' ');
        temp.orien_y = stof(next_goal);
        getline(ss, next_goal, ' ');
        temp.orien_z = stof(next_goal);
        getline(ss, next_goal, ' ');
        temp.orien_w = stof(next_goal);

        waypoints.push_back(temp);
        // ROS_INFO("debug %d [%f %f %f %f]", waypoints.size(), waypoints.at(waypoints.size() - 1).pose_x, 
        //                                                      waypoints.at(waypoints.size() - 1).pose_y, 
        //                                                      waypoints.at(waypoints.size() - 1).orien_z, 
        //                                                      waypoints.at(waypoints.size() - 1).orien_w);
    }

    geometry_msgs::PoseStamped waypoint;
    int waypoint_idx = -1;
    int check = 0;
    bool pub_bool = false;
    int pub_count = 0;
    ros::Rate loop_rate(1);
    while (ros::ok()) {
        // Running mode
        if(reach_msg == "reach"){
            ROS_INFO("Robot%d reach!", ns);
            check = (check + 1) % 3;
            if(check == 1){
                waypoint_idx++;
                if (waypoint_idx >= waypoints.size()) {
                    waypoint_idx = 0;
                }
                ROS_INFO("Robot%d`s next goal index : %d", ns, waypoint_idx);
                reach_msg = "not";

                waypoint.header.frame_id = "map";
                waypoint.pose.position.x = waypoints.at(waypoint_idx).pose_x;
                waypoint.pose.position.y = waypoints.at(waypoint_idx).pose_y;
                waypoint.pose.orientation.z = waypoints.at(waypoint_idx).orien_z;
                waypoint.pose.orientation.w = waypoints.at(waypoint_idx).orien_w;
                pub_bool = true;
                // ROS_INFO("%f\n%f\n%f\n%f\n", waypoints.at(waypoint_idx).pose_x, 
                //                              waypoints.at(waypoint_idx).pose_y, 
                //                              waypoints.at(waypoint_idx).orien_z, 
                //                              waypoints.at(waypoint_idx).orien_w);
            }
        }
        if(pub_bool){
            waypoint_pub.publish(waypoint);
            pub_count++;
        }
        if(pub_count > 5){
            pub_count = 0;
            pub_bool = false;
        }

        ros::spinOnce();
        loop_rate.sleep();
    }

    open_file.close();
    return 0;
}
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct curr_point {
    float x;
    float y;
    float z;
    float w;
};
bool _subGoal_enable = true;

vector<curr_point> waypoints;

std_msgs::String msg_;
void reach_sub(std_msgs::String msg) {
    msg_ = msg;
}

string mode_ = "";
void KeyboardSub(const std_msgs::String::ConstPtr& msg) {
    mode_ = msg->data.c_str();
}

void GetGoalPose(const geometry_msgs::PoseStamped& msg) {
    //map_frame -> base_footprint
    if(_subGoal_enable){
        ROS_INFO("get goal");
        curr_point temp;
        temp.x = msg.pose.position.x;
        temp.y = msg.pose.position.y;
        temp.z = msg.pose.orientation.z;
        temp.w = msg.pose.orientation.w;

        waypoints.push_back(temp);
    }
    //ROS_INFO("%f\n%f\n%f\n%f\n", temp.x, temp.y, temp.z, temp.w);
}

int main(int argc, char **argv)
{
    int waypoint_idx = 0;

    ros::init(argc, argv, "waypoint_repeat");
    ros::NodeHandle sn;
    ros::Subscriber reached = sn.subscribe("reach", 10, reach_sub);
    ros::NodeHandle sn2;
    ros::Subscriber key_input_sub = sn2.subscribe("/key_input", 10, KeyboardSub);
    ros::NodeHandle sn3;
    ros::Subscriber goal_sub = sn3.subscribe("/move_base_simple/goal", 10, GetGoalPose);
    
    ros::NodeHandle pn("move_base_simple");
    ros::Publisher waypoint_pub = pn.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);
    geometry_msgs::PoseStamped waypoint;

    msg_.data = "reach";
    int check = 0;
    ros::Rate loop_rate(0.3);
    while (ros::ok())
    {
        ROS_INFO("Debug");
        if (mode_ == "start") { 
            _subGoal_enable = false;
            // Running mode
            if(msg_.data == "reach"){
                ROS_INFO("reach!");
                check = (check + 1) % 2;
                if(check == 1){
                    waypoint_idx++;
                    if (waypoint_idx >= waypoints.size()) {
                        waypoint_idx = 0;
                    }
                    ROS_INFO("index : %d", waypoint_idx);
                    //ROS_INFO("%d", waypoint_idx);
                    msg_.data = "not";

                    waypoint.header.frame_id = "map";
                    waypoint.pose.position.x = waypoints.at(waypoint_idx).x;
                    waypoint.pose.position.y = waypoints.at(waypoint_idx).y;
                    waypoint.pose.orientation.z = waypoints.at(waypoint_idx).z;
                    waypoint.pose.orientation.w = waypoints.at(waypoint_idx).w;
                    // ROS_INFO("%f\n%f\n%f\n%f\n", waypoints.at(waypoint_idx).x, 
                    //                              waypoints.at(waypoint_idx).y, 
                    //                              waypoints.at(waypoint_idx).z, 
                    //                              waypoints.at(waypoint_idx).w);
                }
            }

            waypoint_pub.publish(waypoint);
        }
        else {
            // Making waypoints...
            if (msg_.data == "reach") {
                ROS_INFO("save waypoint, debug");
            }
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}

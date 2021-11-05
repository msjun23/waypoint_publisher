#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"
//#include "actionlib_msgs.h"

#include <iostream>
#include <string>

using namespace std;

static float w = 0.9;

static float waypoints[34][4] = {{1.39288163185, 0.501529932022, 0.0, w}, 
{1.44538664818, 0.264548838139, 0.0, w}, 
{1.44428741932, 0.0637175738811, 0.0, w}, 
{1.45748591423, -0.271748185158, 0.0, w}, 
{1.45677947998, -0.447296530008, 0.0, w}, 
{1.20461630821, 0.479806780815, 0.0, w}, 
{1.24384105206, 0.202786386013, 0.0, w}, 
{1.22377765179, -0.0320476442575, 0.0, w}, 
{1.28410375118, -0.265572547913, 0.0, w}, 
{1.24308216572, -0.490028172731, 0.0, w}, 
{0.956461668015, 0.465537875891, 0.0, w}, 
{0.938845098019, 0.184643223882, 0.0, w}, 
{0.97351193428, -0.0211029648781, 0.0, w}, 
{0.945072174072, -0.241790115833, 0.0, w}, 
{1.00309848785, -0.513796269894, 0.0, w}, 
{0.70426428318, 0.459053784609, 0.0, w}, 
{0.728764176369, 0.211073637009, 0.0, w}, 
{0.727212071419, -0.288311779499, 0.0, w}, 
{0.76317024231, -0.528119623661, 0.0, w}, 
{0.442829400301, 0.455644965172, 0.0, w}, 
{0.512372553349, 0.161041527987, 0.0, w}, 
{0.481452345848, -0.0441848561168, 0.0, w}, 
{0.529084563255, -0.256915360689, 0.0, w}, 
{0.485272884369, -0.533475875854, 0.0, w}, 
{0.205370143056, 0.464855790138, 0.0, w}, 
{0.195748627186, 0.176556169987, 0.0, w}, 
{0.201441645622, -0.0688424408436, 0.0, w}, 
{0.216495752335, -0.324451714754, 0.0, w}, 
{0.247255802155, -0.517094135284, 0.0, w}, 
{0.0093994345516, 0.404471725225, 0.0, w}, 
{-0.0114145874977, 0.184398666024, 0.0, w}, 
{-0.00103088468313, -0.0696457698941, 0.0, w}, 
{0.0123437903821, -0.327161729336, 0.0, w}, 
{0.0471469461918, -0.541730999947, 0.0, w}, };

static float waypoints2[10][4] = {{0.339293897152, 0.315911471844, -0.739529463595, 0.673124188003}, 
{0.20833927393, -0.0210000872612, -0.708949449923, 0.705259298027}, 
{0.2345174402, -0.223268002272, -0.708917122933, 0.705291792673}, 
{0.201813220978, -0.400459587574, -0.708943816979, 0.7052649604}, 
{0.205417156219, -0.568574547768, -0.723496934069, 0.690327593533}, 
{0.234491378069, -0.657529592514, 0.956473860091, -0.291818016858}, 
{-0.110023662448, -0.505435466766, 0.824285098723, 0.56617495178}, 
{0.0190562456846, -0.19245198369, 0.70936960724, 0.704836690535}, 
{-0.0177500396967, 0.100400701165, 0.718351389823, 0.695680444413}, 
{0.0652136728168, 0.330965936184, 0.519730468986, 0.854330287189}};

string move_base_result = "Goal reached.";

// void MoveBaseResultSub(const std_msgs::GoalStatus::text& msg) {
//   ROS_INFO("I heard: [%s]", msg->data.c_str());
//   move_base_result = msg->data.c_str();
// }

std_msgs::String msg_;
void reach_sub(std_msgs::String msg){
  msg_ = msg;
}

int main(int argc, char **argv)
{
  float* path_1[10] = {0,};
  int waypoint_idx = 0;

  // path_1[0] = waypoints[24];
  // path_1[1] = waypoints[25];
  // path_1[2] = waypoints[26];
  // path_1[3] = waypoints[27];
  // path_1[4] = waypoints[28];
  // path_1[5] = waypoints[33];
  // path_1[6] = waypoints[32];
  // path_1[7] = waypoints[31];
  // path_1[8] = waypoints[30];
  // path_1[9] = waypoints[29];
  
  path_1[0] = waypoints2[0];
  path_1[1] = waypoints2[1];
  path_1[2] = waypoints2[2];
  path_1[3] = waypoints2[3];
  path_1[4] = waypoints2[4];
  path_1[5] = waypoints2[5];
  path_1[6] = waypoints2[6];
  path_1[7] = waypoints2[7];
  path_1[8] = waypoints2[8];
  path_1[9] = waypoints2[9];

  ros::init(argc, argv, "waypoint_pub");
  // ros::NodeHandle sn;
  // ros::Subscriber sub = sn.subscribe("/move_base/status", 1000, MoveBaseResultSub);
  // ros::spin();
  ros::NodeHandle sn;
  ros::Subscriber reached = sn.subscribe("reach",10,reach_sub);

  ros::NodeHandle pn("move_base_simple");
  ros::Publisher waypoint_pub = pn.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);
  geometry_msgs::PoseStamped waypoint;

  bool run_flag = true;
  msg_.data = "reach";
  ros::Rate loop_rate(2);
  while (ros::ok())
  {
    if(msg_.data == "reach"){
      ROS_INFO("reach!");
      waypoint_idx++;
      if (waypoint_idx >= 10) {
        waypoint_idx = 0;
      }
      ROS_INFO("%d", waypoint_idx);
      msg_.data = "not";

      waypoint.header.frame_id = "map";
      waypoint.pose.position.x = *path_1[waypoint_idx];
      waypoint.pose.position.y = *(path_1[waypoint_idx] + 1);
      waypoint.pose.orientation.z = *(path_1[waypoint_idx] + 2);
      waypoint.pose.orientation.w = *(path_1[waypoint_idx] + 3);
      waypoint_pub.publish(waypoint);
    }

    // if (run_flag && move_base_result == "Goal reached.") {
    //   run_flag = false;

    //   waypoint_idx++;
    //   if (waypoint_idx >= 10) {
    //     waypoint_idx = 0;
    //   }
    //   ROS_INFO("%d", waypoint_idx);
      
    //   waypoint.header.frame_id = "map";
    //   waypoint.pose.position.x = *path_1[waypoint_idx];
    //   waypoint.pose.position.y = *(path_1[waypoint_idx] + 1);
    //   waypoint.pose.orientation.z = *(path_1[waypoint_idx] + 2);
    //   waypoint.pose.orientation.w = *(path_1[waypoint_idx] + 3);
    //   waypoint_pub.publish(waypoint);
    //   ROS_INFO("Sibal!");
    // }
    // else if (!run_flag && move_base_result != "Goal reached.") {
    //   run_flag = true;
    // }
    // waypoint.header.frame_id = "map";
    // waypoint.pose.position.x = *path_1[1];
    // waypoint.pose.position.y = *(path_1[1] + 1);
    // waypoint.pose.orientation.z = *(path_1[1] + 2);
    // waypoint.pose.orientation.w = *(path_1[1] + 3);
    // waypoint_pub.publish(waypoint);

    waypoint_pub.publish(waypoint);

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}

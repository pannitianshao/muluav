#include "ros/ros.h"
#include <iostream>
#include "std_msgs/String.h"
#include <geometry_msgs/PoseStamped.h>


int main(int argc, char **argv)
{

  ros::init(argc, argv, "home_positon_set");
  ros::NodeHandle n_pub;

  ros::Publisher pub = n_pub.advertise<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 1000);
  ros::Rate loop_rate(10);
  geometry_msgs::PoseStamped msg;
  
   while(ros::ok())
    {
        std::cout<<"please input the X Y of the position\n ";
        std::cin>>msg.pose.position.x >>msg.pose.position.y;


        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }

  return 0;
}

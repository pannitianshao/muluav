#include <ros/ros.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/AttitudeTarget.h>
#include <mavros_msgs/PositionTarget.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/ActuatorControl.h>
#include <sensor_msgs/Imu.h>
#include <bitset>
#include <iostream>
#include <Eigen/Eigen>

using namespace std;

mavros_msgs::State current_state_uav0;
mavros_msgs::State current_state_uav1;


void uav0_state_sub_cb(const mavros_msgs::State::ConstPtr &msg)
{
    current_state_uav0 = *msg;
}

void uav1_state_sub_cb(const mavros_msgs::State::ConstPtr &msg)
{
    current_state_uav1 = *msg;
}


// void mode_select(string mode);
// void armCMD_select(bool CMD);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "formation_gcs_setting");
    ros::NodeHandle nh("~");

    ros::Subscriber uav0_state_sub = nh.subscribe<mavros_msgs::State>("/uav0/mavros/state", 10, uav0_state_sub_cb);
    ros::ServiceClient uav0_set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/uav0/mavros/set_mode");
    ros::ServiceClient uav0_arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/uav0/mavros/cmd/arming");

    ros::Subscriber uav1_state_sub = nh.subscribe<mavros_msgs::State>("/uav1/mavros/state", 10, uav1_state_sub_cb);
    ros::ServiceClient uav1_set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/uav1/mavros/set_mode");
    ros::ServiceClient uav1_arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/uav1/mavros/cmd/arming");

    mavros_msgs::SetMode uav0_mode_cmd;
    mavros_msgs::CommandBool uav0_arm_cmd;

    mavros_msgs::SetMode uav1_mode_cmd;
    mavros_msgs::CommandBool uav1_arm_cmd;

    ros::Rate rate(10.0);
    // 先读取一些飞控的数据
    int i =0;
    for(i=0;i<20;i++)
    {
        ros::spinOnce();
        rate.sleep();
    }
    int select_flag;
    int mode_flag;
    uint8_t timeout_count;
    string control_mode = "POSCTL";
    bool armed_cmd = true;
    while(ros::ok())
    {
        //执行回调函数
        ros::spinOnce();
        cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>gcs control<<<<<<<<<<<<<<<<<<<<<<<<<<<<< "<< endl;
        cout << "Input the selection:  0 for set mode,1 for arm or disarm..."<<endl;
        cin >> select_flag;

        if(select_flag == 0)
        {
            cout << "Input the mode:  0 for set _OFFBOARD_,1 for _POSCTL_"<<endl;
            cin >> mode_flag;
            switch(mode_flag)
            {
                case 0: control_mode = "OFFBOARD"; break;

                case 1: control_mode = "POSCTL"; break;

                default:cout << "Input error,please retry..."<<endl; cout << endl; break;
            }
            if(mode_flag < 2)//暂时设置两种模式
            {
                timeout_count= 0;
                cout << "Setting to "<<control_mode<<" Mode..." <<endl;
                ros::spinOnce();
                while(current_state_uav0.mode != control_mode && current_state_uav1.mode != control_mode)
                {
                    uav0_mode_cmd.request.custom_mode = control_mode;
                    uav0_set_mode_client.call(uav0_mode_cmd);

                    uav1_mode_cmd.request.custom_mode = control_mode;
                    uav1_set_mode_client.call(uav1_mode_cmd);


                    ros::spinOnce();
                    rate.sleep();
                    timeout_count++;
                    if(timeout_count > 10)
                        break;
                }
                if(timeout_count > 10)
                    cout << "Set to "<<control_mode<<" Mode Timeout!!!" <<endl;
                else
                    cout << "Set to "<<control_mode<<" Mode Susscess!!!" <<endl;
                cout << endl;
            } 
        }
        else if(select_flag == 1)
        {
            cout << "Input the CMD:  0 for Disarmed,1 for Armed"<<endl;
            cin >> mode_flag;
            switch(mode_flag)
            {
                case 0: armed_cmd = false; break;

                case 1: armed_cmd = true; break;
                default:cout << "Input error,please retry..."<<endl; break;
            }
            if(mode_flag < 2)
            {
                timeout_count = 0;
                if(armed_cmd)
                    cout << "Uav Armed..." <<endl;
                else
                    cout << "Uav Disarmed..." <<endl;
                ros::spinOnce();
                while(current_state_uav0.armed != armed_cmd && current_state_uav1.armed != armed_cmd)
                {
                    uav0_arm_cmd.request.value = armed_cmd;
                    uav0_arming_client.call(uav0_arm_cmd);

                    uav1_arm_cmd.request.value = armed_cmd;
                    uav1_arming_client.call(uav1_arm_cmd);

                    ros::spinOnce();
                    rate.sleep();
                    timeout_count++;
                    if(timeout_count > 10)
                        break;
                }
                if(timeout_count > 10)
                {
                    if(armed_cmd)
                        cout << "Armed Timeout!!!" <<endl;
                    else
                        cout << "Disarmed Timeout!!!" <<endl;
                    
                }
                else
                {
                    if(armed_cmd)
                        cout << "Armed Susscess!!!" <<endl;
                    else
                        cout << "Disarmed Susscess!!!" <<endl;
                }
                cout << endl;
            }
                
        }
        else
        {
            cout << "Input error,please retry..."<<endl;
            cout << endl;
        }
        
        rate.sleep();
    }


    return 0;
}




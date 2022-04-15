//
// Created by lab on 2022/4/14.
//

//
// Created by ypf on 2021/12/7.
//

// offboard example connected to Airsim

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/PositionTarget.h>
#include <std_msgs/Int8.h>
#include <quadrotor_msgs/PositionCommand.h>


mavros_msgs::State current_state;
std_msgs::Int8 avoid_state;
quadrotor_msgs::PositionCommand pos_cmd;

void ego_cb(const quadrotor_msgs::PositionCommandConstPtr& msg){
    pos_cmd = *msg;
}

void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}

void avoid_cb(const std_msgs::Int8::ConstPtr& msg){
    avoid_state = *msg;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;

    avoid_state.data = 0;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>
            ("mavros/state", 10, state_cb);
    ros::Publisher local_pos_pub = nh.advertise<mavros_msgs::PositionTarget>
            ("mavros/setpoint_raw/local", 10);

    ros::Subscriber ego_sub = nh.subscribe<quadrotor_msgs::PositionCommand>
            ("position_cmd", 10, ego_cb);
    ros::Subscriber avoid_sub = nh.subscribe<std_msgs::Int8>
            ("avoid/state", 10, avoid_cb);


    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
            ("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
            ("mavros/set_mode");

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(50.0);


    float xc0 = 0;
    float yc0 = 0;
    float zc0 = 2;

    // wait for FCU connection
    while(ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }

    mavros_msgs::PositionTarget pos_target_raw;

    pos_target_raw.coordinate_frame = 1;
    pos_target_raw.type_mask = 0b100111111000;
    pos_target_raw.position.x = xc0;
    pos_target_raw.position.y = yc0;
    pos_target_raw.position.z = zc0;
    pos_target_raw.yaw = 3.1415926/2;

    //send a few setpoints before starting
    for(int i = 100; ros::ok() && i > 0; --i){
        local_pos_pub.publish(pos_target_raw);
        ros::spinOnce();
        rate.sleep();
    }

    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

    ros::Time last_request = ros::Time::now();

    while(ros::ok()){
        if( current_state.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request > ros::Duration(5.0))){
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent){
                ROS_INFO("Offboard enabled");
                ROS_INFO("!");
            }
            last_request = ros::Time::now();
        } else {
            if( !current_state.armed &&
                (ros::Time::now() - last_request > ros::Duration(5.0))){
                if( arming_client.call(arm_cmd) &&
                    arm_cmd.response.success){
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }
        }

//        geometry_msgs::PoseStamped circle_pose;
//
//        circle_pose.pose.position.x = xc0 - r*sin(wn*counter*t_step);
//        circle_pose.pose.position.y = yc0 - r*cos(wn*counter*t_step);
//        circle_pose.pose.position.z = zc0;

        mavros_msgs::PositionTarget  ego_position;

        if (avoid_state.data == 0) {
            ego_position.coordinate_frame = 1;
            ego_position.type_mask = 0b100111111000;
            ego_position.position.x = xc0 ;
            ego_position.position.y = yc0 ;
            ego_position.position.z = zc0;
            ego_position.yaw = 3.1415926/2;
        }

        if (avoid_state.data == 99) {

            ego_position.coordinate_frame = 1;
            ego_position.type_mask = 0b100111111000;
            ego_position.position.x = pos_cmd.position.x ;
            ego_position.position.y = pos_cmd.position.y ;
            ego_position.position.z = pos_cmd.position.z ;
            ego_position.yaw = 3.1415926/2;

        }

        local_pos_pub.publish(ego_position);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}





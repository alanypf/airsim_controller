//
// Created by ypf on 2021/12/7.
//

// offboard example connected to Airsim

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>

float hmax = 2.5;
float hmin = 0.8;
float rmax = 2;
float rmin = 0.8;
bool up = true;
bool down = false;
float detH = 0;
float detR = 0;
float h_sp = hmin;

float n_circle = 5;



mavros_msgs::State current_state;
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>
            ("mavros/state", 10, state_cb);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>
            ("mavros/setpoint_position/local", 10);
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
            ("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
            ("mavros/set_mode");

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    //
    double circle_rate = 60;
    double t_step = 1.0/ circle_rate;

    // parameters for circle
    double wn = 0.6;
    double r = 1;
    double counter = 0;

    // circle center
    float xc0 = 0;
    float yc0 = 0;
    float zc0 = hmin;

    h_sp = zc0;

    detH = (hmax - hmin) * (wn * t_step) / (2 * 3.1415926) /n_circle;
    detR = (rmax - rmin) * (wn * t_step) / (2 * 3.1415926) /n_circle;


    // wait for FCU connection
    while(ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }

    geometry_msgs::PoseStamped pose;
    pose.pose.position.x = xc0;
    pose.pose.position.y = yc0;
    pose.pose.position.z = zc0;

    //send a few setpoints before starting
    for(int i = 100; ros::ok() && i > 0; --i){
        local_pos_pub.publish(pose);
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

        geometry_msgs::PoseStamped circle_pose;

        
        if (h_sp > hmax)
        {
            down = true;
            up = false;
            ROS_INFO("down is: %d\n",down);
        }
        else if (h_sp < hmin)
        {
            up = true;
            down = false;
            ROS_INFO("up is: %d\n",up);
        }

        if(up){
            h_sp = h_sp + detH;
            r = r + detR;
            ROS_INFO("h_sp is: %f\n",h_sp);
            ROS_INFO("detH is: %f\n",detH);
            ROS_INFO("r is: %f\n",r);
        }
        if(down){
            h_sp = h_sp - detH;
            r = r - detR;
            ROS_INFO("r is: %f\n",r);
        }

        if(r>2.8){
            r = 2.8;
        }
        if (h_sp > 3)
        {
            h_sp = 3;
        }
        

        circle_pose.pose.position.x = r*sin(wn*counter*t_step);
        circle_pose.pose.position.y = r*cos(wn*counter*t_step);
        circle_pose.pose.position.z = h_sp;


        local_pos_pub.publish(circle_pose);

        ros::spinOnce();
        counter++;
        if(counter >= 100 * (2 * 3.1415926) / (wn * t_step)){
            counter = 0;
        }
        rate.sleep();
    }

    return 0;
}



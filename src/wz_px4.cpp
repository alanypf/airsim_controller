#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include "math.h"



int main(int argc, char *argv[])
{
    /* code for main function */
    float wn = 1;
    float r = 0.8;

    int flag = 0;
    int counterc0 = 0;
    int counterc1 = 0;

    float xc0 = 0;
    float yc0 = 0;
    float zc0 = 1;

    float xc1 = xc0;
    float yc1 = yc0 - 2*r;

    float zc1 = zc0;


    ros::init(argc, argv, "whoop_8");
    ros::NodeHandle nh;

    
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local",10);


    ros::Rate rate = 50;



    geometry_msgs::PoseStamped d_pos_sp;
    d_pos_sp.pose.position.x = xc0;
    d_pos_sp.pose.position.y = yc0 - r;
    d_pos_sp.pose.position.z = zc0;

    for(int i = 1000; ros::ok() && i>0; i--){
        local_pos_pub.publish(d_pos_sp);
        ros::spinOnce();
        rate.sleep();
    }



    while(ros::ok()){         

            geometry_msgs::PoseStamped c_pos_sp;

            switch(flag){
                case 0:
                        
                        c_pos_sp.pose.position.x = xc0 - r*sin(wn*0.01*counterc0);
                        c_pos_sp.pose.position.y = yc0 - r*cos(wn*0.01*counterc0);
                        c_pos_sp.pose.position.z = zc0;                        
                        local_pos_pub.publish(c_pos_sp);
                        counterc0++;
                        if(counterc0 > 2*3.1415926/wn/0.01){
                            flag = 1;
                            counterc0 = 0;
                        }
                        break;
                case 1:
                        
                        c_pos_sp.pose.position.x = xc1 - r*sin(wn*0.01*counterc1);
                        c_pos_sp.pose.position.y = yc1 + r*cos(wn*0.01*counterc1);
                        c_pos_sp.pose.position.z = zc1;
                        local_pos_pub.publish(c_pos_sp);
                        counterc1++;
                        if(counterc1 > 2*3.1415926/wn/0.01){
                            flag = 0;
                            counterc1 = 0;
                        }
                        break;
            }



            // c_pos_sp.pose.position.x = 0.2;
            // c_pos_sp.pose.position.y = 1 ;
            // c_pos_sp.pose.position.z = 1;
            ros::spinOnce();
            rate.sleep();

    }



    return 0;
}
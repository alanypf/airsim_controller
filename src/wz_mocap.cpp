#include <ros/ros.h>
#include "mavros_msgs/PositionTarget.h"
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


    ros::init(argc, argv, "wz_mocap");
    ros::NodeHandle nh;

    
    ros::Publisher local_pos_pub = nh.advertise<mavros_msgs::PositionTarget>("/mavros/setpoint_raw/local",10);

    ros::Rate rate = 100;

    mavros_msgs::PositionTarget d_pos_sp;
    d_pos_sp.type_mask = 0b000111000000;
    d_pos_sp.coordinate_frame = 1;
    d_pos_sp.position.x = xc0;
    d_pos_sp.position.y = yc0 - r;
    d_pos_sp.position.z = zc0;

    for(int i = 1000; ros::ok() && i>0; i--){
        local_pos_pub.publish(d_pos_sp);
        ros::spinOnce();
        rate.sleep();
    }



    while(ros::ok()){         

            mavros_msgs::PositionTarget c_pos_sp;
            c_pos_sp.type_mask = 0b000111000000;
            c_pos_sp.coordinate_frame = 1;


            switch(flag){
                case 0:
                        
                        c_pos_sp.position.x = xc0 - r*sin(wn*0.01*counterc0);
                        c_pos_sp.position.y = yc0 - r*cos(wn*0.01*counterc0);
                        c_pos_sp.position.z = zc0;
                        c_pos_sp.velocity.x = -wn*0.01*r*cos(wn*0.01*counterc0);
                        c_pos_sp.velocity.y = wn*0.01*r*sin(wn*0.01*counterc0);                        
                        local_pos_pub.publish(c_pos_sp);
                        counterc0++;
                        if(counterc0 > 2*3.1415926/wn/0.01){
                            flag = 1;
                            counterc0 = 0;
                        }
                        break;
                case 1:
                        
                        c_pos_sp.position.x = xc1 - r*sin(wn*0.01*counterc1);
                        c_pos_sp.position.y = yc1 + r*cos(wn*0.01*counterc1);
                        c_pos_sp.position.z = zc1;
                        c_pos_sp.velocity.x = -wn*0.01*r*cos(wn*0.01*counterc0);
                        c_pos_sp.velocity.y = wn*0.01*r*sin(wn*0.01*counterc0);    
                        local_pos_pub.publish(c_pos_sp);
                        counterc1++;
                        if(counterc1 > 2*3.1415926/wn/0.01){
                            flag = 0;
                            counterc1 = 0;
                        }
                        break;
            }

            // c_pos_sp.position.x = 0.2;
            // c_pos_sp.position.y = 1 ;
            // c_pos_sp.position.z = 1;
            ros::spinOnce();
            rate.sleep();

    }

    return 0;
}
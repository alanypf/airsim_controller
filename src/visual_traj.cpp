#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <visualization_msgs/Marker.h>

geometry_msgs::PoseStamped current_pose;
geometry_msgs::PoseStamped past_pose;
geometry_msgs::PoseStamped current_ref;
geometry_msgs::PoseStamped past_ref;
visualization_msgs::Marker current_marker;
visualization_msgs::Marker current_marker_f;

void pose_cb(const geometry_msgs::PoseStamped::ConstPtr& msg){
    current_pose = *msg;
    current_marker.header.frame_id = "map";
    current_marker.header.stamp    = ros::Time::now();
    current_marker.ns              = "trajectory";
    current_marker.type            = visualization_msgs::Marker::LINE_LIST;
    current_marker.action          = visualization_msgs::Marker::ADD;
    current_marker.pose.position.x    = 0;
    current_marker.pose.position.y    = 0;
    current_marker.pose.position.z    = 0;
    current_marker.pose.orientation.w = 1;
    current_marker.pose.orientation.x = 0;
    current_marker.pose.orientation.y = 0;
    current_marker.pose.orientation.z = 0;
    current_marker.scale.x = 0.1;
    current_marker.scale.y = 0;
    current_marker.scale.z = 0;
    current_marker.color.r = 0.0;
    current_marker.color.g = 1.0;
    current_marker.color.b = 0.0;
    current_marker.color.a = 0.8;
    geometry_msgs::Point p;
    p.x = past_pose.pose.position.x;
    p.y = past_pose.pose.position.y;
    p.z = past_pose.pose.position.z;
    current_marker.points.push_back(p);
    p.x = current_pose.pose.position.x;
    p.y = current_pose.pose.position.y;
    p.z = current_pose.pose.position.z;
    current_marker.points.push_back(p);
    std_msgs::ColorRGBA color;
    color.r = 1;
    color.g = 0;
    color.b = 0;
    color.a = 1;
    current_marker.colors.push_back(color);
    current_marker.colors.push_back(color);
    past_pose = current_pose;

}


void ref_cb(const geometry_msgs::PoseStamped::ConstPtr& msg){
    current_ref = *msg;
    current_marker_f.header.frame_id = "map";
    current_marker_f.header.stamp    = ros::Time::now();
    current_marker_f.ns              = "trajectory";
    current_marker_f.type            = visualization_msgs::Marker::LINE_LIST;
    current_marker_f.action          = visualization_msgs::Marker::ADD;
    current_marker_f.pose.position.x    = 0;
    current_marker_f.pose.position.y    = 0;
    current_marker_f.pose.position.z    = 0;
    current_marker_f.pose.orientation.w = 1;
    current_marker_f.pose.orientation.x = 0;
    current_marker_f.pose.orientation.y = 0;
    current_marker_f.pose.orientation.z = 0;
    current_marker_f.scale.x = 0.05;
    current_marker_f.scale.y = 0;
    current_marker_f.scale.z = 0;
    current_marker_f.color.r = 0.0;
    current_marker_f.color.g = 1.0;
    current_marker_f.color.b = 0.0;
    current_marker_f.color.a = 0.8;
    geometry_msgs::Point p;
    p.x = past_ref.pose.position.x;
    p.y = past_ref.pose.position.y;
    p.z = past_ref.pose.position.z;
    current_marker_f.points.push_back(p);
    p.x = current_ref.pose.position.x;
    p.y = current_ref.pose.position.y;
    p.z = current_ref.pose.position.z;
    current_marker_f.points.push_back(p);
    std_msgs::ColorRGBA color;
    color.r = 0;
    color.g = 1;
    color.b = 0;
    color.a = 1;
    current_marker_f.colors.push_back(color);
    current_marker_f.colors.push_back(color);
    past_ref = current_ref;

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "visual_traj");
    ros::NodeHandle nh;

    ros::Subscriber pose_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 10, pose_cb);
    ros::Subscriber ref_sub = nh.subscribe<geometry_msgs::PoseStamped>("mavros/setpoint_position/local",10, ref_cb);
    ros::Publisher traj_pub = nh.advertise<visualization_msgs::Marker>("/trajectory", 100, true);
    ros::Publisher ref_pub = nh.advertise<visualization_msgs::Marker>("/ref_trajectory", 100, true);

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    while(ros::ok()){
        traj_pub.publish(current_marker);
        ref_pub.publish(current_marker_f);
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}




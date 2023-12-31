#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <geometry_msgs/Twist.h>

// Global joint publisher variables
ros::Publisher motor_command_publisher;

// This callback function executes whenever a safe_move service is requested
bool handle_drive_request(
    ball_chaser::DriveToTarget::Request& req,
    ball_chaser::DriveToTarget::Response& res) { 
    
    ROS_INFO("DriveToTargetRequest received - linear_x:%1.2f, angular_z:%1.2f", (float)req.linear_x, (float)req.angular_z);
    // Create a motor_command object of type geometry_msgs::Twist
    geometry_msgs::Twist motor_command;
    // Set wheel velocities, forward [0.5, 0.0]
    motor_command.linear.x = req.linear_x;
    motor_command.angular.z = req.angular_z;
    // Publish angles to drive the robot
    motor_command_publisher.publish(motor_command);

    // Return a response message
    res.msg_feedback = "Velocities set - linear_x: " + std::to_string(req.linear_x) + " , angular_z: " + std::to_string(req.angular_z);
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
}

int main(int argc, char** argv) {
    // Initialize the arm_mover node and create a handle to it
    ros::init(argc, argv, "drive_bot");
    ros::NodeHandle n;

    // Define two publishers to publish geometry_msgs::Twist messages on joints respective topics
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // Define a command_robot service with a handle_drive_request callback function
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);
    ROS_INFO("Ready to send drive commands");

    // Handle ROS communication events
    ros::spin();

    return 0;
}
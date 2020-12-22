#include <ros/ros.h>
#include <cmath>
#include <std_msgs/String.h>
#include <sensor_msgs/JointState.h>
#include <std_msgs/Float64.h>



std::string double2string(double input);
std::string combinemsg(std::vector<double> velocity, double acc = 1);


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "speedl_test");
    ros::NodeHandle nh;

    

    ros::Publisher ur_pub = nh.advertise<std_msgs::String>("/ur_driver/URScript", 1000);
    ros::Duration(1.0).sleep();
    std_msgs::String ur_script_msgs;
    std::vector<double> command_velocity = {0, 0, 0, 0, 0, 0};
   
    command_velocity[1]=0.2;
    ur_script_msgs.data = combinemsg(command_velocity,1.5);
    ur_pub.publish(ur_script_msgs);
    ros::Rate loop_rate(50);
    ros::Time init_time=ros::Time::now();

    while(ros::ok()){
        
        double time_duration = (ros::Time::now() - init_time).toSec();
        if(time_duration<1){
            command_velocity[1]=0.2;
        }
        else{
            command_velocity[1]=-0.2;
        }
       
        if(time_duration>5) ros::shutdown();
        ur_script_msgs.data = combinemsg(command_velocity);
        ur_pub.publish(ur_script_msgs);
        ros::spinOnce();
        loop_rate.sleep();
    }
    std::string move_msg="stopl(1)\n";
    ur_script_msgs.data = move_msg;
    ur_pub.publish(ur_script_msgs);
    ROS_INFO("finished");
    return 0;
}

std::string double2string(double input)
{
    std::string string_temp;
    std::stringstream stream;
    stream<<input;
    string_temp = stream.str();
    return string_temp;
}

std::string combinemsg(std::vector<double> velocity, double acc)
{
    double time2move = 1;
    std::string move_msg;
    move_msg = "speedl([";
    move_msg = move_msg + double2string(velocity[0]) + ",";
    move_msg = move_msg + double2string(velocity[1]) + ",";
    move_msg = move_msg + double2string(velocity[2]) + ",";
    move_msg = move_msg + double2string(velocity[3]) + ",";
    move_msg = move_msg + double2string(velocity[4]) + ",";
    move_msg = move_msg + double2string(velocity[5]) + "]";
    move_msg = move_msg + ",";
    move_msg = move_msg + double2string(1.5) + ",";
    move_msg = move_msg + double2string(time2move) + ")";
    move_msg = move_msg + "\n";
    return move_msg; 
}
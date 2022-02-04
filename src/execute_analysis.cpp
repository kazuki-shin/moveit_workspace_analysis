#include "ros/ros.h"
#include "std_msgs/String.h"

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>

#include <moveit_visual_tools/moveit_visual_tools.h>

#include <fstream>
#include <iostream>

int main(int argc, char **argv)
{
  ros::init (argc, argv, "execute_analysis");
  ros::AsyncSpinner spinner(1);
  spinner.start();
  ros::NodeHandle node_handle("~");

  static const std::string PLANNING_GROUP = "arm1";
  moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);
  const moveit::core::JointModelGroup* joint_model_group =
    move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);
  ROS_INFO_NAMED("analysis", "Planning frame: %s", move_group.getPlanningFrame().c_str());

  namespace rvt = rviz_visual_tools;
  moveit_visual_tools::MoveItVisualTools visual_tools("world");
  visual_tools.deleteAllMarkers();

  std::string analysis_filename;
  if (!node_handle.getParam("filename", analysis_filename))
    ROS_ERROR("Will not write to file");

  // load the set of quaternions
  std::vector<geometry_msgs::Quaternion> orientations;
  std::ifstream analysis_file;
  analysis_file.open(analysis_filename.c_str());

  while(!analysis_file.eof())
  {
    std::string curr_line;
    getline(analysis_file, curr_line, '\n');
    std::istringstream iss(curr_line);

    // create pose and joint vector
    geometry_msgs::Pose pose;
    std::vector<double> joint_values(6);

    for(int idx = 0; idx < 13; idx++){
      std::string subs;
      std::getline(iss, subs, ',');
      ROS_INFO(subs.c_str());

      if(subs.empty())
        break;

      // position 0 1 2
      // orientation 3 4 5 6 
      // joint values 7 8 9 10 11 12

      if(idx == 0)
        pose.position.x = std::stof(subs);
      else if(idx == 1)
        pose.position.y = std::stof(subs);
      else if(idx == 2)
        pose.position.z = std::stof(subs);
      else if(idx == 3)
        pose.orientation.x = std::stof(subs);
      else if(idx == 4)
        pose.orientation.y = std::stof(subs);
      else if(idx == 5)
        pose.orientation.z = std::stof(subs);
      else if(idx == 6)
        pose.orientation.w = std::stof(subs);
      else if(idx == 7)
        joint_values[0] = std::stof(subs);
      else if(idx == 8)
        joint_values[1] = std::stof(subs);
      else if(idx == 9)
        joint_values[2] = std::stof(subs);
      else if(idx == 10)
        joint_values[3] = std::stof(subs);
      else if(idx == 11)
        joint_values[4] = std::stof(subs);
      else
        joint_values[5] = std::stof(subs);
    }

    // use pose and joint val for moveit
    
    move_group.setMaxVelocityScalingFactor(1);
    move_group.setMaxAccelerationScalingFactor(1);
    move_group.setPlanningTime(10);
    move_group.setNumPlanningAttempts(30);
    move_group.setJointValueTarget(joint_values);
    // move_group.setPoseTarget(pose);

    ROS_INFO("Joint values: %f, %f, %f, %f, %f, %f",
        joint_values[0],
        joint_values[1],
        joint_values[2],
        joint_values[3],
        joint_values[4],
        joint_values[5]
    );

    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("analysis", "Visualizing plan 1 (joint space goal) %s", success ? "" : "FAILED");

    ROS_INFO_NAMED("analysis", "Visualizing plan 1 as trajectory line");
    visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
    visual_tools.trigger();
    visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to continue the demo");

    move_group.execute(my_plan);
  }

  ROS_INFO("Done");
  ros::shutdown();
  return 0;
}

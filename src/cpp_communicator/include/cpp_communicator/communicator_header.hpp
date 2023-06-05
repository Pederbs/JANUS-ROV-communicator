#pragma once
#include "rclcpp/rclcpp.hpp"

class CommunicatorNode : public rclcpp::Node
{
public:
    CommunicatorNode() : Node("Communicator node")
    {
        RCLCPP_INFO(this->get_logger(), "Hello from Cpp");
    }
//private:
    
};
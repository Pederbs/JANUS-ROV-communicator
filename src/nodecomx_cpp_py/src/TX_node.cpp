#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <stdio.h>
#include <functional>
#include <memory>

// #include "nodecomx_cpp_py/cpp_header.hpp"
#include "lib/Evo_janusXsdm/Evo_janusXsdm.cpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"


//Constructor parameters for Evo_janusXsdm.h
std::string JANUS_PATH = "src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/bin/";
std::string SDM_PATH = "src/nodecomx_cpp_py/include/lib/sdmsh/";
std::string IP ="192.168.0.189";
int JANUS_RX_PORT = 9920;
int JANUS_TX_PORT = 9914;
float STREAMFS = 250000.0;

class ComSubNode : public rclcpp::Node
{
public:
    ComSubNode() : Node("evo_communicator")
    {
        //Constructing a connection object
        Evo_janusXsdm::connection modem(IP,JANUS_PATH,SDM_PATH, JANUS_RX_PORT,JANUS_TX_PORT,STREAMFS); 
        //Configures modem and sets preamble
        modem.sdmConfigAir();
        std::this_thread::sleep_for(500ms);
        modem.setPreamble();
        std::this_thread::sleep_for(500ms);

        // Creating subsciber
        subscriber_ = this->create_subscription<std_msgs::msg::String>
        (
            "topicA", 
            10, 
            std::bind(&ComSubNode::callbackSensorValues, this, std::placeholders::_1)
        );
        RCLCPP_INFO(this->get_logger(), "EvoLogistics Subscriber Node has been started.");
    }
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;

    void callbackSensorValues(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Heard: '%s'", msg->data.c_str());
        //Constructing a connection object
        Evo_janusXsdm::connection modem(IP,JANUS_PATH,SDM_PATH, JANUS_RX_PORT,JANUS_TX_PORT,STREAMFS); 
        modem.startTX(msg->data.c_str());
    }
};



int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ComSubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

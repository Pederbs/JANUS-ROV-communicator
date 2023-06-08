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

// RX varaibles
int timeout = 30000; // milliseconds [ms]
std::string response;
std::string comment;


Evo_janusXsdm::connection modem;

class ComPubNode : public rclcpp::Node
{
public:
    ComPubNode() : Node("Evo")
    {

        Evo_janusXsdm::connection modem(IP, JANUS_PATH, SDM_PATH, JANUS_RX_PORT, JANUS_TX_PORT, STREAMFS);
        // Configures modem and sets preamble
        modem.sdmConfigAir();
        std::this_thread::sleep_for(500ms);
        modem.setPreamble();
        std::this_thread::sleep_for(500ms);

        // Create subscriber
        subscriber_ = this->create_subscription<std_msgs::msg::String>
        (
            "topicA", 
            10, 
            std::bind(&ComPubNode::callback, this, std::placeholders::_1)
        );

        // Create publisher
        publisher_ = this->create_publisher<std_msgs::msg::String>("topicB", 10);
        timer_ = this->create_wall_timer
        (
            1000ms, 
            std::bind(&ComPubNode::timer_callback, this)
        );
        RCLCPP_INFO(this->get_logger(), "Node has been started.");
    }

private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
    std::string modemMSG;
    bool update = 0;

    void callback (const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Heard: '%s'", msg->data.c_str());
        modemMSG = msg->data;
        update = 1;

    }

    void timer_callback()
    {
        // Listen to modem
        std::array<std::string,4> responsFromFrame = modem.listenOnceRXsimple(response,timeout);
        // std::cout << "listen to modem\n";

        // Populate and publish message
        auto message = std_msgs::msg::String();
        message.data = response;
        // message.data = "response";
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        publisher_->publish(message);

        // std::cout << "bool: " << update << "\n";
        if (update == 1) 
        {
            std::this_thread::sleep_for(500ms);
            modem.startTX(modemMSG);
            // std::cout << "Sent data to modem: " << modemMSG <<"\n";
            update = 0;
        }
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ComPubNode>());
    rclcpp::shutdown();
    return 0;
}

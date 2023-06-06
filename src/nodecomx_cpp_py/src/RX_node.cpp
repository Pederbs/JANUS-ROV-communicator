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
std::string JANUSPATH = "src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/bin/";
std::string SDMPATH = "src/nodecomx_cpp_py/include/lib/sdmsh/";
std::string IP ="192.168.0.189";
int JANUS_RX_PORT = 9920;
int JANUS_TX_PORT = 9914;
float STREAMFS = 250000.0;
int timeout = 6000;

// using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class ComPubNode : public rclcpp::Node
{
public:
    ComPubNode() : Node("minimal_publisher"), count_(0)
    {

        Evo_janusXsdm::connection modem(IP, JANUSPATH, SDMPATH, JANUS_RX_PORT, JANUS_TX_PORT, STREAMFS);
        // Configures modem and sets preamble
        modem.sdmConfigAir();
        std::this_thread::sleep_for(500ms);
        modem.setPreamble();
        std::this_thread::sleep_for(500ms);
        // Create publisher
        publisher_ = this->create_publisher<std_msgs::msg::String>("modemTopic", 10);
        timer_ = this->create_wall_timer
        (
            10000ms, 
            std::bind(&ComPubNode::timer_callback, this)
        );
        RCLCPP_INFO(this->get_logger(), "EvoLogistics Publisher Node has been started.");
    }

private:
    void timer_callback()
    {
        std::string responce;
        Evo_janusXsdm::connection modem(IP, JANUSPATH, SDMPATH, JANUS_RX_PORT, JANUS_TX_PORT, STREAMFS);
        int fd_listen = modem.startRX();
        std::this_thread::sleep_for(500ms);
        modem.listenRX(fd_listen, responce, timeout);
        auto message = std_msgs::msg::String();
        message.data;
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ComPubNode>());
    rclcpp::shutdown();
    return 0;
}
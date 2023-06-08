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

// using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class ComPubNode : public rclcpp::Node
{
public:
    ComPubNode() : Node("minimal_publisher")
    {

        Evo_janusXsdm::connection modem(IP, JANUS_PATH, SDM_PATH, JANUS_RX_PORT, JANUS_TX_PORT, STREAMFS);
        // Configures modem and sets preamble
        modem.sdmConfigAir();
        std::cout << "Print: 1\n";
        std::this_thread::sleep_for(500ms);
        modem.setPreamble();
        std::cout << "Print: 2\n";
        std::this_thread::sleep_for(500ms);

        // Create publisher
        publisher_ = this->create_publisher<std_msgs::msg::String>("modemData", 10);
        timer_ = this->create_wall_timer
        (
            10000ms, 
            std::bind(&ComPubNode::timer_callback, this)
        );
        RCLCPP_INFO(this->get_logger(), "EvoLogistics Publisher Node has been started.");
        ac();
    }

private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

    void ac()
    {
        Evo_janusXsdm::connection modem(IP, JANUS_PATH, SDM_PATH, JANUS_RX_PORT, JANUS_TX_PORT, STREAMFS);
        std::this_thread::sleep_for(500ms);
        //int fd_listen = modem.startRX();
        //std::cout << "Print: 3\n";
        while(rclcpp::ok())
        {
	    std::cout << "hello from ROS while\n";
        //std::array<std::string,4> responsFromFrame = modem.listenRX(fd_listen, response, 10000);
        std::array<std::string,4> responsFromFrame = modem.listenOnceRXsimple(response,timeout);
        std::cout << "Print: 4\n";

            // Populate and publish message
            auto message = std_msgs::msg::String();
            message.data = response;
            std::cout << response << "hello \n";
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
            publisher_->publish(message);
            std::getline(std::cin,comment);
            std::this_thread::sleep_for(500ms);
            modem.startTX("HELLO UNDERWATER");
            std::this_thread::sleep_for(500ms);

        }

        // Close JANUS pipe
        //modem.closePipeRX(fd_listen);
        // std::cout << "Print: 5\n";
        // modem.stopRX();
        // std::cout << "Print: 6\n";
    }

    void timer_callback()
    {
        // // Receive from modem
        // Evo_janusXsdm::connection modem(IP, JANUS_PATH, SDM_PATH, JANUS_RX_PORT, JANUS_TX_PORT, STREAMFS);
        // int fd_listen = modem.startRX();
        // std::this_thread::sleep_for(500ms);
        // modem.listenRX(fd_listen, response, timeout);

        // // Populate and publish message
        // auto message = std_msgs::msg::String();
        // message.data = response;
        // RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        // publisher_->publish(message);

        // // Close JANUS pipe
        // modem.closePipeRX(fd_listen);
        // modem.stopRX();
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ComPubNode>());
    std::cout << "Shutting down\n";
    rclcpp::shutdown();
    std::cout << "Skal dette kjore?\n";
    return 0;
}

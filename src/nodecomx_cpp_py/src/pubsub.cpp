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
int timeout = 10000; // milliseconds [ms]
std::string response;
std::string comment;

class PubSubNode : public rclcpp::Node
{
    public:
        PubSubNode() : Node("evo_communicator")
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
                std::bind(&PubSubNode::callbackSensorValues, this, std::placeholders::_1)
            );
            publisher_ = this->create_publisher<std_msgs::msg::String>("modemData", 10);
        }
    private:
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

        void callbackSensorValues(const std_msgs::msg::String::SharedPtr msg)
        {
            // 
            RCLCPP_INFO(this->get_logger(), "Heard: '%s'", msg->data.c_str());
            std::string transmit_string = msg->data.c_str();

            // Constructing a connection object
            Evo_janusXsdm::connection modem(IP, JANUS_PATH, SDM_PATH, JANUS_RX_PORT, JANUS_TX_PORT, STREAMFS);
            std::this_thread::sleep_for(500ms);
            std::array<std::string,4> responsFromFrame = modem.listenOnceRXsimple(response,timeout);

            // Populate and publish message
            auto message = std_msgs::msg::String();
            message.data = response;
            std::cout << response << "hello \n";
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
            publisher_->publish(message);


            std::getline(std::cin,comment);
            std::this_thread::sleep_for(500ms);
            modem.startTX(transmit_string);
            std::this_thread::sleep_for(500ms);
        }

        // void ac ()
        // {
        //     Evo_janusXsdm::connection modem(IP, JANUS_PATH, SDM_PATH, JANUS_RX_PORT, JANUS_TX_PORT, STREAMFS);
        //     std::this_thread::sleep_for(500ms);
        //     //int fd_listen = modem.startRX();
        //     //std::cout << "Print: 3\n";
        //     while(rclcpp::ok())
        //     {
        //     std::cout << "hello from ROS while\n";
        //     //std::array<std::string,4> responsFromFrame = modem.listenRX(fd_listen, response, 10000);
        //     std::array<std::string,4> responsFromFrame = modem.listenOnceRXsimple(response,timeout);
        //     std::cout << "Print: 4\n";

        //         // Populate and publish message
        //         auto message = std_msgs::msg::String();
        //         message.data = response;
        //         std::cout << response << "hello \n";
        //         RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        //         publisher_->publish(message);
        //         std::getline(std::cin,comment);
        //         std::this_thread::sleep_for(500ms);
        //         modem.startTX("HELLO UNDERWATER");
        //         std::this_thread::sleep_for(500ms);

        //     }
        // }
};



int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PubSubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

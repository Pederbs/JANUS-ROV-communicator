/*
This example shows how the "Evo_janusXsdm" library can be used. 
The code below uses the startTX function to transmit the JANUS packets. 
*/
/*
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

#include "lib/Evo_janusXsdm/Evo_janusXsdm.cpp"

// Constructor parameters for Evo_janusXsdm.h
std::string JANUSPATH = "src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/bin/";
std::string SDMPATH = "src/nodecomx_cpp_py/include/lib/sdmsh/";
std::string IP ="192.168.0.189";
int JANUS_RX_PORT = 9920;
int JANUS_TX_PORT = 9914;
float STREAMFS = 250000.0;

//Global variables
std::string myString = "1";
std::string comment;

int main()
{ 
    //Constructing a connection object
    Evo_janusXsdm::connection modem(IP,JANUSPATH,SDMPATH, JANUS_RX_PORT,JANUS_TX_PORT,STREAMFS); 

    //Configures modem and sets preamble
    // modem.sdmConfigAir();
    // std::this_thread::sleep_for(500ms);        //TODO:Test if sleep is needed
    // modem.setPreamble();
    // std::this_thread::sleep_for(500ms);        //TODO:Test if sleep is needed

    while(true)
    {                                 
        std::cout <<"Press enter to send message" <<std::endl;
        std::getline(std::cin,comment);
        modem.startTX(myString);  
        myString += "0";
        //std::this_thread::sleep_for(6000ms);    //interval between sending
    }
}
*/

// #include "nodecomx_cpp_py/cpp_header.hpp"
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

// #include "src/nodecomx_cpp_py/include/lib/Evo_janusXsdm/Evo_janusXsdm.h"
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

class ComSubNode : public rclcpp::Node
{
public:
    ComSubNode() : Node("evo_communicator")
    {
        subscriber_ = this->create_subscription<std_msgs::msg::String>
        (
            "modemTopic", 
            10, 
            std::bind(&ComSubNode::callbackSensorValues, this, std::placeholders::_1)
        );
        RCLCPP_INFO(this->get_logger(), "EvoLogistics Subscriber Node has been started.");
        //Constructing a connection object
        Evo_janusXsdm::connection modem(IP,JANUSPATH,SDMPATH, JANUS_RX_PORT,JANUS_TX_PORT,STREAMFS); 
        //Configures modem and sets preamble
        modem.sdmConfigAir();
        std::this_thread::sleep_for(500ms);
        modem.setPreamble();
        std::this_thread::sleep_for(500ms);
    }
private:
    void callbackSensorValues(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Heard: '%s'", msg->data.c_str());
        //Constructing a connection object
        Evo_janusXsdm::connection modem(IP,JANUSPATH,SDMPATH, JANUS_RX_PORT,JANUS_TX_PORT,STREAMFS); 
        modem.startTX(msg->data.c_str());
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
};



int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ComSubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

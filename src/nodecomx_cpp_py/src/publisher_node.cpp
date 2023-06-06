#include <chrono>
#include <functional>
#include <memory>
#include <string>


#include "lib/Evo_janusXsdm/Evo_janusXsdm.cpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class ComPubNode : public rclcpp::Node
{
public:
    ComPubNode() : Node("minimal_publisher"), count_(0)
    {
        publisher_ = this->create_publisher<std_msgs::msg::String>("modemTopic", 10);
        timer_ = this->create_wall_timer
        (
            500ms, 
            std::bind(&ComPubNode::timer_callback, this)
        );
        RCLCPP_INFO(this->get_logger(), "EvoLogistics Publisher Node has been started.");

        //Constructor parameters for Evo_janusXsdm.h
        std::string JANUSPATH = "/lib/janus-c-3.0.5/bin/";
        std::string SDMPATH = "/lib/sdmsh/";
        std::string IP = "192.168.0.198";
        int JANUS_RX_PORT = 9955;
        int JANUS_TX_PORT = 9955;
        float STREAMFS = 250000.0;

        //Global variables
        std::string response;
        int listenCount = 0;

    }

private:
    void timer_callback()
    {
        auto message = std_msgs::msg::String();
        message.data = "Counting: " + std::to_string(count_++);
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
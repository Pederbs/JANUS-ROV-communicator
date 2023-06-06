// #include "nodecomx_cpp_py/cpp_header.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class ComSubNode : public rclcpp::Node
{
public:
    ComSubNode() : Node("evo_communicator")
    {
        // RCLCPP_INFO(this->get_logger(), 'Hello from Cpp');
        subscriber_ = this->create_subscription<std_msgs::msg::String>
        (
            "modemTopic", 
            10, 
            std::bind(&ComSubNode::callbackSensorValues, this, std::placeholders::_1)
        );
        RCLCPP_INFO(this->get_logger(), "comEvo has been started.");
    }
private:
    void callbackSensorValues(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "%s", msg->data.c_str());
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

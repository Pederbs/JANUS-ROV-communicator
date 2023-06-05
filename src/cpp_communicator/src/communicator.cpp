#include "rclcpp/rclcpp.hpp"

class CommunicatorNode : public rclcpp::Node
{
public:
    CommunicatorNode() : Node("my_node")
    {
        RCLCPP_INFO(this->get_logger(), "Hello from Cpp");
    }
//private:
    
};


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    // auto node = std::make_shared<MyNode>("cpp_test");
    auto node = std::make_shared<CommunicatorNode>("Communicator");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

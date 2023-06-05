// #include "rclcpp/rclcpp.hpp"
#include "cpp_communicator/communicator_header.hpp"


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CommunicatorNode>("Communicator");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

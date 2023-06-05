#include "nodecomx_cpp_py/cpp_header.hpp"
#include "rclcpp/rclcpp.hpp"
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("com_node");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

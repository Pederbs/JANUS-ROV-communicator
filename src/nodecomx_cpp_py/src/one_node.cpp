

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
int timeout = 300; // milliseconds [ms]
std::string response;

class MyNode : public rclcpp::Node
{
public:
    MyNode() : Node("my_node")
    {
        // Create the subscriber
        subscription_ = this->create_subscription<std_msgs::msg::String>(
        "topicA", 10, std::bind(&MyNode::topicACallback, this, std::placeholders::_1));

        // Create the publisher
        publisher_ = this->create_publisher<std_msgs::msg::String>("topicB", 10);

        // Start the process AC
        processAC();
    }

private:
    void topicACallback(const std_msgs::msg::String::SharedPtr msg)
    {
        // Process the received data from topic A
        RCLCPP_INFO(this->get_logger(), "Received: %s", msg->data.c_str());

        // Perform computations or actions using the received data


    }

    void processAC()
    {
        // Opening pipe
        Evo_janusXsdm::connection modem(IP, JANUS_PATH, SDM_PATH, JANUS_RX_PORT, JANUS_TX_PORT, STREAMFS);
        // Configures modem and sets preamble
        modem.sdmConfigAir();
        std::this_thread::sleep_for(500ms);
        modem.setPreamble();
        std::this_thread::sleep_for(500ms);

        while (rclcpp::ok())
        {
        // Start process
        int fd_listen = modem.startRX();
        modem.listenRX(fd_listen, response, timeout);

        // Bare dersom noe er tatt imot
        // Publish the processed data to topic B
        std_msgs::msg::String output_msg;
        output_msg.data = response;
        publisher_->publish(output_msg);
        rclcpp::spin_some(this->get_node_base_interface());
        // Close JANUS pipe
        modem.closePipeRX(fd_listen);
        modem.stopRX();

        // Sender
        modem.startTX("ello mate");



        // You can add a sleep or delay here if needed
        }
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MyNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

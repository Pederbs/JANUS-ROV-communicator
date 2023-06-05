#!/usr/bin/env python3
# Import required modules
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from unetpy import *
import numpy as np

# Import of homemade modules
from nodecomx_cpp_py.nodecomx_cpp_py import algo as my_algo
from nodecomx_cpp_py.nodecomx_cpp_py import FP16_converter as FP16_converter
# This module contains our modem information which is used among all local nodes.
from nodecomx_cpp_py.scripts.modem_info import *

# Define ProcessingNode class, which inherits from the Node class
class ProcessingNode(Node):

    def __init__(self):

        # Call the constructor of the parent class
        super().__init__(f'processing_node_{modem_name}')

        # Type of message for publish/subscribe for ROS 2
        self.msg = String()

        # ROS 2 publisher and subscriber       
        self.publisher_ = self.create_publisher(String, f'topic_transmission{modem_name}', 1)
        self.subscription = self.create_subscription(String, f'topic_reception{modem_name}', self.number_callback, 1)
        self.subscription # To ignore unimportant errors. Recommended by ROS 2 documentation.

        # Creating an instance of the NRC algorithm
        self.nrc_algorithm = my_algo.NRC_algo()

    # Callback function to process incoming data with the NRC algorithm from topic_reception
    def number_callback(self, msg):
        
        # Converting the received message into an array of np.float16 values.
        output_floats = FP16_converter.Converter.int8_list_to_FP16(msg.data)
        numpy_float_array = np.array(output_floats)

        # - Algorithm processing - 

        # The variables are being updated inside the instance of the NRC algorithm module.
        self.nrc_algorithm.data_reception(numpy_float_array)
        # We are taking the updated variables for broadcasting
        transmission_array = self.nrc_algorithm.data_transmission()

        # Remove leading zeros from the transmission array
        # and convert float zeros to integer zeros
        removed_leading_zero = [int(num) if num == 0.0 else num for num  in transmission_array]

        # Converting the numpy float16 array into list of unsigned int8 
        msg.data = str(FP16_converter.Converter.FP16_list_to_int8(removed_leading_zero))

        # Publishing to the transmission topic
        self.publisher_.publish(msg)

def main():

    # Basic ROS2 stuff. Recommending to just read the documentation for these.
    rclpy.init(args=None)
    rclpy.spin(ProcessingNode()) # Runs the node in a loop
    # Once loop ends, things get deleted
    ProcessingNode.destroy_node() 
    rclpy.shutdown()

if __name__ == "__main__":
    main()


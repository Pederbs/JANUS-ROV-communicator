cmake_minimum_required(VERSION 3.8)
project(nodecomx_cpp_py)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED) # Python
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)              # Python
find_package(std_msgs REQUIRED)

# Include Cpp "include" directory
# include_directories(include; ${example_interfaces_INCLUDE_DIRS})
include_directories(include)

# Create Subscriber executable
add_executable(subscriberEvo src/TX_node.cpp)
ament_target_dependencies(subscriberEvo rclcpp std_msgs)

# Create Publisher executable
add_executable(publisherEvo src/RX_node.cpp)
ament_target_dependencies(publisherEvo rclcpp std_msgs)

# Install Cpp executables
install(TARGETS
  subscriberEvo
  publisherEvo
  DESTINATION lib/${PROJECT_NAME}
)

# Install Python modules
ament_python_install_package(${PROJECT_NAME})

# Install Python executables
install(PROGRAMS
  scripts/processing_node.py
  DESTINATION lib/${PROJECT_NAME}
)


ament_package()

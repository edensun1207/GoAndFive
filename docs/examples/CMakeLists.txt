# CMakeLists.txt - 示例程序构建文件
cmake_minimum_required(VERSION 3.10)
project(GoAndFive_Examples)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network)

# 自动处理 MOC
set(CMAKE_AUTOMOC ON)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Go-Five
    ${CMAKE_CURRENT_SOURCE_DIR}/../../server
)

# 自定义客户端示例
add_executable(custom_client_example
    custom_client_example.cpp
    ../../Go-Five/board.cpp
)
target_link_libraries(custom_client_example
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
)

# AI 玩家示例
add_executable(ai_player_example
    ai_player_example.cpp
)
target_link_libraries(ai_player_example
    Qt5::Core
    Qt5::Network
)

# 设置输出目录
set_target_properties(custom_client_example ai_player_example
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 安装规则
install(TARGETS custom_client_example ai_player_example
    RUNTIME DESTINATION bin
)

# 显示配置信息
message(STATUS "")
message(STATUS "========================================")
message(STATUS "GoAndFive Examples Configuration:")
message(STATUS "========================================")
message(STATUS "Qt5 Version: ${Qt5Core_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "========================================")
message(STATUS "")
message(STATUS "Build with: cmake --build .")
message(STATUS "Run examples from: ${CMAKE_BINARY_DIR}/bin/")
message(STATUS "")
cmake_minimum_required(VERSION 3.10)    
project(TP4)


add_executable(TP4 client.c segdef.h)

target_link_libraries(TP4 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libseg.a)
target_link_libraries(TP4 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libsegcustom.a)

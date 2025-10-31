# CMake 프로그램의 최소 버전
cmake_minimum_required(VERSION 3.11)

# 프로젝트 정보
project(
  RDL
  VERSION 0.1
  LANGUAGES CXX)

# C++ 표준 지정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 실행 파일 생성 
add_executable(program 
                src/main.cpp
                src/packet/HeaderPacket.cpp
                src/packet/MslComPacket.cpp
                src/packet/MslInfoPacket.cpp
                src/packet/TgtInfoPacket.cpp
                src/SocketManager.cpp 
                src/EpollManager.cpp)

# 헤더 경로 추가
target_include_directories(program PRIVATE ${CMAKE_SOURCE_DIR}/include)
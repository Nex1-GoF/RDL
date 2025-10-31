# ARM(징크 보드)용 크로스 컴파일 설정 파일

# 시스템 정보
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 사용할 크로스 컴파일러 지정
set(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# 기본 컴파일 옵션 (ARMv7 + NEON + 하드플로트)
set(COMMON_FLAGS "-march=armv7-a -mfpu=neon -mfloat-abi=hard")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")

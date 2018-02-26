
cmake_minimum_required(VERSION 3.4.1)

INCLUDE_DIRECTORIES(src/main/cpp/openssllib/include/openssl)

LINK_DIRECTORIES(src/main/cpp/openssllib)

ADD_LIBRARY(native-lib SHARED  src/main/cpp/native-lib.cpp  src/main/cpp/rsa_openssl.cpp  src/main/cpp/my_base64.cpp)

find_library( log-lib log)

TARGET_LINK_LIBRARIES(  native-lib  libcrypto.a libssl.a ${log-lib}) # 链接动态库
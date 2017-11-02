cmake_minimum_required(VERSION 2.7)

project(ecdsa++)

set(CMAKE_MACOSX_RPATH 1)

find_package(OpenSSL REQUIRED)
find_library(SECP256K1 secp256k1)

include_directories(
  "./include/ecdsa"
  ${OPENSSL_INCLUDE_DIR}
  )

set(CMAKE_CXX_FLAGS "-std=c++11")

file(GLOB src "./src/*.cc")

add_library(ecdsa++ SHARED ${src})

target_link_libraries(ecdsa++ ${OPENSSL_LIBRARIES} ${SECP256K1})


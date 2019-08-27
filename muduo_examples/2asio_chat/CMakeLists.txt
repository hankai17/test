cmake_minimum_required(VERSION 3.2)

include_directories("/root/file/muduo", "root/file/muduo/muduo_examples/2asio_chat")
add_executable(server /root/file/muduo/muduo_examples/2asio_chat/server.cc)
add_executable(client /root/file/muduo/muduo_examples/2asio_chat/client.cc)
target_link_libraries(server /root/file/build/release-install-cpp11/lib/libmuduo_net.a /root/file/build/release-install-cpp11/lib/libmuduo_base.a pthread)
target_link_libraries(client /root/file/build/release-install-cpp11/lib/libmuduo_net.a /root/file/build/release-install-cpp11/lib/libmuduo_base.a pthread)


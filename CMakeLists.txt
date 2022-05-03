cmake_minimum_required(VERSION 3.0.0)
project("UDP - Token - Ring" C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wmissing-prototypes -Werror")

# create the library
add_library(primitives src/primitives.h src/trame.c src/recoit.c src/envoie.c src/creePriseEmission.c src/creePriseReception.c)

# compilation rules to hostA
add_executable(hostA src/HostA.c)
target_link_libraries(hostA primitives)

# compilation rules to hostB
add_executable(hostB src/hostB.c)
target_link_libraries(hostB primitives)

# compilation rules to hostC
add_executable(hostC src/HostC.c)
target_link_libraries(hostC primitives)

# compilation rules to hostD
add_executable(hostD src/HostD.c)
target_link_libraries(hostD primitives)
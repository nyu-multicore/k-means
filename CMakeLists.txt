cmake_minimum_required(VERSION 3.17)
project(kmeans)

set(CMAKE_CXX_STANDARD 17)

add_executable(kmeans_seq src/kmeans_seq.cpp src/utils/kmeans_exception.h src/utils/kmeans_exception.cpp src/utils/parse_args.h src/utils/parse_args.cpp src/utils/parse_input.h src/utils/parse_input.cpp src/utils/random.h src/utils/kmeans_helper.h)
add_executable(kmeans_ver3 src/kmeans_ver3.cpp src/utils/kmeans_exception.h src/utils/kmeans_exception.cpp src/utils/parse_args.h src/utils/parse_args.cpp src/utils/parse_input.h src/utils/parse_input.cpp src/utils/random.h src/utils/kmeans_helper.h)
add_executable(kmeans_par1_1 src/kmeans_par1_1.cpp src/utils/kmeans_exception.h src/utils/kmeans_exception.cpp src/utils/parse_args.h src/utils/parse_args.cpp src/utils/parse_input.h src/utils/parse_input.cpp src/utils/random.h src/utils/kmeans_helper.h)
add_executable(kmeans_par1_2 src/kmeans_par1_2.cpp src/utils/kmeans_exception.h src/utils/kmeans_exception.cpp src/utils/parse_args.h src/utils/parse_args.cpp src/utils/parse_input.h src/utils/parse_input.cpp src/utils/random.h src/utils/kmeans_helper.h)
add_executable(kmeans_par1_3 src/kmeans_par1_3.cpp src/utils/kmeans_exception.h src/utils/kmeans_exception.cpp src/utils/parse_args.h src/utils/parse_args.cpp src/utils/parse_input.h src/utils/parse_input.cpp src/utils/random.h src/utils/kmeans_helper.h)



find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    target_link_libraries(kmeans_seq PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(kmeans_ver3 PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(kmeans_par1_1 PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(kmeans_par1_2 PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(kmeans_par1_3 PUBLIC OpenMP::OpenMP_CXX)
endif ()

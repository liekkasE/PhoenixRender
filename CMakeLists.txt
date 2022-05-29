cmake_minimum_required(VERSION 3.10) 

# set the project name 
project(Phoenix VERSION 1.0) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 


macro(source_group_by_dir source_files)
    if(MSVC)
        set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(sgbd_file ${${source_files}})
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)


# add the executable 

set(Third_party_inc_dir "${CMAKE_CURRENT_SOURCE_DIR}/third-party/" "${CMAKE_CURRENT_SOURCE_DIR}/third-party/spdlog")
set(Phoenix_src_dir "${CMAKE_CURRENT_SOURCE_DIR}/source/")

file(GLOB_RECURSE Sources ${Phoenix_src_dir}/*.hpp ${Phoenix_src_dir}/*.h ${Phoenix_src_dir}/*.cpp)
#source_group("Sources" FILES ${Sources}) 

source_group_by_dir(Sources)

#aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/source" Source_cpps_dir)
add_executable(Phoenix ${Sources}) 


target_link_directories(Phoenix PUBLIC ${Third_party_inc_dir} ${headfiles})
message(${Third_party_inc_dir})
target_link_libraries(Phoenix spdlog d3d11 d3dcompiler)

# 指定项目编译的时候需要include的文件路径，PROJECT_BINARY_DIR变量为编译发生的目录，也就是make执行的目录，PROJECT_SOURCE_DIR为工程所在的目录 
# target_include_directories官方文档：https://cmake.org/cmake/help/v3.3/command/target_include_directories.html 
target_include_directories(Phoenix PUBLIC  ${Phoenix_inc_dir} ${Third_party_inc_dir}) 
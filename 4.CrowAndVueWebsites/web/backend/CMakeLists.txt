cmake_minimum_required(VERSION 3.20)
project(WebBackend VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)

set(EXECUTABLE_FILENAME "web_backend")

# 编译选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
endif()

# 查找源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 包含主头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE
        "third_party/crow/include"
        "third_party/asio/include"
        "third_party/openssl/include"
        "third_party/boost/include/boost-1_89"
        "third_party/sqlite/include"
        "include"
)

# 在编译命令中定义静态文件目录和端点，相当于：
# define CROW_STATIC_DIRECTORY   "dist/assets/"         // 修改默认的 static/ 目录为 dist/assets 目录
# define CROW_STATIC_ENDPOINT    "/assets/<path>"       // 修改默认的 /static/<path> 端点为 /assets/<path> 端点
target_compile_definitions(${PROJECT_NAME} PRIVATE
    CROW_STATIC_DIRECTORY=\"dist/assets/\"
    CROW_STATIC_ENDPOINT=\"/assets/<path>\"
)

# 设置目标属性
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${EXECUTABLE_FILENAME}
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# 自动发现第三方库的函数
function(find_third_party_lib lib_name)
    set(lib_root "third_party/${lib_name}")
    
    if(NOT EXISTS ${lib_root})
        message(WARNING "Third-party library ${lib_name} not found at ${lib_root}")
        return()
    endif()
    
    # 查找库文件
    set(lib_patterns
        "${lib_root}/lib/*.a"
        "${lib_root}/lib/*.dll.a"  # Windows MinGW 的导入库
        "${lib_root}/lib/*.lib"
        "${lib_root}/lib64/*.a"
        "${lib_root}/lib64/*.lib"
        "${lib_root}/*.a"
        "${lib_root}/*.lib"
    )
    
    set(found_libs)
    foreach(pattern IN LISTS lib_patterns)
        file(GLOB libs ${pattern})
        if(libs)
            list(APPEND found_libs ${libs})
        endif()
    endforeach()
    
    # 查找头文件目录
    set(include_dirs
        "${lib_root}/include"
        "${lib_root}"
    )
    
    set(found_include)
    foreach(inc_dir IN LISTS include_dirs)
        if(EXISTS ${inc_dir})
            list(APPEND found_include ${inc_dir})
        endif()
    endforeach()
    
    # 链接库和包含头文件
    if(found_libs AND found_include)
        target_link_libraries(${PROJECT_NAME} ${found_libs})
        target_include_directories(${PROJECT_NAME} PRIVATE ${found_include})
        message(STATUS "Found ${lib_name}: ${found_libs}")
    elseif(found_include AND NOT found_libs)
        # 头文件库（如Crow）
        target_include_directories(${PROJECT_NAME} PRIVATE ${found_include})
        message(STATUS "Found header-only library ${lib_name}")
    else()
        message(WARNING "Cannot properly configure ${lib_name}")
    endif()
endfunction()

# 查找第三方库
set(THIRD_PARTY_LIBS sqlite crow)  # 添加您的第三方库名称
foreach(lib IN LISTS THIRD_PARTY_LIBS)
    find_third_party_lib(${lib})
endforeach()

# 系统库依赖
target_link_libraries(${PROJECT_NAME}
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
        ${CMAKE_SOURCE_DIR}/third_party/sqlite/lib/libsqlite3.a  # 手动指定 sqlite 库
)

# 特定平台设置
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
            ws2_32          # Winsocket2 API
            wsock32         # Windows Socket API
    )
endif()

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# 编译后信息
message(STATUS "=== WebBackend Build Configuration ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Output: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE_FILENAME}")

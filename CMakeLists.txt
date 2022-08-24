cmake_minimum_required(VERSION 3.16)
project(decentralized_path_auction_python)

set(CMAKE_CXX_STANDARD 17)

set(PKG_DEPS
    pybind11_catkin
    decentralized_path_auction
)

find_package(PythonLibs REQUIRED)

find_package(
    catkin
    REQUIRED
    COMPONENTS ${PKG_DEPS}
)

catkin_python_setup()

catkin_package(CATKIN_DEPENDS ${PKG_DEPS})

include_directories(${catkin_INCLUDE_DIRS})

pybind11_add_module(bindings src/bindings.cpp)
target_link_libraries(bindings PRIVATE ${catkin_LIBRARIES})

set_target_properties(bindings
        PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_PYTHON_DESTINATION}
        )

install(TARGETS bindings
        DESTINATION "${PYTHON_INSTALL_DIR}/${PROJECT_NAME}"
        )



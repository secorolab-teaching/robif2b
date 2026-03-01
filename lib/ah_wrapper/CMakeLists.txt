include_directories(include)

# Check for Windows platform
if(WIN32)
    add_definitions(-DPLATFORM_WINDOWS)
    set(SERIAL_LIB "src/winserial.cpp")
    message(STATUS "Platform is Windows")

# Check for Linux platform
elseif(UNIX)
    add_definitions(-DPLATFORM_LINUX)
    set(SERIAL_LIB "src/linux_serial.cpp")
    message(STATUS "Platform is Linux")
endif()

add_library(ah_wrapper SHARED
    ${SERIAL_LIB}
    src/api.cpp
    src/ppp.cpp
    src/serial_helper.cpp
    src/hand.cpp
    src/parser.cpp
    src/wrapper.cpp
)

target_include_directories(ah_wrapper
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(
  TARGETS ah_wrapper
  EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

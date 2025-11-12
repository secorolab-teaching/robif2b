add_library(ability STATIC
  ability.cpp
)

target_include_directories(ability
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(ability
  ah_wrapper
)

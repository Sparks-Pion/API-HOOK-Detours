add_executable(inject_app main.cpp)
target_link_libraries(inject_app detours)
set_target_properties(inject_app PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")
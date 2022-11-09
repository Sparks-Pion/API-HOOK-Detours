add_library(test_dll SHARED testdll.cpp)
target_link_libraries(test_dll detours ntdll ws2_32)
set_target_properties(test_dll PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin/lib")
# test_dll.dll will be placed in the mydlls folder
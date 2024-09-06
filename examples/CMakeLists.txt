include_directories("../")
include_directories("../3dparty/stb")

FILE(GLOB EUCLIB_EXAMPLE_0_FILES example_0.c)
add_executable(example_0 ${EUCLIB_EXAMPLE_0_FILES})
target_link_libraries(example_0 stb_image)
target_link_libraries(example_0 stb_image_write)
target_link_libraries(example_0 m)

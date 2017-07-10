cmake_minimum_required(VERSION 2.8)

project(ExampleLoadImage)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

file(GLOB my_source_files "*.h" "*.cpp" "*.txx" ".c")
add_executable(loadImage ${my_source_files} )     

target_link_libraries(loadImage ${ITK_LIBRARIES} ${VTK_LIBRARIES})

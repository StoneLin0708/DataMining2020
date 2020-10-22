set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(util data.cc data.h)
target_link_libraries(util fmt)

add_executable(apriori apriori.cc fp_tree.cc fp_tree.h gen.h)
target_link_libraries(apriori util fmt)

add_executable(fpg fpg.cc fp_tree.cc fp_tree.h gen.h)
target_link_libraries(fpg util fmt)
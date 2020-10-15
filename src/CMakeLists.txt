
add_executable(fpg fpg.cc data.cc data.h fp_tree.cc fp_tree.h)
target_link_libraries(fpg fmt)
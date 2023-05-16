project(main)
aux_source_directory(src SRC_SUB)
add_executable(code ${SRC_SUB})
include_directories(include)




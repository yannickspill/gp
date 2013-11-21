include_directories("${GP_SOURCE_DIR}/include")
#set(CTEST_MEMORYCHECK_COMMAND "/usr/bin/valgrind")
#set(CTEST_MEMORYCHECK_COMMAND_OPTIONS, "--trace-children=yes --leak-check=full")

file(GLOB tests "*.cpp")
foreach(filename ${tests})
    GET_FILENAME_COMPONENT(basename ${filename} NAME_WE)
    add_executable(${basename} ${filename})
    add_test(${basename} ${basename})
endforeach()

target_link_libraries(test_MultivariateFNormalSufficient
    MultivariateFNormalSufficient)


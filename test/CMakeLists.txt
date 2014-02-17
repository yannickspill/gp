include_directories("${GP_SOURCE_DIR}/include")
#set(CTEST_MEMORYCHECK_COMMAND "/usr/bin/valgrind")
#set(CTEST_MEMORYCHECK_COMMAND_OPTIONS, "--trace-children=yes --leak-check=full")

file(GLOB tests "*.cpp")
foreach(filename ${tests})
    GET_FILENAME_COMPONENT(basename ${filename} NAME_WE)
    add_executable(${basename} ${filename})
    if (${basename} STREQUAL "test_GPI")
        configure_file("../data/gpi_params.dat" "gpi_params.dat" COPYONLY)
        configure_file("../data/gpi_test.dat" "gpi_test.dat" COPYONLY)
        add_test(${basename} ${basename} "gpi_params.dat" "gpi_test.dat" )
    else()
        add_test(${basename} ${basename})
    endif()
endforeach()

target_link_libraries(test_MultivariateFNormalSufficient
    MultivariateFNormalSufficient)


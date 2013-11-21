include_directories("${GP_SOURCE_DIR}/include")

#Basic MVN
file(GLOB files1 "*_BasicMVN.cpp")
foreach(filename ${files1})
    GET_FILENAME_COMPONENT(basename ${filename} NAME_WE)
    add_executable(${basename} ${filename})
endforeach()

#old MVN
file(GLOB files2 "*_MultivariateFNormalSufficient.cpp")
foreach(filename ${files2})
    GET_FILENAME_COMPONENT(basename ${filename} NAME_WE)
    add_executable(${basename} ${filename})
    target_link_libraries(${basename} MultivariateFNormalSufficient)
endforeach()

#MVN
file(GLOB files3 "*_MVN.cpp")
foreach(filename ${files3})
    GET_FILENAME_COMPONENT(basename ${filename} NAME_WE)
    add_executable(${basename} ${filename})
endforeach()


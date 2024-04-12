include(FetchContent)

FetchContent_Declare(
        nfd
        GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
        GIT_TAG 17b6e8ce219c0677f94b63636abb9296b28841ca
)
if (NOT nfd_POPULATED)
    FetchContent_Populate(nfd)
endif ()

set(NFD_PORTAL ON CACHE INTERNAL "" FORCE)
add_subdirectory(${nfd_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/build)
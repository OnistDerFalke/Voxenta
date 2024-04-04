find_library(libmagic_LIBRARY NAMES libmagic magic)
find_path(libmagic_INCLUDE_DIR magic.h PATH_SUFFIXED include)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libmagic
        FOUND_VAR libmagic_FOUND
        REQUIRED_VARS libmagic_LIBRARY libmagic_INCLUDE_DIR
        HANDLE_COMPONENTS
)

if(libmagic_FOUND)
    add_library(libmagic UNKNOWN IMPORTED)
    set_target_properties(libmagic PROPERTIES
            IMPORTED_LOCATION "${libmagic_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${libmaigc_INCLUDE_DIR}"
    )
endif()
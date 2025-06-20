set(httplib_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}")

add_library(httplib::httplib INTERFACE IMPORTED)
set_target_properties(httplib::httplib PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${httplib_INCLUDE_DIRS}"
)
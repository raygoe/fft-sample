# Download SFML.
FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        ${SFML_VERSION}
)

FetchContent_GetProperties(sfml)
if(NOT sfml_POPULATED)
  FetchContent_Populate(sfml)
  set(BUILD_SHARED_LIBS FALSE CACHE BOOL "" FORCE)
  add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif()

# SFML include and library paths
set(SFML_INCLUDE_DIR ${sfml_SOURCE_DIR}/include)

# Library name and path specifics based on platform
if(WIN32)
    set(SFML_LIB_DIR $<IF:$<CONFIG:Debug>,${sfml_BINARY_DIR}/lib/Debug,${sfml_BINARY_DIR}/lib/Release>)
    set(SFML_LIB_PREFIX "")
    set(SFML_STATIC_POSTFIX "-s")
    set(SFML_DEBUG_POSTFIX "-d")
    set(SFML_LIB_EXT ".lib")
    set(SFML_PLATFORM_DEPENDENCIES opengl32 winmm gdi32)
elseif(UNIX AND NOT APPLE)
    set(SFML_LIB_DIR ${sfml_BINARY_DIR}/lib)
    set(SFML_LIB_PREFIX "lib")
    set(SFML_STATIC_POSTFIX "")
    set(SFML_DEBUG_POSTFIX "")
    set(SFML_LIB_EXT ".a")
    set(SFML_PLATFORM_DEPENDENCIES GL X11 UDev pthread)
elseif(APPLE)
    set(SFML_LIB_DIR ${sfml_BINARY_DIR}/lib)
    set(SFML_LIB_PREFIX "lib")
    set(SFML_STATIC_POSTFIX "")
    set(SFML_DEBUG_POSTFIX "")
    set(SFML_LIB_EXT ".a")
    set(SFML_PLATFORM_DEPENDENCIES "-framework OpenGL" "-framework Foundation" "-framework AppKit")
endif()
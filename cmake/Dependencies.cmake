include(FetchContent)

#Version information
set(SFML_VERSION "2.6.0")

# Add SFML support
include(cmake/libs/SFML.cmake)

# Add Library Support
MACRO(add_ext_dependencies arg_target)

    # Add SFML
    target_include_directories(${arg_target} PRIVATE ${SFML_INCLUDE_DIR})

    add_dependencies(${arg_target} sfml-window sfml-graphics sfml-audio sfml-network sfml-system)

    # Link libraries
    foreach(LIB window graphics audio network system)
        target_link_libraries(${arg_target} 
            debug ${SFML_LIB_DIR}/${SFML_LIB_PREFIX}sfml-${LIB}${SFML_STATIC_POSTFIX}${SFML_DEBUG_POSTFIX}${SFML_LIB_EXT}
            optimized ${SFML_LIB_DIR}/${SFML_LIB_PREFIX}sfml-${LIB}${SFML_STATIC_POSTFIX}${SFML_LIB_EXT}
        )
    endforeach()

    target_link_libraries(${arg_target} ${SFML_PLATFORM_DEPENDENCIES} ${OPENAL_LIBRARY} ${FLAC_LIBRARY} ${VORBIS_LIBRARY} ${FREETYPE_LIBRARY})
    target_compile_definitions(${arg_target} PRIVATE SFML_STATIC)

ENDMACRO()
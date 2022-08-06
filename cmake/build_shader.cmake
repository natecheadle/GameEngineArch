function(BUILD_SHADERS SHADER_FILES)

set( _SHADER_FILES ${SHADER_FILES} ${ARGN} )

function(set_shader_command_flags FILE)
    if(${FILE} MATCHES ${VERTEX_REGEX})
        set(SHADER_COMMAND_FLAGS "${VS_FLAGS}" "--type" "vertex" PARENT_SCOPE)
    elseif(${FILE} MATCHES ${FRAGMENT_REGEX})
        set(SHADER_COMMAND_FLAGS "${FS_FLAGS}" "--type" "fragment" PARENT_SCOPE)
    elseif(${FILE} MATCHES ${COMPUTE_REGEX})
        set(SHADER_COMMAND_FLAGS "${CS_FLAGS}" "--type" "compute" PARENT_SCOPE)
    else()
        message(SEND_ERROR "Invalid file ${FILE} expected a match for ${VERTEX_REGEX}, ${FRAGMENT_REGEX}, ${COMPUTE_REGEX}")
    endif()

endfunction()

function(set_shader_values TARGET)
    if(${TARGET} STREQUAL "D3D9")
        set(VS_FLAGS "--platform" "windows" "-p" "vs_3_0" "-O" "3" PARENT_SCOPE)
        set(FS_FLAGS "--platform" "windows" "-p" "ps_3_0" "-O" "3" PARENT_SCOPE)
        set(SHADER_PATH "${SHADER_BIN_DIR}/dx9" PARENT_SCOPE)
    elseif(${TARGET} STREQUAL "D3D11")
        set(VS_FLAGS "--platform" "windows" "-p" "vs_5_0" "-O" "3" PARENT_SCOPE)
        set(FS_FLAGS "--platform" "windows" "-p" "ps_5_0" "-O" "3" PARENT_SCOPE)
        set(CS_FLAGS "--platform" "windows" "-p" "cs_5_0" "-O" "1" PARENT_SCOPE)
        set(SHADER_PATH "${SHADER_BIN_DIR}/dx11" PARENT_SCOPE)
    elseif(${TARGET} STREQUAL "ANDROID")
        set(VS_FLAGS "--platform" "android" PARENT_SCOPE)
        set(FS_FLAGS "--platform" "android" PARENT_SCOPE)
        set(CS_FLAGS "--platform" "android" PARENT_SCOPE)
        set(SHADER_PATH "${SHADER_BIN_DIR}/essl" PARENT_SCOPE)
    elseif(${TARGET} STREQUAL "GLSL")
        set(VS_FLAGS "--platform" "linux" "-p" "120" PARENT_SCOPE)
        set(FS_FLAGS "--platform" "linux" "-p" "120" PARENT_SCOPE)
        set(CS_FLAGS "--platform" "linux" "-p" "430" PARENT_SCOPE)
        set(SHADER_PATH "${SHADER_BIN_DIR}/glsl" PARENT_SCOPE)
    elseif(${TARGET} STREQUAL "METAL")
        set(VS_FLAGS "--platform" "osx" "-p" "metal" PARENT_SCOPE)
        set(FS_FLAGS "--platform" "osx" "-p" "metal" PARENT_SCOPE)
        set(CS_FLAGS "--platform" "osx" "-p" "metal" PARENT_SCOPE)
        set(SHADER_PATH "${SHADER_BIN_DIR}/metal" PARENT_SCOPE)
    # elseif(${TARGET} STREQUAL "PSSL")
    #     set(VS_FLAGS "--platform" "orbis" "-p" "pssl" PARENT_SCOPE)
    #     set(FS_FLAGS "--platform" "orbis" "-p" "pssl" PARENT_SCOPE)
    #     set(CS_FLAGS "--platform" "orbis" "-p" "pssl" PARENT_SCOPE)
    #     set(SHADER_PATH "${SHADER_BIN_DIR}/pssl" PARENT_SCOPE)
    elseif(${TARGET} STREQUAL "SPIRV")
        set(VS_FLAGS "--platform" "linux" "-p" "spirv" PARENT_SCOPE)
        set(FS_FLAGS "--platform" "linux" "-p" "spirv" PARENT_SCOPE)
        set(CS_FLAGS "--platform" "linux" "-p" "spirv" PARENT_SCOPE)
        set(SHADER_PATH "${SHADER_BIN_DIR}/spirv" PARENT_SCOPE)
    else()
        message(SEND_ERROR "Invalid shader target ${TARGET}")
    endif()
endfunction()

if(NOT DEFINED SHADER_BIN_DIR)
    message(STATUS "SHADER_BIN_DIR not set using ${CMAKE_CURRENT_BINARY_DIR}/Shaders/")
    set(SHADER_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/Shaders/ CACHE PATH "Output directory for shader binaries")
endif()

if(NOT DEFINED SHADER_TARGETS)
    message(STATUS "SHADER_TARGETS defaulting to build all targets")
    set(SHADER_TARGETS "ALL" CACHE STRING "Shader targets to build. Supported targets: ALL, D3D9, D3D11, ANDROID, GLSL, METAL, PSSL, SPIRV")
endif()

if(NOT DEFINED VERTEX_REGEX)
    message(STATUS "VERTEX_REGEX not set, using default match of vs_*.sc")
    set(VERTEX_REGEX "vs_.*\.sc$" CACHE STRING "Extension for vertex shader files.")
endif()

if(NOT DEFINED FRAGMENT_REGEX)
    message(STATUS "FRAGMENT_REGEX note set, using default match of fs_*.sc")
    set(FRAGMENT_REGEX "fs_.*\.sc$" CACHE STRING "Extension for vertex shader files.")
endif()

if(NOT DEFINED COMPUTE_REGEX)
    message(STATUS "COMPUTE_REGEX not set, using default match of cs_*.sc")
    set(COMPUTE_REGEX "cs_.*\.sc$" CACHE STRING "Extension for vertex shader files.")
endif()

if(NOT DEFINED BGFX_TOOLS_BIN)
    message(WARNING "BGFX_TOOLS_BIN not set assuming bin directory is in path")
    set(BGFX_TOOLS_BIN "" CACHE PATH "Path to BGFX tools binaries directory.")
endif()

if(SHADER_TARGETS STREQUAL "ALL")
    if (WIN32)
    #PSSL not supported
    #set(SHADER_TARGETS "D3D9;D3D11;ANDROID;GLSL;METAL;PSSL;SPIRV" CACHE STRING "Shader targets to build. Supported targets: ALL, D3D9, D3D11, ANDROID, GLSL, METAL, PSSL, SPIRV" FORCE)
    set(SHADER_TARGETS "D3D9;D3D11;ANDROID;GLSL;METAL;SPIRV" CACHE STRING "Shader targets to build. Supported targets: ALL, D3D9, D3D11, ANDROID, GLSL, METAL, SPIRV" FORCE)
    else()
    #set(SHADER_TARGETS "ANDROID;GLSL;METAL;PSSL;SPIRV" CACHE STRING "Shader targets to build. Supported targets: ALL, D3D9, D3D11, ANDROID, GLSL, METAL, PSSL, SPIRV" FORCE)
    set(SHADER_TARGETS "ANDROID;GLSL;METAL;SPIRV" CACHE STRING "Shader targets to build. Supported targets: ALL, D3D9, D3D11, ANDROID, GLSL, METAL, SPIRV" FORCE)
    endif()
else()
endif()

add_custom_command(
  COMMAND
    ${CMAKE_COMMAND} -E make_directory ${SHADER_BIN_DIR}
  OUTPUT ${SHADER_BIN_DIR}
  COMMENT "Creating ${SHADER_BIN_DIR}"
)

set(SHADERC_COMMAND "${BGFX_TOOLS_BIN}shaderc")

foreach(file IN LISTS _SHADER_FILES)
  get_filename_component(FILENAME ${file} NAME)
  foreach(target IN LISTS SHADER_TARGETS)
    set_shader_values(${target})
    set_shader_command_flags(${file})

    add_custom_command(
        COMMAND
            ${SHADERC_COMMAND}
            ${SHADER_COMMAND_FLAGS}
            -o ${SHADER_PATH}/${FILENAME}.bin
            -f ${CMAKE_CURRENT_SOURCE_DIR}/${file}
            --disasm
        OUTPUT ${SHADER_PATH}/${FILENAME}.bin
        DEPENDS ${file} ${SHADER_BINARY_DIR}
        COMMENT "Compiling ${FILENAME} for target ${target}"
    )

    list(APPEND COMPILED_SHADERS ${SHADER_PATH}/${FILENAME}.bin)
  endforeach()
endforeach()

add_custom_target(SHADER_FILES ALL DEPENDS ${COMPILED_SHADERS})

endfunction()

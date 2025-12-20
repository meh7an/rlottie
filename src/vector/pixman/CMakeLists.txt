
# Only include NEON assembly for 32-bit ARM (not arm64/aarch64 and not macOS)
IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm" AND NOT CMAKE_SYSTEM_PROCESSOR MATCHES "arm64" AND NOT CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64" AND NOT APPLE)
SET(CMAKE_ASM_FLAGS "${CFLAGS} -x assembler-with-cpp")
target_sources(rlottie
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/pixman-arm-neon-asm.S"
    )
ENDIF()

target_include_directories(rlottie
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}"
    )

if(TW_ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY} -extra-arg=-Wno-unknown-warning-option)
        message("Clang-Tidy finished setting up.")
    else()
        message(SEND_ERROR "Clang-Tidy requested but executable not found.")
    endif()
endif()

if (TW_ENABLE_PVS_STUDIO)
    macro(tw_add_pvs_studio_target target)
        message(STATUS "PVS-Studio analyzer enabled - ${CMAKE_SOURCE_DIR}/tools/pvs-studio/config.cfg")
        include(cmake/PVS-Studio.cmake)
        pvs_studio_add_target(TARGET TrustWalletCore.analyze ALL
                OUTPUT FORMAT json
                ANALYZE ${target}
                MODE GA:1,2
                LOG target.err
                CONFIG ${CMAKE_SOURCE_DIR}/tools/pvs-studio/config.cfg
                )
    endmacro()
endif()

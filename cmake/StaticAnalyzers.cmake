if (TW_ENABLE_CLANG_TIDY)
    macro(tw_add_clang_tidy_target target)
        find_program(CLANGTIDY clang-tidy)
        if (CLANGTIDY)
            set_property(
                    TARGET ${target}
                    PROPERTY CXX_CLANG_TIDY clang-tidy;-extra-arg=-Wno-unknown-warning-option)
            message("Clang-Tidy finished setting up.")
        else ()
            message(SEND_ERROR "Clang-Tidy requested but executable not found.")
        endif ()
    endmacro()
endif ()

if (TW_ENABLE_PVS_STUDIO)
    macro(tw_add_pvs_studio_target target)
        message(STATUS "PVS-Studio analyzer enabled - ${CMAKE_SOURCE_DIR}/tools/pvs-studio/config.cfg")
        include(cmake/PVS-Studio.cmake)
        if (TW_IDE_VSCODE)
            pvs_studio_add_target(TARGET TrustWalletCore.analyze ALL
                    OUTPUT FORMAT sarif-vscode
                    ANALYZE ${target}
                    MODE GA:1,2
                    LOG result.sarif
                    CONFIG ${CMAKE_SOURCE_DIR}/tools/pvs-studio/config.cfg
                    )
        else ()
            pvs_studio_add_target(TARGET TrustWalletCore.analyze ALL
                    OUTPUT FORMAT json
                    ANALYZE ${target}
                    MODE GA:1,2
                    LOG result.json
                    CONFIG ${CMAKE_SOURCE_DIR}/tools/pvs-studio/config.cfg
                    )
        endif ()
    endmacro()
endif ()

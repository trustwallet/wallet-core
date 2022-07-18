add_library(tw_error_settings INTERFACE)
add_library(tw::error_settings ALIAS tw_error_settings)

add_library(tw_defaults_features INTERFACE)
add_library(tw::defaults_features ALIAS tw_defaults_features)

add_library(tw_optimize_settings INTERFACE)
add_library(tw::optimize_settings ALIAS tw_optimize_settings)

target_compile_options(
        tw_error_settings
        INTERFACE
        -Wall
        #-Wextra # reasonable and standard
        -Wfatal-errors # short error report
        #-Wshadow # warn the user if a variable declaration shadows one from a
        -Wshorten-64-to-32
        # parent context
        #-Wnon-virtual-dtor # warn the user if a class with virtual functions has a
        # non-virtual destructor. This helps catch hard to
        # track down memory errors
        #-Wold-style-cast # warn for c-style casts
        #-Wcast-align     # warn for potential performance problem casts
        #-Wunused         # warn on anything being unused
        #-Woverloaded-virtual # warn if you overload (not override) a virtual
        # function
        #-Wpedantic   # warn if non-standard C++ is used
        #-Wconversion # warn on type conversions that may lose data
        #-Wsign-conversion  # warn on sign conversions
        #-Wnull-dereference # warn if a null dereference is detected
        #-Wdouble-promotion # warn if float is implicit promoted to double
        -Wformat=2 # warn on security issues around functions that format output
)

if (TW_WARNINGS_AS_ERRORS)
    target_compile_options(
            tw_error_settings
            INTERFACE
            -Werror
    )
endif ()

target_compile_features(tw_defaults_features INTERFACE cxx_std_20)

target_compile_options(tw_optimize_settings INTERFACE
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-O0 -g>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-O2>
        )

function(set_project_warnings project_name)
    target_link_libraries(${project_name} INTERFACE tw::error_settings tw::defaults_features tw::optimize_settings)

    if (NOT TARGET ${project_name})
        message(AUTHOR_WARNING "${project_name} is not a target, thus no compiler warning were added.")
    endif ()
endfunction()

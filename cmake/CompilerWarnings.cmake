# from here:
#
# https://github.com/lefticus/cppbestpractices/blob/master/02-Use_the_Tools_Avai
# lable.md
# Courtesy of Jason Turner

function(set_project_warnings project_name)
    set(CLANG_WARNINGS
            -Wall
            #-Wextra  # reasonable and standard
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
            # (ie printf)
            )

    if (TW_WARNINGS_AS_ERRORS)
        set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
    endif()

    target_compile_options(${project_name} INTERFACE ${CLANG_WARNINGS})

    if(NOT TARGET ${project_name})
        message(AUTHOR_WARNING "${project_name} is not a target, thus no compiler warning were added.")
    endif()
endfunction()

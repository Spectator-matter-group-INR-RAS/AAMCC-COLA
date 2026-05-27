# MIT License

# Copyright (c) 2026 Savva Savenkov

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.24)

project(
    AAMCC-COLA
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

option(BUILTIN_MODULES
       "Fetch and build missing modules. This also adds the COLA provider as a fallback!" OFF
)

if (BUILTIN_MODULES)
    include(FetchContent)
    set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE ALWAYS)
    set(FETCHCONTENT_QUIET FALSE)
    # Disable installation and linters in modules
    set(ENV{COLA_FETCH_MODE} ON)

    # Declare fetch content distros

    FetchContent_Declare(
        CGlauber
        GIT_REPOSITORY https://github.com/Spectator-matter-group-INR-RAS/CGlauber.git
        GIT_TAG main
        GIT_PROGRESS TRUE
    )
    FetchContent_Declare(
        CPreEquilibrium
        GIT_REPOSITORY https://github.com/Spectator-matter-group-INR-RAS/CPreEquilibrium.git
        GIT_TAG main
        GIT_PROGRESS TRUE
    )
    FetchContent_Declare(
        CRoot
        GIT_REPOSITORY https://github.com/Spectator-matter-group-INR-RAS/CRoot.git
        GIT_TAG main
        GIT_PROGRESS TRUE
    )
    FetchContent_Declare(
        Deexcitation
        GIT_REPOSITORY https://github.com/Spectator-matter-group-INR-RAS/DeexcitationHandler.git
        GIT_TAG main
        GIT_PROGRESS TRUE
    )

    # Try to find COLA, configure FetchContent to override find_package() in modules if not found.
    find_package(COLA)
    if (NOT COLA_FOUND)
        FetchContent_Declare(
            COLA
            GIT_REPOSITORY https://github.com/Spectator-matter-group-INR-RAS/COLA.git
            GIT_TAG main
            GIT_PROGRESS TRUE
            OVERRIDE_FIND_PACKAGE TRUE
        )
        FetchContent_MakeAvailable(COLA)
        if (NOT EXISTS ${CMAKE_FIND_PACKAGE_REDIRECTS_DIR}/cola-extra.cmake
            AND NOT EXISTS ${CMAKE_FIND_PACKAGE_REDIRECTS_DIR}/COLAExtra.cmake
        )
            file(
                WRITE ${CMAKE_FIND_PACKAGE_REDIRECTS_DIR}/COLAExtra.cmake
                [=[
                        set(COLA_EVENT_DATA ${cola_SOURCE_DIR}/COLA/EventData.hh)
                        ]=]
            )
        endif ()
    endif ()

    # Already installed modules will be found with find_package()
    FetchContent_MakeAvailable(CGlauber CPreEquilibrium CRoot Deexcitation)
else ()
    find_package(CGlauber REQUIRED)
    find_package(CPreEquilibrium REQUIRED)
    find_package(CRoot REQUIRED)
    find_package(Deexcitation REQUIRED)
endif ()

add_executable(AAMCC-COLA main.cpp)

target_link_libraries(AAMCC-COLA PRIVATE CPreEquilibrium CRoot CGlauber Deexcitation)

if (NOT BUILTIN_MODULES)
    setup_cola_quality_tools()
endif ()

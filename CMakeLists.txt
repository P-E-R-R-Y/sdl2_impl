#----------- general -----------

cmake_minimum_required(VERSION 3.24)
project(sdl2_impl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#----------- project -----------

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(CMakeUtils REQUIRED)
include(${cmakeutils_SOURCE_DIR}/ToolVar.cmake)

cmake_var_extract(BUILD_SHARED_LIBS)
cmake_var_extract(CMAKE_POSITION_INDEPENDENT_CODE)

find_package(System REQUIRED)
find_package(modulemanager REQUIRED)
find_package(igraphic REQUIRED)
find_package(iaudio REQUIRED)
find_package(Sdl2 REQUIRED)

cmake_var_restore(BUILD_SHARED_LIBS)
cmake_var_restore(CMAKE_POSITION_INDEPENDENT_CODE)

set(SOURCE_FILES sources/sdl2.cpp)

# Deux facons de recuperer ce vendor, deux cibles explicites :
#
#   sdl2_impl         SHARED  sdl2_impl.dylib   -> a dlopen (ModuleManager)
#   sdl2_impl_static  STATIC  libsdl2_impl.a    -> a linker en dur
#
# Le type n'est VOLONTAIREMENT pas gouverne par BUILD_SHARED_LIBS : la
# raison d'etre du plugin est d'etre charge a chaud, et un .a echouerait au
# runtime avec un message obscur. Le consommateur choisit par le NOM de la
# cible.
add_library(${PROJECT_NAME}        SHARED ${SOURCE_FILES})
add_library(${PROJECT_NAME}_static STATIC ${SOURCE_FILES})

#----------- dependencies -----------

foreach(flavour ${PROJECT_NAME} ${PROJECT_NAME}_static)
  target_link_libraries(${flavour} PUBLIC
    igraphic
    iaudio

    Sdl2::All
  )
  # la version statique peut finir dans une shared en aval : PIC des le depart
  set_target_properties(${flavour} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endforeach()

# pas de dependance a maths : SdlPolygon porte son propre decoupage en
# oreilles, et Triangle / Vector2f / epsilond viennent de system.

set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

#----------- includes -----------

foreach(flavour ${PROJECT_NAME} ${PROJECT_NAME}_static)
  target_include_directories(${flavour} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/window
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/graphic
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/event
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
  )
endforeach()

# sdl2_impl.dylib, pas libsdl2_impl.dylib : c'est ce nom que
# ModuleManager::Load() recoit dans les tests et que la borne lit.
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

#----------- tests -----------

find_package(GoogleTest REQUIRED)

enable_testing()

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
add_dependencies(${PROJECT_NAME}_tests ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME}_tests PRIVATE
  SDL2_IMPL_PATH="$<TARGET_FILE:${PROJECT_NAME}>"
  ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

target_link_libraries(${PROJECT_NAME}_tests
  gtest_main
  igraphic
  iaudio
  modulemanager
  ${CMAKE_DL_LIBS}
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

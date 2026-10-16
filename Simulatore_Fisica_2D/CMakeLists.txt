# CMakeList.txt: progetto CMake per Simulatore_Fisica_2D. Includere l'origine e definire
# qui la logica specifica del progetto.

# 1) Il motore: libreria con i tuoi file
add_library(engine STATIC Vec2.cpp Body.cpp World.cpp)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(engine PROPERTIES POSITION_INDEPENDENT_CODE ON)

# 2) Il tuo programma di test C++
add_executable(Simulatore_Fisica_2D Simulatore_Fisica_2D.cpp)
target_link_libraries(Simulatore_Fisica_2D PRIVATE engine)

# 3) Il modulo Python
find_package(Python 3.14 EXACT COMPONENTS Interpreter Development.Module REQUIRED)
execute_process(
    COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
    OUTPUT_VARIABLE pybind11_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(fisica bindings.cpp)
target_link_libraries(fisica PRIVATE engine)
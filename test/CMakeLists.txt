string(ASCII 27 Esc)
set(ColorReset "${Esc}[m")
set(Red        "${Esc}[31m")
set(Green      "${Esc}[32m")
set(Yellow     "${Esc}[33m")

add_subdirectory(unit_test)
add_subdirectory(system_test)

set(target ${MP2_TESTS})

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

# Добавляем путь к include директории
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../gtest")

add_executable(${target} ${srcs} ${hdrs})

# Линкуем библиотеки
target_link_libraries(${target} 
    gtest 
    ${MP2_LIBRARY}
)

# Добавляем флаги компиляции
target_compile_definitions(${target} PRIVATE _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)

if(MSVC)
    target_compile_options(${target} PRIVATE /W3 /wd4996)
endif()
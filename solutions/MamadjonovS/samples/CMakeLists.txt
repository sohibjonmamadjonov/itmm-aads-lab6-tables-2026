# Get all cpp-files in the current directory
file(GLOB samples_list RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

foreach(sample_filename ${samples_list})
  # Get file name without extension
  get_filename_component(sample ${sample_filename} NAME_WE)

  # Add and configure executable file to be produced
  add_executable(${sample} ${sample_filename})
  target_link_libraries(${sample} ${MP2_LIBRARY})
  target_include_directories(${sample} PRIVATE ${MP2_INCLUDE})
  
  # Подавление предупреждений для samples
  target_compile_definitions(${sample} PRIVATE _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
  
  if(MSVC)
      target_compile_options(${sample} PRIVATE /W3 /wd4996)
  endif()
  
  set_target_properties(${sample} PROPERTIES
    OUTPUT_NAME "${sample}"
    PROJECT_LABEL "${sample}"
    RUNTIME_OUTPUT_DIRECTORY "../")
endforeach()
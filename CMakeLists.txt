cmake_minimum_required(VERSION 3.16)

project(programming_tech_lab_2 LANGUAGES CXX)

add_executable(${PROJECT_NAME}
	main.cpp
	unit/src/unit.cpp
	unit/src/class_unit.cpp
	unit/src/method_unit.cpp
	unit/src/print_operator_unit.cpp
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/unit
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_EXTENSIONS OFF
)

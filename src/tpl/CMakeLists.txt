include_directories(${PROJECT_SOURCE_DIR}/tpl)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/bookshelf)

#1.TplDB
add_library(db_obj OBJECT tpl_db.cpp)

#2.TplStandardNetModel
add_library(net_model_obj OBJECT tpl_standard_net_model.cpp)

#3.TplStandardNetForceModel
add_library(net_force_model_obj OBJECT tpl_standard_net_force_model.cpp)

#4.TplStandardThermalForceModel
add_library(thermal_force_model_obj OBJECT tpl_standard_thermal_force_model.cpp)

#5.TplStandardAlgorithm
add_library(standard_algorithm_obj OBJECT tpl_standard_algorithm.cpp)

add_subdirectory(unittest)

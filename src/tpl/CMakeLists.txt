include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/bookshelf)

#1.TplDB
add_library(tpl_db_obj OBJECT tpl_db.cpp)

#2.TplNetModel
add_library(tpl_net_model_obj OBJECT tpl_net_model.cpp)

#3.TplThermalModel
add_library(tpl_thermal_model_obj OBJECT tpl_thermal_model.cpp)

#5.TplAlgorithm
add_library(tpl_algorithm_obj OBJECT tpl_algorithm.cpp)

add_subdirectory(unittest)

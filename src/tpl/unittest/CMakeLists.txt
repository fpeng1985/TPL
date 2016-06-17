include_directories(${PROJECT_SOURCE_DIR}/tpl)

#1.test TplDB
add_executable(test_tpl_db $<TARGET_OBJECTS:tpl_db_obj> test_tpl_db.cpp)

#2.test TplNetModel
set(TPL_NET_MODEL_SRC
		$<TARGET_OBJECTS:tpl_db_obj>
		$<TARGET_OBJECTS:tpl_net_model_obj>
		test_tpl_net_model.cpp
		)
add_executable(test_tpl_net_model ${TPL_NET_MODEL_SRC} )

#3.test TplThermalModel
set(TPL_THERMAL_MODEL_SRC
		$<TARGET_OBJECTS:tpl_db_obj>
		$<TARGET_OBJECTS:tpl_thermal_model_obj>
		test_tpl_thermal_model.cpp
		)
add_executable(test_tpl_thermal_model ${TPL_THERMAL_MODEL_SRC} )

#4.test TplAlgorithm
set(TPL_ALGORITHM_SRC
		$<TARGET_OBJECTS:tpl_db_obj>
		$<TARGET_OBJECTS:tpl_net_model_obj>
		$<TARGET_OBJECTS:tpl_thermal_model_obj>
		$<TARGET_OBJECTS:tpl_algorithm_obj>
		test_tpl_algorithm.cpp
		)
add_executable(test_tpl_algorithm ${TPL_ALGORITHM_SRC} )
target_link_libraries(test_tpl_algorithm ${Boost_LIBRARIES} )

#Checks
#add_executable(check_free_nodes_order $<TARGET_OBJECTS:db_obj> check_free_nodes_order.cpp)
#target_link_libraries(check_free_nodes_order bookshelf ${Boost_LIBRARIES})

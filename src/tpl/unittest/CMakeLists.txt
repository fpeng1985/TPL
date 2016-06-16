include_directories(${PROJECT_SOURCE_DIR}/tpl)

#1.TplDB
add_executable(test_tpl_db $<TARGET_OBJECTS:db_obj> test_tpl_db.cpp)
target_link_libraries(test_tpl_db bookshelf ${Boost_LIBRARIES})

#2.TplStandardNetModel
set(TPL_STANDARD_NET_MODEL_SRC
		$<TARGET_OBJECTS:db_obj>
		$<TARGET_OBJECTS:net_model_obj>
		)
add_executable(test_tpl_net_model
		${TPL_STANDARD_NET_MODEL_SRC}
		test_tpl_standard_net_model.cpp
		)
target_link_libraries(test_tpl_net_model bookshelf ${Boost_LIBRARIES})

#3.TplStandardNetForceModel
set(TPL_STANDARD_NET_FORCE_MODEL_SRC
		${TPL_STANDARD_NET_MODEL_SRC}
		$<TARGET_OBJECTS:net_force_model_obj>
		)
add_executable(test_tpl_net_force_model
		${TPL_STANDARD_NET_FORCE_MODEL_SRC}
		test_tpl_standard_net_force_model.cpp
		)
target_link_libraries(test_tpl_net_force_model bookshelf ${Boost_LIBRARIES})

#4.TplStandardThermalForceModel
set(TPL_STANDARD_THERMAL_FORCE_MODEL_SRC
		$<TARGET_OBJECTS:db_obj>
		$<TARGET_OBJECTS:thermal_force_model_obj>
		)
add_executable(test_tpl_thermal_force_model
		${TPL_STANDARD_THERMAL_FORCE_MODEL_SRC}
		test_tpl_standard_thermal_force_model.cpp
		)
target_link_libraries(test_tpl_thermal_force_model
		bookshelf
		${Boost_LIBRARIES}
#		${STXXL_LIBRARIES}
		)

#5.TplStandardAlgorithm
set(TPL_STANDARD_ALGORITHM_SRC
		$<TARGET_OBJECTS:db_obj>
		$<TARGET_OBJECTS:net_model_obj>
		$<TARGET_OBJECTS:net_force_model_obj>
		$<TARGET_OBJECTS:thermal_force_model_obj>
		$<TARGET_OBJECTS:standard_algorithm_obj>
		)
add_executable(test_tpl_standard_algorithm
		${TPL_STANDARD_ALGORITHM_SRC}
		test_tpl_standard_algorithm.cpp
		)
target_link_libraries(test_tpl_standard_algorithm
		bookshelf
		${Boost_LIBRARIES}
#		${STXXL_LIBRARIES}
#		pthread
        )

#4.v2. TplStandardThermalForceAlgorithm
set(TPL_STANDARD_THERMAL_FORCE_SRC
		$<TARGET_OBJECTS:db_obj>
		$<TARGET_OBJECTS:thermal_force_model_obj>
		)
add_executable(test_tpl_standard_thermal_force_model
		${TPL_STANDARD_THERMAL_FORCE_SRC}
		test_tpl_standard_thermal_force_model.cpp
		)
target_link_libraries(test_tpl_standard_thermal_force_model
		bookshelf
		${Boost_LIBRARIES}
#		${STXXL_LIBRARIES}
		pthread
		)


#Checks
add_executable(check_free_nodes_order $<TARGET_OBJECTS:db_obj> check_free_nodes_order.cpp)
target_link_libraries(check_free_nodes_order bookshelf ${Boost_LIBRARIES})

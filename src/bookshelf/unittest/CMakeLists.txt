include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/bookshelf)

#executables
#1.NodeParser
set(BOOKSHELF_NODE_PARSER_TEST_SRC bookshelf_node_parser_test.cpp)
add_executable(test_bks_node_parser  ${BOOKSHELF_NODE_PARSER_TEST_SRC})

#2.PlParser
set(BOOKSHELF_PL_PARSER_TEST_SRC bookshelf_pl_parser_test.cpp)
add_executable(test_bks_pl_parser    ${BOOKSHELF_PL_PARSER_TEST_SRC})

#3.PlGenerator
set(BOOKSHELF_PL_GENERATOR_TEST_SRC bookshelf_pl_generator_test.cpp)
add_executable(test_bks_pl_generator ${BOOKSHELF_PL_GENERATOR_TEST_SRC})

#4.NetParser
set(BOOKSHELF_NET_PARSER_TEST_SRC bookshelf_net_parser_test.cpp)
add_executable(test_bks_net_parser   ${BOOKSHELF_NET_PARSER_TEST_SRC})

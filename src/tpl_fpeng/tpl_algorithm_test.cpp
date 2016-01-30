/*!
 * \file tpl_algorithm_test.cpp
 * \author Peng Fei
 * \brief tpl algorithm unittest.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

#include "tpl_algorithm.h"

using namespace std;
using namespace tpl;

/*
SCENARIO("Test compute_net_weight", "[case 1]") {

    GIVEN("A circuit bigblue1 and a algorithm solver") {
        char path[200];
        char *home;
        home = std::getenv("HOME");
        std::strcpy(path, home);
        std::strcat(path, "/Workspace/TPL/benchmark/ispd2005/bigblue1");

        string _path(path);
        cout << _path << endl;
        db.load_circuit(_path);

        TplAlgorithm alg;

        WHEN("we compute net weight") {
            TplDB::net_iterator nit = db.net_begin();
            cout << nit->pins.size();
            TplAlgorithm::NetWeight x_net_weight, y_net_weight;
            alg.compute_net_weight(nit, x_net_weight, y_net_weight);

            THEN("the net weight is not empty") {
                REQUIRE( x_net_weight.size() != 0);
                REQUIRE( y_net_weight.size() != 0);
            }
        }
    }//end WHEN
}//end SCENARIO

SCENARIO("Test compute_net_force_target", "[case 2]") {

    GIVEN("A circuit bigblue1 and a algorithm solver") {
        char path[200];
        char *home;
        home = std::getenv("HOME");
        std::strcpy(path, home);
        std::strcat(path, "/Workspace/TPL/benchmark/ispd2005/bigblue1");

        string _path(path);
        db.load_circuit(_path);

        TplAlgorithm alg;

        WHEN("we use bound2bound net model to get target positions") {
           vector<double> x_target, y_target; 
           alg.compute_net_force_target(x_target, y_target);

            THEN("the net force target is got") {
                REQUIRE(x_target.size() == 277604);
            }
        }
    }//end GIVEN

}//end SCENARIO
*/

SCENARIO("Test initial_placement", "[case 3]") {

    GIVEN("A circuit bigblue1 and a algorithm solver") {
        char path[200];
        char *home;
        home = std::getenv("HOME");
        std::strcpy(path, home);
        std::strcat(path, "/Workspace/TPL/benchmark/ispd2005/bigblue1");

        string _path(path);
        db.load_circuit(_path);

        TplAlgorithm alg;

        WHEN("we initialize the placement"){
            alg.initial_placement();
            THEN("the placement is changed") {
                db.generate_placement_snapshot();
            }
        }
    }//end GIVEN

}//end SCENARIO


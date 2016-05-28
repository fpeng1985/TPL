/*!
 * \file tpl_standard_algorithm_test.cpp
 * \author Peng Fei
 * \brief Standard tpl algorithm unittest.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tpl_db.h"
#include "tpl_standard_algorithm.h"

#include <iostream>

using namespace std;
using namespace tpl;

SCENARIO("adaptec1", "[adaptec1]") {

    GIVEN("A circuit adaptec1") {
        char path[200];
        char *benchmark;
        benchmark = std::getenv("BENCHMARK");
        std::strcpy(path, benchmark);
        std::strcat(path, "/ispd2005/adaptec1");
        cout << "path = " << path << endl;
        TplDB::db().load_circuit(path);
        cout << "load circuit finished" << endl;


        unique_ptr<TplStandardAlgorithm> alg(new TplStandardAlgorithm());
        unique_ptr<TplStandardNetModel> snm(new TplStandardNetModel());


        WHEN("we shred macros") {
            int sizebefore = TplDB::db().modules.size();
            cout << "size before " << sizebefore << endl;
            alg->shred();

            int sizeafter = TplDB::db().modules.size();
            cout << "size after " << sizeafter << endl;
            THEN("shred size is not equal") {
                REQUIRE(sizebefore != sizeafter);
            }
        }
        /*
        WHEN("we compute the first net's weight") {
            NetWeight x_net_weight, y_net_weight;
            SpMat Cx, Cy;
            snm->compute_net_weight(x_net_weight, y_net_weight);

            THEN("the net weight is not empty") {
                REQUIRE( x_net_weight.size() != 0);
                REQUIRE( y_net_weight.size() != 0);
            }
        }

        WHEN("we compute net force target") {
            vector<double> x_target, y_target;
            alg->net_force_model().compute_net_force_target(x_target, y_target);

            THEN("the targets are not empty") {
                REQUIRE( x_target.size() != 0);
                REQUIRE( y_target.size() != 0);
            }
        }

        WHEN("we make the initial placement") {
            alg->make_initial_placement();

            THEN("the free modules' coordinates will be chaned") {
                TplDB::db().generate_placement_snapshot();
            }
        }
         */

    }

}//end SCENARIO


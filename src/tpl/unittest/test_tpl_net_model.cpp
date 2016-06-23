/*!
 * \file tpl_standard_net_model_test.cpp
 * \author Peng Fei
 * \brief Standard tpl net model unittest.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tpl_net_model.h"

using namespace std;
using namespace tpl;

SCENARIO("adaptec1", "[adaptec1]") {

    GIVEN("A circuit adaptec1") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec1";

        double t0 = double(clock());
        TplDB::db().load_data(path);
        double t1 = double(clock());
        printf("load circuit finish, took %.3lf seconds\n", (t1 - t0) / CLOCKS_PER_SEC);

        TplNetModel nmodel;

        WHEN("we compute the first net's weight") {
            NetWeight x_net_weight, y_net_weight;

            double t2 = double(clock());
            nmodel.compute_net_weight(x_net_weight, y_net_weight);
            double t3 = double(clock());
            printf("compute net weight, took %.3lf seconds\n", (t3 - t2) / CLOCKS_PER_SEC);

            THEN("the net weight is not empty") {
                REQUIRE( x_net_weight.size() != 0);
                REQUIRE( y_net_weight.size() != 0);

                for(NetWeight::iterator it=x_net_weight.begin(); it!=x_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }

                for(NetWeight::iterator it=y_net_weight.begin(); it!=y_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }
            }
        }

        WHEN("We compute the net force target") {
            NetWeight NWx, NWy;
            nmodel.compute_net_weight(NWx, NWy);

            int num_free = TplDB::db().num_free();

            vector<double> x_target, y_target;
            x_target.reserve(num_free);
            y_target.reserve(num_free);

            nmodel.compute_net_force_target(NWx, NWy, x_target, y_target);
            TplDB::db().set_free_module_coordinates(x_target, y_target);

            THEN("We get the free nodes' new positions") {
                double width  = TplDB::db().chip_width();
                double height = TplDB::db().chip_height();

//                ofstream fout("/home/gaoy/tem");

                for(auto x : x_target) {
//                    fout << x << endl;
                    REQUIRE(x >= 0);
                    REQUIRE(x <= width);
                }

                for(auto y : y_target) {
                    REQUIRE(y >= 0);
                    REQUIRE(y <= height);
                }
            }
        }
    }
}//end SCENARIO

/*
SCENARIO("adaptec2", "[adaptec2]") {

    GIVEN("A circuit adaptec2") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/adaptec2";

        TplDB::db().load_circuit(path);

        TplStandardNetModel nmodel;

        WHEN("we compute the first net's weight") {
            NetWeight x_net_weight, y_net_weight;
            nmodel.compute_net_weight(x_net_weight, y_net_weight);

            THEN("the net weight is not empty") {
                REQUIRE( x_net_weight.size() != 0);
                REQUIRE( y_net_weight.size() != 0);

                for(NetWeight::iterator it=x_net_weight.begin(); it!=x_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }

                for(NetWeight::iterator it=y_net_weight.begin(); it!=y_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }
            }
        }
    }
}//end SCENARIO

SCENARIO("adaptec3", "[adaptec3]") {

    GIVEN("A circuit adaptec3") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/adaptec3";

        TplDB::db().load_circuit(path);

        TplStandardNetModel nmodel;

        WHEN("we compute the first net's weight") {
            NetWeight x_net_weight, y_net_weight;
            nmodel.compute_net_weight(x_net_weight, y_net_weight);

            THEN("the net weight is not empty") {
                REQUIRE( x_net_weight.size() != 0);
                REQUIRE( y_net_weight.size() != 0);

                for(NetWeight::iterator it=x_net_weight.begin(); it!=x_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }

                for(NetWeight::iterator it=y_net_weight.begin(); it!=y_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }
            }
        }
    }
}//end SCENARIO

SCENARIO("adaptec4", "[adaptec4]") {

    GIVEN("A circuit adaptec4") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/adaptec4";

        TplDB::db().load_circuit(path);

        TplStandardNetModel nmodel;

        WHEN("we compute the first net's weight") {
            NetWeight x_net_weight, y_net_weight;
            nmodel.compute_net_weight(x_net_weight, y_net_weight);

            THEN("the net weight is not empty") {
                REQUIRE( x_net_weight.size() != 0);
                REQUIRE( y_net_weight.size() != 0);

                for(NetWeight::iterator it=x_net_weight.begin(); it!=x_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }

                for(NetWeight::iterator it=y_net_weight.begin(); it!=y_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }
            }
        }
    }
}//end SCENARIO

SCENARIO("bigblue1", "[bigblue1]") {

    GIVEN("A circuit bigblue1") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/bigblue1";

        TplDB::db().load_circuit(path);

        TplStandardNetModel nmodel;

        WHEN("we compute the first net's weight") {
            NetWeight x_net_weight, y_net_weight;
            nmodel.compute_net_weight(x_net_weight, y_net_weight);

            THEN("the net weight is not empty") {
                REQUIRE( x_net_weight.size() != 0);
                REQUIRE( y_net_weight.size() != 0);

                for(NetWeight::iterator it=x_net_weight.begin(); it!=x_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }

                for(NetWeight::iterator it=y_net_weight.begin(); it!=y_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }
            }
        }
    }
}//end SCENARIO

SCENARIO("bigblue2", "[bigblue2]") {

    GIVEN("A circuit bigblue2") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/bigblue2";

        TplDB::db().load_circuit(path);

        TplStandardNetModel nmodel;

        WHEN("we compute the first net's weight") {
            NetWeight x_net_weight, y_net_weight;
            nmodel.compute_net_weight(x_net_weight, y_net_weight);

            THEN("the net weight is not empty") {
                REQUIRE( x_net_weight.size() != 0);
                REQUIRE( y_net_weight.size() != 0);

                for(NetWeight::iterator it=x_net_weight.begin(); it!=x_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }

                for(NetWeight::iterator it=y_net_weight.begin(); it!=y_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }
            }
        }
    }
}//end SCENARIO

SCENARIO("bigblue3", "[bigblue3]") {

    GIVEN("A circuit bigblue3") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/bigblue3";

        TplDB::db().load_circuit(path);

        TplStandardNetModel nmodel;

        WHEN("we compute the first net's weight") {
            NetWeight x_net_weight, y_net_weight;
            nmodel.compute_net_weight(x_net_weight, y_net_weight);

            THEN("the net weight is not empty") {
                REQUIRE( x_net_weight.size() != 0);
                REQUIRE( y_net_weight.size() != 0);

                for(NetWeight::iterator it=x_net_weight.begin(); it!=x_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }

                for(NetWeight::iterator it=y_net_weight.begin(); it!=y_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }
            }
        }
    }
}//end SCENARIO

SCENARIO("bigblue4", "[bigblue4]") {

    GIVEN("A circuit bigblue4") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/bigblue4";

        TplDB::db().load_circuit(path);

        TplStandardNetModel nmodel;

        WHEN("we compute the first net's weight") {
            NetWeight x_net_weight, y_net_weight;
            nmodel.compute_net_weight(x_net_weight, y_net_weight);

            THEN("the net weight is not empty") {
                REQUIRE( x_net_weight.size() != 0);
                REQUIRE( y_net_weight.size() != 0);

                for(NetWeight::iterator it=x_net_weight.begin(); it!=x_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }

                for(NetWeight::iterator it=y_net_weight.begin(); it!=y_net_weight.end(); ++it) {
                    REQUIRE( it->second > 0 );
                }
            }
        }
    }
}//end SCENARIO
 */



/*!
 * \file tpl_standard_net_model_test.cpp
 * \author Peng Fei
 * \brief Standard tpl net model unittest.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tpl_db.h"
#include "tpl_standard_net_model.h"

using namespace std;
using namespace tpl;
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

SCENARIO("adaptec1", "[adaptec1]") {

    GIVEN("A circuit adaptec1") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/adaptec1";

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

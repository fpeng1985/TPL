/*!
 * \file tpl_circuit_test.cpp
 * \author Peng Fei
 * \brief tpl circuit related data structures unittest.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tpl_db.h"

using namespace std;
using namespace tpl;
SCENARIO("adaptec1", "[adaptec1]") {

    GIVEN("A circuit adaptec1") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/adaptec1";

        WHEN("We load the circuit") {
            bool load_status = TplDB::db().load_circuit(path);

            THEN("We get its size and cell number") {
                REQUIRE(load_status == true);
                REQUIRE(TplDB::db().modules.chip_width() == 11589);
                REQUIRE(TplDB::db().modules.chip_height() == 11589);
                REQUIRE(TplDB::db().modules.num_free() == 210904);
            }
        }
    }
}//end adaptec1

/*
SCENARIO("adaptec2", "[adaptec2]") {

    GIVEN("A circuit adaptec2") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/adaptec2";

        WHEN("We load the circuit") {
            bool load_status = TplDB::db().load_circuit(path);

            THEN("We get its size and cell number") {
                REQUIRE(load_status == true);
                REQUIRE(TplDB::db().modules.chip_width() == 15244);
                REQUIRE(TplDB::db().modules.chip_height() == 15244);
                REQUIRE(TplDB::db().modules.num_free() == 254457);
            }
        }
    }
}//end adaptec2

SCENARIO("adaptec3", "[adaptec3]") {

    GIVEN("A circuit adaptec3") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/adaptec3";

        WHEN("We load the circuit") {
            bool load_status = TplDB::db().load_circuit(path);

            THEN("We get its size and cell number") {
                REQUIRE(load_status == true);
                REQUIRE(TplDB::db().modules.chip_width() == 23190);
                REQUIRE(TplDB::db().modules.chip_height() == 23386);
                REQUIRE(TplDB::db().modules.num_free() == 450927);
            }
        }
    }
}//end adaptec3

SCENARIO("adaptec4", "[adaptec4]") {

    GIVEN("Test case adaptec4") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/adaptec4";

        WHEN("We load the circuit") {
            bool load_status = TplDB::db().load_circuit(path);

            THEN("We get its size and cell number") {
                REQUIRE(load_status == true);
                REQUIRE(TplDB::db().modules.chip_width() == 23226);
                REQUIRE(TplDB::db().modules.chip_height() == 23386);
                REQUIRE(TplDB::db().modules.num_free() == 494716);
            }
        }
    }
}//end adaptec4

SCENARIO("bigblue1", "[bigblue1]") {

    GIVEN("A circuit bigblue1") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/bigblue1";

        WHEN("We load the circuit") {
            bool load_status = TplDB::db().load_circuit(path);

            THEN("We get its size and cell number") {
                REQUIRE(load_status == true);
                REQUIRE(TplDB::db().modules.chip_width()  == 11589);
                REQUIRE(TplDB::db().modules.chip_height() == 11589);
                REQUIRE(TplDB::db().modules.num_free()    == 277604);
            }
        }
    }
}//end bigblue1

SCENARIO("bigblue2", "[bigblue2]") {

    GIVEN("A circuit bigblue2") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/bigblue2";

        WHEN("We load the circuit") {
            bool load_status = TplDB::db().load_circuit(path);

            THEN("We get its size and cell number") {
                REQUIRE(load_status == true);
                REQUIRE(TplDB::db().modules.chip_width() == 18721);
                REQUIRE(TplDB::db().modules.chip_height() == 18796);
                REQUIRE(TplDB::db().modules.num_free() == 534782);
            }
        }
    }
}//end bigblue2

SCENARIO("bigblue3", "[bigblue3]") {

    GIVEN("A circuit bigblue3") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/bigblue3";

        WHEN("We load the circuit") {
            bool load_status = TplDB::db().load_circuit(path);

            THEN("We get its size and cell number") {
                REQUIRE(load_status == true);
                REQUIRE(TplDB::db().modules.chip_width() == 27693);
                REQUIRE(TplDB::db().modules.chip_height() == 27868);
                REQUIRE(TplDB::db().modules.num_free() == 1095519);
            }
        }
    }
}//end bigblue3

SCENARIO("bigblue4", "[bigblue4]") {

    GIVEN("A circuit bigblue4") {
        string path(getenv("BENCHMARK"));
        path += "/ispd2005/bigblue4";

        WHEN("We load the circuit") {
            bool load_status = TplDB::db().load_circuit(path);

            THEN("We get its size and cell number") {
                REQUIRE(load_status == true);
                REQUIRE(TplDB::db().modules.chip_width() == 32223);
                REQUIRE(TplDB::db().modules.chip_height() == 32386);
                REQUIRE(TplDB::db().modules.num_free() == 2169183);
            }
        }
    }
}//end bigblue4
 */



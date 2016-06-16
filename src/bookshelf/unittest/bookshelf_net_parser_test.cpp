/*!
 * \file bookshelf_net_parser_test.cpp
 * \author Peng Fei
 * \brief bookshelf net parser unittest.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "bookshelf_net.hpp"

using namespace std;
using namespace bks;

SCENARIO("adaptec1", "[adaptec1]") {

    GIVEN("A .nets file and a BookshelfNetParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec1/adaptec1.nets";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;
        BookshelfNets nets;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_net(iter, end, nets);

            THEN("we get a bunch of Bookshelf nets") {
                cout << "data.size = " << nets.data.size() << endl;
                cout << "num_nets = " << nets.num_nets << endl;

                REQUIRE(ret == true);
                REQUIRE(nets.data.size() == nets.num_nets);
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec1

SCENARIO("adaptec2", "[adaptec2]") {

    GIVEN("A .nets file and a BookshelfNetParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec2/adaptec2.nets";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;
        BookshelfNets nets;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_net(iter, end, nets);

            THEN("we get a bunch of Bookshelf nets") {
                cout << "data.size = " << nets.data.size() << endl;
                cout << "num_nets = " << nets.num_nets << endl;

                REQUIRE(ret == true);
                REQUIRE(nets.data.size() == nets.num_nets);
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec2

SCENARIO("adaptec3", "[adaptec3]") {

    GIVEN("A .nets file and a BookshelfNetParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec3/adaptec3.nets";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;
        BookshelfNets nets;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_net(iter, end, nets);

            THEN("we get a bunch of Bookshelf nets") {
                cout << "data.size = " << nets.data.size() << endl;
                cout << "num_nets = " << nets.num_nets << endl;

                REQUIRE(ret == true);
                REQUIRE(nets.data.size() == nets.num_nets);
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec3

SCENARIO("adaptec4", "[adaptec4]") {

    GIVEN("A .nets file and a BookshelfNetParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec4/adaptec4.nets";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;
        BookshelfNets nets;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_net(iter, end, nets);

            THEN("we get a bunch of Bookshelf nets") {
                cout << "data.size = " << nets.data.size() << endl;
                cout << "num_nets = " << nets.num_nets << endl;

                REQUIRE(ret == true);
                REQUIRE(nets.data.size() == nets.num_nets);
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec4

SCENARIO("bigblue1", "[bigblue1]") {

    GIVEN("A .nets file and a BookshelfNetParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue1/bigblue1.nets";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;
        BookshelfNets nets;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_net(iter, end, nets);

            THEN("we get a bunch of Bookshelf nets") {
                cout << "data.size = " << nets.data.size() << endl;
                cout << "num_nets = " << nets.num_nets << endl;

                REQUIRE(ret == true);
                REQUIRE(nets.data.size() == nets.num_nets);
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue1

SCENARIO("bigblue2", "[bigblue2]") {

    GIVEN("A .nets file and a BookshelfNetParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue2/bigblue2.nets";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;
        BookshelfNets nets;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_net(iter, end, nets);

            THEN("we get a bunch of Bookshelf nets") {
                cout << "data.size = " << nets.data.size() << endl;
                cout << "num_nets = " << nets.num_nets << endl;

                REQUIRE(ret == true);
                REQUIRE(nets.data.size() == nets.num_nets);
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue2

SCENARIO("bigblue3", "[bigblue3]") {

    GIVEN("A .nets file and a BookshelfNetParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue3/bigblue3.nets";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;
        BookshelfNets nets;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_net(iter, end, nets);

            THEN("we get a bunch of Bookshelf nets") {
                cout << "data.size = " << nets.data.size() << endl;
                cout << "num_nets = " << nets.num_nets << endl;

                REQUIRE(ret == true);
                REQUIRE(nets.data.size() == nets.num_nets);
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue3

SCENARIO("bigblue4", "[bigblue4]") {

    GIVEN("A .nets file and a BookshelfNetParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue4/bigblue4.nets";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;
        BookshelfNets nets;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_net(iter, end, nets);

            THEN("we get a bunch of Bookshelf nets") {
                cout << "data.size = " << nets.data.size() << endl;
                cout << "num_nets = " << nets.num_nets << endl;

                REQUIRE(ret == true);
                REQUIRE(nets.data.size() == nets.num_nets);
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue4

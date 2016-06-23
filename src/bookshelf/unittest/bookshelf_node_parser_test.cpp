/*!
 * \file bookshelf_node_parser_test.cpp
 * \author Peng Fei
 * \brief bookshelf node parser unittest.
 */

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "bookshelf_node.hpp"

using namespace std;
using namespace bks;

SCENARIO("adaptec1", "[adaptec1]") {

    GIVEN("A .nodes file and a BookshelfNodeParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec1/adaptec1.nodes";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;

        ofstream out("adaptec1.nodes.out", ios_base::out);
        boost::spirit::ostream_iterator ositer(out);
        BookshelfNodes nodes;

        WHEN("we parse the input") {
            //test parsing
            bool ret = parse_bookshelf_node(iter, end, nodes);

            cout << "data.size " << nodes.data.size() << endl;
            cout << "num_nodes " << nodes.num_nodes << endl;
            cout << "num_terminals " << nodes.num_terminals << endl;

            THEN("we get a bunch of Bookshelf nodes") {
                REQUIRE(ret == true);
            }

            //test generating
            ret = generate_bookshelf_node(ositer, nodes);

            THEN("we generate a nodes file") {
                REQUIRE(ret == true);
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec1

SCENARIO("adaptec2", "[adaptec2]") {

    GIVEN("A .nodes file and a BookshelfNodeParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec2/adaptec2.nodes";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;

        BookshelfNodes nodes;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_node(iter, end, nodes);

            cout << "data.size " << nodes.data.size() << endl;
            cout << "num_nodes " << nodes.num_nodes << endl;
            cout << "num_terminals " << nodes.num_terminals << endl;

            THEN("we get a bunch of Bookshelf nodes") {
                REQUIRE(ret == true);
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec2


SCENARIO("adaptec3", "[adaptec3]") {

    GIVEN("A .nodes file and a BookshelfNodeParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec3/adaptec3.nodes";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;

        BookshelfNodes nodes;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_node(iter, end, nodes);

            cout << "data.size " << nodes.data.size() << endl;
            cout << "num_nodes " << nodes.num_nodes << endl;
            cout << "num_terminals " << nodes.num_terminals << endl;

            THEN("we get a bunch of Bookshelf nodes") {
                REQUIRE(ret == true);
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec3

SCENARIO("adaptec4", "[adaptec4]") {

    GIVEN("A .nodes file and a BookshelfNodeParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec4/adaptec4.nodes";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;

        BookshelfNodes nodes;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_node(iter, end, nodes);

            cout << "data.size " << nodes.data.size() << endl;
            cout << "num_nodes " << nodes.num_nodes << endl;
            cout << "num_terminals " << nodes.num_terminals << endl;

            THEN("we get a bunch of Bookshelf nodes") {
                REQUIRE(ret == true);
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec4

SCENARIO("bigblue1", "[bigblue1]") {

    GIVEN("A .nodes file and a BookshelfNodeParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue1/bigblue1.nodes";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;

        BookshelfNodes nodes;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_node(iter, end, nodes);

            cout << "data.size " << nodes.data.size() << endl;
            cout << "num_nodes " << nodes.num_nodes << endl;
            cout << "num_terminals " << nodes.num_terminals << endl;

            THEN("we get a bunch of Bookshelf nodes") {
                REQUIRE(ret == true);
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue1

SCENARIO("bigblue2", "[bigblue2]") {

    GIVEN("A .nodes file and a BookshelfNodeParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue2/bigblue2.nodes";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;

        BookshelfNodes nodes;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_node(iter, end, nodes);

            cout << "data.size " << nodes.data.size() << endl;
            cout << "num_nodes " << nodes.num_nodes << endl;
            cout << "num_terminals " << nodes.num_terminals << endl;

            THEN("we get a bunch of Bookshelf nodes") {
                REQUIRE(ret == true);
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue2


SCENARIO("bigblue3", "[bigblue3]") {

    GIVEN("A .nodes file and a BookshelfNodeParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue3/bigblue3.nodes";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;

        BookshelfNodes nodes;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_node(iter, end, nodes);

            cout << "data.size " << nodes.data.size() << endl;
            cout << "num_nodes " << nodes.num_nodes << endl;
            cout << "num_terminals " << nodes.num_terminals << endl;

            THEN("we get a bunch of Bookshelf nodes") {
                REQUIRE(ret == true);
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue3

SCENARIO("bigblue4", "[bigblue4]") {

    GIVEN("A .nodes file and a BookshelfNodeParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue4/bigblue4.nodes";

        ifstream in(path, ios_base::in);
        in.unsetf(ios::skipws);
        boost::spirit::istream_iterator iter(in), end;

        BookshelfNodes nodes;

        WHEN("we parse the input") {
            bool ret = parse_bookshelf_node(iter, end, nodes);

            cout << "data.size " << nodes.data.size() << endl;
            cout << "num_nodes " << nodes.num_nodes << endl;
            cout << "num_terminals " << nodes.num_terminals << endl;

            THEN("we get a bunch of Bookshelf nodes") {
                REQUIRE(ret == true);
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue4
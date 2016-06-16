/*!
 * \file bookshelf_pl_parser_test.cpp
 * \author Peng Fei
 * \brief bookshelf pl parser unittest.
 */

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "bookshelf_node.hpp"
#include "bookshelf_pl.hpp"

using namespace std;
using namespace bks;

SCENARIO("adaptec1", "[adaptec1]") {

    GIVEN("A .pl file and a BookshelfPlParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec1/adaptec1";

        ifstream nodes_in(path+".nodes", ios_base::in);
        nodes_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator nodes_iter(nodes_in), nodes_end;
        BookshelfNodes nodes;

        ifstream pl_in(path+".pl", ios_base::in);
        pl_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator pl_iter(pl_in), pl_end;
        BookshelfPls pls;

        WHEN("we parse the input") {
            bool nodes_ret = parse_bookshelf_node(nodes_iter, nodes_end, nodes);
            bool pl_ret = parse_bookshelf_pl(pl_iter, pl_end, pls);

            cout << "data.size " << pls.data.size() << endl;
            cout << "nodes num " << nodes.num_nodes << endl;

            THEN("we get a bunch of Bookshelf pls") {
                REQUIRE(nodes_ret == true);
                REQUIRE(pl_ret == true);
                REQUIRE(pls.data.size() == nodes.data.size());
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec1

SCENARIO("adaptec2", "[adaptec2]") {

    GIVEN("A .pl file and a BookshelfPlParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec2/adaptec2";

        ifstream nodes_in(path+".nodes", ios_base::in);
        nodes_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator nodes_iter(nodes_in), nodes_end;
        BookshelfNodes nodes;

        ifstream pl_in(path+".pl", ios_base::in);
        pl_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator pl_iter(pl_in), pl_end;
        BookshelfPls pls;

        WHEN("we parse the input") {
            bool nodes_ret = parse_bookshelf_node(nodes_iter, nodes_end, nodes);
            bool pl_ret = parse_bookshelf_pl(pl_iter, pl_end, pls);

            cout << "data.size " << pls.data.size() << endl;
            cout << "nodes num " << nodes.num_nodes << endl;

            THEN("we get a bunch of Bookshelf pls") {
                REQUIRE(nodes_ret == true);
                REQUIRE(pl_ret == true);
                REQUIRE(pls.data.size() == nodes.data.size());
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec2

SCENARIO("adaptec3", "[adaptec3]") {

    GIVEN("A .pl file and a BookshelfPlParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec3/adaptec3";

        ifstream nodes_in(path+".nodes", ios_base::in);
        nodes_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator nodes_iter(nodes_in), nodes_end;
        BookshelfNodes nodes;

        ifstream pl_in(path+".pl", ios_base::in);
        pl_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator pl_iter(pl_in), pl_end;
        BookshelfPls pls;

        WHEN("we parse the input") {
            bool nodes_ret = parse_bookshelf_node(nodes_iter, nodes_end, nodes);
            bool pl_ret = parse_bookshelf_pl(pl_iter, pl_end, pls);

            cout << "data.size " << pls.data.size() << endl;
            cout << "nodes num " << nodes.num_nodes << endl;

            THEN("we get a bunch of Bookshelf pls") {
                REQUIRE(nodes_ret == true);
                REQUIRE(pl_ret == true);
                REQUIRE(pls.data.size() == nodes.data.size());
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec3

SCENARIO("adaptec4", "[adaptec4]") {

    GIVEN("A .pl file and a BookshelfPlParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/adaptec4/adaptec4";

        ifstream nodes_in(path+".nodes", ios_base::in);
        nodes_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator nodes_iter(nodes_in), nodes_end;
        BookshelfNodes nodes;

        ifstream pl_in(path+".pl", ios_base::in);
        pl_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator pl_iter(pl_in), pl_end;
        BookshelfPls pls;

        WHEN("we parse the input") {
            bool nodes_ret = parse_bookshelf_node(nodes_iter, nodes_end, nodes);
            bool pl_ret = parse_bookshelf_pl(pl_iter, pl_end, pls);

            cout << "data.size " << pls.data.size() << endl;
            cout << "nodes num " << nodes.num_nodes << endl;

            THEN("we get a bunch of Bookshelf pls") {
                REQUIRE(nodes_ret == true);
                REQUIRE(pl_ret == true);
                REQUIRE(pls.data.size() == nodes.data.size());
            }
        }

        cout << endl;
    }//end GIVEN

}//end adaptec4

SCENARIO("bigblue1", "[bigblue1]") {

    GIVEN("A .pl file and a BookshelfPlParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue1/bigblue1";

        ifstream nodes_in(path+".nodes", ios_base::in);
        nodes_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator nodes_iter(nodes_in), nodes_end;
        BookshelfNodes nodes;

        ifstream pl_in(path+".pl", ios_base::in);
        pl_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator pl_iter(pl_in), pl_end;
        BookshelfPls pls;

        WHEN("we parse the input") {
            bool nodes_ret = parse_bookshelf_node(nodes_iter, nodes_end, nodes);
            bool pl_ret = parse_bookshelf_pl(pl_iter, pl_end, pls);

            cout << "data.size " << pls.data.size() << endl;
            cout << "nodes num " << nodes.num_nodes << endl;

            THEN("we get a bunch of Bookshelf pls") {
                REQUIRE(nodes_ret == true);
                REQUIRE(pl_ret == true);
                REQUIRE(pls.data.size() == nodes.data.size());
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue1

SCENARIO("bigblue2", "[bigblue2]") {

    GIVEN("A .pl file and a BookshelfPlParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue2/bigblue2";

        ifstream nodes_in(path+".nodes", ios_base::in);
        nodes_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator nodes_iter(nodes_in), nodes_end;
        BookshelfNodes nodes;

        ifstream pl_in(path+".pl", ios_base::in);
        pl_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator pl_iter(pl_in), pl_end;
        BookshelfPls pls;

        WHEN("we parse the input") {
            bool nodes_ret = parse_bookshelf_node(nodes_iter, nodes_end, nodes);
            bool pl_ret = parse_bookshelf_pl(pl_iter, pl_end, pls);

            cout << "data.size " << pls.data.size() << endl;
            cout << "nodes num " << nodes.num_nodes << endl;

            THEN("we get a bunch of Bookshelf pls") {
                REQUIRE(nodes_ret == true);
                REQUIRE(pl_ret == true);
                REQUIRE(pls.data.size() == nodes.data.size());
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue2

SCENARIO("bigblue3", "[bigblue3]") {

    GIVEN("A .pl file and a BookshelfPlParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue3/bigblue3";

        ifstream nodes_in(path+".nodes", ios_base::in);
        nodes_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator nodes_iter(nodes_in), nodes_end;
        BookshelfNodes nodes;

        ifstream pl_in(path+".pl", ios_base::in);
        pl_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator pl_iter(pl_in), pl_end;
        BookshelfPls pls;

        WHEN("we parse the input") {
            bool nodes_ret = parse_bookshelf_node(nodes_iter, nodes_end, nodes);
            bool pl_ret = parse_bookshelf_pl(pl_iter, pl_end, pls);

            cout << "data.size " << pls.data.size() << endl;
            cout << "nodes num " << nodes.num_nodes << endl;

            THEN("we get a bunch of Bookshelf pls") {
                REQUIRE(nodes_ret == true);
                REQUIRE(pl_ret == true);
                REQUIRE(pls.data.size() == nodes.data.size());
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue3

SCENARIO("bigblue4", "[bigblue4]") {

    GIVEN("A .pl file and a BookshelfPlParser") {
        string path(getenv("EDACOMMON"));
        path += "/benchmark/ispd2005/bigblue4/bigblue4";

        ifstream nodes_in(path+".nodes", ios_base::in);
        nodes_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator nodes_iter(nodes_in), nodes_end;
        BookshelfNodes nodes;

        ifstream pl_in(path+".pl", ios_base::in);
        pl_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator pl_iter(pl_in), pl_end;
        BookshelfPls pls;

        WHEN("we parse the input") {
            bool nodes_ret = parse_bookshelf_node(nodes_iter, nodes_end, nodes);
            bool pl_ret = parse_bookshelf_pl(pl_iter, pl_end, pls);

            cout << "data.size " << pls.data.size() << endl;
            cout << "nodes num " << nodes.num_nodes << endl;

            THEN("we get a bunch of Bookshelf pls") {
                REQUIRE(nodes_ret == true);
                REQUIRE(pl_ret == true);
                REQUIRE(pls.data.size() == nodes.data.size());
            }
        }

        cout << endl;
    }//end GIVEN

}//end bigblue4


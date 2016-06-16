/*!
 * \file bookshelf_pl_generator_test.cpp
 * \author Peng Fei
 * \brief bookshelf pl generator unittest.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "bookshelf_pl.hpp"

using namespace std;
using namespace bks;

SCENARIO("adaptec1", "[adaptec1]") {

    GIVEN("BookshelfPls constructed from a BookshelfPlParser.") {
        string path(getenv("EDACOMMON"));
        path += "/benchamrk/ispd2005/adaptec1/adaptec1";

        ifstream pl_in(path+".pl", ios_base::in);
        pl_in.unsetf(ios::skipws);
        boost::spirit::istream_iterator pl_iter(pl_in), pl_end;
        BookshelfPls pls;
        parse_bookshelf_pl(pl_iter, pl_end, pls);

        ofstream out("adaptec1.pl", ios_base::out);
        ostream_iterator<char> ositer(out, "");

        WHEN("we generate the output") {
            bool ret = generate_bookshelf_pl(ositer, pls);

            THEN("we get a .pl file the same as the previous one except the comments") {
                REQUIRE(ret == true);
            }
        }
    }//end GIVEN

}//end SCENARIO


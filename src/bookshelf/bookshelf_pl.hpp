/*!
 * \file bookshelf_pl.hpp
 * \author Peng Fei
 * \brief In-memory data structure definition for bookshelf .pl files.
 */

#ifndef BOOKSHELF_PL_HPP
#define BOOKSHELF_PL_HPP

#include "utils.h"

namespace bks {

    //! public class storing one node's x and y coordinate information.
    /*!
     * When BookshelfNodeParser parse .pl files, the nodes' information are stored in this struct. 
     * id is the node's name, and x and y stores the node's bounding box's location information. 
     * A bool fixed is used to indicate wether this node is movable within the chip's boundary.
     * A default constructor is explicitly provided mainly for setting the fixed bool to false.
     */
    struct BookshelfPl {
        Id      id;  //!< A node's id.
        int      x;  //!< A node's x coordinate.
        int      y;  //!< A node's y coordinate.
        bool fixed;  //!< A node's move mode flag.

        BookshelfPl() = default;
        BookshelfPl(Id id, Coordinate x, Coordinate y, bool fixed) : id(id), x(x), y(y), fixed(fixed) {
        }
    };

    /*!
     * \fn std::ostream & operator<<(std::ostream &out, const BookshelfPl &pl);
     * \brief A help function used for printing the BookshelfPl's information.
     * \param out The output stream object.
     * \param pl The pl information to be printed.
     */
    inline std::ostream & operator<<(std::ostream &out, const BookshelfPl &pl)
    {
        using std::setw;
        using std::fixed;
        using std::setprecision;

        out << std::left
        << setw(8) << pl.id
        << std::right
        << setprecision(10) << fixed << pl.x << "\t"
        << setprecision(10) << fixed << pl.y << "\t"
        << "\t: N";

        if(pl.fixed) {
            out << " /FIXED";
        }
        return out;
    }

    //! The in-memory representation class for the .pl files.
    /*!
     * This struct can be seen as the in-memory equivalent of the .pl files, 
     * which contains all the nodes' locations on the chip. 
     * A default constructor is provided for allocating some storage space in the first time.
     */
    struct BookshelfPls {
        std::vector<BookshelfPl> data;  //!< Vector storing all the pl information in .pl files

        //!< Default constructor for BookshelfPl
        BookshelfPls() {
            data.reserve(200000);
        }
    };

}//end namespace bks

BOOST_FUSION_ADAPT_STRUCT(
        bks::BookshelfPl,
        (bks::Id, id)
        (int,      x)
        (int,      y)
        (bool, fixed)
)

BOOST_FUSION_ADAPT_STRUCT(
        bks::BookshelfPls,
        (std::vector<bks::BookshelfPl>, data)
)

namespace bks {

    //! Symbols tables for the Bookshelf Pl grammar.
    /*!
     *  When the parser reads in a line ending with "/FIXED",
     *  it set the fixed bool to true using this symbol table.
     */
    struct PlMoveTypeSymbolTable : qi::symbols<char, bool> {
        PlMoveTypeSymbolTable() {
            add("/FIXED", true);
        }
    };

    //! The grammar definition for .pl files.
    /*!
     * Using PEG format and regular expression, the grammar can be written as follows:
     *
     * \code
     * header_rule        := "^UCLA\s*pl\s*1.0"
     * comment_rule       := "^#.*$"
     * id_rule            := [^\s]*
     * pl_rule            := id_rule int_ int_ ":\sN"| id int_ int_":\sN\s\/FIXED$"
     * pls_rule           := pl_rule*
     * start              := header_rule comment_rule* pls_rule
     * \endcode
     */
    template<typename Iterator>
    struct BookshelfPlParser: qi::grammar<Iterator, BookshelfPls(), asc::space_type> {

        BookshelfPlParser() : BookshelfPlParser::base_type(start) {

            using qi::int_;
            using qi::lit;
            using qi::lexeme;
            using qi::eoi;

            header_rule  = lit("UCLA") >> lit("pl") >> lit("1.0");

            comment_rule = lexeme[lit("#") >> *~lit('\n') ];

            id_rule     %= lexeme[+~qi::space];
            pl_rule     %= id_rule >> int_ >> int_ >> lit(":") >> lit("N") >> -pl_move_type_symbol;
            pls_rule    %= *pl_rule;

            start       %=
                    header_rule   >>
                    *comment_rule >>
                    pls_rule      >>
                    eoi;
        }

        static PlMoveTypeSymbolTable  pl_move_type_symbol;                        //!< Pl file symbol table

        qi::rule<Iterator, qi::unused_type(), asc::space_type>       header_rule; //!< escape headers
        qi::rule<Iterator, qi::unused_type(), asc::space_type>      comment_rule; //!< escape comments
        qi::rule<Iterator,     std::string(), asc::space_type>           id_rule; //!< cache node id
        qi::rule<Iterator,     BookshelfPl(), asc::space_type>           pl_rule; //!< parse one line of pl
        qi::rule<Iterator, std::vector<BookshelfPl>(), asc::space_type> pls_rule; //!< parse all lines of pls
        qi::rule<Iterator,    BookshelfPls(), asc::space_type>             start; //!< the start rule, i.e. the entry

    };//end template

    template<typename Iterator>
    PlMoveTypeSymbolTable BookshelfPlParser<Iterator>::pl_move_type_symbol;

    /*!
     * \fn bool parse_bookshelf_pl(Iterator &iter, Iterator &end, BookshelfPls &pls);
     *
     */
    template<typename Iterator>
    bool parse_bookshelf_pl(Iterator &iter, Iterator &end, BookshelfPls &pls) {

        BookshelfPlParser<Iterator> p;
        bool ret = qi::phrase_parse(iter, end, p, qi::ascii::space_type(), pls);

        return ret;
    }
/*!
     * \fn bool generate_bookshelf_pl(OutputIterator &sink, const BookshelfPls &pls);
     *
     */
    template<typename OutputIterator>
    bool generate_bookshelf_pl(OutputIterator &sink, const BookshelfPls &pls) {
        using karma::generate;
        using karma::lit;
        using karma::stream;
        using karma::eol;

        bool r = generate(
                sink,
                lit("UCLA pl 1.0\n") << stream % eol,
                pls.data
        );
        return r;
    };
}//end namespace bks

#endif//BOOKSHELF_PL_HPP


/*!
 * \file bookshelf_node.hpp
 * \author Peng Fei
 * \brief In-memory data structure definition for bookshelf .nodes files.
 */

#ifndef BOOKSHELF_NODE_HPP
#define BOOKSHELF_NODE_HPP

#include "utils.h"

namespace bks {

    //! public class storing one node's width and height information.
    /*!
     * When BookshelfNodeParser parse .nodes files, the nodes' information are stored in this struct. 
     * A bool fixed is used to indicate whether this node is movable within the chip's boundary.
     * A default constructor is explicitly provided mainly for setting the fixed bool to false.
     */
    struct BookshelfNode {
        Id id;       //!< A node's id
        int width;   //!< A node's width
        int height;  //!< A node's height
        bool fixed;  //!< A node's move mode flag

        //! Default constructor for BookshelfNode
        BookshelfNode() : id(""), width(0), height(0), fixed(false) {
        }
    };

    /*!
     * \fn std::ostream & operator<<(std::ostream &out, const BookshelfNode &node);
     * \brief A help function used for printing the BookshelfNode's information.
     * \param out  the output stream object
     * \param node the node to be printed
     */
    inline
    std::ostream &operator<<(std::ostream &out, const BookshelfNode &node) {
        out << node.id << "\t" << node.width << "\t" << node.height;
        if (node.fixed) {
            out << "\tterminal";
        }
        return out;
    }

    //! The in-memory representation class for the .nodes files.
    /*!
     * This struct can be seen as the in-memory equivalent of the .nodes files, 
     * which contains all the nodes' information as well as other metadata, such as number of nodes.
     * A default constructor is provided for allocating some storage space in the first place.
     */
    struct BookshelfNodes {
        unsigned int num_nodes;     //!< Number of node in .node files
        unsigned int num_terminals; //!< Number of terminals in .node files
        std::vector<BookshelfNode> data;  //!< Vector storing all the nodes in .nodes files

        //! Default constructor for BookshelfNodes
        BookshelfNodes() : num_nodes(0), num_terminals(0) {
            data.reserve(200000);
        }
    };

    //! Symbols tables for the Bookshelf Node grammar.
    /*!
     *  When the parser reads in a line ending with terminal,
     *  it set the fixed bool to true using this symbol table.
     */
    struct NodeMoveTypeSymbolTable : qi::symbols<char, bool> {
        NodeMoveTypeSymbolTable() {
            add("terminal", true);
        }
    };

    //! The grammar definition for .nodes files.
    /*!
     * Using PEG format and regular expression, the grammar can be written as follows:
     *
     * \code
     * school_rule        := [^\s]*
     * version_rule       := \d | "."
     * header_rule        := "school_rule \s*nodes\s* version_rule"
     * comment_rule       := "^#.*$"
     * num_nodes_rule     := "^NumNodes\s*:\s*" uint_
     * num_terminals_rule := "^NumTerminals\s*:\s*" uint_
     * id_rule            := [^\s]*
     * node_rule          := id_rule uint_ uint_ | id unit_ unit_ "terminal$"
     * nodes_rule         := node_rule*
     * start              := header_rule comment_rule* num_nodes_rule num_terminals_rule nodes_rule
     * \endcode
     */
    template<typename Iterator>
    struct BookshelfNodeParser: qi::grammar<Iterator, BookshelfNodes(), asc::space_type> {

        BookshelfNodeParser() : BookshelfNodeParser::base_type(start) {

            using qi::uint_;
            using qi::lit;
            using qi::lexeme;
            using qi::eoi;
            using qi::digit;


            school_rule  = lexeme[+~qi::space];
            version_rule = lexeme[+(qi::digit|lit("."))];
            header_rule  = school_rule >> lit("nodes") >> version_rule;

            comment_rule = lexeme[lit("#") >> *~lit('\n') ];

            num_nodes_rule      %=  lit("NumNodes")     >> lit(":") >> uint_;
            num_terminals_rule  %=  lit("NumTerminals") >> lit(":") >> uint_;

            id_rule     %= lexeme[+~qi::space];
            node_rule   %= id_rule >> uint_ >> uint_ >> -node_move_type_symbol;
            nodes_rule  %= *node_rule;

            start       %=
                    header_rule        >>
                    *comment_rule      >>
                    num_nodes_rule     >>
                    num_terminals_rule >>
                    nodes_rule         >>
                    eoi;
        }

        static NodeMoveTypeSymbolTable  node_move_type_symbol;                        //!< Node file symbol table

        qi::rule<Iterator,     std::string(), asc::space_type>           school_rule; //!< school name
        qi::rule<Iterator,     std::string(), asc::space_type>          version_rule; //!< version number
        qi::rule<Iterator, qi::unused_type(), asc::space_type>           header_rule; //!< escape headers
        qi::rule<Iterator,    unsigned int(), asc::space_type>        num_nodes_rule; //!< store NumNodes
        qi::rule<Iterator,    unsigned int(), asc::space_type>    num_terminals_rule; //!< store NumTerminals
        qi::rule<Iterator, qi::unused_type(), asc::space_type>          comment_rule; //!< escape comments
        qi::rule<Iterator,     std::string(), asc::space_type>               id_rule; //!< cache node id
        qi::rule<Iterator,   BookshelfNode(), asc::space_type>             node_rule; //!< parse one line of node
        qi::rule<Iterator, std::vector<BookshelfNode>(), asc::space_type> nodes_rule; //!< parse all lines of nodes
        qi::rule<Iterator,  BookshelfNodes(), asc::space_type>                 start; //!< the start rule, i.e. the entry

    };//end template

    template<typename Iterator>
    NodeMoveTypeSymbolTable BookshelfNodeParser<Iterator>::node_move_type_symbol;

    /*!
     * \fn bool parse_bookshelf_node(Iterator &iter, Iterator &end, BookshelfNodes &nodes);
     *
     */
    template<typename Iterator>
    bool parse_bookshelf_node(Iterator &iter, Iterator &end, BookshelfNodes &nodes) {

        BookshelfNodeParser<Iterator> p;
        bool ret = qi::phrase_parse(iter, end, p, qi::ascii::space_type(), nodes);

        return ret;
    }

    /*!
     * \fn bool generate_bookshelf_node(OutputIterator &sink, const BookshelfNodes &nodes);
     *
     */
    template<typename OutputIterator>
    bool generate_bookshelf_node(OutputIterator &sink, const BookshelfNodes &nodes) {
        using karma::generate;
        using karma::lit;
        using karma::stream;
        using karma::eol;

        bool r = generate(
                sink,
                lit("THU nodes 1.0") << eol <<
                lit("NumNodes :\t") << nodes.num_nodes << eol <<
                lit("NumTerminals :\t") << nodes.num_terminals << eol <<
                stream % eol,
                nodes.data
        );
        return r;
    };
}//end namespace bks

BOOST_FUSION_ADAPT_STRUCT(
        bks::BookshelfNode,
        (bks::Id, id)
        (int,  width)
        (int, height)
        (bool, fixed)
)

BOOST_FUSION_ADAPT_STRUCT(
        bks::BookshelfNodes,
        (unsigned int,               num_nodes)
        (unsigned int,           num_terminals)
        (std::vector<bks::BookshelfNode>, data)
)

#endif//BOOKSHELF_NODE_HPP


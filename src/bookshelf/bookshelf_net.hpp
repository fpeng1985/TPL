/*!
 * \file bookshelf_net.hpp
 * \author Peng Fei
 * \brief In-memory data structure definition for bookshelf .nets files.
 */

#ifndef BOOKSHELF_NET_HPP
#define BOOKSHELF_NET_HPP

#include "utils.h"

namespace bks {

    /*!
     * \enum IOType {Output, Input};
     * \brief Pin's IO type
     */
    enum class IOType {Output, Input, Bidirection};

    //! public class storing one pin's IO, and x y offset information.
    /*!
     * When BookshelfNetParser parses .nets files, the pins' information are stored in this struct. 
     */
    struct BookshelfPin {
        Id     id; //!< A pin's id
        IOType io; //!< A pin's IO type
        double dx; //!< A pin's x offset from the center of its node
        double dy; //!< A pin's y offset from the center of its node
    };

    /*!
     * \fn std::ostream & operator<<(std::ostream &out, const BookshelfPin &pin);
     * \brief A help function used for printing the BookshelfPin's information.
     * \param out  the output stream object
     * \param pin the pin to be printed
     */
    inline std::ostream & operator<<(std::ostream &out, const BookshelfPin &pin)
    {
        using std::setw;

        out << std::left << setw(8) << pin.id;
        out << std::right;
        switch(pin.io) {
            case IOType::Input:
                out << " I : ";
                break;
            case IOType::Output:
                out << " O : ";
                break;
            case IOType::Bidirection:
                out << " B : ";
                break;
        }
        out << setw(12) << pin.dx
        << setw(12) << pin.dy;

        return out;
    }

    //! public class storing one net's degree and its associated pins.
    /*!
     * When BookshelfNetParser parses .nets files, the nets' information are stored in this struct. 
     */
    struct BookshelfNet {
        Id                           id; //!< A net's id
        int                      degree; //!< A net's degree, i.e. number of pins attached on this net
        std::vector<BookshelfPin>  pins; //!< A net's attached pins
    };

    /*!
     * \fn std::ostream & operator<<(std::ostream &out, const BookshelfNet &net);
     * \brief A help function used for printing the BookshelfNet's information.
     * \param out  the output stream object
     * \param net the net to be printed
     */
    inline std::ostream & operator<<(std::ostream &out, const BookshelfNet &net)
    {
        using std::setw;

        out << "NetDegree : ";
        out << setw(4) << net.degree
        << setw(8) << net.id
        << std::endl;

        for(std::vector<BookshelfPin>::const_iterator it=net.pins.begin(); it!=net.pins.end(); ++it) {
            out << *it << std::endl;
        }

        return out;
    }

    //! The in-memory representation class for the .nets files.
    /*!
     * This struct can be seen as the in-memory equivalent of the .nets files, 
     * which contains all the pins' and the net' information.
     * A default constructor is provided for allocating some storage space in the first time.
     */
    struct BookshelfNets {
        unsigned int num_nets;              //!< Number of nets in .nets files
        unsigned int num_pins;              //!< Number of pins in .nets files
        std::vector<BookshelfNet> data;     //!< Vector storing all the nets in .nets files

        //! Default constructor for BookshelfNets
        BookshelfNets() {
            data.reserve(200000);
        }
    };

}//end namespace bks

BOOST_FUSION_ADAPT_STRUCT(
        bks::BookshelfPin,
        (bks::Id,     id)
        (bks::IOType, io)
        (double,      dx)
        (double,      dy)
)

BOOST_FUSION_ADAPT_STRUCT(
        bks::BookshelfNet,
        (int,                          degree)
        (bks::Id,                          id)
        (std::vector<bks::BookshelfPin>, pins)
)

BOOST_FUSION_ADAPT_STRUCT(
        bks::BookshelfNets,
        (unsigned int,               num_nets)
        (unsigned int,               num_pins)
        (std::vector<bks::BookshelfNet>, data)
)

namespace bks {

    //! Symbols tables for the Bookshelf Net grammar.
    /*!
     *  When the parser reads a pin,
     *  it set the its IOType according to this symbol table.
     */
    struct PinIOTypeSymbolTable : qi::symbols<char, IOType> {
        PinIOTypeSymbolTable() {
            add("I", IOType::Input );
            add("O", IOType::Output);
            add("B", IOType::Bidirection);
        }
    };

    //! The grammar definition for .net files.
    /*!
     * Using PEG format and regular expression, the grammar can be written as follows:
     *
     * \code
     * header_rule        := "^UCLA\s*net\s*1.0"
     * comment_rule       := "^#.*$"
     * num_nets_rule      := "^NumNets\s*:\s*" uint_
     * num_pins_rule      := "^NumPins\s*:\s*" uint_
     * id_rule            := [^\s]*
     * pin_rule           := id_rule pin_io_type_symbol ":" double_ double_
     * net_rule           := "^NetDegree :\s*" uint_ id_rule pin_rule*
     * nets_rule          := net_rule*
     * start              := header_rule comment_rule* pls_rule
     * \endcode
     */
    template<typename Iterator>
    struct BookshelfNetParser: qi::grammar<Iterator, BookshelfNets(), asc::space_type> {

        BookshelfNetParser() : BookshelfNetParser::base_type(start) {

            using qi::uint_;
            using qi::int_;
            using qi::double_;
            using qi::lit;
            using qi::lexeme;
            using qi::eol;
            using qi::eoi;

            header_rule    = lit("UCLA") >> lit("nets") >> lit("1.0");

            comment_rule   = lexeme[lit("#") >> *~lit('\n') ];

            num_nets_rule %= lit("NumNets") >> lit(":") >> uint_;
            num_pins_rule %= lit("NumPins") >> lit(":") >> uint_;

            id_rule       %= lexeme[+~qi::space];
            pin_rule      %= id_rule >> pin_io_type_symbol >> lit(":") >> double_ >> double_;
            net_rule      %= lit("NetDegree") >> lit(":") >> int_ >> id_rule >> *pin_rule;
            nets_rule     %= *net_rule;

            start         %=
                    header_rule   >>
                    *comment_rule >>
                    num_nets_rule >>
                    num_pins_rule >>
                    nets_rule     >>
                    eoi;
        }

        static PinIOTypeSymbolTable pin_io_type_symbol;                             //!< Net file symbol table

        qi::rule<Iterator, qi::unused_type(), asc::space_type>         header_rule; //!< escape headers
        qi::rule<Iterator, qi::unused_type(), asc::space_type>        comment_rule; //!< escapte comments
        qi::rule<Iterator,    unsigned int(), asc::space_type>       num_nets_rule; //!< store NumNets
        qi::rule<Iterator,    unsigned int(), asc::space_type>       num_pins_rule; //!< store NumPins
        qi::rule<Iterator,     std::string(), asc::space_type>             id_rule; //!< cache net id
        qi::rule<Iterator,    BookshelfPin(), asc::space_type>            pin_rule; //!< parse one line of pin
        qi::rule<Iterator,    BookshelfNet(), asc::space_type>            net_rule; //!< parse one net
        qi::rule<Iterator, std::vector<BookshelfNet>(), asc::space_type> nets_rule; //!< parse all lines of nets
        qi::rule<Iterator,   BookshelfNets(), asc::space_type>               start; //!< the start rule, i.e. the entry

    };//end template

    template<typename Iterator>
    PinIOTypeSymbolTable BookshelfNetParser<Iterator>::pin_io_type_symbol;

    /*!
     * \fn bool parse_bookshelf_net(Iterator &iter, Iterator &end, BookshelfNets &nets);
     *
     */
    template<typename Iterator>
    bool parse_bookshelf_net(Iterator &iter, Iterator &end, BookshelfNets &nets) {

        BookshelfNetParser<Iterator> p;
        bool ret = qi::phrase_parse(iter, end, p, qi::ascii::space_type(), nets);

        return ret;
    }

}//end namespace bks

#endif//BOOKSHELF_NET_H


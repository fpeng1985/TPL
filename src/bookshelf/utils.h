/*!
 * \file utils.h
 * \author Peng Fei
 * \brief typedefs, enums, helper functions, and other common utilities
 */

#ifndef BOOKSHELF_UTILS_H
#define BOOKSHELF_UTILS_H

#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>


namespace bks {

    namespace qi    = boost::spirit::qi;
    namespace karma = boost::spirit::karma;
    namespace asc   = boost::spirit::ascii;

    using std::size_t;

    /*!
     * \typedef std::string Id
     * \brief Id type
     */
    typedef std::string  Id;
}

#endif//BOOKSHELF_UTILS_H


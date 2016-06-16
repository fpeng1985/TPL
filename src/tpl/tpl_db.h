/*!
 * \file tpl_db.h
 * \author Peng Fei
 * \brief Definition for circuit related data structures
 */

#ifndef TPL_DB_H
#define TPL_DB_H

#include "bookshelf_node.hpp"
#include "bookshelf_pl.hpp"
#include "bookshelf_net.hpp"

#include <vector>
#include <map>
#include <string>

namespace tpl {
    using namespace bks;

    //! struct storing a module's position and size information.
    /*!
     * Please note that the module's coordinates is its center point's location.
     */
    struct TplModule {
        Id         id; //!< A module's id
        int         x; //!< A module's x coordinate, lower left corner.
        int         y; //!< A module's y coordinate, lower left corner.
        int     width; //!< A module's width
        int    height; //!< A module's height
        bool    fixed; //!< A module's fixed flag
        double power_density; //!< A module's power density

        //! Default constructor.
        TplModule() = default;
        //! TplModule constructor.
        TplModule(Id id, int x, int y, int width, int height, bool fixed, double power_density) :
                id(id), x(x), y(y), width(width), height(height), fixed(fixed), power_density(power_density) {
        }
    };

    //! \typedef BookshelfPin TplPin;
    typedef BookshelfPin TplPin;
    //! \typedef BookshelfNet TplNet;
    typedef BookshelfNet TplNet;

    class TplDB {
    public:
        ///////////////////////// Member Type //////////////////////////////////////////
        //! \typedef std::vector<TplModule>::iterator mod_iterator;
        typedef std::vector<TplModule>::iterator mod_iterator;

        //! \typedef std::vector<TplModule>::const_iterator const_mod_iterator;
        typedef std::vector<TplModule>::const_iterator const_mod_iterator;

        //! \typedef std::list<TplNet>::iterator net_iterator;
        typedef std::list<TplNet>::iterator net_iterator;

        //! \typedef std::list<TplNet>::const_iterator const_net_iterator;
        typedef std::list<TplNet>::const_iterator const_net_iterator;

        //! \typedef std::vector<TplPin>::iterator pin_iterator;
        typedef std::vector<TplPin>::iterator pin_iterator;

        //! \typedef std::vector<TplPin>::iterator iterator;
        typedef std::vector<TplPin>::const_iterator const_pin_iterator;
        ///////////////////////// Member Type //////////////////////////////////////////

        ///////////////////////// Iterators   //////////////////////////////////////////
        //! Iterator indicating the first TplModule.
        mod_iterator mod_begin() {
            return _modules.begin();
        }

        //! Iterator indicating the first TplModule, const version.
        const_mod_iterator cmod_begin() const {
            return _modules.begin();
        }

        //! Iterator indicating the past-the-last TplModule.
        mod_iterator mod_end() {
            return _modules.end();
        }

        //! Iterator indicating the past-the-last TplModule, const version.
        const_mod_iterator cmod_end() const {
            return _modules.end();
        }

        //! Iterator indicating the first TplNet.
        net_iterator net_begin() {
            return _nets.begin();
        }

        //! Iterator indicating the first TplNet, const version.
        const_net_iterator cnet_begin() const {
            return _nets.begin();
        }

        //! Iterator indicating the past-the-last TplNet.
        net_iterator net_end() {
            return _nets.end();
        }

        //! Iterator indicating the past-the-last TplNet, const version.
        const_net_iterator cnet_end() const {
            return _nets.end();
        }

        //! Iterator indicating the first TplPin.
        pin_iterator pin_begin(const net_iterator &nit) {
            return nit->pins.begin();
        }

        //! Iterator indicating the first TplPin, const version.
        const_pin_iterator cpin_begin(const net_iterator &nit) const {
            return nit->pins.begin();
        }

        //! Iterator indicating the past-the-last TplPin.
        pin_iterator pin_end(const net_iterator &nit) {
            return nit->pins.end();
        }

        //! Iterator indicating the past-the-last TplPin, const version.
        const_pin_iterator cpin_end(const net_iterator &nit) const {
            return nit->pins.end();
        }
        ///////////////////////// Iterators   //////////////////////////////////////////

        ///////////////////////// Properties //////////////////////////////////////////
        //! Total number of modules.
        unsigned int num_module() const {
            return _num_modules;
        }

        //! Number of free modules.
        unsigned int num_free()  const {
            return _num_free_modules;
        }

        //! Number of fixed modules.
        unsigned int num_fixed() const {
            return _num_modules-_num_free_modules;
        }

        //! Number of nets.
        unsigned int num_nets() const {
            return _num_nets;
        }

        //! Number of pins.
        unsigned int num_pins() const {
            return _num_pins;
        }

        //! Query interface for chip width.
        int chip_width()  const {
            return _chip_width;
        }

        //! Query interface for chip height.
        int chip_height() const {
            return _chip_height;
        }

        ///////////////////////// Properties //////////////////////////////////////////

        ///////////////////////// Individual Module and Net Access ////////////////////
        //! Get a TplModule reference with a ID id.
        TplModule&       module(const std::string &id);

        //! Get a const TplModule reference with a ID id.
        const TplModule& module(const std::string &id) const;

        //! Get a TplModule reference with a index pos.
        TplModule& module(size_t pos);

        //! Get a const TplModule reference with a index pos.
        const TplModule& module(size_t pos) const;

        //! Get a module's index, with a ID id.
        size_t module_index(const std::string &id) const;

        //! Query interface checking wether a module is fixed.
        bool is_module_fixed(const std::string &id) const;
        ///////////////////////// Individual Module and Net Access ///////////////////

        ///////////////////////// Global Operations //////////////////////////////////////////
        //! Load benchmark circuit data into in-memory data structures.
        /*!
         * \param path The benchmark file's directory path.
         * \return A boolean variable indicating wether the operation is success.
         */
        bool load_data(const std::string &path);

        //! Clear the data in the db.
        void clear_data();

        //! Set the free modules' coordinates.
        /*!
         * \param xs The free module center points' x coordinates, need to be transformed into lower left coordinates.
         * \param ys The free module center points' y coordinates, need to be transformed into lower left coordinates.
         */
        void set_free_module_coordinates(const std::vector<double> &xs, const std::vector<double> &ys);

        //! Take a snapshot of the current placement.
        /*!
         * \param with_version Indicates whether the generated pl file name is version numbered.
         */
        void generate_placement_snapshot(bool with_version=true) const;
        ///////////////////////// Global Operations //////////////////////////////////////////

        //! Realize the singleton design pattern.
        static TplDB &db();

    private:
        //! Default constructor.
        TplDB() {
            _modules.reserve(200000);
        }

        //! Private helper routine initialize the modules member variable.
        void initialize_modules(const std::string &node_file, const std::string &pl_file);

        //! Private helper routine initialize the nets member variable.
        void initialize_nets   (const std::string &net_file);

        unsigned int _num_modules;      //!< Number of modules.
        unsigned int _num_free_modules; //!< Number of free modules.
        unsigned int _num_nets;         //!< Number of nets.
        unsigned int _num_pins;         //!< Number of pins.

        int _chip_width;  //!< Chip width.
        int _chip_height; //!< Chip height.

        std::map<std::string, size_t> _mod_id_index_map;    //!< A ID index map for all the modules

        std::vector<TplModule> _modules; //!< storing the modules
        std::list<TplNet> _nets;         //!< storing the nets

        std::string _benchmark_name;//!< The current loaded circuit's name.
    };

}//namespace tpl

#endif //TPL_TPLDB_H

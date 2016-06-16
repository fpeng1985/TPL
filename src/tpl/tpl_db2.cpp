#include "tpl_db.h"

#include <cmath>
#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include "../bookshelf/bookshelf_node_parser.hpp"
#include "../bookshelf/bookshelf_pl_parser.hpp"
#include "../bookshelf/bookshelf_net_parser.hpp"
#include "../bookshelf/bookshelf_pl_generator.hpp"

#include "debug.h"


namespace tpl {
    using namespace std;
    using namespace thueda;

    TplModule::TplModule(Id id, Coordinate x, Coordinate y, Length width, Length height,
                         bool fixed, double power_density) :
            id(id), x(x), y(y), width(width), height(height), fixed(fixed), power_density(power_density)
    {
    }

    TplModules::TplModules(const BookshelfNodes &bnodes, const BookshelfPls &bpls) :
            _num_modules(bnodes.num_nodes),
            _num_free(bnodes.num_nodes - bnodes.num_terminals),
            _chip_width(0),
            _chip_height(0)
    {
        assert(bnodes.data.size() == bpls.data.size());

        double width(0), height(0);
        for(size_t i=0; i<bnodes.data.size(); ++i) {
            const BookshelfNode &bnode = bnodes.data[i];
            const BookshelfPl   &bpl   = bpls.data[i];

            _modules.emplace_back(bnode.id, bpl.x, bpl.y, bnode.width, bnode.height, bnode.fixed, 1);
            _id_index_map.insert( make_pair(bnode.id, i) );

            double right_border = bpl.x + bnode.width;
            if(right_border>width) width  = right_border;
            double top_border = bpl.y + bnode.height;
            if(top_border>height)  height = top_border;
        }

        _chip_width  = static_cast<Length>(width);
        _chip_height = static_cast<Length>(height);
    }

    TplModules::TplModules(BOOST_RV_REF(TplModules) temp) :
            _num_modules(temp._num_modules),
            _num_free(temp._num_free),
            _chip_width(temp._chip_width),
            _chip_height(temp._chip_height),
            _modules(std::move(temp._modules)),
            _id_index_map(std::move(temp._id_index_map))
    {
    }

    TplModules &TplModules::operator=(BOOST_RV_REF(TplModules) temp)
    {
        _num_modules  = temp._num_modules;
        _num_free     = temp._num_free;

        _chip_width   = temp._chip_width;
        _chip_height  = temp._chip_height;

        _modules      = std::move(temp._modules);
        _id_index_map = std::move(temp._id_index_map);

        return *this;
    }

    void TplModules::clear()
    {
        _num_modules = 0;
        _num_free    = 0;

        _chip_width  = 0;
        _chip_height = 0;

        _modules.clear();
        _id_index_map.clear();
    }

    TplModule& TplModules::module(const std::string &id)
    {
        assert(_id_index_map.count(id) != 0);

        return _modules[_id_index_map[id]];
    }

    const TplModule& TplModules::module(const std::string &id) const
    {
        assert(_id_index_map.count(id) != 0);

        return _modules.at(_id_index_map.at(id));
    }

    size_t TplModules::module_index(const std::string &id) const
    {
        assert(_id_index_map.count(id) != 0);

        return _id_index_map.at(id);
    }

    bool TplModules::is_module_fixed(const std::string &id) const
    {
        assert(_id_index_map.count(id) != 0);

        return _modules.at(_id_index_map.at(id)).fixed;
    }

    void TplModules::move_to_center() {
        for (size_t i = 0; i < _num_free; i++) {
            _modules[i].x += chip_width() / 2.0;
            _modules[i].y += chip_height() / 2.0;
        }
    }

    double TplModules::set_free_module_coordinates(const std::vector<double> &xs, const std::vector<double> &ys)
    {
        assert( xs.size() == _num_free );
        assert( ys.size() == _num_free );

        double moveDis = 0.0;

        for(size_t i=0; i<_num_free; ++i) {
            //module x and y denotes lower left corner
            double x_new = xs[i] - _modules[i].width / 2.0;
            double y_new = ys[i] - _modules[i].height / 2.0;
            moveDis += pow(x_new - _modules[i].x, 2) + pow(y_new - _modules[i].y, 2);
            _modules[i].x = x_new;
            _modules[i].y = y_new;
        }
        return sqrt(moveDis);
    }

    void TplModules::get_bookshelf_pls(thueda::BookshelfPls &bpls) const
    {
        bpls.data.clear();

        for(size_t i=0; i<_num_modules; ++i) {
            const TplModule &m = _modules.at(i);
            bpls.data.emplace_back( m.id, m.x, m.y, m.fixed );
        }
    }

    void TplModules::add_shredded_cells(map<Id, vector<TplModule> > macro_cells) {
        TplModules::iterator iterBegin = _modules.begin() + _num_free;
        TplModules::iterator iterEnd = _modules.begin() + _num_modules;

        // back up macros
        _macros.clear();
        _macros.insert(_macros.end(), iterBegin, iterEnd);

        // delete macros
        _modules.erase(iterBegin, iterEnd);

        // delete from id_index_map and add new cells
        for (map<Id, vector<TplModule>>::iterator iter = macro_cells.begin();
                iter != macro_cells.end(); iter++) {
            _id_index_map.erase(iter->first);
            vector<TplModule> cells = iter->second;
            _modules.insert(_modules.end(), cells.begin(), cells.end());
            for (size_t i = 0; i < cells.size(); ++i) {
                _id_index_map.insert(make_pair(cells[i].id, _modules.size() - 1));
            }
        }

        // update modules number
        _num_modules = _modules.size();
    }

	void TplModules::aggregate_cells() {
		// for each macro, calculate mean and variance of shredded cells
		for (vector<TplModule>::iterator macro_iter = _macros.begin();
				macro_iter != _macros.end(); macro_iter++) {
			vector<TplModule> cells = _shredded_cells[macro_iter->id];
			int n = cells.size();
			double x_mean = 0.0, y_mean = 0.0, x_var = 0.0, y_var = 0.0;
			// calculate mean and variance
			for (vector<TplModule>::iterator it = cells.begin();
					it != cells.end(); it++) {
				x_mean += it->x;
				y_mean += it->y;
				x_var += it->x * it->x;
				y_var += it->y * it->y;
			}
			x_mean /= n;
			y_mean /= n;
			x_var = (x_var / n) - (x_mean * x_mean);
			y_var = (y_var / n) - (y_mean * y_mean);
			macro_iter->x = x_mean;
			macro_iter->y = y_mean;
			// make max(width, height) correspond to max(x_var, y_var)
			if ((x_var > y_var && macro_iter->width < macro_iter->height) ||
					(x_var < y_var && macro_iter->width > macro_iter->height)) {
				swap(macro_iter->width, macro_iter->height);
			}
		}

		// delete cells from _modules
		_modules.erase(_modules.begin() + _num_free, _modules.end());
		// find begin position of shredded cells in _id_index_map
		// and delete all cells from that position to end
		map<Id, size_t>::iterator iter = _id_index_map.begin();
        advance(iter, _num_free);
		_id_index_map.erase(iter, _id_index_map.end());

		// add macros back to _modules
		_modules.insert(_modules.end(), _macros.begin(), _macros.end());
        _macros.clear();
		_num_modules = _modules.size();

		// add macros back to _id_index_map
		for (size_t i = _num_free; i < _num_modules; i++) {
			_id_index_map[_modules[i].id] = i;
		}
	}


    TplNets::TplNets(const BookshelfNets &bnets)
    {
        _num_nets = bnets.num_nets;
        _num_pins = bnets.num_pins;

        copy(bnets.data.begin(), bnets.data.end(), back_inserter(_netlist));
    }

    TplNets::TplNets(BOOST_RV_REF(TplNets) temp) :
            _num_nets(temp._num_nets),
            _num_pins(temp._num_pins),
            _netlist(std::move(temp._netlist))
    {
    }

    TplNets& TplNets::operator=(BOOST_RV_REF(TplNets) temp)
    {
        _num_nets = temp._num_nets;
        _num_pins = temp._num_pins;
        _netlist  = std::move(temp._netlist);

        return *this;
    }

    void TplNets::clear()
    {
        _num_nets = 0;
        _num_pins = 0;

        _netlist.clear();
    }

	void TplNets::add_net(list<TplNet> newNets) {
        _num_shred_nets = newNets.size();
		_num_nets += _num_shred_nets;
		_netlist.insert(_netlist.end(), newNets.begin(), newNets.end());
	}

	void TplNets::backup_net() {
        _netlist_backup.clear();
		_netlist_backup = _netlist;
	}

	void TplNets::delete_net() {
		_num_nets -= _num_shred_nets;
		_num_shred_nets = 0;
		_netlist = _netlist_backup;
		_netlist_backup.clear();
	}


    TplDB &TplDB::db()
    {
        static TplDB _instance;
        return _instance;
    }


    //////////////////////////////////Helper Functions///////////////////////////////////
    bool TplDB::load_circuit(const std::string &path)
    {
        try {
            boost::filesystem::path   benchmark_path(path);
            _benchmark_name = benchmark_path.filename().string();

            ///////////////////////////////////////////////////////////////////
            Timer t;
            t.timeit();
            //load modules
            boost::filesystem::path node_file_path(benchmark_path);
            node_file_path /= _benchmark_name + string(".nodes");
            boost::filesystem::path pl_file_path(benchmark_path);
            pl_file_path /= _benchmark_name + string(".pl");

            initialize_modules(node_file_path.string(), pl_file_path.string());
            t.timeit("load module");
            ///////////////////////////////////////////////////////////////////

            ///////////////////////////////////////////////////////////////////
            t.timeit();
            //load nets
            boost::filesystem::path net_file_path(benchmark_path);
            net_file_path /= _benchmark_name + string(".nets");

            initialize_nets(net_file_path.string());
            t.timeit("load net");
            ///////////////////////////////////////////////////////////////////

            return true;
        } catch(...) {
            return false;
        }
    }//end TplDB::load_circuit

    void TplDB::generate_placement_snapshot() const
    {
        static int version = 0;

        BookshelfPls bpls;
        modules.get_bookshelf_pls(bpls);

        string out_file_name;
        stringstream ss(out_file_name);
        ss << _benchmark_name << "_" << version++ << ".pl";

        ofstream out(out_file_name.c_str(), ios_base::out);
        ostream_iterator<char> ositer(out, "");

        generate_bookshelf_pl(ositer, bpls);
    }

    //private routines

    void TplDB::initialize_modules(const std::string &node_file, const std::string &pl_file)
    {
        modules.clear();

        //process .nodes file
        ifstream node_stream(node_file);
        node_stream.unsetf(ios::skipws);

        ///////////////////////////////////////////////////////////////
        //quick but memory eager version
        string storage;
        copy(istream_iterator<char>(node_stream),
             istream_iterator<char>(),
             back_inserter(storage));
        string::const_iterator node_begin = storage.begin();
        string::const_iterator node_end   = storage.end();
        //slow but memory efficient version
        //boost::spirit::istream_iterator node_begin(node_stream), node_end;
        ///////////////////////////////////////////////////////////////

        BookshelfNodes bnodes;
        parse_bookshelf_node(node_begin, node_end, bnodes);

        //process .pl file
        ifstream pl_stream(pl_file);
        pl_stream.unsetf(ios::skipws);

        ///////////////////////////////////////////////////////////////
        //quick but memory eager version
        storage.clear();
        copy(istream_iterator<char>(pl_stream),
             istream_iterator<char>(),
             back_inserter(storage));
        string::const_iterator pl_begin = storage.begin();
        string::const_iterator pl_end   = storage.end();
        //slow but memory efficient version
        //boost::spirit::istream_iterator pl_begin(pl_stream), pl_end;
        ///////////////////////////////////////////////////////////////

        BookshelfPls bpls;
        parse_bookshelf_pl(pl_begin, pl_end, bpls);

        modules = std::move( TplModules(bnodes, bpls) );
    }

    void TplDB::initialize_nets(const std::string &net_file)
    {
        nets.clear();

        //process .nets file
        ifstream net_stream(net_file);
        net_stream.unsetf(ios::skipws);

        ///////////////////////////////////////////////////////////////
        //quick but memory eager version
        string storage;
        copy(istream_iterator<char>(net_stream),
             istream_iterator<char>(),
             back_inserter(storage));
        string::const_iterator net_begin = storage.begin();
        string::const_iterator net_end   = storage.end();
        //slow but memory efficient version
        //boost::spirit::istream_iterator net_begin(net_stream), net_end;
        ///////////////////////////////////////////////////////////////

        BookshelfNets bnets;
        parse_bookshelf_net(net_begin, net_end, bnets);

        nets = std::move( TplNets( std::move(bnets) ) );
    }

}//end namespace tpl


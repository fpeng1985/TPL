//
// Created by Administrator on 2016/6/16 0016.
//

#include "tpl_db.h"
#include <cassert>
using std::assert;

#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>

#ifndef NDEBUG
#include "tpl_debug.h"
#endif

namespace tpl {

    TplModule& TplDB::module(const std::string &id)
    {
#ifndef NDEBUG
        assert(_mod_id_index_map.count(id) != 0);
        return _modules.at(_mod_id_index_map.at(id));
#else
        return _modules[_mod_id_index_map[id]];
#endif
    }

    const TplModule& TplDB::module(const std::string &id) const
    {
        assert(_mod_id_index_map.count(id) != 0);

        return _modules.at(_mod_id_index_map.at(id));
    }

    TplModule& TplDB::module(size_t pos)
    {
#ifndef NDEBUG
        assert(pos < _modules.size());
        return _modules.at(pos);
#else
        return _modules[pos];
#endif
    }

    const TplModule& TplDB::module(size_t pos) const
    {
        assert(pos < _modules.size());

        return _modules.at(pos);
    }

    size_t module_index(const std::string &id) const
    {
        assert(_mod_id_index_map.count(id) != 0);

        return _mod_id_index_map.at(id);
    }

    bool TplDB::is_module_fixed(const std::string &id) const
    {
        assert(_mod_id_index_map.count(id) != 0);

        return _modules.at(_mod_id_index_map.at(id)).fixed;
    }

    bool TplDB::load_data(const std::string &path)
    {
        try {
            clear_data();

            auto const pos = path.find_last_of('/');
            _benchmark_name = path.substr(pos+1);
            auto common_dir = path.substr(0, pos+1);

            Timer t;
            ///////////////////////////////////////////////////////////////////
            //load modules
            t.timeit();
            string node_file_path = common_dir + _benchmark_name + ".nodes";
            string pl_file_path   = common_dir + _benchmark_name + ".pl";

            initialize_modules(node_file_path, pl_file_path);
            t.timeit("load module");
            ///////////////////////////////////////////////////////////////////

            ///////////////////////////////////////////////////////////////////
            t.timeit();
            //load nets
            string net_file_path = common_dir + _benchmark_name + ".nets";

            initialize_nets(net_file_path);
            t.timeit("load net");
            ///////////////////////////////////////////////////////////////////

            return true;
        } catch(...) {
            return false;
        }
    }

    void TplDB::clear_data()
    {
        _num_modules = 0;
        _num_free_modules = 0;
        _num_nets = 0;
        _num_pins = 0;

        _chip_width = 0;
        _chip_height = 0;

        _mod_id_index_map.clear();
        _net_id_index_map.clear();

        _modules.clear();
        _nets.clear();

        _benchmark_name = "";
    }

    void TplDB::set_free_module_coordinates(const std::vector<double> &xs, const std::vector<double> &ys)
    {
        assert( xs.size() == _num_free_modules );
        assert( ys.size() == _num_free_modules );

        for(size_t i=0; i<_num_free_modules; ++i) {
            //module x and y denotes lower left corner, but xs and ys denotes center point coordinate.
            _modules[i].x = xs[i] - _modules[i].width  / 2.0;
            _modules[i].y = ys[i] - _modules[i].height / 2.0;
        }
    }

    void TplDB::generate_placement_snapshot(bool with_version) const
    {
        static int version = 0;

        BookshelfPls bpls;
        for (auto m : _modules) {
            bpls.data.emplace_back( m.id, m.x, m.y, m.fixed );
        }

        string out_file_name;
        stringstream ss(out_file_name);
        if (!with_version) {
            ss << _benchmark_name << ".pl";
        } else {
            ss << _benchmark_name << "_" << version++ << ".pl";
        }

        ofstream out(out_file_name.c_str(), ios_base::out);
        ostream_iterator<char> ositer(out, "");

        generate_bookshelf_pl(ositer, bpls);
    }

    TplDB& TplDB::db()
    {
        static TplDB _instance;
        return _instance;
    }

    void TplDB::initialize_modules(const std::string &node_file, const std::string &pl_file)
    {
        //local variables
        ifstream stream;
        string storage;

        //process .nodes file
        stream.open(node_file);
        stream.unsetf(ios::skipws);

        copy(istream_iterator<char>(stream),
             istream_iterator<char>(),
             back_inserter(storage));
        string::const_iterator node_begin = storage.begin();
        string::const_iterator node_end   = storage.end();

        BookshelfNodes bnodes;
        parse_bookshelf_node(node_begin, node_end, bnodes);

        //clear stream and storage
        stream.close();
        storage.clear();

        //process .pl file
        stream.open(pl_file);
        stream.unsetf(ios::skipws);

        copy(istream_iterator<char>(stream),
             istream_iterator<char>(),
             back_inserter(storage));
        string::const_iterator pl_begin = storage.begin();
        string::const_iterator pl_end   = storage.end();

        BookshelfPls bpls;
        parse_bookshelf_pl(pl_begin, pl_end, bpls);

        //set other module related data
        assert(bnodes.data.size() == bpls.data.size());

        _num_modules      = bnodes.num_nodes;
        _num_free_modules = bnodes.num_nodes - bnodes.num_terminals;

        _chip_width  = 0;
        _chip_height = 0;
        for(size_t i=0; i<bnodes.data.size(); ++i) {
            const BookshelfNode &bnode = bnodes.data[i];
            const BookshelfPl   &bpl   = bpls.data[i];

            _modules.emplace_back(bnode.id, bpl.x, bpl.y, bnode.width, bnode.height, bnode.fixed, 1);
            _mod_id_index_map.insert( make_pair(bnode.id, i) );

            double right_border = bpl.x + bnode.width;
            if(right_border>_chip_width) _chip_width  = right_border;

            double top_border = bpl.y + bnode.height;
            if(top_border>_chip_height)  _chip_height = top_border;
        }
    }

    void TplDB::initialize_nets(const std::string &net_file)
    {
        //local variables
        ifstream stream;
        string storage;

        //process .nets file
        stream.open(net_file);
        stream.unsetf(ios::skipws);

        copy(istream_iterator<char>(stream),
             istream_iterator<char>(),
             back_inserter(storage));
        string::const_iterator net_begin = storage.begin();
        string::const_iterator net_end   = storage.end();

        BookshelfNets bnets;
        parse_bookshelf_net(net_begin, net_end, bnets);

        //set other net related data
        _num_nets = bnets.num_nets;
        _num_pins = bnets.num_pins;

        for (size_t i=0; i<bnets.size(); ++i) {
            _nets.push_back(bnets[i]);
            _net_id_index_map.insert( make_pair(bnets[i].id, i) );
        }
        copy(bnets.data.begin(), bnets.data.end(), back_inserter(_nets));
    }
}

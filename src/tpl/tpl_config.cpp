#include "tpl_config.h"

#include <map>


#include <boost/property_tree/json_parser.hpp>

namespace tpl {
    using std::string;
    using std::map;
    using std::make_pair;


    TplConfig &TplConfig::instance()
    {
        static TplConfig _instance;
        return _instance;
    }

    bool TplConfig::load_configuration(const std::string &configfile)
    {
        try {
            namespace pt = boost::property_tree;
            pt::ptree tree;
            pt::read_json(configfile, tree);

            _data.insert(make_pair(string("init_grid_size"), tree.get<int>("init_grid_size")));
            _data.insert(make_pair(string("r1"), tree.get<double>("r1")));
            _data.insert(make_pair(string("r2"), tree.get<double>("r2")));
            _data.insert(make_pair(string("mu"), tree.get<int>("mu")));

            return true;

        } catch(...) {
            return false;
        }
    }

    const boost::variant<int, double> &TplConfig::operator[](const string &key)
    {
        return _data[key];
    }

}

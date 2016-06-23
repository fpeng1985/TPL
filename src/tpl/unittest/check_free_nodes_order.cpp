//
// Created by Administrator on 2016/3/25 0025.
//

#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include "tpl_db.h"

using namespace std;
using namespace tpl;

int check_free_nodes_first(const string &benchmark) {
    string path(getenv("BENCHMARK"));
    path += "/ispd2005/" + benchmark;

    TplDB::db().load_circuit(path);

    bool free_node_first = true;
    int num_free = TplDB::db().modules.num_free();

    cout << benchmark << " has " << num_free << " free nodes." << endl;

    for(int i=0; i<num_free; ++i) {
        free_node_first = free_node_first && !TplDB::db().modules[i].fixed;
    }

    if(free_node_first) {
        cout << benchmark << " free nodes first." << endl;
    } else {
        cout << benchmark << " has fixed node first." << endl;
    }

    return 0;
}

int main()
{
    vector<string> benchs = { "bigblue1", "bigblue2", "bigblue3", "bigblue4",
                              "adaptec1", "adaptec2", "adaptec3", "adaptec4" };

    for(auto bench : benchs) {
        check_free_nodes_first(bench);
    }

    return 0;
}

//
// Created by fpeng on 16-6-5.
//

#ifndef TPL_DEBUG_H
#define TPL_DEBUG_H

#include <cassert>

#include <ctime>
#include <chrono>

#include <cstdio>
#include <iostream>
#include <iomanip>

#include <string>

#define checkStatus(status, message) \
if (status) { \
    cout << "error in " << message << endl; \
    return; \
}

class Timer {

public:
    void timeit() {
        t = std::clock();
    }
    void timeit(std::string task) {
        std::cout << task << " finished, took "
        << std::setprecision(4) << double(std::clock()-t)/CLOCKS_PER_SEC << " seconds." << std::endl;
    }

private:
    std::clock_t t;
};


#endif //TPL_DEBUG_H

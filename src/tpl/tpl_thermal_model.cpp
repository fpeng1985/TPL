#include "tpl_thermal_model.h"

#include <boost/property_tree/json_parser.hpp>

#include "tpl_debug.h"

namespace tpl {
    using std::cout;
    using std::endl;
    using std::vector;


    TplThermalModel::TplThermalModel()
    {
        initialize_model();


        _gw_num = floor( TplDB::db().chip_width()  / BIN_WIDTH );
        _gh_num = floor( TplDB::db().chip_height() / BIN_HEIGHT );

        BIN_WIDTH  = TplDB::db().chip_width()  * 1.0 / _gw_num;
        BIN_HEIGHT = TplDB::db().chip_height() * 1.0 / _gh_num;

#ifndef NDEBUG
        _power_density.resize(_gw_num+1, vector<double>(_gh_num+1, 0));

        _xhf_grid.resize(_gw_num+1, vector<double>(_gh_num+1, 0));

        _yhf_grid.resize(_gw_num+1, vector<double>(_gh_num+1, 0));
#else
        _power_density = (double**)malloc( sizeof(double*) * (_gw_num+1) );
        assert(_power_density != nullptr);
        for (int i=0; i<_gw_num+1; ++i) {
            *(_power_density+i) = (double*)malloc( sizeof(double) * (_gh_num+1) );
            assert(*(_power_density+i) != nullptr);
            memset(*(_power_density+i), 0, sizeof(double) * (_gh_num+1) );
        }

        _xhf_grid = (double**)malloc( sizeof(double*) * (_gw_num+1) );
        assert(_xhf_grid != nullptr);
        for (int i=0; i<_gw_num+1; ++i) {
            *(_xhf_grid+i) = (double*)malloc( sizeof(double) * (_gh_num+1) );
            assert(*(_xhf_grid+i) != nullptr);
            memset(*(_xhf_grid+i), 0, sizeof(double) * (_gh_num+1) );
        }

        _yhf_grid = (double**)malloc( sizeof(double*) * (_gw_num+1) );
        assert(_yhf_grid != nullptr);
        for (int i=0; i<_gw_num+1; ++i) {
            *(_yhf_grid+i) = (double*)malloc( sizeof(double) * (_gh_num+1) );
            assert(*(_yhf_grid+i) != nullptr);
            memset(*(_yhf_grid+i), 0, sizeof(double) * (_gh_num+1) );
        }
#endif

        ////////////////////////////////////////////////////////////////////////////
        //init green function
        gdx = static_cast<int>( ceil(R2 / BIN_WIDTH));
        gdy = static_cast<int>( ceil(R2 / BIN_HEIGHT));


#ifndef NDEBUG
        _green_function.resize(gdx, vector<double>(gdy+1, 0));
#else
        _green_function = (double**)malloc( sizeof(double*) * gdx);
        assert(_green_function != nullptr);
        for (int i=0; i<gdx; ++i) {
            *(_green_function+i) = (double*)malloc( sizeof(double) * gdy);
            assert(*(_green_function+i) != nullptr);
            memset(*(_green_function+i), 0, sizeof(double) * gdy );
        }
#endif

        double green_func_val = 0;
        double distance_squre = 0;
        for (int i=0; i<gdx; ++i) {
            for (int j=0; j<gdy; ++j) {
                if (i==0 && j==0) {
                    green_func_val = 0;
                } else {
                    distance_squre = i*i*BIN_WIDTH*BIN_WIDTH + j*j*BIN_HEIGHT*BIN_HEIGHT;
                    if (distance_squre <= R1*R1) {
                        green_func_val =  1.0 / sqrt(distance_squre);
                    } else if (distance_squre <= R2*R2) {
                        green_func_val = 0.6 / sqrt(sqrt(distance_squre));
                    } else {
                        green_func_val = 0;
                    }
                }

                _green_function[i][j] = green_func_val;
            }
        }
        ////////////////////////////////////////////////////////////////////////////
    }

    TplThermalModel::~TplThermalModel()
    {
#ifdef NDEBUG

        for (int i=0; i<_gw_num+1; ++i) {
            free( *(_power_density+i) );
        }
        free(_power_density);

        for (int i=0; i<_gw_num+1; ++i) {
            free( *(_xhf_grid+i) );
        }
        free(_xhf_grid);

        for (int i=0; i<_gw_num+1; ++i) {
            free( *(_yhf_grid+i) );
        }
        free(_yhf_grid);

        for (int i=0; i<gdx; ++i) {
            free(*(_green_function+i));
        }
        free(_green_function);
#endif
    }

    bool TplThermalModel::initialize_model()
    {
        try {
            namespace pt = boost::property_tree;
            pt::ptree tree;
            pt::read_json(getenv("TPLCONFIG"), tree);

            BIN_WIDTH  = tree.get<double>("bin_width");
            BIN_HEIGHT = tree.get<double>("bin_height");
            R1         = tree.get<double>("r1");
            R2         = tree.get<double>("r2");
            MU         = tree.get<double>("mu");

            return true;
        } catch(...) {
            return false;
        }
    }

    void TplThermalModel::compute_heat_flux_vector(VectorXd &HFx, VectorXd &HFy) {
        //preconditions
        assert(HFx.rows() == TplDB::db().num_free());
        assert(HFy.rows() == TplDB::db().num_free());

        try {
            generate_power_density();

            generate_heat_flux_grid();

            ///////////////////////////////////////////////////////////////////////////////////////
            //compute module heat flux using bilinear interpolation method
            double x = 0, y = 0;//for module's coordinates
            int idx_x = 0, idx_y = 0;//for (x,y)'s containing grid point index
            double x1 = 0, x2 = 0, y1 = 0, y2 = 0;//for (x,y)'s containing grid coordinates
            double xhf = 0, yhf = 0;//for x and y heat flux value
            for (size_t i = 0; i != TplDB::db().num_free(); ++i) {
                x = TplDB::db().module(i).x + TplDB::db().module(i).width / 2.0;  //module center x
                y = TplDB::db().module(i).y + TplDB::db().module(i).height / 2.0; //module center y

                idx_x = static_cast<int>(floor(x / BIN_WIDTH));
                idx_y = static_cast<int>(floor(y / BIN_HEIGHT));

                x1 = idx_x * BIN_WIDTH;
                x2 = x1 + BIN_WIDTH;
                y1 = idx_y * BIN_HEIGHT;
                y2 = y1 + BIN_HEIGHT;

                xhf = _xhf_grid[idx_x][idx_y] * (x2 - x) * (y2 - y) +
                      _xhf_grid[idx_x + 1][idx_y] * (x - x1) * (y2 - y) +
                      _xhf_grid[idx_x][idx_y + 1] * (x2 - x) * (y - y1) +
                      _xhf_grid[idx_x + 1][idx_y + 1] * (x - x1) * (y - y1);
                xhf = xhf / (BIN_WIDTH * BIN_HEIGHT);

                yhf = _yhf_grid[idx_x][idx_y] * (x2 - x) * (y2 - y) +
                      _yhf_grid[idx_x + 1][idx_y] * (x - x1) * (y2 - y) +
                      _yhf_grid[idx_x][idx_y + 1] * (x2 - x) * (y - y1) +
                      _yhf_grid[idx_x + 1][idx_y + 1] * (x - x1) * (y - y1);
                yhf = yhf / (BIN_WIDTH * BIN_HEIGHT);

                HFx(i) = xhf;
                HFy(i) = yhf;
            }
            ///////////////////////////////////////////////////////////////////////////////////////
        } catch(...) {
            std::cout << "compute heat flux exception." << std::endl;
        }
    }



    void TplThermalModel::generate_power_density()
    {
        try {
            // reset _power_density
#ifndef NDEBUG
            _power_density.clear();
            _power_density.resize(_gw_num+1, vector<double>(_gh_num+1, 0));
#else
            for (int i=0; i<_gw_num+1; ++i) {
                memset(*(_power_density+i), 0, sizeof(double) * (_gh_num+1) );
            }
#endif

            ///////////////////////////////////////////////////////////////////////////////////
            //iterate modules to generate the density grid
            double left, right, bottom, top;
            unsigned int idx_left, idx_right, idx_bottom, idx_top;
            for(TplDB::mod_iterator it=TplDB::db().mod_begin(); it!=TplDB::db().mod_end(); ++it) {

                left   = it->x;
                right  = it->x + it->width;
                bottom = it->y;
                top    = it->y + it->height;

                idx_left   = static_cast<unsigned int>( ceil (left   / BIN_WIDTH ) );
                idx_right  = static_cast<unsigned int>( floor(right  / BIN_WIDTH ) );
                idx_bottom = static_cast<unsigned int>( ceil (bottom / BIN_HEIGHT) );
                idx_top    = static_cast<unsigned int>( floor(top    / BIN_HEIGHT) );

                for(unsigned int i=idx_left; i<=idx_right; ++i) {
                    for(unsigned int j=idx_bottom; j<=idx_top; ++j) {
                        _power_density[i][j] += it->power_density;
                    }
                }
            }
            ///////////////////////////////////////////////////////////////////////////////////
        } catch(...) {
            std::cout << "update power density exception"  << std::endl;
        }
    }

    double TplThermalModel::power_density(int i, int j)
    {
        try {

            assert(-_gw_num-1 <= i && i <= 2*_gw_num+1);
            assert(-_gh_num-1 <= j && j <= 2*_gh_num+1);

            int                                      x_idx = i;//[0...gw_num] remain
            if     (i < 0       && i >= -_gw_num-1 ) x_idx = -1-i;//-1<==>0
            else if(i > _gw_num && i <= 2*_gw_num+1) x_idx = 2*_gw_num+1-i;//gw_num+1<==>gw_num
            else if(i < -_gw_num-1 || i > 2*_gw_num+1) {
                std::cout << "i index error : " << i << std::endl;
                exit(-1);
            }

            int                                      y_idx = j;
            if     (j < 0       && j >= -_gh_num-1 ) y_idx = -1-j;
            else if(j > _gw_num && j <= 2*_gh_num+1) y_idx = 2*_gh_num+1-j;
            else if(j < -_gh_num-1 || j > 2*_gh_num+1) {
                std::cout << "j index error : " << j << std::endl;
                exit(-1);
            }

            assert(0<= x_idx && x_idx < _gw_num+1);
            assert(0<= y_idx && y_idx < _gh_num+1);
            return _power_density[x_idx][y_idx];

        } catch(...) {
            std::cout << "access power density exception"  << std::endl;
            return -1;
        }
    }


    void TplThermalModel::generate_heat_flux_grid()
    {
        try {

#ifndef NDEBUG
            _xhf_grid.clear();
            _xhf_grid.resize(_gw_num+1, vector<double>(_gh_num+1, 0));

            _yhf_grid.clear();
            _yhf_grid.resize(_gw_num+1, vector<double>(_gh_num+1, 0));
#else
            for (int i=0; i<_gw_num+1; ++i) {
                memset(*(_xhf_grid+i), 0, sizeof(double) * (_gh_num+1) );
            }

            for (int i=0; i<_gw_num+1; ++i) {
                memset(*(_yhf_grid+i), 0, sizeof(double) * (_gh_num+1) );
            }
#endif

            //hfx[i][j] = (tss[i+1][j] - tss[i-1][j]) / 2
            //tss[i+1][j] : x <- [i+1-(gdx-1), i+1+(gdx-1)] = [i-gdx+2, i+gdx]
            //tss[i-1][j] : x <- [i-1-(gdx-1), i-1+(gdx-1)] = [i-gdx, i+gdx-2]
            //since gdx>2 so we and cut the whole interval to 3 sections: [i-gdx, i-gdx+2), [i-gdx+2, i+gdx-2], (i+gdx-2, i+gdx]
            //and the Green function in the 3 section are -G[i-1,j; i0, j0], G[i+1,j; i0,j0]-G[i-1,j; i0, j0], G[i+1,j; i0, j0]
            //hfy computation is similar
            assert(gdx > 2);
            assert(gdy > 2);
            for (int i=0; i<=_gw_num; ++i) { //[0, _gw_num]
                for (int j=0; j<=_gh_num; ++j) { //[0, _gh_num]

                    /////////////////////////////////////////////////////////////////////////////////
                    // x heat flux
                    _xhf_grid[i][j] = 0;
                    for (int j0=j-gdy+1; j0<=j+gdy-1; ++j0) { // j0 <- [j-(gdy-1), j+(gdy-1)]
                        int gy_idx = abs(j-j0);//g for green function

                        for (int i0=i-gdx; i0<i-gdx+2; ++i0) { //i0 <= [ i-1 - (gdx-1), i+1 - (gdx-1) )
                            _xhf_grid[i][j] += (-_green_function[abs(i-1-i0)][gy_idx]) * power_density(i0, j0);
                        }

                        for (int i0=i-gdx+2; i0<=i+gdx-2; ++i0) { //i0 <- [ i+1 - (gdx-1), i-1 + (gdx-1) ]
                            _xhf_grid[i][j] += (_green_function[abs(i+1-i0)][gy_idx] - _green_function[abs(i-1-i0)][gy_idx])
                                               * power_density(i0, j0);
                        }

                        for (int i0=i+gdx-1; i0<=i+gdx; ++i0) { //i0 <- ( i-1 + (gdx-1), i+1 + (gdx-1) ]
                            _xhf_grid[i][j] += _green_function[abs(i+1-i0)][gy_idx] * power_density(i0, j0);
                        }
                    }
                    _xhf_grid[i][j] /= 2.0;
                    /////////////////////////////////////////////////////////////////////////////////

                    /////////////////////////////////////////////////////////////////////////////////
                    // y heat flux
                    _yhf_grid[i][j] = 0;
                    for (int i0=i-gdx+1; i0<=i+gdx-1; ++i0) { //i0 <- [i-(gdx-1), i+(gdx-1)]
                        int gx_idx = abs(i-i0);//g for green function

                        for (int j0=j-gdy; j0<j-gdy+2; ++j0) { //j0 <- [ j-1 - (gdy-1), j+1 - (gdy-1) )
                            _yhf_grid[i][j] += (-_green_function[gx_idx][abs(j-1-j0)]) * power_density(i0, j0);
                        }

                        for (int j0=j-gdy+2; j0<=j+gdy-2; ++j0) { //j0 <- [ j+1 - (gdy-1), j-1 + (gdy-1) ]
                            _yhf_grid[i][j] += (_green_function[gx_idx][abs(j+1-j0)] - _green_function[gx_idx][abs(j-1-j0)])
                                               * power_density(i0, j0);
                        }

                        for (int j0=j+gdy-1; j0<=j+gdy; ++j0) { //j0 <- ( j-1 + (gdy-1), j+1 + (gdy-1) ]
                            _yhf_grid[i][j] += _green_function[gx_idx][abs(j+1-j0)] * power_density(i0, j0);
                        }
                    }
                    _yhf_grid[i][j] /= 2.0;
                    /////////////////////////////////////////////////////////////////////////////////
                }
            }// end grid iteration
        } catch(...) {
            std::cout << "compute heat flux exception"  << std::endl;
        }
    }

}//namespace tpl

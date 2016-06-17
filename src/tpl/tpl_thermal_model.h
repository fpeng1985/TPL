/*!
 * \file tpl_thermal_model.h
 * \author Peng Fei
 * \brief Standard implementation for Thermal Model.
 */

#ifndef TPL_THERMAL_FORCE_MODEL_H
#define TPL_THERMAL_FORCE_MODEL_H


#include "linear_algebra_lib.h"
#include "tpl_db.h"

#include <map>
#include <utility>
#include <vector>


namespace tpl {

    using std::vector;

    //! Standard implementation for tpl move model.
    class TplThermalModel {
    public:
        //! Constructor.
        TplThermalModel();

        //Destructor.
        ~TplThermalModel();

        bool initialize_model();

        //! Standard implementation for compute_head_flux_vector.
        void compute_heat_flux_vector(VectorXd &x_heat_flux, VectorXd &y_heat_flux);

        double get_mu() const {
            return MU;
        }

    protected:
        //! Generate the power density.
        void generate_power_density();

        //! Fetch power density for grid (i,j).
        /*!
         * \param i x index for power density.
         * \param j y index for power density.
         */
        double power_density(int i, int j);

        //! Generate heat flux grid for both x and y direction.
        /*!
         * The heat flux grid's i and j index is different from the Eigen matrix system,
         * but accordance with the chip layout.
         * The [0][0] grid point is the lower left corner.
         * Please note that.
         */
        void generate_heat_flux_grid();

        int _gw_num;     //!< Number of bin in x direction, g for grid.
        int _gh_num;     //!< Number of bin in y direction, g for grid.

#ifndef NDEBUG
        vector<vector<double>> _power_density;
        vector<vector<double>> _xhf_grid;
        vector<vector<double>> _yhf_grid;
#else
        double **_power_density; //!< 2 dimentional array of size col:(_gw_num+1), row:(_gh_num+1).
        double **_xhf_grid;      //!< 2 dimentional array of size col:(_gw_num+1), row:(_gh_num+1).
        double **_yhf_grid;      //!< 2 dimentional array of size col:(_gw_num+1), row:(_gh_num+1).
#endif

        int gdx; //!< Number of values of green function in x direction, g for green function.
        int gdy; //!< Number of values of green function in y direction, g for green function.

#ifndef NDEBUG
        vector<vector<double>> _green_function;
#else
        double **_green_function; //!< 2 dimentional array of size col:gdx, row:gdy.
#endif

        double BIN_WIDTH;  //!< Algorithm parameter : a grid bin's width.
        double BIN_HEIGHT; //!< Algorithm parameter : a grid bin's height.
        double R1; //!< Algorithm parameter : green function R1.
        double R2; //!< Algorithm parameter : green function R2.
        double MU; //!< Algorithm parameter : qanlity control parameter.
    };
}

#endif //TPL_STANDARD_THERMAL_FORCE_MODEL_H

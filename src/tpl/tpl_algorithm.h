/*!
 * \file tpl_algorithm.h
 * \author Peng Fei
 * \brief Standard definition for TPL algorithm interface
 */

#ifndef TPL_ALGORITHM_H
#define TPL_ALGORITHM_H

#include <vector>
#include <memory>

#include "linear_algebra_lib.h"
#include "tpl_net_model.h"
#include "tpl_thermal_model.h"

namespace tpl {
    using std::vector;

    //! Standard implementation for tpl algorithm.
    class TplAlgorithm {
    public:
        //! Constructor.
        TplAlgorithm();

        //! Virtual destructor.
        virtual ~TplAlgorithm() {}

        //! Standard implementation for interface initialize_models.
        virtual void initialize_models();

        //! Standard implementation for shredding macros into cells
//        virtual void shred();

        //! Standard implementation for aggregating cells into macros
//        virtual void aggregate();

        //! Standard implementation for interface make_initial_placement.
        virtual void make_initial_placement();

        //! standard implementation for interface make_global_placement.
        virtual void make_global_placement();

        //! standard implementation of detail placement using NTUPlacer3
        /*!
         * \param filename name of the global placement result file
         */
        virtual void make_detail_placement(std::string benchmark);

        //! main control function of a complete placement flow of a single benchmark
        /*!
         * \param path path of benchmark
         * \param mmp flag of whether enable macro placement
         */
        virtual void control(std::string path, bool mmp);

    protected:
        void initialize_move_force_matrix();
        void update_move_force_matrix(const VectorXd &delta_x, const VectorXd &delta_y, double mu);
        bool should_stop_global_placement() const;
		bool should_stop_initial_placement(double &lmd, double &cmd) const;

        std::shared_ptr<TplNetModel>          _net_model;           //!< Pointer to a TplNetModel.
        std::shared_ptr<TplThermalModel> _thermal_model; //!< Pointer to a TplThermalModel.

        NetWeight NWx, NWy;
        SpMat Cx,  Cy;
        VectorXd HFx, HFy;
        SpMat Cx0, Cy0;
    };

    struct SegmentEvent {
        double x1 , x2, y;
        int f;

        SegmentEvent(double a, double b, double c, int d) : x1(a) , x2(b) , y(c) , f(d) {}

        bool operator < (const SegmentEvent &cmp) const {
            return y < cmp.y;
        }
    };

    class SegmentTree {
    public:
        void build (const vector<double> &xpos);

        void update(const SegmentEvent &e);

        inline double get_sum() { return sum[1]; }

    private:
        void update(int L,int R,int c, int rt, int l,int r);

        int search(double key);

        void push_up(int rt,int l,int r);

        vector<double> pos;
        vector<double> sum;
        vector<int> cov;
    };

}//end namespace tpl

#endif //TPL_STANDARD_ALGORITHM_H

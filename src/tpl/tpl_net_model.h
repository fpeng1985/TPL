/*!
 * \file tpl_net_model.h
 * \author Peng Fei
 * \brief Standard implementation for Net Model.
 */

#ifndef TPL_NET_MODEL_H
#define TPL_NET_MODEL_H

#include "tpl_db.h"
#include "linear_algebra_lib.h"

#include <vector>
#include <map>
#include <utility>

namespace tpl {

    /*!
     * \typedef std::map<std::pair<TplPin*, TplPin*>, double> NetWeight;
     * \brief NetWeight type
     */
    using NetWeight = std::map<std::pair<TplPin*, TplPin*>, double>;

    class TplNetModel {
    public:

        //! Standard implementation for interface compute_net_weight.
        /*!
         * \param NWx The net weight in x directions to be computed.
         * \param NWy The net weight in y directions to be computed.
         */
        virtual void compute_net_weight(NetWeight &NWx, NetWeight &NWy);

        //! Standard implementation for interface compute_net_force_matrix.
        /*!
         * \param NWx Net weight int x direction.
         * \param NWy Net weight int y direction.
         * \param Cx  SpMat The net force matrix in x direction to be computed.
         * \param Cy  SpMat The net force matrix in y direction to be computed.
         * \param dx  VectorXd storing the net force vector in x direction.
         * \param dy  VectorXd storing the net force vector in y direction.
         */
        virtual void compute_net_force_matrix(const NetWeight &NWx, const NetWeight &NWy,
                                              SpMat &Cx, SpMat &Cy, VectorXd &dx, VectorXd &dy);

        //! Standard implementation for interface compute_net_force_target.
        /*!
         * \param NWx Net weight int x direction.
         * \param NWy Net weight int y direction.
         * \param x_target vector storing the target x positions
         * \param y_target vector storing the target y positions
         */
        virtual void compute_net_force_target(const NetWeight &NWx, const NetWeight &NWy,
                                              std::vector<double> &x_target, std::vector<double> &y_target);

    };

    struct PinPos {
        TplPin *pin;
        double pos;
        PinPos(TplPin *p=nullptr, double c=0) : pin(p), pos(c) {}
    };

    bool compare_pin(const PinPos &lhs, const PinPos &rhs);

}//namespace tpl

#endif //TPL_STANDARD_NET_MODEL_H

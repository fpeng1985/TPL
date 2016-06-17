/*!
 * \file tpl_standard_net_model.h
 * \author Peng Fei
 * \brief Standard implementation for Net Model.
 */

#ifndef TPL_STANDARD_NET_MODEL_H
#define TPL_STANDARD_NET_MODEL_H

namespace tpl {
    /*!
     * \typedef std::map<std::pair<TplPin*, TplPin*>, double> NetWeight;
     * \brief NetWeight type
     */
    using NetWeight = std::map<std::pair<TplPin*, TplPin*>, double>;

    class TplStandardNetModel : public TplAbstractNetModel {
    public:
        //! Default constructor.
        TplStandardNetModel() : TplAbstractNetModel() {}

        //! Virtual destructor.
        virtual ~TplStandardNetModel() {}

        //! Standard implementation for interface compute_net_weight.
        /*!
         * \param NWx The net weight in x directions to be computed.
         * \param NWy The net weight in y directions to be computed.
         */
        virtual void compute_net_weight(NetWeight &NWx, NetWeight &NWy);


        //! Standard implementation for update_shred_net_weight
        virtual void update_shred_net_weight(NetWeight &NWx, NetWeight &NWy, int i);
    };

    struct PinPos {
        TplPin *pin;
        double pos;
        PinPos(TplPin *p=nullptr, double c=0) : pin(p), pos(c) {}
    };

    bool compare_pin(const PinPos &lhs, const PinPos &rhs);

}//namespace tpl

#endif //TPL_STANDARD_NET_MODEL_H

#include "tpl_standard_net_model.h"

#include <cassert>
#include <algorithm>

namespace tpl {

    using std::string;
    using std::vector;
    using std::map;
    using std::pair;
    using std::make_pair;
    using std::sort;

    void TplNetModel::compute_net_weight(NetWeight &NWx, NetWeight &NWy)
	{
		//preconditions
		NWx.clear();
		NWy.clear();

//		vector<TplPin*> xpins;
//		vector<TplPin*> ypins;

		vector<PinPos> xpins;
		vector<PinPos> ypins;
		size_t degree = 0;

		const double DELTA = 0.001;

		for (TplDB::net_iterator nit=TplDB::db().net_begin(); nit!=TplDB::db().net_end(); ++nit) {
			degree = nit->pins.size();
			if ( degree < 2 ) continue;

			xpins.clear();
			ypins.clear();

			/////////////////////////////////////////////////////////////////////
			//get the current net's pins, and sort them by x and y separately
			for (TplDB::pin_iterator pit = TplDB::db().pin_begin(nit);
				 pit != TplDB::db().pin_end(nit); ++pit) {
				TplModule &module = TplDB::db().module(pit->id);
				xpins.emplace_back(&(*pit), module.x + module.width /2.0 + pit->dx);
				ypins.emplace_back(&(*pit), module.y + module.height/2.0 + pit->dy);
			}

			assert(xpins.size() == degree);
			assert(ypins.size() == degree);

			sort(xpins.begin(), xpins.end(), compare_pin);
			sort(ypins.begin(), ypins.end(), compare_pin);
			/////////////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////////
			//compute the net weight by B2B net model, ignore the pin on the boundary
			double x1 = xpins[0].pos;
			double x2 = xpins[degree-1].pos;

			double y1 = ypins[0].pos;
			double y2 = ypins[degree-1].pos;

			for (size_t i=1; i<degree-1; ++i) {
				//process by x
				double &x = xpins[i].pos;

				if (fabs(x-x1) > DELTA) {
					assert( NWx.count(make_pair(xpins[0].pin, xpins[i].pin)) == 0 );
					NWx[make_pair(xpins[0].pin, xpins[i].pin)] = 2.0 / (degree - 1) / (fabs(x-x1));
				}

				if (fabs(x-x2) > DELTA) {
					assert( NWx.count(make_pair(xpins[degree-1].pin, xpins[i].pin)) == 0 );
					NWx[make_pair(xpins[degree-1].pin, xpins[i].pin)] = 2.0 / (degree - 1) / (fabs(x-x2));
				}

				//process pins by y
				double &y = ypins[i].pos;

				if (fabs(y-y1) > DELTA) {
					assert( NWy.count(make_pair(ypins[0].pin, ypins[i].pin)) == 0 );
					NWy[make_pair(ypins[0].pin, ypins[i].pin)] = 2.0 / (degree - 1) / (fabs(y-y1));
				}

				if (fabs(y-y2) > DELTA) {
					assert( NWy.count(make_pair(ypins[degree-1].pin, ypins[i].pin)) == 0 );
					NWy[make_pair(ypins[degree-1].pin, ypins[i].pin)] = 2.0 / (degree - 1) / (fabs(y-y2));
				}
			}

			/////////////////////////////////////////////////////////////////////
		}//end net traversal
	}
    void TplNetModel::compute_net_force_matrix(const NetWeight &NWx, const NetWeight &NWy,
                                                            SpMat &Cx, SpMat &Cy, VectorXd &dx, VectorXd &dy)
    {
        //preconditions
        assert(Cx.cols() == static_cast<int>(TplDB::db().num_free()));
        assert(Cx.rows() == static_cast<int>(TplDB::db().num_free()));
        assert(Cy.cols() == static_cast<int>(TplDB::db().num_free()));
        assert(Cy.rows() == static_cast<int>(TplDB::db().num_free()));
        assert(dx.rows() == static_cast<int>(TplDB::db().num_free()));
        assert(dy.rows() == static_cast<int>(TplDB::db().num_free()));

        Cx.setZero();
        Cy.setZero();
        dx.setZero();
        dy.setZero();

        //prepare data
        TplPin *pin1, *pin2;
        string id1, id2;
        size_t idx1, idx2;
        double weight;
        map<pair<size_t, size_t>, double> nw;
        vector<SpElem> coefficients;

        //compute Cx and dx
        for(NetWeight::const_iterator it=NWx.begin(); it!=NWx.end(); ++it) {
            pin1 = it->first.first;
            pin2 = it->first.second;
            id1 = pin1->id;
            id2 = pin2->id;
            idx1    = TplDB::db().module_index(id1);
            idx2    = TplDB::db().module_index(id2);
            weight = it->second;

            if(!TplDB::db().is_module_fixed(id1) && !TplDB::db().is_module_fixed(id2)) {
                nw[make_pair(idx1, idx1)] += weight;
                nw[make_pair(idx2, idx2)] += weight;
                nw[make_pair(idx1, idx2)] -= weight;
                nw[make_pair(idx2, idx1)] -= weight;

                dx(idx1) += weight*(pin1->dx - pin2->dx);
                dx(idx2) += weight*(pin2->dx - pin1->dx);
            } else if(!TplDB::db().is_module_fixed(id1) && TplDB::db().is_module_fixed(id2)) {
                nw[make_pair(idx1, idx1)] += weight;

                dx(idx1) += weight*(pin1->dx - pin2->dx - TplDB::db().module(id2).x);
            } else if(TplDB::db().is_module_fixed(id1) && !TplDB::db().is_module_fixed(id2)) {
                nw[make_pair(idx2, idx2)] += weight;

                dx(idx2) += weight*(pin2->dx - pin1->dx - TplDB::db().module(id1).x);
            } else {
                continue;
            }
        }

        for(map<pair<size_t, size_t>, double>::iterator it=nw.begin(); it!=nw.end(); ++it) {
            coefficients.push_back( SpElem(it->first.first, it->first.second, it->second) );
        }
        Cx.setFromTriplets(coefficients.begin(), coefficients.end());

        //clear temp container
        nw.clear();
        coefficients.clear();

        //compute Cy and dy
        for(NetWeight::const_iterator it=NWy.begin(); it!=NWy.end(); ++it) {
            pin1 = it->first.first;
            pin2 = it->first.second;
            id1 = pin1->id;
            id2 = pin2->id;
            idx1    = TplDB::db().module_index(id1);
            idx2    = TplDB::db().module_index(id2);
            weight = it->second;

            if(!TplDB::db().is_module_fixed(id1) && !TplDB::db().is_module_fixed(id2)) {
                nw[make_pair(idx1, idx1)] += weight;
                nw[make_pair(idx2, idx2)] += weight;
                nw[make_pair(idx1, idx2)] -= weight;
                nw[make_pair(idx2, idx1)] -= weight;

                dy(idx1) += weight*(pin1->dy - pin2->dy);
                dy(idx2) += weight*(pin2->dy - pin1->dy);
            } else if(!TplDB::db().is_module_fixed(id1) && TplDB::db().is_module_fixed(id2)) {
                nw[make_pair(idx1, idx1)] += weight;

                dy(idx1) += weight*(pin1->dy - pin2->dy - TplDB::db().module(id2).y);
            } else if(TplDB::db().is_module_fixed(id1) && !TplDB::db().is_module_fixed(id2)) {
                nw[make_pair(idx2, idx2)] += weight;

                dy(idx2) += weight*(pin2->dy - pin1->dy - TplDB::db().module(id1).y);
            } else {
                continue;
            }
        }

        for(map<pair<size_t, size_t>, double>::iterator it=nw.begin(); it!=nw.end(); ++it) {
            coefficients.push_back( SpElem(it->first.first, it->first.second, it->second) );
        }
        Cy.setFromTriplets(coefficients.begin(), coefficients.end());
    }

    void TplNetModel::compute_net_force_target(const NetWeight &NWx, const NetWeight &NWy,
                                                            std::vector<double> &x_target, std::vector<double> &y_target)
    {
        //preconditions
//        x_target.clear();
//        y_target.clear();
        unsigned int num_free = TplDB::db().num_free();
        x_target.resize(num_free, 0);
        y_target.resize(num_free, 0);

        SpMat Cx(num_free, num_free), Cy(num_free, num_free);
        Cx.setZero();
        Cy.setZero();

        VectorXd dx(num_free), dy(num_free);
        dx.setZero();
        dy.setZero();

        compute_net_force_matrix(NWx, NWy, Cx, Cy, dx, dy);

        LLTSolver solver;
        VectorXd x_eigen_target = solver.compute(Cx).solve(dx*-1);
        VectorXd y_eigen_target = solver.compute(Cy).solve(dy*-1);

        assert(static_cast<long>(x_target.size()) == x_eigen_target.size() );
        assert(static_cast<long>(y_target.size()) == y_eigen_target.size() );

        VectorXd::Map(&x_target[0], x_eigen_target.size()) = x_eigen_target;
        VectorXd::Map(&y_target[0], y_eigen_target.size()) = y_eigen_target;
    }

	bool compare_pin(const PinPos &lhs, const PinPos &rhs)
	{
		const TplModule & m1 = TplDB::db().module(lhs.pin->id);
		const TplModule & m2 = TplDB::db().module(rhs.pin->id);

		return ( (m1.x + m1.width/2.0 + lhs.pin->dx) < (m2.x + m2.width/2.0 + rhs.pin->dx) );
	}

}//namespace tpl


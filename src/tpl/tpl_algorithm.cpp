#include "tpl_algorithm.h"
#include "tpl_debug.h"

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <algorithm>
#include <unistd.h>

#include "tpl_debug.h"

#include <boost/filesystem.hpp>



namespace tpl {
    using namespace std;

    TplAlgorithm::TplAlgorithm()
    {
       initialize_models();

        const unsigned &msize = TplDB::db().num_free();

        Cx  = SpMat(msize, msize);
        Cy  = SpMat(msize, msize);

        Cx0 = SpMat(msize, msize);
        Cy0 = SpMat(msize, msize);

        HFx.resize(msize);
        HFy.resize(msize);
    }

	void TplAlgorithm::initialize_models()
	{
		_net_model     = make_shared<TplNetModel>();
		_thermal_model = make_shared<TplThermalModel>();
	}

    void TplAlgorithm::control(std::string path, bool mmp) {
		Timer timer;
//		timer.timeit();
        /*
        if (mmp) {
			timer.timeit();
            shred();
			timer.timeit("shred");
        }
         */
//		timer.timeit();
        make_initial_placement();
//		timer.timeit("initial placement");
        /*
        if (mmp) {
			timer.timeit();
            aggregate();
			timer.timeit("aggregate");
        }
         */
		timer.timeit();
         make_global_placement();
		timer.timeit("global placement");
		timer.timeit();
        // make_detail_placement(path);
		timer.timeit("detail placement");
    }

    void TplAlgorithm::make_initial_placement()
    {
        //TODO:should stop initial placement
//        TplDB::db().modules.move_to_center();
        vector<double> x_target, y_target;
        NetWeight NWx, NWy;
        _net_model->compute_net_weight(NWx, NWy);

        double lmd = -1, cmd = 0.0;
		while(!should_stop_initial_placement(lmd, cmd)) {
			// x_target and y_target will clear and resize in compute_net_force_target
            _net_model->compute_net_force_target(NWx, NWy, x_target, y_target);
//            cmd = TplDB::db().set_free_module_coordinates(x_target, y_target);
            TplDB::db().set_free_module_coordinates(x_target, y_target);
		}
    }

    void TplAlgorithm::make_global_placement()
    {
        const unsigned &msize = TplDB::db().num_free();

        initialize_move_force_matrix();//compute Cx0 and Cy0
        VectorXd dx(msize), dy(msize);
        VectorXd rhsx(msize), rhsy(msize);

        while (!should_stop_global_placement()) {
            _net_model->compute_net_weight(NWx, NWy);
            _net_model->compute_net_force_matrix(NWx, NWy, Cx, Cy, dx, dy);//compute Cx and Cy
            _thermal_model->compute_heat_flux_vector(HFx, HFy);//Compute HFx and HFy

            LLTSolver llt;

            rhsx = Cx0*HFx*-1;
            rhsy = Cy0*HFy*-1;
            VectorXd delta_x = llt.compute(Cx+Cx0).solve(rhsx);
            VectorXd delta_y = llt.compute(Cy+Cy0).solve(rhsy);

            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //the following code breaks the incapsulation, but more quickly, please note!!!
            for (unsigned i=0; i<msize; ++i) {
                TplDB::db().module(i).x += delta_x(i);
                TplDB::db().module(i).y += delta_y(i);
            }

            update_move_force_matrix(delta_x, delta_y, _thermal_model->get_mu());//udpate Cx0 and Cy0
        }
    }

	void TplAlgorithm::make_detail_placement(string path) {
		// save global placement to .pl file
		string fn = path + "/gp.pl";
		ofstream fout(fn);
		fout << "TPL GP 1.0\n";
		for (TplDB::mod_iterator mit = TplDB::db().mod_begin();
			 mit != TplDB::db().mod_end(); mit++) {
			fout << mit->id << "\t" << mit->x << "\t" << mit->y << endl;
		}
		fout.close();

		// run ntuplace3 to do legalization and detail placement
		boost::filesystem::path dir(path);
		string file = path + "/" + dir.filename().string();
		string cmd = getenv("BENCHMARK");
		cmd += "/ispd2005/ntuplace3 -aux " + file + ".aux -loadpl " + fn + " -noglobal\n";
		int ret = system(cmd.c_str());
		cout << ret << endl;
		if (ret != 0) {
			cout << "error in ntuplace3" << endl;
		}
	}

    void TplAlgorithm::initialize_move_force_matrix()
    {
        vector<SpElem> coefficients;
        unsigned int num_free = TplDB::db().num_free();
        vector<double> module_power;

        double avg_power = 0;
        double power;
        for(TplDB::mod_iterator it=TplDB::db().mod_begin(); it!=TplDB::db().mod_end(); ++it) {
            power = it->width * it->height * it->power_density;
            avg_power +=  power;

            if(!it->fixed) module_power.push_back(power);
        }
        avg_power /= TplDB::db().num_module();

        for(size_t i=0; i<module_power.size(); ++i) {
            coefficients.push_back(SpElem(i, i, module_power[i]/avg_power/num_free));
        }

        Cx0.setFromTriplets(coefficients.begin(), coefficients.end());
        Cy0.setFromTriplets(coefficients.begin(), coefficients.end());
    }

    void TplAlgorithm::update_move_force_matrix(const VectorXd &delta_x, const VectorXd &delta_y, double mu)
    {
        const unsigned &msize = TplDB::db().num_free();

        double avg_mu = 0;
        for (unsigned int i=0; i<msize; ++i) {
            avg_mu = sqrt( pow(delta_x(i), 2) + pow(delta_y(i), 2) );
        }
        avg_mu /= msize;

        double k = 1 + tanh(log(mu/avg_mu));

        Cx0 *= k;
        Cy0 *= k;
    }

	bool TplAlgorithm::should_stop_initial_placement(double &lmd, double &cmd) const {
		const double DELTA = 0.1;

        if (lmd > 0 && cmd > lmd) {
            cout << "error: current move distance > last move distance" << endl;
            exit(EXIT_FAILURE);
        }
        printf("last move distance = %lf\ncurrent move distance = %lf\n", lmd, cmd);
        if (lmd > 0) {
            printf("ratios = %.2lf%%", cmd / lmd * 100);
        }
        bool ret =  (lmd > 0 && cmd / lmd < DELTA);
        lmd = cmd;
        return ret;
	}

    bool TplAlgorithm::should_stop_global_placement() const
    {
       SegmentTree segtree;
        vector<SegmentEvent> events;

        vector<double> xpos;
        double x1, x2, y1, y2;

        double total_area = 0;

        for (TplDB::mod_iterator it=TplDB::db().mod_begin(); it!=TplDB::db().mod_end(); ++it) {
            x1 = it->x;
            x2 = it->x + it->width;
            y1 = it->y;
            y2 = it->y + it->height;

            events.emplace_back(x1, x2, y1, 1);
            events.emplace_back(x1, x2, y2, -1);

            xpos.push_back(x1);
            xpos.push_back(x2);

            total_area += it->width * it->height;
        }

        //set up tree and events
        segtree.build(xpos);
        sort(events.begin(), events.end());

        //feed event to the segtree, and get the total unioned area
        double unioned_area = 0;
        for (size_t i = 0 ; i < events.size()-1 ; ++i) {
            segtree.update(events[i]);
            unioned_area += segtree.get_sum() * (events[i+1].y - events[i].y);
        }

        cout << "unioned area : " << setprecision(8) << unioned_area << endl;
        cout << "total area: "    << setprecision(8) << total_area << endl;

        const double STOP = 0.2;

        cout << "ratio is " << (1-unioned_area/total_area) << endl;

        return ( (1- unioned_area/total_area) < STOP );
    }

	void SegmentTree::build (const vector<double> &xpos) {
        pos = xpos;
        sort(pos.begin() , pos.end());
        pos.erase(unique(pos.begin(), pos.end()), pos.end());

        sum.resize((pos.size() << 2) + 10);
        cov.resize((pos.size() << 2) + 10);
    }

	void SegmentTree::push_up(int rt,int l,int r) {
		if (cov[rt]) {
			sum[rt] = pos[r+1] - pos[l];
		} else if (l == r) {
			sum[rt] = 0;
		} else {
			sum[rt] = sum[rt<<1] + sum[rt<<1|1];
		}
	}

    void SegmentTree::update(const SegmentEvent &e) {

        int l = search(e.x1);
        int r = search(e.x2)-1;
        if (l <= r) {
            update(l, r, e.f, 1, 0, pos.size()-1);
        }
    }

    void SegmentTree::update(int L,int R,int c, int rt, int l,int r) {
        //l and r for the segtree, and L and R for the event.
        if (L <= l && r <= R) {
            cov[rt] += c;
            push_up(rt , l , r);
            return ;
        }

        int m = (l + r) >> 1;
        if (L <= m)  {
            update(L , R , c , rt<<1, l, m);
        }
        if (m < R) {
            update(L , R , c , rt<<1|1, m+1, r);
        }
        push_up(rt , l , r);
    }

    int SegmentTree::search(double key) {
        int l = 0 , r = pos.size()-1;
        while (l <= r) {
            int m = (l + r) >> 1;
            if (pos[m] == key) return m;
            if (pos[m] < key) l = m + 1;
            else r = m - 1;
        }
        return -1;
    }

    /*
	void TplAlgorithm::shred() {
		// list<TplNet> shreddedNets;
		// find shredded cells of a module through the module id
		map<Id, vector<TplModule>> macro_cells;
		list<TplNet> shreddedNets;
		unsigned int rowHeight = TplDB::db().modules[0].height;
		unsigned int colWidth = rowHeight;
		// iterate over macros
		for (TplModules::iterator macro_iter = TplDB::db().modules.begin() + TplDB::db().modules.num_free();
			 macro_iter != TplDB::db().modules.end(); macro_iter++) {
			int rowNum = (macro_iter->height - 1) / rowHeight + 1;
			int colNum = (macro_iter->width - 1) / colWidth + 1;
			//int colNum = 1;
			vector<TplModule> cells;
			for (int m = 0; m < rowNum; m++) {
				for (int n = 0; n < colNum; n++) {
					// create new small cells
					Id id = macro_iter->id + "_" + to_string(m * colNum + n);
					Coordinate x = macro_iter->x + n * colWidth;
					Coordinate y = macro_iter->y + m * rowHeight;
					//Length width = colWidth;
					Length width = macro_iter->width;
					Length height = rowHeight;
					bool fixed = false;
					double density = 1.0;
					TplModule cell(id, x, y, width, height, fixed, density);
					cells.push_back(cell);

					// add new cells to new nets
					TplPin pin;
					pin.id = id;
					pin.io = IOType::Input;
					pin.dx = 0;
					pin.dy = 0;
					// new net with under cell
					if (m != 0) {
						TplModule underCell = cells[cells.size() - 1 - colNum];
						TplPin underPin;
						underPin.id = underCell.id;
						underPin.io = IOType::Input;
						underPin.dx = 0;
						underPin.dy = 0;
						TplNet net;
						net.id = macro_iter->id + "_row_" + to_string(m * colNum + n);
						net.degree = 2;
						net.pins.push_back(underPin);
						net.pins.push_back(pin);
						shreddedNets.push_back(net);
					}

					// new net with left cell
					if (n != 0) {
						TplModule frontCell = cells[cells.size() - 2];
						TplPin frontPin;
						frontPin.id = frontCell.id;
						frontPin.io = IOType::Input;
						frontPin.dx = 0;
						frontPin.dy = 0;
						TplNet net;
						net.id = macro_iter->id + "_col_" + to_string(m * rowNum + n);
						net.degree = 2;
						net.pins.push_back(frontPin);
						net.pins.push_back(pin);
						shreddedNets.push_back(net);
					}
				}
			}
			macro_cells[macro_iter->id] = cells;
		}

		// back up original nets
		TplDB::db().nets.backup_net();

		// modify original nets
		for (list<TplNet>::iterator net_iter = TplDB::db().nets.net_begin();
			 net_iter != TplDB::db().nets.net_end(); net_iter++) {

			// iterate over original pins, exclude pins of new added cells
			const int len = net_iter->pins.size();
			// net_iter->pins[p] is pin on a macro, and i is used to control loop times
			for (int i = 0, p = 0; i < len; i++) {
				TplModule module = TplDB::db().modules.module(net_iter->pins[p].id);
				if (!module.fixed) {
					p++;
					continue;
				}

				// if this net contains a macro, then add new shredded cells to this net
				vector<TplModule> cells = macro_cells[module.id];
				for (TplModules::iterator cell_iter = cells.begin();
					 cell_iter != cells.end(); cell_iter++) {
					TplPin pin;
					pin.id = cell_iter->id;
					pin.io = IOType::Input;
					pin.dx = 0;
					pin.dy = 0;
					net_iter->pins.push_back(pin);
				}

				// delete original macro from this net, since next element become i, need to i--
				net_iter->pins.erase(net_iter->pins.begin() + p);
			}
		}

		// delete macros and add shredded cells
		TplDB::db().modules.add_shredded_cells(macro_cells);

		// add new nets between shredded cells
		TplDB::db().nets.add_net(shreddedNets);
	}

	void TplAlgorithm::aggregate() {
		TplDB::db().modules.aggregate_cells();
		TplDB::db().nets.delete_net();
	}
     */


}//end namespace tpl


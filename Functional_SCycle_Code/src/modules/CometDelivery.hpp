class CometDelivery: public Module {
public:
    CometDelivery(string name): Module(name) { init(); }

    double s_red, tau, F0, F1, sol_red, VOcean; //this
    bool uniform_growth;

    void init(void) {
        this->links.push_back("Space -> Oceans0"); //    (red)
        this->links.push_back("Space -> Sediments0"); // (red)
        this->links.push_back("Space -> CCrust0"); //    (red)
        this->numOutputs = 1;
        this->init_fluxes(3);
        
        s_red = s->element_map["s_red"];

        F0 = config->data["CometDelivery"]["F0"].as<double>();
        F1 = config->data["CometDelivery"]["F1"].as<double>();
        tau = config->data["CometDelivery"]["tau"].as<double>();
        sol_red = config->data["Precipitation"]["sol_red"].as<double>(); //this
        VOcean = config->data["Precipitation"]["VOcean"].as<double>(); //Volume of the Ocean in Liters 
        uniform_growth = config->data["Erosion"]["uniform_growth"].as<bool>();
    }

    void evolve(void) {
        int oc = s->reservoir_map["Oceans"];
        int sed = s->reservoir_map["Sediments"];   //this
        int co = s->reservoir_map["CCrust"];
        
        
        double relative_area;
        if (s->time < 1.5e9) { 
            relative_area = 0.4*(0.02 + 0.64*s->time/1.5e9);
        } else {
            relative_area = 0.4*(0.66 + 0.34*(s->time-1.5e9)/3e9);
        }
        if (uniform_growth) { relative_area = 0.4 - 2.613e-3*exp(- s->time/0.15e9); 
        }
        
        double max_mass = sol_red*VOcean*0.032;

        double flux = F0 + (F1-F0)*exp(-s->time/tau);
        
        double total_flux = F0 + (F1-F0)*exp(-s->time/tau);
        
        double Cont_flux = flux*relative_area;
        
        flux = flux*(1-relative_area);
        
        double outflux = 0; //this
        double oce_flux = flux; //this
        
        if(s->world[oc]->masses[s_red] + flux*s->timestep > max_mass && s->world[oc]->masses[s_red] <= max_mass) //all this
        {
        	outflux = (s->world[oc]->masses[s_red] + flux*s->timestep - max_mass)/s->timestep;
		
		oce_flux = flux - outflux;
        }
        
        if (s->world[oc]->masses[s_red] + flux*s->timestep > max_mass && s->world[oc]->masses[s_red] > max_mass) //all this
	{
		oce_flux = 0;
		outflux = flux;
	}

        s->world[oc]->fluxes[s_red] += oce_flux; // this is a net source, so no need to balance //this
        s->world[sed]->fluxes[s_red] += outflux; //this
        s->world[co]->fluxes[s_red] += Cont_flux; //this

	cout << "CometDelivery to ocean flux:: " << oce_flux << " to sediments flux " << outflux << endl;
	
        if(DEBUG) cout << "CometDelivery::flux::" << flux << endl;

        vector<double> output = {total_flux};
        this->fluxes.push_back(output);
    }

    bool exec(string param) {
        if (param == "Init") {
            init();
            return true;
        }
        if (param == "Evolve") {
            evolve();
            return true;
        }
        return false;
    }
};
REGISTER_MODULE(CometDelivery)

class Rainout: public Module {
public:


    Rainout(string name): Module(name) { init(); }

    double rainout, satur, sol_ox, VOcean, flux_oc, flux_cc, relative_area, remaining_atm_fraction;
    int s_ox, atm, oc, sed, co; //From the Atmosphere into Oceans and CCrust

    void init(void) {
        this->links.push_back("Atmosphere1 -> Oceans1");    // Absorption by the oceans of atmospheric sulfur (ox ->ox)
        this->links.push_back("Atmosphere1 -> CCrust1");    // Absorption by the continents of atmospheric sulfur (ox ->ox)
        this->links.push_back("Atmosphere1 -> Sediments1"); // Excess sulfate in the oceans directly precipitates into sediments (ox->ox)
        this->isBidirectional = false;
        this->numOutputs = 2;
        this->init_fluxes(3);
        
        remaining_atm_fraction = config->data["Rainout"]["remaining_atm_fraction"].as<double>();
        sol_ox = config->data["Precipitation"]["sol_ox"].as<double>(); //Molar Concentration of S_ox on the ocean (mol/liter)
        VOcean = config->data["Precipitation"]["VOcean"].as<double>(); //Original Volume of the Ocean in Liters


        s_ox = s->element_map["s_ox"];

        oc = s->reservoir_map["Oceans"];
        co = s->reservoir_map["CCrust"];
        sed = s->reservoir_map["Sediments"];
        atm = s->reservoir_map["Atmosphere"];
	
    }

    void evolve(void) {
    
    	double relative_area;
        if (s->time < 1.5e9) { 
            relative_area = 0.4*(0.02 + 0.64*s->time/1.5e9);
        } else {
            relative_area = 0.4*(0.66 + 0.34*(s->time-1.5e9)/3e9);
        }
        //if (uniform_growth) { relative_area = 0.4 - 2.613e-3*exp(- s->time/0.15e9); }
	
	flux_cc = ((s->world[atm]->masses[s_ox])/s->timestep)*(1-remaining_atm_fraction)*relative_area; //goes to CC
	flux_oc = ((s->world[atm]->masses[s_ox])/s->timestep)*(1-remaining_atm_fraction)*(1-relative_area); //goes to oce
	
	double max_mass = sol_ox*VOcean*0.032;
        
        double outflux = 0; //this
        double oce_flux = flux_oc; //this
        
        if(s->world[oc]->masses[s_ox] + flux_oc*s->timestep > max_mass && s->world[oc]->masses[s_ox] <= max_mass) //all this
        {
        	outflux = (s->world[oc]->masses[s_ox] + flux_oc*s->timestep - max_mass)/s->timestep;
		
		oce_flux = flux_oc - outflux;
        }
        
        if (s->world[oc]->masses[s_ox] + flux_oc*s->timestep > max_mass && s->world[oc]->masses[s_ox] > max_mass) //all this
	{
		oce_flux = 0;
		outflux = flux_oc;
	}
	
	s->world[sed]->fluxes[s_ox] += outflux;
        s->world[oc]->fluxes[s_ox] += oce_flux;
        s->world[co]->fluxes[s_ox] += flux_cc;
        s->world[atm]->fluxes[s_ox] += - flux_oc - flux_cc;

	cout << " Rainout flux: " << oce_flux << endl;
	
	bool DEBUG = true;
	
        if(DEBUG) {
            cout << "Rainout::flux::" << (flux_cc+flux_oc)*s->timestep << endl;
            cout << "Rainout::Next_Atm_Value" << s->world[atm]->masses[s_ox] -(flux_cc+flux_oc)*s->timestep << endl;
        }

        vector<double> output = {oce_flux, outflux};
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
REGISTER_MODULE(Rainout)



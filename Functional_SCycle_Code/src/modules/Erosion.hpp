class Erosion: public Module {
public:
    Erosion(string name): Module(name) { init(); } //Assuming s_red and s_ox are eroded at different rates (alpha_ox and alpha_red)
    
    
    double alpha, epsilon_red, A0, M0, rho, sol_ox, sol_red, VOcean;
    bool uniform_growth, GOE;
    int s_red, s_ox, co, oc, sed, Ocean_function;

    void init(void) {
        this->links.push_back("CCrust0 -> Oceans0");  // (red-> red)
        this->links.push_back("CCrust0 -> Sediments0");   // (red-> red)
        this->links.push_back("CCrust0 -> Oceans1");  // (red-> ox), OWS (Oxidative Weathering of Sulfide)
        this->links.push_back("CCrust0 -> Sediments1");   // (red-> ox) (Oxidative Weathering of Sulfide)
        this->links.push_back("CCrust1 -> Oceans1");  // (ox-> ox)
        this->links.push_back("CCrust1 -> Sediments1");  // (ox-> ox)
        this->isBidirectional = false;
        this->numOutputs = 8;
        this->init_fluxes(4);
        
        s_red = s->element_map["s_red"];
        s_ox = s->element_map["s_ox"];

        alpha = config->data["Erosion"]["alpha"].as<double>();
        GOE = config->data["Erosion"]["GOE"].as<bool>();
        epsilon_red = config->data["Erosion"]["epsilon_red"].as<double>();
        Ocean_function = config->data["Erosion"]["Ocean_function"].as<double>();
        uniform_growth = config->data["Erosion"]["uniform_growth"].as<bool>();
        sol_ox = config->data["Precipitation"]["sol_ox"].as<double>(); //Molar Concentration of S_ox on the ocean (mol/liter)
        sol_red = config->data["Precipitation"]["sol_red"].as<double>(); //Molar Concentration of S_red on the ocean (mol/liter)
        VOcean = config->data["Precipitation"]["VOcean"].as<double>(); //Original Volume of the Ocean in Liters

        
        cout << "HEREEEEE" << endl;
        
        A0 = 1.48e14; // m2, today
        M0 = 2.28e22; // kg, today
        rho = 2700; // kg/m3

        co = s->reservoir_map["CCrust"];
        oc = s->reservoir_map["Oceans"];
        sed = s->reservoir_map["Sediments"];
    }

    void evolve(void) {
        double relative_area;
        if (s->time < 1.5e9) { 
            relative_area = 0.4*(0.02 + 0.64*s->time/1.5e9);
        } else {
            relative_area = 0.4*(0.66 + 0.34*(s->time-1.5e9)/3e9);
        }
        if (uniform_growth) { relative_area = 0.4 - 2.613e-3*exp(- s->time/0.15e9); }
        

	double concentration_sred = (s->world[co]->masses[s_red])/(M0); // wt% of S_Red in the continental crust
        double concentration_sox = (s->world[co]->masses[s_ox])/(M0); // wt% of S_Ox in the continental crust 
        //double concentration = 0.001; //Other option? is it better?
        double flux_red = alpha*epsilon_red*A0*rho*relative_area*(concentration_sred);   // kg of s_red eroded per year. 
        double flux_ox = alpha*A0*rho*relative_area*(concentration_sox);    // kg of s_ox eroded per year. 
        
        double flux_OWS = 0;   // kg of OWS (s_red -> s_ox) eroded per year. 
        if (GOE){
            flux_OWS = alpha*(1-epsilon_red)*A0*rho*relative_area*(concentration_sred);   // kg of OWS (s_red -> s_ox) eroded per year. 
        }
        
        double flux_SOX_and_OWS = flux_ox + flux_OWS; // kg of Sox reaching the ocean (sum of Sox->Sox and Sred->Sox)
        
	double outflux_ox = 0;
	double outflux_red = 0;
	double oce_flux_ox = flux_SOX_and_OWS; //flux dissolving in the ocean (Sox)
	double oce_flux_red = flux_red; //flux dissolving in the ocean (Sred)
	
	double max_mass_ox = sol_ox*VOcean*0.032;  //maximum carrying capacity of the ocean (Sox)
	double max_mass_red = sol_red*VOcean*0.032;  //maximum carrying capacity of the ocean (Sred)

	//Ocean flux calculations for Sox
	if (s->world[oc]->masses[s_ox] + flux_SOX_and_OWS*s->timestep > max_mass_ox && s->world[oc]->masses[s_ox] <= max_mass_ox)
	{
		outflux_ox = (s->world[oc]->masses[s_ox] + flux_SOX_and_OWS*s->timestep - max_mass_ox)/s->timestep;
		
		oce_flux_ox = flux_SOX_and_OWS - outflux_ox;
		
		//if (oce_flux < 0)
		//{
		//	oce_flux = 0;
		//	outflux = flux;
		//}
		
	}
	
	if (s->world[oc]->masses[s_ox] + flux_SOX_and_OWS*s->timestep > max_mass_ox && s->world[oc]->masses[s_ox] > max_mass_ox)
	{
		oce_flux_ox = 0;
		outflux_ox = flux_SOX_and_OWS;
	}
	
	//Ocean flux calculations for Sred
	if (s->world[oc]->masses[s_red] + flux_red*s->timestep > max_mass_red && s->world[oc]->masses[s_red] <= max_mass_red)
	{
		outflux_red = (s->world[oc]->masses[s_red] + flux_red*s->timestep - max_mass_red)/s->timestep;
		
		oce_flux_red = flux_red - outflux_red;
		
		//if (oce_flux < 0)
		//{
		//	oce_flux = 0;
		//	outflux = flux;
		//}
		
	}
	
	if (s->world[oc]->masses[s_red] + flux_red*s->timestep > max_mass_red && s->world[oc]->masses[s_red] > max_mass_red)
	{
		oce_flux_red = 0;
		outflux_red = flux_red;
	}

	cout << "Ocean mass ox" << s->world[oc]->masses[s_ox] <<"riverine flux ox: " << flux_SOX_and_OWS << " into_ocean ox " << oce_flux_ox << " out ox" << outflux_ox << endl;
	cout << "Ocean mass red" << s->world[oc]->masses[s_red] <<"riverine flux red: " << flux_red << " into_ocean red " << oce_flux_red << " out red" << outflux_red << endl;

        s->world[co]->fluxes[s_ox] += -flux_ox;
        s->world[co]->fluxes[s_red] += -(flux_red + flux_OWS);
        s->world[oc]->fluxes[s_ox] += oce_flux_ox;
        s->world[oc]->fluxes[s_red] += oce_flux_red;
        s->world[sed]->fluxes[s_ox] += outflux_ox;
        s->world[sed]->fluxes[s_red] += outflux_red;
        
        bool DEBUG = true;

        if(DEBUG) {
        
            cout << "Erosion::relative_area::" << relative_area << endl;
        
            cout << "Erosion::flux ox::" << flux_ox*s->timestep << endl;
            cout << "Erosion::flux OWS::" << flux_OWS*s->timestep << endl;
            cout << "Erosion::flux_into_ocean ox::" << oce_flux_ox*s->timestep << endl;
            cout << "Erosion::flux_into_sediments ox::" << outflux_ox*s->timestep << endl;
            cout << "Erosion::Current_Ocean_Value ox::" << s->world[oc]->masses[s_ox] << endl;
            cout << "Erosion::Next_Ocean_Value ox::" << s->world[oc]->masses[s_ox] + oce_flux_ox*s->timestep << endl;
            
            cout << "Erosion::flux red::" << flux_red*s->timestep << endl;
            cout << "Erosion::flux_into_ocean red::" << oce_flux_red*s->timestep << endl;
            cout << "Erosion::flux_into_sediments red::" << outflux_red*s->timestep << endl;
            cout << "Erosion::Current_Ocean_Value red::" << s->world[oc]->masses[s_red] << endl;
            cout << "Erosion::Next_Ocean_Value red::" << s->world[oc]->masses[s_red] + oce_flux_red*s->timestep << endl;
        }

        vector<double> output = {flux_ox, flux_OWS, oce_flux_ox, outflux_ox, VOcean, flux_red, oce_flux_red, outflux_red};
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
REGISTER_MODULE(Erosion)

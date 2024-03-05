class Volcanism: public Module {
public:
    Volcanism(string name): Module(name) { init(); }

    double F_MORB, F_hotspot, Factor, tau, S_Conc_Ratio_MORB, S_Conc_Ratio_HS, Volc_outgas;
    int s_ox, s_red, oc, atm, co, cr, um, lm;
    bool uniform_growth;
    

    void init(void) {
        this->links.push_back("LMantle0 -> CCrust0"); // Hotspot to Continent (red-> red)
        this->links.push_back("LMantle0 -> OCrust0"); // Hotspot to Oceanic Crust (red -> red)
        this->links.push_back("LMantle0 -> Atmosphere1"); // Hotspot to Atmosphere (subaereal Outgassing red-> ox)
        this->links.push_back("LMantle0 -> Oceans1"); // Hotspot to Oceans (subaquatic Outgassing, red-> ox)
        
        this->links.push_back("UMantle0 -> OCrust0"); // Mid-Ocean Ridge Volcanism to Oceanic Crust (red-> red)
        this->links.push_back("UMantle0 -> Oceans1"); // Mid-Ocean Ridge Volcanism to Oceans (subaquatic Outgassing, red->ox)
        
        this->isBidirectional = false;
        this->numOutputs = 2;
        this->init_fluxes(6);

        // these are given in km3/yr
        //F_arc = config->data["Volcanism"]["F_arc"].as<double>();
        F_MORB = config->data["Volcanism"]["F_MORB"].as<double>();
        F_hotspot = config->data["Volcanism"]["F_hotspot"].as<double>();
        
        Factor = config->data["Volcanism"]["Factor"].as<double>();
        tau = config->data["Volcanism"]["tau"].as<double>();
        S_Conc_Ratio_MORB = config->data["Volcanism"]["S_Conc_Ratio_MORB"].as<double>();
        
        S_Conc_Ratio_HS = config->data["Volcanism"]["S_Conc_Ratio_MORB"].as<double>();
        
        Volc_outgas = config->data["Volcanism"]["Volc_outgas"].as<double>();
        
        uniform_growth = config->data["Erosion"]["uniform_growth"].as<bool>();
        
        // oxidizing = 0.2 corresponds to partitioning in Jim's notes
        //oxidizing = config->data["man_ox"].as<double>();
        //density = config->data["Volcanism"]["density"].as<double>();
        //scaling = config->data["Volcanism"]["scaling"].as<double>();

        s_ox = s->element_map["s_ox"];
        s_red = s->element_map["s_red"];

        um = s->reservoir_map["UMantle"];
        lm = s->reservoir_map["LMantle"];
        cr = s->reservoir_map["OCrust"];
        co = s->reservoir_map["CCrust"];
        oc = s->reservoir_map["Oceans"];
        atm = s->reservoir_map["Atmosphere"];

    }

    void evolve(void) {
        
        double vol_factor = 1 + (Factor - 1)*exp(-s->time/tau);  //Expelled volume evolving with time (decaying as earth cools)

	double lm_volume = 5.9e11; //Volume of the Lower Mantle in km³
	double um_volume = 2.95e11; //Volume of the Upper Mantle in km³
	double lm_concentration = s->world[lm]->masses[s_red]/lm_volume;  //mass concentration of S in the Lower Mantle in kg/km³
	double um_concentration = s->world[um]->masses[s_red]/um_volume;  //mass concentration of S in the Upper Mantle in kg/km³

	//double flux_arc = vol_factor*F_arc*um_concentration*S_Conc_Ratio_ARC;        //mass flux of P in kg for Arc volcanism
	double flux_MORB = vol_factor*F_MORB*um_concentration*S_Conc_Ratio_MORB;       //mass flux of S in kg for MORB volcanism
	double flux_hotspot = vol_factor*F_hotspot*lm_concentration*S_Conc_Ratio_HS;   //mass flux of S in kg for Hotspot volcanism

	double relative_area;
        if (s->time < 1.5e9) { 
            relative_area = 0.4*(0.02 + 0.64*s->time/1.5e9);
        } else {
            relative_area = 0.4*(0.66 + 0.34*(s->time-1.5e9)/3e9);
        }
        if (uniform_growth) { relative_area = 0.4 - 2.613e-3*exp(- s->time/0.15e9);  }


        s->world[um]->fluxes[s_red] -= (flux_MORB);
        s->world[lm]->fluxes[s_red] -= flux_hotspot;
        //s->world[atm]->fluxes[p] += flux_arc;
        // s->fluxes[atm+1] += (flux_MORB + flux_hotspot); this goes directly
        // back to the oceans in practice
        //s->world[oc]->fluxes[p] += (flux_MORB + flux_hotspot);
        s->world[cr]->fluxes[s_red] += (1-Volc_outgas)*(flux_MORB + (1-relative_area)*flux_hotspot); // into Oceanic Crust
        s->world[oc]->fluxes[s_ox] += Volc_outgas*(flux_MORB + (1-relative_area)*flux_hotspot); // into Ocean (Outgas)
        
        s->world[co]->fluxes[s_red] += (1-Volc_outgas)*(relative_area*flux_hotspot);  // into Continental Crust (solid)
        s->world[atm]->fluxes[s_ox] += Volc_outgas*(relative_area*flux_hotspot);  // into Atmosphere (outgas)
        
        if(DEBUG) {
            //cout << "Volcanism::flux_arc::" << flux_arc*s->timestep << endl;
            cout << "Volcanism::flux_MORB::" << flux_MORB*s->timestep << endl;
            cout << "Volcanism::flux_hotspot::" << flux_hotspot*s->timestep << endl;
        }

        vector<double> output = {flux_MORB, flux_hotspot}; //(Volcanism0 = MORB flux, Volcanism1 = Hotspot flux)
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
REGISTER_MODULE(Volcanism)

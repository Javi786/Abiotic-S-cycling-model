class Subduction_Volcanism_Rainout: public Module {
public:
    Subduction_Volcanism_Rainout(string name): Module(name) { init(); }

    double tau, accretion, Arc_fraction, Arc_outgas, S_Enrichment_Arc, F_MORB, F_hotspot, Factor, Volcanism_tau, S_Conc_Ratio_MORB, S_Conc_Ratio_HS, Volc_outgas, rainout, satur, sol_ox, VOcean, flux_oc, flux_cc, relative_area, atm_timescale; 
    int s_ox, s_red, cr, co, oc, um, lm, sed, atm;
    bool uniform_growth;

    void init(void) {
        this->links.push_back("OCrust0 -> UMantle0"); //subducted OC (red->red)
        this->links.push_back("OCrust1 -> UMantle0"); //subducted OC (ox-> red)
        this->links.push_back("OCrust0 -> CCrust0"); //for solid eruptiong arc volcanism material (red->red)
        this->links.push_back("OCrust1 -> CCrust0"); //for solid eruptiong arc volcanism material (ox-> red)
        this->links.push_back("OCrust0 -> Atmosphere1"); //for subaereal eruptions (red->ox)
        this->links.push_back("OCrust1 -> Atmosphere1"); //for subaereal eruptions (ox-> ox)
        this->links.push_back("OCrust0 -> Oceans1"); // for subacquatic eruptions, the gaseous S dissolves into sea-water (red->ox)
        this->links.push_back("OCrust1 -> Oceans1"); // for subacquatic eruptions, the gaseous S dissolves into sea-water (ox-> ox)
        this->links.push_back("OCrust1 -> OCrust0"); // for solid eruptiong arc volcanism material (ox-> red)

        this->links.push_back("Sediments0 -> CCrust0");  //accreted material + arc volcanism (red-> red)
        this->links.push_back("Sediments1 -> CCrust0");  //arc volcanism  (ox -> red)
        this->links.push_back("Sediments1 -> CCrust1");  //accreted material (ox->ox)
        this->links.push_back("Sediments0 -> OCrust0");  //arc volcanism (into the oceanic crust); (red->red)
        this->links.push_back("Sediments1 -> OCrust0");  //arc volcanism (into the oceanic crust); (ox ->red)
        this->links.push_back("Sediments0 -> UMantle0"); //subducted Sed (red-> red)
        this->links.push_back("Sediments1 -> UMantle0"); //subducted Sed (ox -> red)
        this->links.push_back("Sediments0 -> Atmosphere1"); //for subaereal eruptions (red-> ox)
        this->links.push_back("Sediments1 -> Atmosphere1"); //for subaereal eruptions (ox -> ox)
        this->links.push_back("Sediments0 -> Oceans1"); // for subacquatic eruptions, the gaseous S dissolves into sea-water (red-> ox)
        this->links.push_back("Sediments1 -> Oceans1"); // for subacquatic eruptions, the gaseous S dissolves into sea-water (ox -> ox)
        
        //Now Volcanism Fluxes:
        
        this->links.push_back("LMantle0 -> CCrust0"); // Hotspot to Continent (red-> red)
        this->links.push_back("LMantle0 -> OCrust0"); // Hotspot to Oceanic Crust (red -> red)
        this->links.push_back("LMantle0 -> Atmosphere1"); // Hotspot to Atmosphere (subaereal Outgassing red-> ox)
        this->links.push_back("LMantle0 -> Oceans1"); // Hotspot to Oceans (subaquatic Outgassing, red-> ox)
        
        this->links.push_back("UMantle0 -> OCrust0"); // Mid-Ocean Ridge Volcanism to Oceanic Crust (red-> red)
        this->links.push_back("UMantle0 -> Oceans1"); // Mid-Ocean Ridge Volcanism to Oceans (subaquatic Outgassing, red->ox)
        
        //Now Rainout Fluxes:
        this->links.push_back("Atmosphere1 -> Oceans1");    // Absorption by the oceans of atmospheric sulfur (ox ->ox)
        this->links.push_back("Atmosphere1 -> CCrust1");    // Absorption by the continents of atmospheric sulfur (ox ->ox)
        this->links.push_back("Atmosphere1 -> Sediments1"); // Excess sulfate in the oceans directly precipitates into sediments (ox->ox)
        
        this->isBidirectional = false;
        this->numOutputs = 11;
        this->init_fluxes(29);
        
        //Subduction Config Parameters

        //VCrust = config->data["Subduction"]["VCrust"].as<double>();
        tau = config->data["Subduction_Volcanism_Rainout"]["tau"].as<double>();
        accretion = config->data["Subduction_Volcanism_Rainout"]["accretion"].as<double>();  //accretion fraction: fraction of sediments that accrete into CCrust at subduction zones
        Arc_fraction = config->data["Subduction_Volcanism_Rainout"]["Arc_fraction"].as<double>(); //fraction of subducted material erupting as arc volcanism 
        Arc_outgas = config->data["Subduction_Volcanism_Rainout"]["Arc_outgas"].as<double>(); // fraction of arc volcanism erupting as gas (into Atm)
        S_Enrichment_Arc = config->data["Subduction_Volcanism_Rainout"]["S_Enrichment_Arc"].as<double>(); // S enrichment factor from OC slab to arc volcanism
        uniform_growth = config->data["Erosion"]["uniform_growth"].as<bool>();
        
        //Volcanism Config Parameters
        
        F_MORB = config->data["Subduction_Volcanism_Rainout"]["F_MORB"].as<double>();  // Volume of MORB volcanism (km³/yr)
        F_hotspot = config->data["Subduction_Volcanism_Rainout"]["F_hotspot"].as<double>(); // Volume of Hotspot volcanism (km³/yr)
        
        Factor = config->data["Subduction_Volcanism_Rainout"]["Factor"].as<double>(); //factor of initial conditions (how many times more active the volcanism initially was)
        
        Volcanism_tau = config->data["Subduction_Volcanism_Rainout"]["Volcanic_tau"].as<double>();  //Volcanic Activity Exp Decrease characteristic timescale (yr)
        
        S_Conc_Ratio_MORB = config->data["Subduction_Volcanism_Rainout"]["S_Conc_Ratio_MORB"].as<double>(); //Present-day ratio of S content in MORB Lavas vs Upper Mantle
        
        S_Conc_Ratio_HS = config->data["Subduction_Volcanism_Rainout"]["S_Conc_Ratio_MORB"].as<double>(); //Present-day ratio of S content in Hotspot Volcanism Lavas vs Lower Mantle
        
        Volc_outgas = config->data["Subduction_Volcanism_Rainout"]["Volc_outgas"].as<double>(); //mass fraction of the MORB and Hotspot Volcanism S that ends up being outgassed as SO2 into the atmosphere
        
        //Rainout Config Parameters
        
        atm_timescale = config->data["Subduction_Volcanism_Rainout"]["atm_timescale"].as<double>();  //atmospheric mean residence time of sulfur (in years!)
        sol_ox = config->data["Precipitation"]["sol_ox"].as<double>(); //Molar Concentration of S_ox on the ocean (mol/liter)
        VOcean = config->data["Precipitation"]["VOcean"].as<double>(); //Original Volume of the Ocean in Liters
        
        
        s_ox = s->element_map["s_ox"];
        s_red = s->element_map["s_red"];

        cr = s->reservoir_map["OCrust"];
        co = s->reservoir_map["CCrust"];
        um = s->reservoir_map["UMantle"];
        lm = s->reservoir_map["UMantle"];
        sed = s->reservoir_map["Sediments"];
        atm = s->reservoir_map["Atmosphere"];
        oc = s->reservoir_map["Oceans"];
    }

    void evolve(void) {
    	
    	double relative_area;
        if (s->time < 1.5e9) { 
            relative_area = 0.4*(0.02 + 0.64*s->time/1.5e9);
        } else {
            relative_area = 0.4*(0.66 + 0.34*(s->time-1.5e9)/3e9);
        }
        //if (uniform_growth) { relative_area = 0.4 - 2.613e-3*exp(- s->time/0.15e9);
        
        // --------------------------------- VOLCANISM PROCESSES ---------------------------------
        
        double vol_factor = 1 + (Factor - 1)*exp(-s->time/Volcanism_tau);  //Expelled volume evolving with time (decaying as earth cools)

	double lm_volume = 5.9e11; //Volume of the Lower Mantle in km³
	double um_volume = 2.95e11; //Volume of the Upper Mantle in km³
	double lm_concentration = s->world[lm]->masses[s_red]/lm_volume;  //mass concentration of S in the Lower Mantle in kg/km³
	double um_concentration = s->world[um]->masses[s_red]/um_volume;  //mass concentration of S in the Upper Mantle in kg/km³

	//double flux_arc = vol_factor*F_arc*um_concentration*S_Conc_Ratio_ARC;        //mass flux of P in kg for Arc volcanism
	double flux_MORB = vol_factor*F_MORB*um_concentration*S_Conc_Ratio_MORB;       //mass flux of S in kg for MORB volcanism
	double flux_hotspot = vol_factor*F_hotspot*lm_concentration*S_Conc_Ratio_HS;   //mass flux of S in kg for Hotspot volcanism

        s->world[um]->fluxes[s_red] -= (flux_MORB);
        s->world[lm]->fluxes[s_red] -= flux_hotspot;

        s->world[cr]->fluxes[s_red] += (1-Volc_outgas)*(flux_MORB + (1-relative_area)*flux_hotspot); // into Oceanic Crust
        s->world[oc]->fluxes[s_ox] += Volc_outgas*(flux_MORB + (1-relative_area)*flux_hotspot); // into Ocean (Outgas)
        
        s->world[co]->fluxes[s_red] += (1-Volc_outgas)*(relative_area*flux_hotspot);  // into Continental Crust (solid)
        
        double volcanic_outgas_sox = Volc_outgas*(relative_area*flux_hotspot);  // into Atmosphere (outgas, volcanic)
        
        // --------------------------------- SUBDUCTION PROCESSES ---------------------------------
        
    
        double flux_cr_ox = s->world[cr]->masses[s_ox]/tau;      //Oceanic Crust, ox, Material entering this process
        double flux_cr_red = s->world[cr]->masses[s_red]/tau;    //Oceanic Crust, red, Material entering this process
        double flux_sed_ox = s->world[sed]->masses[s_ox]/tau;    //MS ox, Material entering this process
        double flux_sed_red = s->world[sed]->masses[s_red]/tau;  //MS red, Material entering this process
        
        s->world[sed]->fluxes[s_ox] += -flux_sed_ox; //Marine Sedimentary (ox) material entering the subduction processes (removal)
        s->world[sed]->fluxes[s_red] += -flux_sed_red; //Marine Sedimentary (red) material entering the subduction processes (removal)
        
        s->world[um]->fluxes[s_red] += (1-Arc_fraction*S_Enrichment_Arc)*(flux_cr_ox+flux_cr_red) + (1-Arc_fraction)*(1-accretion)*(flux_sed_ox + flux_sed_red); //Out of the material entering subduction processes only a fraction undergoes actual subduction into the upper mantle. This corresponds to the OC material which did not get uplifted as Arc Volcanism and and the Sedimentary material fraction that got neither accreted nor expelled upwards as arc volcanism
        
        
        double Arc_Volcanism = (Arc_fraction*S_Enrichment_Arc)*(flux_cr_ox+flux_cr_red) + Arc_fraction*(1-accretion)*(flux_sed_ox + flux_sed_red); //Material expelled as arc volcanism is constituted by previous Oceanic Crust material that did not subduct and by Sedimentary material that neither got accreted nor got subducted
        
        
        s->world[co]->fluxes[s_red] += relative_area*((1-Arc_outgas)*Arc_Volcanism) + accretion*flux_sed_red; //solid Arc Volcanism into the Continental Crust + Accreted Material into the continental crust (all reduced)
        
        s->world[co]->fluxes[s_ox] += accretion*flux_sed_ox; //Oxidised Accreted Material into the continental crust
        
        double Arc_outgas_sox = relative_area*Arc_outgas*Arc_Volcanism; //Outgassing Arc Volcanism into Atmosphere (subaereal volcanism, all oxidised)
        
        s->world[oc]->fluxes[s_ox] += (1-relative_area)*Arc_outgas*Arc_Volcanism; //Arc Volcanism Outgassing into the Ocean (subaquatic volcanism)
        
        s->world[cr]->fluxes[s_red] += -flux_cr_red + (1-relative_area)*((1-Arc_outgas)*Arc_Volcanism);  //Oceanic Crust material entering the subduction processes (removal, reduced) + solid Arc Volcanism into the Oceanic Crust (reduced)
        
        s->world[cr]->fluxes[s_ox] += -flux_cr_ox; //Oceanic Crust material entering the subduction processes (removal, oxidised)
        
        
        // --------------------------------- RAINOUT PROCESSES ---------------------------------
        
        
        double Atmospheric_input_Mass = volcanic_outgas_sox + Arc_outgas_sox ; //Yearly mass of S injected into the atmosphere
        
        double Atmospheric_Sox_Mass = s->world[atm]->masses[s_ox]; //Previous' timescale final Atm S mass
        
        int number_of_timescales = int(s->timestep/atm_timescale); //Number of ATM residence times of S pass per timescale
        
        double Rainout_Mass = 0; //Will collect the rainout-ed material removed from the atmosphere
        
        //Running a cycle for the number_of_timescales in which a an average amount of S is injected in the ATM - and half is removed
        
        
        for(int t=0; t < number_of_timescales; t++){
        	
        	Atmospheric_Sox_Mass = (Atmospheric_Sox_Mass + Atmospheric_input_Mass*atm_timescale)/2;
        	
        	Rainout_Mass += Atmospheric_Sox_Mass; 
        	
        }
        
        flux_cc = Rainout_Mass*relative_area/s->timestep; //ATM S that goes to CC, per year
	flux_oc = Rainout_Mass*(1-relative_area)/s->timestep; //ATM S that goes to oce, per year
	
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
	s->world[atm]->fluxes[s_ox] += (Atmospheric_Sox_Mass - s->world[atm]->masses[s_ox])/s->timestep;
	//s->world[atm]->masses[s_ox] = Atmospheric_Sox_Mass;
	
	cout << "Atmospheric_Sox_Mass: "  << Atmospheric_Sox_Mass << endl;
	
        bool DEBUG = true;
        if(DEBUG) cout << "Subduction::flux::" << ((1-Arc_fraction)*(flux_cr_ox+flux_cr_red) + (1-Arc_fraction)*(1-accretion)*(flux_sed_ox + flux_sed_red))*s->timestep << endl;

        //vector<double> output = {(1-accretion)*flux, accretion*flux};
        vector<double> output = {-flux_cr_red, -flux_cr_ox, -flux_sed_red, -flux_sed_ox, accretion*flux_sed_red, accretion*flux_sed_ox, Arc_Volcanism*(1-Arc_outgas), Arc_outgas*Arc_Volcanism, flux_MORB, flux_hotspot, flux_cc, oce_flux, outflux, flux_cc+flux_oc}; //legend TBC: subduction0 = Sred Flux out of OC; subduction1 = SOx Flux out of OC; subduction2 =  Sred Flux out of Sed; subduction3 =  Sox Flux out of Sed; subduction4 = Sred Accretion flux into CC; subduction5 = Sox Accretion flux into CC; subduction6 = Sred Arc Volcanism into CC; subduction7 = Sox Arc Volcanism into ATM, subduction8 = Sred MORB flux; subduction9 = Sred+Sox Hotspot flux, subduction10 = Sox ATM flux into CC, subduction11 = Sox ATM flux into oceans, subduction12 = Sox ATM flux into Sed, subduction13 = Sox Total Flux out of the Atmosphere;
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
REGISTER_MODULE(Subduction_Volcanism_Rainout)

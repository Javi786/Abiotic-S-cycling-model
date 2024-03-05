class Subduction: public Module {
public:
    Subduction(string name): Module(name) { init(); }

    double tau, accretion, Arc_fraction, Arc_outgas, S_Enrichment_Arc; // VCrust
    int s_ox, s_red, cr, co, oc, um, sed, atm;
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
        this->isBidirectional = false;
        this->numOutputs = 8;
        this->init_fluxes(20);

        //VCrust = config->data["Subduction"]["VCrust"].as<double>();
        tau = config->data["Subduction"]["tau"].as<double>();
        accretion = config->data["Subduction"]["accretion"].as<double>();  //accretion fraction: fraction of sediments that accrete into CCrust at subduction zones
        Arc_fraction = config->data["Subduction"]["Arc_fraction"].as<double>(); //fraction of subducted material erupting as arc volcanism 
        Arc_outgas = config->data["Subduction"]["Arc_outgas"].as<double>(); // fraction of arc volcanism erupting as gas (into Atm)
        S_Enrichment_Arc = config->data["Subduction"]["S_Enrichment_Arc"].as<double>(); // S enrichment factor from OC slab to arc volcanism
        uniform_growth = config->data["Erosion"]["uniform_growth"].as<bool>();
        
        s_ox = s->element_map["s_ox"];
        s_red = s->element_map["s_red"];

        cr = s->reservoir_map["OCrust"];
        co = s->reservoir_map["CCrust"];
        um = s->reservoir_map["UMantle"];
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
        if (uniform_growth) { relative_area = 0.4 - 2.613e-3*exp(- s->time/0.15e9);  }
    
    
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
        
        s->world[atm]->fluxes[s_ox] += relative_area*Arc_outgas*Arc_Volcanism; //Outgassing Arc Volcanism into Atmosphere (subaereal volcanism, all oxidised)
        
        s->world[oc]->fluxes[s_ox] += (1-relative_area)*Arc_outgas*Arc_Volcanism; //Arc Volcanism Outgassing into the Ocean (subaquatic volcanism)
        
        s->world[cr]->fluxes[s_red] += -flux_cr_red + (1-relative_area)*((1-Arc_outgas)*Arc_Volcanism);  //Oceanic Crust material entering the subduction processes (removal, reduced) + solid Arc Volcanism into the Oceanic Crust (reduced)
        
        s->world[cr]->fluxes[s_ox] += -flux_cr_ox; //Oceanic Crust material entering the subduction processes (removal, oxidised)
        
        bool DEBUG = true;
        if(DEBUG) cout << "Subduction::flux::" << ((1-Arc_fraction)*(flux_cr_ox+flux_cr_red) + (1-Arc_fraction)*(1-accretion)*(flux_sed_ox + flux_sed_red))*s->timestep << endl;

        //vector<double> output = {(1-accretion)*flux, accretion*flux};
        vector<double> output = {-flux_cr_red, -flux_cr_ox, -flux_sed_red, -flux_sed_ox, accretion*flux_sed_red, accretion*flux_sed_ox, Arc_Volcanism*(1-Arc_outgas), Arc_outgas*Arc_Volcanism}; //legend TBC: subduction0 = Flux out of OC; subduction1 = Flux out of Sed; subduction2 = Accretion flux into CC; subduction3 = Arc Volcanism into CC; subduction4 = Arc Volcanism into ATM;
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
REGISTER_MODULE(Subduction)

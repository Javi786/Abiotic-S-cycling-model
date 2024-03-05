class Precipitation: public Module {
public:

    // Precipitation of S is based on the concentration
    // of S in the ocean and when it is higher than the 
    // concentration of max saturation than the amount
    // of S that is over the maximum saturation will
    // precipitate out of the ocean and accumulate as
    // sediments on the Sediments' reservoir. For both Sox and Sred

    Precipitation(string name): Module(name) { init(); }

    double alpha, F0, precip_ox, precip_red, sol_ox, sol_red, baryte_frac, flux_ox, flux_red, VOcean;
    int s_ox, s_red, sed, oc, Ocean_function;

    void init(void) {
        this->links.push_back("Oceans0 -> Sediments0");  //(red->red)
        this->links.push_back("Oceans1 -> Sediments1");  //(ox -> ox)
        this->isBidirectional = false;
        this->numOutputs = 2;
        this->init_fluxes(2);

        sol_ox = config->data["Precipitation"]["sol_ox"].as<double>(); //Molar Concentration of S_ox on the ocean (mol/liter)
        sol_red = config->data["Precipitation"]["sol_red"].as<double>(); //Molar Concentration of S_red on the ocean (mol/liter)
        baryte_frac = config->data["Precipitation"]["baryte_frac"].as<double>();
        Ocean_function = config->data["Erosion"]["Ocean_function"].as<double>();
        VOcean = config->data["Precipitation"]["VOcean"].as<double>(); //Volume of the Ocean in Liters

        s_ox = s->element_map["s_ox"];
        s_red = s->element_map["s_red"];

        oc = s->reservoir_map["Oceans"];
        sed = s->reservoir_map["Sediments"];
    }

    void evolve(void) {

        // concentration of P in the ocean for comparison to the 
        // maximum concentration of P. If the value is greater
        // that take the amount of P over the max saturation 
        // that will precipitate out.

        double satur_mass_ox = sol_ox*VOcean*0.032; //Maximum mass of S_ox allowed on the oceans (saturation mass in kg)
        double satur_mass_red = sol_red*VOcean*0.032; //Maximum mass of S_red allowed on the oceans (saturation mass in kg)
        precip_ox = 0;
        precip_red = 0;

        if (s->world[oc]->masses[s_ox] > satur_mass_ox)
        {
            precip_ox = s->world[oc]->masses[s_ox] - satur_mass_ox; 
            // Precip will be a negative value representing 
            // the amount of solute over the maximum concentration
            // that is precipitated out of the oceans. This will be
            // added to the flux as it more volitile than the flux
        }
        
        if (s->world[oc]->masses[s_red] > satur_mass_red)
        {
            precip_red = s->world[oc]->masses[s_red] - satur_mass_red; 
            // Precip will be a negative value representing 
            // the amount of solute over the maximum concentration
            // that is precipitated out of the oceans. This will be
            // added to the flux as it more volitile than the flux
        }
        
        //Including barite (Ba sulfate) precipitation, due to its very low solubility.
        //Precipitates 1% of sulfate in the ocean over the timestep;
        
        double baryte_precip = s->world[oc]->masses[s_ox]*baryte_frac;
        
        double baryte_flux = baryte_precip/ s->timestep;
        
        flux_ox = precip_ox/s->timestep;
        flux_red = precip_red/s->timestep;

        // add the flux to the world resevoirs within the flux
        // attribute.
        s->world[oc]->fluxes[s_ox] += -flux_ox - baryte_flux;
        s->world[sed]->fluxes[s_ox] += flux_ox + baryte_flux;
        
        s->world[oc]->fluxes[s_red] += -flux_red;
        s->world[sed]->fluxes[s_red] += flux_red;

        bool DEBUG = true;
        if(DEBUG) {
            cout << "Precipitation::saturation mass ox::" << satur_mass_ox << endl;
            cout << "Precipitation::saturation mass red::" << satur_mass_red << endl;
            cout << "Precipitation::precip::" << precip_ox + baryte_precip << endl;
            cout << "Precipitation::flux ox::" << flux_ox + baryte_flux << endl;
            cout << "Precipitation::flux red::" << flux_red << endl;
            cout << "Precipitation::Current ocean value ox:: " << s->world[oc]->masses[s_ox] << endl;
            cout << "Precipitation::Current ocean value red:: " << s->world[oc]->masses[s_red] << endl;
            cout << "Precipitation::Next ocean value:: " << s->world[oc]->masses[s_ox] - flux_ox*s->timestep - baryte_precip << endl;
            cout << "Precipitation::Next ocean value:: " << s->world[oc]->masses[s_red] - flux_red*s->timestep << endl;
        }

        vector<double> output = {flux_ox, flux_red};
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
REGISTER_MODULE(Precipitation)


class Convection: public Module {
public:
    Convection(string name): Module(name) { init(); }

    double tau, F0, F1, density;
    double vol_umantle;
    double vol_lmantle;
    int s_red, um, lm;

    void init(void) {
        this->links.push_back("UMantle0 -> LMantle0");
        this->isBidirectional = true;
        this->numOutputs = 1;
        this->init_fluxes(1);

        F0 = config->data["Convection"]["F0"].as<double>();
        F1 = 3*F0;
        tau = config->data["Convection"]["tau"].as<double>();
        density = config->data["Subduction_Volcanism_Rainout"]["density"].as<double>();

        vol_umantle = 2.95e20; // m3
        vol_lmantle = 5.9e20; // m3

        s_red = s->element_map["s_red"];

        um = s->reservoir_map["UMantle"];
        lm = s->reservoir_map["LMantle"];
    }

    void evolve(void) {
        // convection should homogeneize both reservoirs
        double vol_fraction = F0 + (F1-F0)*exp(-s->time/tau);
        double c_umantle = s->world[um]->masses[s_red]/9.6e17/1e6; // wt.%
        double c_lmantle = s->world[lm]->masses[s_red]/2.5e18/1e6; // wt.%

        double flux = density*vol_lmantle*vol_fraction*(c_umantle-c_lmantle)*0.5;

        s->world[um]->fluxes[s_red] += -flux;
        s->world[lm]->fluxes[s_red] += flux;

        if(DEBUG) cout << "Convection::flux::" << flux << endl;

        vector<double> output = {flux};
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
REGISTER_MODULE(Convection)

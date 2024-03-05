class HydrothermalCirculation: public Module {
public:
    HydrothermalCirculation(string name): Module(name) { init(); }

    double tau, F_s, F0, F1;
    int s_ox, cr, oc;

    void init(void) {
        this->links.push_back("Oceans1 -> OCrust1"); //(ox -> ox)
        this->isBidirectional = false;
        this->numOutputs = 1;
        this->init_fluxes(1);

        F0 = config->data["HydrothermalCirculation"]["F0"].as<double>();
        F1 = 3*F0;
        //F_NOx = config->data["HydrothermalCirculation"]["F_NOx"].as<double>();
        //F_N2 = config->data["HydrothermalCirculation"]["F_N2"].as<double>();
        F_s = config->data["HydrothermalCirculation"]["F_s"].as<double>();
        tau = config->data["HydrothermalCirculation"]["tau"].as<double>();

        //n2 = s->element_map["n2"];
        //nox = s->element_map["nox"];
        //nhx = s->element_map["nhx"];
        s_ox = s->element_map["s_ox"];

        cr = s->reservoir_map["OCrust"];
        oc = s->reservoir_map["Oceans"];
    }

    void evolve(void) {
        double vol_fraction = F0 + (F1-F0)*exp(-s->time/tau);
        //double f_NOx = s->world[oc]->masses[nox]*vol_fraction*F_NOx;
        //double f_N2 = s->world[oc]->masses[n2]*vol_fraction*F_N2;
        double f_s = s->world[oc]->masses[s_ox]*vol_fraction*F_s;

        //s->world[oc]->fluxes[n2] += -f_N2;
        //s->world[oc]->fluxes[nox] += -f_NOx;
        //s->world[oc]->fluxes[nhx] += (f_N2+f_NOx);
        s->world[oc]->fluxes[s_ox] += -f_s;
        s->world[cr]->fluxes[s_ox] += f_s;

	//cout << "HydrothermalCirculation::f_p::" << -f_p << endl;


	bool DEBUG = true;	
        if(DEBUG) {
            //cout << "HydrothermalCirculation::f_NOx::" << f_NOx << endl;
            //cout << "HydrothermalCirculation::f_N2::" << f_N2 << endl;
            cout << "HydrothermalCirculation::f_s::" << -f_s*s->timestep << endl;
            cout << "HydrothermalCirculation::Current Ocean Mass::" << s->world[oc]->masses[s_ox] << endl;
            cout << "HydrothermalCirculation::Next Ocean Mass::" << s->world[oc]->masses[s_ox] - f_s*s->timestep  << endl;
        }

        vector<double> output = {f_s};
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
REGISTER_MODULE(HydrothermalCirculation)

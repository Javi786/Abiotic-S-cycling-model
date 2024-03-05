void Simulation::run(void) {

    double lastout = 0;
    double tstep = this->timestep; ////1000;
    int lastscreen = 0;
    cout << "Tstep::" << tstep << endl;
    while(this->time < this->tmax) {
        // fetch deltas
        for (int j=0; j<this->mchain.size(); j++) { this->mchain[j]->exec("Evolve"); }

	cout << this->time << endl;

        // apply deltas
        double curr_mass = 0;
        for (int i=0; i<this->world.size(); i++) {
            for (int j=0; j<this->world[i]->masses.size(); j++) {
            	 cout << "---->>>> before "<< "i " << i << "; j " << j << " -> " << this->world[i]->masses[j] << endl;
                this->world[i]->masses[j] += tstep*this->world[i]->fluxes[j]; //tstep*this->world[i]->fluxes[j];
                cout << "---->>>> fluxes "<< "i " << i << "; j " << j << " -> " << this->world[i]->fluxes[j]*tstep << endl;
                this->world[i]->fluxes[j] = 0; // reset to 0 for next timestep
                
                cout << "->>>>>> after "<< "i " << i << "; j " << j << " -> " << this->world[i]->masses[j] << endl;
                cout << "HERE" << endl;
                assert(this->world[i]->masses[j] >= -1000); //ATTENTION! Allowing ocasional very small negative reservoir values to avoid errors due to digital calculations (e.g. subtracting very large, very close numbers)
                curr_mass += this->world[i]->masses[j];
            }
        }
       // assert(abs(curr_mass-this->m0)/this->m0 < 1e-4);

        this->current_iter++;
        this->time += this->timestep;

        int ctime = floor(this->time/1e6);
        if (ctime % 100 == 0 and ctime != lastscreen) {
            this->to_screen();
            lastscreen = ctime;
        }

        if (this->time - lastout >= this->output) {
            this->to_file();
            lastout = this->time;
        }

        tstep = min(2*tstep, this->timestep);
    }

    return; 
}


bool Simulation::init(string suffix) {

    fstream file;
    this->output_file = config->data["OutFolder"].as<string>() + "/out_" + suffix + ".txt";
    this->output_file_graph = config->data["OutFolder"].as<string>() + "/out_" + suffix + ".dot";
    file.open(this->output_file.c_str(), fstream::out);
    if (file.fail()) {
        cout << "Couldn't create output file. Your output folder likely doesn't exist." << endl;
        cout << "Check 'OutFolder' value in your config file." << endl;
        return false;
    }
    file.close();

    // saves a copy of the config file in the output folder
    if (not fs::exists(config->data["OutFolder"].as<string>() + "/config_" + suffix + ".yaml")) {
        fs::copy(config->filename,
                 config->data["OutFolder"].as<string>() + "/" + fs::path(config->filename).filename().string(),
                 fs::copy_options::overwrite_existing);
    }

    DEBUG = config->data["Debug"].as<bool>();

    this->current_iter = 0;
    this->timestep = config->data["Timestep"].as<double>();
    this->output = config->data["Output"].as<double>();
    this->tmax = config->data["Tmax"].as<double>();
    this->time = 0;
    this->m0 = 0;

    // species
    YAML::Node species = config->data["Species"];
    for(int i=0; i<species.size(); i++){
        this->element_map.insert( pair<string,int>(species[i].as<string>(), i) );
        cout << "Inserted element: " << i << species[i] << endl;
        cout << "Element map size: " << element_map.size() << endl;
    }

    // reservoirs
    YAML::Node reservoirs = config->data["Reservoirs"];
    this->num_reservoirs = reservoirs.size();

    cout << "Number of reservoirs considered: " << this->num_reservoirs << endl;
    cout << "Number of elementss considered: " << this->element_map.size() << endl;

    int i=0;
    for(YAML::const_iterator it=reservoirs.begin(); it != reservoirs.end(); ++it) {
        string name = it->first.as<string>();
        this->world.push_back( new Reservoir(name) );
        s->reservoir_map.insert( pair<string,int>(name, i) );
        i++;
        cout << name << endl;
    }

    // processes
    YAML::Node processes = config->data["Processes"];

    if(DEBUG) cout << "Number of processes considered: " << processes.size() << endl;

    for(int i=0;i<processes.size();i++) {
        this->mchain.push_back( factory.forge(processes[i].as<string>()) );
    }

    cout << "=========================" << endl;
    this->generate_graph();
    this->to_screen();
    this->file_header();
    this->to_file();

    return true;
}


void Simulation::to_screen(void) {
    printf("TS : %8d, Time: %4g, ", current_iter, time/1e6);

    double m, tot_m;
    for (int i=0; i<this->world.size(); i++) {
        m = 0;
        cout << this->world[i]->name.substr(0,2) << ": ";
        for (int j=0; j<this->world[i]->masses.size(); j++){
            m += this->world[i]->masses[j];
            cout << "j: " << j << " masses_size: " << this->world[i]->masses.size() << endl;
            cout << " Element map:" << this->element_map.size() << endl;
            printf("%7.5e ", m);
            tot_m += m;
            }
    }
    cout << "TOT: " << tot_m << endl;
}


void Simulation::to_file(void) {
    fstream file;
    file.open(this->output_file.c_str(), fstream::app);
    file << current_iter << ",";
    file << time/1e6;
    for (int i=0; i<this->world.size(); i++)
        for (int j=0; j<this->world[i]->masses.size(); j++)
            file << "," << setprecision(8) << this->world[i]->masses[j];

    for (int j=0; j<this->mchain.size(); j++)
        for (int k=0; k<this->mchain[j]->numOutputs; k++)
            file << "," << this->mchain[j]->fluxes.back()[k];

    file << endl;
    file.close();
}


void Simulation::file_header(void) {
    fstream file;
    file.open(this->output_file.c_str(), fstream::app);
    file << "iter,time";

    for (int i=0; i<world.size(); i++)
        for (int j=0; j<this->world[i]->masses.size(); j++)
            file << "," << world[i]->name << j;

    for (int j=0; j<this->mchain.size(); j++) {
        for (int k=0; k<this->mchain[j]->numOutputs; k++) {
            file << "," << this->mchain[j]->name << k;
        }
    }

    file << endl;
    file.close();
}


void Simulation::generate_graph(void) {
    fstream file;
    vector<string> reservoirs;

    map<string, string> elements;
    elements["0"] = "N2";
    elements["1"] = "NOx";
    elements["2"] = "NHx";

    map<string, string> colors;
    colors["Atmosphere"] = "/pastel16/1";
    colors["Oceans"] = "/pastel16/2";
    colors["OCrust"] = "/pastel16/3";
    colors["CCrust"] = "/pastel16/4";
    colors["UMantle"] = "/pastel16/5";
    colors["LMantle"] = "/pastel16/6";

    file.open(this->output_file_graph.c_str(), fstream::out);
    file << "digraph { ranksep=1.2 nodesep=1.0" << endl;

    for (int j=0; j<this->mchain.size(); j++) {
        for (int k=0; k<this->mchain[j]->links.size(); k++) { // over links
            file << "\t" << this->mchain[j]->links[k];
            file << "[label=" << this->mchain[j]->name.substr(0, 3);
            if (this->mchain[j]->isBidirectional) file << ", dir=\"both\"";
            file << "]" << endl;

            // add reservoirs to reservoir list to plot later
            size_t pos = 0;
            string s = this->mchain[j]->links[k];
            string delimiter = "->";
            string token;
            while ((pos = s.find(delimiter)) != string::npos) {
                token = s.substr(0, pos-1);
                if (find(reservoirs.begin(), reservoirs.end(), token) == reservoirs.end()) {
                    reservoirs.push_back(token);
                }
                s.erase(0, pos + delimiter.length());
            }
            token = s.substr(1, s.size());
            if (find(reservoirs.begin(), reservoirs.end(), token) == reservoirs.end())
                reservoirs.push_back(token);
        }
    }

    for (int i=0; i<reservoirs.size(); i++) { // loop over reservoirs
        file << reservoirs[i];
        file << "[shape=\"octagon\", style=\"filled\", ";
        file << "label=\"" << reservoirs[i].substr(0, reservoirs[i].size()-1) << "\\n";
        file << elements[reservoirs[i].substr(reservoirs[i].size()-1, reservoirs[i].size())] << "\", ";
        file << "fillcolor=\"" << colors[reservoirs[i].substr(0, reservoirs[i].size()-1)] << "\"]" << endl;
    }

    file << "}" << endl;
}

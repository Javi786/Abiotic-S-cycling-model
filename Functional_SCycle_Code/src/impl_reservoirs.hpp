class Reservoir {
public:
    vector<double> masses;
    vector<double> fluxes;

    void init(void);

    string name;

    Reservoir(string name) {
        this->name = name;
        this->init();
    }
};


void Reservoir::init(void) {
    cout << "Loading reservoir '" << name << "' ..." << endl;

    YAML::Node values = config->data["Reservoirs"][this->name]["InitMasses"];
    this->masses.assign(s->element_map.size(), 0);
    this->fluxes.assign(s->element_map.size(), 0);

    double v;
    int idx;
    for(YAML::const_iterator it=values.begin();it!=values.end();++it) {
        idx = s->element_map[it->first.as<string>()];
        this->masses[idx] = it->second.as<double>();
        s->m0 += it->second.as<double>();
        cout << " idx: " << idx << endl;
        cout << " masses[idx]: " << this->masses[idx] << endl;
        //cout << " masses: " << masses << endl;
        cout << " s->m0 " << s->m0  << endl;
        cout << "masses.size: " << masses.size() << endl; 
    }

    return;
}

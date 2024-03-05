class Module {
public:
    string name;
    vector< vector<double> > fluxes;
    vector<string> links;
    int numOutputs;
    bool isBidirectional;

    Module(string name) {
        this->name = name;
        this->isBidirectional = false;
    }

    void init_fluxes(int num) {
        vector<double> init;
        init.assign(num, 0);
        this->fluxes.push_back(init);
    }

    virtual bool exec(string param) = 0;
};

class basic_factory {
public:
    virtual Module* forge() = 0;
};

class ModuleFactory {
    map<string, basic_factory*> factories;

public:
    void register_module(string name, basic_factory* factory) {
        factories[name] = factory;
    }

    Module* forge(string name) {
        if (factories.find(name) == factories.end()) {
            cout << "Module '" << name << "' was never registered! Exiting..." << endl;
            exit(1);
        }
        cout << "Loading module '" << name << "' ..." << endl;
        return factories[name]->forge();
    }
};



ModuleFactory factory;


#define REGISTER_MODULE(name) \
class name##_factory : public basic_factory { \
public: \
    name##_factory() { \
        factory.register_module(#name, this);\
    }; \
    Module* forge() { \
        return new name(#name); \
    } \
};\
name##_factory _##name##_factory;

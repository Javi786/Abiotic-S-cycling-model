#include "main.hpp"

using namespace std;

int main(int argc, char** argv) {

    string fname = "config_default.yaml";
    string suffix = "default";

    if (argc == 2) {
        // choose config file to use with an argument of the format
        // config_blah.yaml; the output files will then have the "blah" suffix.
        // e.g., ./evolve config_test.yaml
        fname = argv[1];

        int begin = string(argv[1]).find("config_") + 7;
        int end = string(argv[1]).find(".yaml", 4);
        if (begin < 0 or end < 0) {
            cout << "Config file name doesn't appear to follow the right format." << endl;
            cout << "Format should be `config_blah.yaml'." << endl;
            return -1;
        }
        suffix = string(argv[1]).substr(begin, end-begin);
    }

    if (config->Load(fname)) 
        if (s->init(suffix))
            s->run();
    
    return 0;
}

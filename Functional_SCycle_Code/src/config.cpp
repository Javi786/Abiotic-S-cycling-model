#include <iostream>
#include "config.hpp"

bool Config::Load(string fname) {
    this->filename = fname;
    try {
        this->data = YAML::LoadFile(fname);
    } catch (YAML::BadFile e) {
        cout << "Config file doesn't exist! Please check and try again. (" << fname << ")" << endl;
        return false;
    }
    cout << "Loading config file " << this->filename << endl;
    return true;
}

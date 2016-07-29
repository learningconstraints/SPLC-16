#ifndef CONSTRAINT_SIMPLIFIER_H
#define CONSTRAINT_SIMPLIFIER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

class constraint_simplifier
{
    public:
        constraint_simplifier();
        virtual ~constraint_simplifier();
        void simplify();
        void set_file(const string fname);
    protected:
    private:
        string filename;
        fstream f_in_out;
        stringstream ss;
        stringstream ss_res;
        vector<pair<string,string> > vec_feat_val;

        void read_file();
        void simplify_cst();
        void write_file();

};

#endif // CONSTRAINT_SIMPLIFIER_H

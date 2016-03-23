#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <stdlib.h>
#include <list>
#include <map>

#include <constraint_simplifier.h>

using namespace std;

string parse_input_file(ifstream& ifs);
void write_constraints(string constraints, ofstream& ofs);
void parse_input_file(ifstream& ifs, ofstream& ofs, list<pair<string, string> >& stack, map<string,string>& lut);


int main(int argc, char** argv)
{
    cout << "Hello world!" << endl;

    ofstream output_file;
    ifstream input_file;
    list<pair<string, string> > stac;
    map<string,string> lut;

    switch(argc)
    {
        case 2 : input_file.open(argv[1]); output_file.open("constraints.txt"); break;
        case 3 : cout << argv[1]; input_file.open(argv[1]); output_file.open(argv[2]); break;
        default : input_file.open("output2.txt"); output_file.open("constraints.txt"); break;
    }

    if(!input_file.is_open() || !output_file.is_open())
    {
        cout << "Could not open one of the files" << endl;
        cerr << "ERROR while opening files" << endl;
        return EXIT_FAILURE;
    }

    /*string s = parse_input_file(input_file);

    cout << "constraints : " << endl << s;

    write_constraints(s, output_file);*/

    parse_input_file(input_file,output_file, stac, lut);

    output_file.close();
    input_file.close();

    constraint_simplifier cst_simpl;

    if(argc == 3)
    {
        cst_simpl.set_file(argv[2]);
    }

    cst_simpl.simplify();

    return EXIT_SUCCESS;
}


/*string parse_input_file(ifstream& ifs)
{
    stringstream constraints;
    string cus;
    string line;
    string l;
    getline(ifs,line); // first line can be discarded
    while(!ifs.eof())
    {
        if(line.find('N') == 0)//not interesting
        {
            size_t pos = line.find_first_of("\"");
            size_t stop = line.find_last_of("\"");

            l = line.substr(pos+1, stop-(pos+1) );
            cout << l << endl;
            cout << pos << endl;
            cout << stop << endl;
            if(line.find("shape=box") != string::npos) //it's a leaf
            {
                if(l.substr(0,1)=="1")
                {
                    constraints << "!(" + cus + ")" << endl;
                    cus = "";
                }
            }
            else if(l.substr(0,1)=="<"  || l.substr(0,1)==">")
            {
                cus += l;
            }
            else
            {
                if(cus == "")
                {
                    cus = l;
                }
                else
                {
                    cus += " && "+l.substr(0,string::npos);
                }
                //cout << cus << endl;
            }
        }
        getline(ifs,line);
    }

    return constraints.str();
}*/

void write_constraints(string constraints, ofstream& ofs)
{
    ofs << constraints << endl;
}

void parse_input_file(ifstream& ifs, ofstream& ofs, list<pair<string, string> >& stac, map<string,string>& lut)
{
    string line;
    string l;
    size_t stop;
    size_t start_label;
    size_t stop_label;
    getline(ifs,line); // first line can be discarded
    while(!ifs.eof())
    {
        while(line.find("shape=box") == string::npos && !ifs.eof())
        {
            cout << line << endl;
            if(line.find_first_of('N') == 0)
            {
                stop = line.find("->N");
                if(stop != string::npos)
                {
                    //add to stack
                    start_label = line.find(" [label=");
                    start_label += 9;
                    stop_label = line.find("\"]");
                    stac.push_back(pair<string,string>(line.substr(0,stop),line.substr(start_label,stop_label-start_label)));
                }
                else
                {
                    //add to lut
                    stop = line.find(" [label=");
                    start_label = stop +9;
                    stop_label = line.find("\" ]");

                    cout << line.substr(start_label,stop_label - start_label) << endl;

                    lut[line.substr(0,stop)] = line.substr(start_label,stop_label - start_label);
                }
            }
            getline(ifs,line);
        }
        if(!ifs.eof())
        {
            cout << "REACH CONSTRAINT" << endl;
            //bordel de contraintes
            start_label = line.find(" [label=");
            start_label += 9;
            cout << line.substr(start_label,1) << endl;
            if(line.substr(start_label,1) == "1")
            {
                string constraint;
                constraint = "!(";
                list<pair<string,string> >::iterator it = stac.begin();
                for(int i =0; i< stac.size()-1; i++)
                {

                    constraint += lut[it->first];
                    constraint += it->second +" && ";
                    ++it;
                }
                constraint += lut[it->first];
                constraint += it->second;
                constraint += ")";
                cout << "constraints : " << constraint << endl;
                write_constraints(constraint,ofs);
                //écrire contrainte
            }

            getline(ifs,line);
            //manage stack
            if(line.find_first_of('N') != 0)
            {
                break;
            }
            stop = line.find("->N");
            string id = line.substr(0,stop);

            cout << "EXTRACTED label :" << id << endl;
            start_label = line.find(" [label=");
            start_label += 9;
            stop_label = line.find("\"]");
            cout << "REMOVE" << endl;
            list<pair<string,string> >::reverse_iterator it = stac.rbegin();
            while(it->first != id && !stac.empty())
            {
                pair<string,string> p = stac.back();
                cout << "To be removed : " << p.first << "\t" << p.second << endl;
                stac.pop_back();
                //++it;
            }
            cout << it->first << "\t" << id << endl;
            if(!stac.empty())
            {
                pair<string,string> p = stac.back();
                cout << p.first <<"\t" << p.second << endl;
                stac.pop_back();
            }
            stac.push_back(pair<string,string>(line.substr(0,stop),line.substr(start_label,stop_label-start_label)));
        }
        getline(ifs,line);
    }
}

#include "conf_file_parser.h"

conf_file_parser::conf_file_parser(const string filename, const string output_filename)
{
    //ctor
    csv_file.open(output_filename.c_str(), ofstream::out | ofstream::app);
    conf_file.open(filename.c_str(), ifstream::in);

    if(!conf_file.is_open() || !csv_file.is_open())
    {
        cerr << "something went wrong trying to access the input and output files" << endl;
        exit(EXIT_FAILURE);
    }

    /*csv_file.close();
    conf_file.close();*/
}

conf_file_parser::~conf_file_parser()
{
    //dtor
}

void conf_file_parser::parse_file()
{
    string line, value;
    while(!conf_file.eof())
    {
        line = getLine();
        value = parse(line);
        //cout << "value to write in the file : " << value << endl;
        if(value != "")
        {
            writeIntoFile(value);
        }
        /*else
        {
            cout << "There was an empty line in the file :" << endl;
            cout << "\t " << line << endl;
        }*/
    }

    conf_file.close();
    csv_file.close();

}

string conf_file_parser::getLine()
{
    string line;
    getline(conf_file,line);
    return line;
}

string conf_file_parser::parse(const string line)
{
    string res="";
    size_t sep = line.find("=");
    string baseline = line.substr(0,sep);
    if(sep != string::npos && baseline != "sequence.comment ")
    {
        res = line.substr(sep+1);
    }
    return res;
}

void conf_file_parser::writeIntoFile(const string line)
{
    if(csv_file.is_open())
    {
        csv_file << atof(line.c_str()) << ",";
    }
    else
    {
        cerr << "Could not right the value into the output file" << endl;
        exit(EXIT_FAILURE);
    }

}

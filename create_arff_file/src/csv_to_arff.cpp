#include "csv_to_arff.h"

csv_to_arff::csv_to_arff()
{
    //ctor
}

csv_to_arff::csv_to_arff(const string input_filename, const string output_filename)
:input_file(input_filename.c_str(),ifstream::in),output_file(output_filename.c_str(),ofstream::out | ofstream::app)
{
    if(!input_file.is_open() || !output_file.is_open())
    {
        cerr << "Could not open input and/or output files" << endl;
        exit(EXIT_FAILURE);
    }
}

csv_to_arff::~csv_to_arff()
{
    //dtor
}

void csv_to_arff::write()
{
    write_title();
    write_columns();
    write_data();
}

void csv_to_arff::write_title()
{
    output_file << "% Title : MOTIV video generator \n";
    output_file << "%\n";
    output_file << "%The class presented here is 0 for videos that can be used and 1 for others\n";
    output_file << "@relation videos\n \n";
}

void csv_to_arff::write_columns()
{
    string line;
    input_file >> line;

    size_t pos_prec = 0;
    size_t pos = line.find(',');

    //first is the id (can be discard)
    pos_prec = pos+1;
    pos = line.find(',',pos_prec);

    //process the line
    string substring;
    while(pos != string::npos)
    {
        substring = line.substr(pos_prec,pos-pos_prec);
        //cout << substring << endl;

        //write into output
        output_file << "@attribute "+substring + " NUMERIC\n";

        pos_prec = pos+1;
        pos = line.find(',',pos_prec);
    }

    //process the last item (class)
    output_file << "@attribute class {0,1}\n \n";
}

void csv_to_arff::write_data()
{

    output_file << "@data\n";

    string line;
    input_file >> line;
    //input_file >> line;

    cout << "line : " << line << endl;

    size_t pos_prec;
    size_t pos;
    string substring;

    while(!input_file.eof())
    {

        pos_prec = 0;
        pos = line.find(',');

        //first is the id (can be discard)
        pos_prec = pos+1;
        pos = line.find(',',pos_prec);

        while(pos != string::npos)
        {
            substring = line.substr(pos_prec,pos-pos_prec);

            //write into output
            output_file << substring + ",";

            pos_prec = pos+1;
            pos = line.find(',',pos_prec);
        }
        //process the last item (class)
        substring = line.substr(pos_prec);
        output_file << substring << "\n";

        input_file >> line;
    }

}

void csv_to_arff::close()
{
    input_file.close();
    output_file.close();
}

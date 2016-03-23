#ifndef CONF_FILE_PARSER_H
#define CONF_FILE_PARSER_H

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <istream>

using namespace std;

class conf_file_parser
{
    public:
        conf_file_parser(const string filename, const string output_filename);
        virtual ~conf_file_parser();
        void parse_file();
    protected:
    private:

    ofstream csv_file;
    ifstream conf_file;

    string getLine();
    string parse(const string line);
    void writeIntoFile(const string line);

};

#endif // CONF_FILE_PARSER_H

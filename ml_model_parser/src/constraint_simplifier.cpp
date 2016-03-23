#include "constraint_simplifier.h"
constraint_simplifier::constraint_simplifier()
:filename("constraints.txt")
{
    //ctor
}


constraint_simplifier::~constraint_simplifier()
{
    //dtor
}

void constraint_simplifier::simplify()
{
    cout << "TRYING TO SIMPLIFY ------------------" << endl;
    read_file();
    simplify_cst();
    write_file();
}

void constraint_simplifier::set_file(const string fname)
{
    filename = fname;
}

void constraint_simplifier::read_file()
{
    f_in_out.open(filename.c_str(), fstream::in);
    string line;
    while(getline(f_in_out,line))
    {
        line+="\n";
        ss << line;
    }
    f_in_out.close();
}

void constraint_simplifier::simplify_cst()
{
    /*cout << "ATTENTION !!!" << endl;
    cout << ss.str() << endl;*/
    string line;
    string subline;
    string feature;
    string val;
    bool inf=true;
    while(getline(ss,line))//retrieve a constraint
    {
        size_t start = 2;
        size_t stop = line.find("&&");
        while (stop != string::npos)
        {
            subline = line.substr(start,stop-start);
            size_t start_subl = 0;
            size_t stop_subl = subline.find("<=");
            if(stop_subl != string::npos) //extract feature
            {
                feature = subline.substr(start_subl,stop_subl+2);
                inf = true;
            }
            else
            {
                stop_subl = subline.find(">");
                feature = subline.substr(start_subl,stop_subl+1);
                inf = false;
            }
            if(inf) //extract value
            {
                val = subline.substr(stop_subl+2, stop - (stop_subl+2));
            }
            else
            {
                val = subline.substr(stop_subl+1, stop - (stop_subl+1) );
            }

            cout << "feature/value couple : " << feature << "\t" << val << endl;

            ///add in the map and vector
            //test if feature exists
            if(vec_feat_val.empty())
            {
                vec_feat_val.push_back(pair<string,string>(feature,val));
            }
            else
            {
                vector<pair<string,string> >::iterator it = vec_feat_val.begin();
                for( ; it != vec_feat_val.end(); ++it)
                {
                    cout << "comparison" << endl;
                cout << (*it).first << endl << feature << endl;
                string res;
                if((*it).first == feature)
                {
                    res = "true";
                }
                else
                {
                    res = "false";
                }
                cout << res << endl;
                    if((*it).first == feature)
                    {
                        if( (inf && ( ( (*it).second > val) ) ) || (!inf && ( ( (*it).second < val) ) ) )
                        {
                            (*it).second = val;
                            break;
                        }
                    }
                }

                if(it == vec_feat_val.end())
                {
                    vec_feat_val.push_back(pair<string,string>(feature,val));
                }
            }

            //cout << feature << "\t" << val << "\n";

            start = stop + 3;
            stop = line.find("&&",start);
        }
        cout << "OUT OF THE LOOP !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        subline = line.substr(start);

        size_t start_subl = 0;
        size_t stop_subl = subline.find("<=");

        if(stop_subl != string::npos) //extract feature
        {
            feature = subline.substr(start_subl,stop_subl+2);
        }
        else
        {
            stop_subl = subline.find(">");
            feature = subline.substr(start_subl,stop_subl+1);
            inf = false;
        }
        if(inf) //extract value
        {
            val = subline.substr(stop_subl+2, stop - (stop_subl+2) -1 );
            val = val.substr(0,val.size()-1);
        }
        else
        {
            val = subline.substr(stop_subl+1, stop - (stop_subl+1) -1 );
            val = val.substr(0,val.size()-1);
        }
        cout << "feature/value couple : " << feature << "\t" << val << endl;

        if(vec_feat_val.empty())
        {
            vec_feat_val.push_back(pair<string,string>(feature,val));
        }
        else
        {
            vector<pair<string,string> >::iterator it = vec_feat_val.begin();
            for( ; it != vec_feat_val.end(); ++it)
            {
                cout << "comparison" << endl;
                cout << (*it).first << endl << feature << endl;
                string res;
                if((*it).first == feature)
                {
                    res = "true";
                }
                else
                {
                    res = "false";
                }
                cout << res << endl;
                if((*it).first == feature)
                {
                    if( (inf && ( ( (*it).second > val) ) ) || (!inf && ( ( (*it).second < val) ) ) )
                    {
                        (*it).second = val;
                        break;
                    }
                }
            }

            if(it == vec_feat_val.end())
            {
                vec_feat_val.push_back(pair<string,string>(feature,val));
            }
        }

        //build result constraint
        string res = "!(";
        int i =0;
        for( ; i < vec_feat_val.size()-1; i++)
        {
            pair<string,string> elem = vec_feat_val[i];
            res += elem.first+elem.second+"&&";
        }
        pair<string,string> elem = vec_feat_val[i];
        res += elem.first+elem.second+")\n";
        //put back into streamstring
        ss_res << res;

        vec_feat_val.clear();
    }
}

void constraint_simplifier::write_file()
{
    f_in_out.open(filename.c_str(), fstream::out);

    f_in_out << ss_res.str();

    f_in_out.close();
}

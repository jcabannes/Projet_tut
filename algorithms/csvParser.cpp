#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <string>
//#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include <boost/tokenizer.hpp>

int main()
{
    using namespace std;
    using namespace boost;

    string data("/home/matchi/Desktop/ST/cov.csv");

    ifstream in(data.c_str());
    if (!in.is_open()) return 1;

    typedef tokenizer< escaped_list_separator<char> > Tokenizer;

    vector< string > vec;
    string line;

    while (getline(in,line))
    {
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());

        //if (vec.size() < 2) continue;

        //copy(vec.begin(), vec.end(),
             //ostream_iterator<string>(cout, "|"));
	cout <<vec.at(0)/*<<"|"<<vec.at(1)*/;

        cout << "\n----------------------" << endl;
    }
}
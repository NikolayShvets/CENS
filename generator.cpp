#include "generator.h"

using namespace std;
generator::generator(long double m, long double d)
{
    this->m = m;
    this->d = d;
    t_corr = 2*3.14159/w;
}
vector<string> &split(const string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}
vector<string> split(const std::string &s, char delim)
{
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}
long double generator::get_nu(long double time)
{
    ifstream w_n_file("/home/nikonikoni/QtProjects/OPRS/linearAlgebra/w_n_file.txt");
    string str;
    getline(w_n_file, str);
    vector<string> nu_time = split(str, '|');
    int number = (int)(time/t_corr);
    long double res = stold(nu_time[number]);
    return res;
}
long double generator::white_noise_generator()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<long double> distribution(m, d);
    nu_0 = distribution(generator);
    return nu_0;
}
generator::~generator()
{

}

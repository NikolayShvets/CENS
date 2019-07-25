#include <iostream>
#include <vector>
#include <math.h>
#include "generator.h"

long double corr(std::vector<std::vector<long double>> shot, std::vector<std::vector<long double>> map);
std::vector<std::vector<long double>> getMap(int n, long double mm, long double dm);
std::vector<std::vector<long double>> getShot(int n, std::vector<std::vector<long double>> m, int h, int w);
void showMatrix(std::vector<std::vector<long double>> m);
int main()
{
    double maxCorr{0.0}, cov{0.0};
    int delitel{0};
    long double mm{1000.}, dm{250.};
    int maxCorrX, maxCorrY, mapSize, shotSize;
    std::cout<<"Input size of the map"<<std::endl;
    std::cin>>mapSize;
    std::cout<<"Input size of the shot. Map and shot sizes must be multiple"<<std::endl;
    std::cin>>shotSize;
    std::cout<<"Input mathematical expectation of elevation for the map"<<std::endl;
    std::cin>>mm;
    std::cout<<"Input dispersion of elevation for the map"<<std::endl;
    std::cin>>dm;

    std::vector<std::vector<long double>> map = getMap(mapSize, mm, dm);
    std::vector<std::vector<long double>> recshot = getShot(shotSize, map, 2, 2);
    std::vector<std::vector<long double>> curShot;
    curShot.resize(4);
    delitel = map.size()/recshot.size();
    showMatrix(map);
    std::cout<<std::endl;
    std::cout<<"Required shot: "<<std::endl;
    showMatrix(recshot);
    //std::cout<<"Covarience = "<<corr(shot, map)<<std::endl;
    for(int i = 0; i < map.size()/delitel; ++i)
    {
        for(int j = 0; j < map.size()/2; ++j)
        {
            curShot = getShot(4, map, i, j);
            std::cout<<"Current shot "<<i<<" "<<j<<"\n";
            cov = corr(curShot, recshot);
            std::cout<<"Current covarience: "<<cov<<std::endl;
            if(maxCorr < cov)
            {
                  maxCorr = cov;
                  maxCorrX = i;
                  maxCorrY = j;
            }
            showMatrix(curShot);
        }
    }
    std::cout<<"=================================\n";
    std::cout<<"Maximal covariense = "<<maxCorr<<" at x = "<<maxCorrX<<", "<<" y = "<<maxCorrY<<std::endl;
    return 0;
}
long double corr(std::vector<std::vector<long double>> shot, std::vector<std::vector<long double>> map)
{
    if (shot.size() != map.size())
                    return 10;
    long double sumX = 0;
    long double sumX2 = 0;
    long double sumY = 0;
    long double sumY2 = 0;
    long double sumXY = 0;

    int n = shot.size() < map.size() ? shot.size() : map.size();
    for (int i = 0; i < n; ++i)
    {
        long double xval = shot[i][i];
        long double yval = map[i][i];
        sumX += xval;
        sumX2 += xval * xval;
        sumY += yval;
        sumY2 += yval * yval;
        sumXY += xval * yval;
    }
    long double stdX = sqrt(sumX2 / n - sumX * sumX / n / n);
    long double stdY = sqrt(sumY2 / n - sumY * sumY / n / n);
    long double covariance = (sumXY / n - sumX * sumY / n / n);
    return covariance / stdX / stdY;
}

void showMatrix(std::vector<std::vector<long double>> m)
{
    for(int i = 0; i < m.size(); ++i)
    {
        for(int j = 0; j < m.back().size(); ++j)
        {
            std::cout<<m[i][j]<<"|";
        }
        std::cout<<"\n";
    }
}
std::vector<std::vector<long double>> getMap(int n, long double mm, long double dm)
{
    generator gen(mm, dm);
    std::vector<std::vector<long double>> res;
    res.resize(n);
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            res[i].push_back(gen.white_noise_generator());
        }
    }
    return res;
}
std::vector<std::vector<long double>> getShot(int n, std::vector<std::vector<long double>> m, int h, int w)
{
    generator gen(10, 5);
    std::vector<std::vector<long double>> res;
    res.resize(n);
    for(int i = h; i < res.size() + h; ++i)
    {
        for(int j = w; j < res.size() + w; ++j)
        {
            res[i - h].push_back(m[i][j] + gen.white_noise_generator());
        }
    }
    return res;
}

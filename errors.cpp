#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include "stats.hpp"

int main(int argc, char* argv[])
{
   // input 
   // Check the number of parameters
   if (argc < 2) {
      // Tell the user how to run the program
      std::cerr << "Usage: " << argv[0] << " file.dat" << std::endl;
      return 1;
   }  
   std::vector<float> x;
   int n = 0;
   float r;
   std::ifstream infile;
   infile.open(argv[1]);
   if (!infile.good()) {   // error trap bad file
      std::cerr << " error opening file" << std::endl;
      return 1;
   }
   while (infile >> r) {
      x.push_back(r);
      ++n;
   }
   infile.close();
   // processing
   Pop population;
   Sample sample;
   population.load(x);
   sample.load(x);
   float m = population.computeMean();
   float var = population.computeVar();
   float std = population.computeStd();
   float aad = population.computeMeanDev();
   float skw = population.computeSkewness();
   float mdn = population.computeMedian();
   float mad = population.computeMedianDev();
   float mdnskw = population.computeMedianSkew();
   std::ofstream outfile;
   // output
   outfile.open("errors.csv");
   outfile << "mean,median,var,std,meandev,mdndev,skw,mdnskw\n";
   for (int i = 2; i <= n; i++) {
      sample.randomSample(x, i);
      outfile << std::abs(m - sample.computeMean()) << ',';
      outfile << std::abs(mdn - sample.computeMedian()) << ',';
      outfile << std::abs(var - sample.computeVar()) << ',';
      outfile << std::abs(std - sample.computeStd()) << ',';
      outfile << std::abs(aad - sample.computeMeanDev()) << ',';
      outfile << std::abs(mad - sample.computeMedianDev()) << ',';
      outfile << std::abs(skw - sample.computeSkewness()) << ',';
      outfile << std::abs(mdnskw - sample.computeMedianSkew()) << std::endl;
   }
   outfile.close();
   std::cout << " >> errors.csv\n";
   return 0;
}
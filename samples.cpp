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
   // output
   std::ofstream outfile;
   outfile.open("samples.txt");
   outfile << " population" << std::endl;
   outfile << " n = " << population.getSize() << std::endl;
   outfile << " mean = " << m << std::endl;
   outfile << " median = " << mdn << std::endl;
   outfile << " variance = " << var << std::endl;
   outfile << " standard deviation = " << std << std::endl;
   outfile << " mean deviation = " << aad << std::endl;
   outfile << " median deviation = " << mad << std::endl;
   outfile << " skewness = " << skw << std::endl;
   outfile << " median skewness = " << mdnskw << std::endl;
   outfile << "\n random samples" << std::endl;
   outfile << std::setw(6) << "size";
   outfile << std::setw(9) << "mean";
   outfile << std::setw(9) << "median";
   outfile << std::setw(9) << "var";
   outfile << std::setw(9) << "std";
   outfile << std::setw(9) << "meandev";
   outfile << std::setw(9) << "mdndev";
   outfile << std::setw(9) << "skw";
   outfile << std::setw(9) << "mdnskw" << std::endl;
   outfile << std::fixed << std::setprecision(3);
   for (int i = 2; i <= n; i++) {
      sample.randomSample(x, i);
      outfile << std::setw(6) << sample.getSize();
      outfile << std::setw(9) << sample.computeMean();
      outfile << std::setw(9) << sample.computeMedian();
      outfile << std::setw(9) << sample.computeVar();
      outfile << std::setw(9) << sample.computeStd();
      outfile << std::setw(9) << sample.computeMeanDev();
      outfile << std::setw(9) << sample.computeMedianDev();
      outfile << std::setw(9) << sample.computeSkewness();
      outfile << std::setw(9) << sample.computeMedianSkew() << std::endl;  
   }
   outfile.close();
   std::cout << " >> samples.txt\n";
   return 0;
}

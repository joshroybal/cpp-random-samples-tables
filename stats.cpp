#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include "stats.hpp"

// stats class public methods - constructors, destructors and accessors

// load array into object
void Stats::load(const std::vector<float>& v)
{
   n = v.size();
   x.clear();
   x = v;
}

// method accesses size n
int Stats::getSize() const
{
   return n;
}

// method accesses mean
float Stats::computeMean() const
{
   return mean(x, n);
}

// method accesses variance
float Stats::computeVar() const
{
   return var();
}

// method accesses standard deviation
float Stats::computeStd() const
{
   return std();
}

// method accesses minimum
float Stats::computeMin() const
{
   return minimum();
}

// method accesses maximum
float Stats::computeMax() const
{
   return maximum();
}

// method accesses median
float Stats::computeMedian() const
{
   return median(x, n);
}

// method accesses median deviation
float Stats::computeMedianDev() const
{
   return median_dev();
}

// method accesses mean deviation
float Stats::computeMeanDev() const
{
   return mean_dev();
}

// method accesses skewness
float Stats::computeSkewness() const
{
   return skewness();
}

// method accesses median skewness
float Stats::computeMedianSkew() const
{
   return median_skew();
}

// method accesses coefficient of variation
float Stats::computeCV() const
{
   return coefficient_of_variation();
}

// method computes cumulative from center tables
void Stats::computeTables()
{
   float m = computeMean();
   float mdn = computeMedian();
   float std = computeStd();
   float mad = computeMedianDev();
   float aad = computeMeanDev();
   compute_tables(standard_right, standard_left, m, std);
   compute_tables(median_right, median_left, mdn, mad);
   compute_tables(mean_right, mean_left, m, aad);
}

// method displays cumulative from center tables
void Stats::displayTables() const
{
   std::cout << "\n standard deviation tables - cumulative from mean";
   display_tables(standard_right, standard_left);
   std::cout << "\n median deviation tables - cumulative from median";
   display_tables(median_right, median_left);
   std::cout << "\n mean deviation tables - cumulative from mean";
   display_tables(mean_right, mean_left);
}

// method displays cumulative from center tables - html format
void Stats::htmlTables() const
{
   std::cout << "\n<p>standard deviation tables - cumulative from mean</p>";
   html_tables(standard_right, standard_left);
   std::cout << "\n<p>median deviation tables - cumulative from median</p>";
   html_tables(median_right, median_left);
   std::cout << "\n<p>mean deviation tables - cumulative from mean</p>";
   html_tables(mean_right, mean_left);
}

// stats class protected methods - computers

// method computes mean
float Stats::mean(const std::vector<float>& x, int n) const
{
   float s = 0;
   for (int i = 0; i < n; i++)
      s += x[i];
   return s/n;
}

// method returns kth element of x
// C. A. R. Hoare's quick select algorithm
float Stats::quick_select(const std::vector<float>& x, int n, int k) const
{
   int i, j, left = 0, right = n - 1;
   float pivot;
   std::vector<int> idx;   // index vector
   // initialize index vector to natural order (zero indexed)
   for (i = 0; i < n; i++) idx.push_back(i);
   while (left < right) {
      pivot = x[idx[k]];
      i = left;
      j = right;
      do {
         while (x[idx[i]] < pivot) i++;
         while (pivot < x[idx[j]]) j--;
         if (i <= j) {
            int idxtmp = idx[i];
            idx[i] = idx[j];
            idx[j] = idxtmp;
            i++;
            j--;
          }
      } while (i <= j);
      if (j < k) left = i;
      if (k < i) right = j;
   }
   float result = x[idx[k]];
   return result;
}

// method finds minimum
float Stats::minimum() const
{
   float min = x[0];
   for (int i = 1; i < n; i++)
      if (x[i] < min) min = x[i];
   return min;
}

// method finds maximum
float Stats::maximum() const
{
   float max = x[0];
   for (int i = 1; i < n; i++)
      if (x[i] > max) max = x[i];
   return max;
}

// method returns median
float Stats::median(const std::vector<float>& x, int n) const
{
   if (n % 2 != 0)
      return quick_select(x, n, n/2);
   else {
      float mdn = quick_select(x, n, n/2) + quick_select(x, n, n/2-1);
      return mdn/2;
   }
}

// method computes median deviation
float Stats::median_dev() const
{
   float mdn = median(x, n);
   std::vector<float> abs_devs;
   for (int i = 0; i < n; i++) abs_devs.push_back( std::abs(x[i]-mdn) );
   return median(abs_devs, n);
}

// method computes mean deviation
float Stats::mean_dev() const
{
   float m = mean(x, n);
   std::vector<float> abs_devs;
   for (int i = 0; i < n; i++) abs_devs.push_back( std::abs(x[i]-m) );
   return mean(abs_devs, n);
}

// method computes skewness
float Stats::skewness() const
{
   float m = mean(x, n);
   float s = 0;
   for (int i = 0; i < n; i++)
      s = ((i * s) + pow(x[i] - m, 3)) / (i + 1);
   return s / pow(std(), 3);
}

// method computes median skewness - Pearson's second skewness coefficient
float Stats::median_skew() const
{
   return 3*(mean(x,n)-median(x,n))/std();
}

// method computes coefficient of variation
float Stats::coefficient_of_variation() const
{
   return std() / mean(x, n);
}

// method computes right and left z tables
void Stats::compute_tables(float z_right[][10], float z_left[][10], float avg, float dev)
{
   int right_counter[41][10];
   int left_counter[41][10];
   std::vector<float> z;
   for (int i = 0; i < n; i++)
      z.push_back( (avg-x[i])/dev );
   // initialize counter arrays to all zeros
   for (int i = 0; i < 41; i++)
      for (int j = 0; j < 10; j++)
         right_counter[i][j] = 0;
   for (int i = 0; i < 41; i++)
      for (int j = 0; j < 10; j++)
         left_counter[i][j] = 0;
   // count z value occurences - both tails - to be parallelized
   for (int i = 0; i < 41; i++)
      for (int j = 0; j < 10; j++) {
         float lub = i/10.0 + j/100.0;
         for (int k = 0; k < n; k++)
            if (std::abs(z[k]) <= lub) {
               if (z[k] > 0) ++(right_counter[i][j]);
               if (z[k] < 0) ++(left_counter[i][j]);
            }
      }

   // normalize to float - right tail
   for (int i = 0; i < 41; i++)
      for (int j = 0; j < 10; j++)
         z_right[i][j] = (float)right_counter[i][j] / (float)n;
   // normalize to float - left tail
   for (int i = 0; i < 41; i++)
      for (int j = 0; j < 10; j++)
         z_left[i][j] = (float)left_counter[i][j] / (float)n;
}

// method displays cumulative from central tendency tables
void Stats::display_tables(const float z_right[][10], const float z_left[][10]) const
{
   // display right tail table
   // print header row
   std::cout << "\n right tail" << std::endl;
   std::cout << "  z ";
   for (int j = 0; j < 10; j++)
      std::cout << " +" << std::fixed << std::setprecision(2) << float(j)/float(100);
   std::cout << std::endl;
   for (int i = 0; i < 41; i++) {
      std::cout << " " << std::fixed << std::setprecision(1) << float(i)/float(10);
      for (int j = 0; j < 10; j++)
         std::cout << " " << std::fixed << std::setprecision(3) << z_right[i][j];
      std::cout << std::endl;
   }
   // display left tail table
   // print header row
   std::cout << "\n left tail" << std::endl;
   std::cout << "  z ";
   for (int j = 0; j < 10; j++)
      std::cout << " +" << std::fixed << std::setprecision(2) << float(j)/float(100);
   std::cout << std::endl;
   for (int i = 0; i < 41; i++) {
      std::cout << " " << std::fixed << std::setprecision(1) << float(i)/float(10);
      for (int j = 0; j < 10; j++)
         std::cout << " " << std::fixed << std::setprecision(3) << z_left[i][j];
      std::cout << std::endl;
   }
   // display unity table - has nobody thought of this before??
   // print header row
   std::cout << "\n right and left tails" << std::endl;
   std::cout << "  z ";
   for (int j = 0; j < 10; j++)
      std::cout << " +" << std::fixed << std::setprecision(2) << float(j)/float(100);
   std::cout << std::endl;
   for (int i = 0; i < 41; i++) {
      std::cout << " " << std::fixed << std::setprecision(1) << float(i)/float(10);
      for (int j = 0; j < 10; j++)
         std::cout << " " << std::fixed << std::setprecision(3) << z_right[i][j]+z_left[i][j];
      std::cout << std::endl;
   }
}

// method displays cumulative from central tendency tables - html format
void Stats::html_tables(const float z_right[][10], const float z_left[][10]) const
{
   // display right tail table
   // print header row
   std::cout << "\n<p>right tail</p>" << std::endl;
   std::cout << "\n<table>" << std::endl;
   std::cout << "\n<tr><th>z</th>";
   for (int j = 0; j < 10; j++)
      std::cout << "<th>+" << std::fixed << std::setprecision(2) << float(j)/float(100) << "</th>";
   std::cout << "</tr>\n";
   for (int i = 0; i < 41; i++) {
      std::cout << "\n<tr><th>" << std::fixed << std::setprecision(1) << float(i)/float(10) << "</th>";
      for (int j = 0; j < 10; j++)
         std::cout << "<td>" << std::fixed << std::setprecision(5) << z_right[i][j] << "</td>";
      std::cout << "</tr>\n";
   }
   std::cout << "\n</table>" << std::endl;

   // display right tail table
   // print header row
   std::cout << "\n<p>left tail</p>" << std::endl;
   std::cout << "\n<table>" << std::endl;
   std::cout << "\n<tr><th>z</th>";
   for (int j = 0; j < 10; j++)
      std::cout << "<th>+" << std::fixed << std::setprecision(2) << float(j)/float(100) << "</th>";
   std::cout << "</tr>\n";
   for (int i = 0; i < 41; i++) {
      std::cout << "\n<tr><th>" << std::fixed << std::setprecision(1) << float(i)/float(10) << "</th>";
      for (int j = 0; j < 10; j++)
         std::cout << "<td>" << std::fixed << std::setprecision(5) << z_left[i][j] << "</td>";
      std::cout << "</tr>\n";
   }
   std::cout << "\n</table>" << std::endl;

   // display unity table - has nobody thought of this before??
   // print header row
   // display right tail table
   // print header row
   std::cout << "\n<p>right and left tail</p>" << std::endl;
   std::cout << "\n<table>" << std::endl;
   std::cout << "\n<tr><th>z</th>";
   for (int j = 0; j < 10; j++)
      std::cout << "<th>+" << std::fixed << std::setprecision(2) << float(j)/float(100) << "</th>";
   std::cout << "</tr>\n";
   for (int i = 0; i < 41; i++) {
      std::cout << "\n<tr><th>" << std::fixed << std::setprecision(1) << float(i)/float(10) << "</th>";
      for (int j = 0; j < 10; j++)
         std::cout << "<td>" << std::fixed << std::setprecision(5) << z_right[i][j]+z_left[i][j] << "</td>";
      std::cout << "</tr>\n";
   }
   std::cout << "\n</table>" << std::endl;
}

// population class public methods - constructors and accessors

// population class private methods - computers

// method computes population variance
float Pop::var() const
{
   float s = 0;
   float m = mean(x, n);
   for (int i = 0; i < n; i++)
      s += pow((x[i]-m), 2);
   return s / n;
}

// method computes population standard variation
float Pop::std() const
{
   return sqrt(var());
}

// sample class public methods - return 1 if bad
void Sample::randomSample(const std::vector<float>& pop, int m)
{
   if (m <= 0) {
      n = 0;
      return;
   }
   n = m;
   int pop_size = pop.size();
   // use an index vector - preserve passed vector
   std::vector<int> idx;
   for (int i = 0; i < pop_size; i++) idx.push_back(i);
   // perform pop_size random swaps
   int seed = clock();
   if (seed % 2 == 0) ++seed;
   srand(seed);
   for (int i = 0; i < pop_size; i++) {
      int rndidx = (int)(pop_size*((float)rand()/(float)IMAX));
      int tmp = idx[i];
      idx[i] = idx[rndidx];
      idx[rndidx] = tmp;
   }
   x.clear();  // clear in case something is there
   for (int i = 0; i < n; i++) x.push_back(pop[idx[i]]);
}

// sample class private methods - computers

// method computes population variance
float Sample::var() const
{
   float s = 0;
   float m = mean(x, n);
   for (int i = 0; i < n; i++)
      s += pow((x[i]-m), 2);
   return s/(n-1);
}

// method computes population standard variation
float Sample::std() const
{
   return sqrt(var());
}

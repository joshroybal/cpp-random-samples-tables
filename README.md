# cpp-random-samples-tables
C++ console application to model random sampling from given populations and print general statistical summary information 
for the entire population and random samples ranging from size 2 to a size equal to the population. Input should be 1 column
text files containing real valued numeric data.
./samples file.csv will print a general statistical summary for the entire population of values read from the data file,
and a table of statistics where rows 2 through n (size of population) are general statistical summaries for random samples
taken from the population raning from size 2 to n.
./errors file.csv will print a table in comma delimited .csv text format of the differences between the population statistics
and the analogous statistical values computed for each random sample taken from 2 through n.

Genetic Algorithm README
========================

Introduction:
-------------

This project is a first of my new one-day-build mini-projects serie.
It is my take on implementing a simple version of the Genetic Algorithm using C++,
with a demo use case consisting of fitting a gaussian distribution to a data histogram. 


General Code Description:
-------------------------

This code has been developed and tested on a machine running ubuntu 17.04,
but should work on any linux distribution with gcc instaled.

### Getting the code:

This code is available on GitHub at: <br>
<a href="https://github.com/remizaidan/GeneticAlgorithm">
https://github.com/remizaidan/GeneticAlgorithm
</a>

To download from the command-line: <br>
> git clone https://github.com/remizaidan/GeneticAlgorithm


### Dependencies:
- ROOT Data Analysis Framework (tested with version 6.10). Documentation and download are avaiable here: \n
<a href="https://root.cern.ch">https://root.cern.ch</a>


### Doxygen documentation:
<a href="https://remizaidan.github.io/GeneticAlgorithm">
https://remizaidan.github.io/GeneticAlgorithm
</a>


### Code structure:

The code is structured into the following sub-directories:
- <b>include</b> Contains all header files.
- <b>src</b> Contains all source files that are to be compiled into static libraries.
- <b>utils</b> Contains all source files that are to be compiled into executables.
- <b>doc</b> Contains documentation files.

After comiling, the following directories may be created:
- <b>bin</b> Contains all executables.
- <b>lib</b> Contains project libraries (shared or static).
- <b>.objs</b> Contains static libraries corresponding to source files in the src folder.
- <b>.deps</b> Contains dependency files.


### Compiling and Running the demo

Compile the demo (replace <N> with the number of cores on your machine):
> make -j<N>

Run the demo:
> ./bin/runGA.exe [options]

For the list of available options see @ref runGA#parseCommandLine or run:
> ./bin/runGA.exe -h


### Other compiling options:

Compile into a shared library:
> make -j<N> shared

Compile into a static library:
> make -j<N> static

Generate local doxygen documentation:
> make doc

#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <random>
#include <cmath>
using namespace std;

class SimulatedAnnealing
{
	int** graph;
	int size;
	int weight;
	int *path;
	double temperature = 1000000;
	double endTemp = 0.0001;
	double cooling = 0.9999;
	int iterations = 10;
	const double e = 2.71828;
public:
	SimulatedAnnealing(int n);
	SimulatedAnnealing(string file_name);// Read date from TSTP text file. Exaplary file are in folder data
	~SimulatedAnnealing();
	void setInitTemp(double temp);// Set temperature
	void setEndTemp(double temp);// Set end temperature. When beggining temperature will fall under end temperature algorithm will stop
	void setCooling(double rate);// Set cooling rate. On each iterate teperature will be multipy by cooling rate until fall unred end temperature
	void setIterations(int i);// Amount of inside iteration in each main iteration
	void print_graph();// Printing graph matrnix
	void print_path(int tempPath[]);// Printing given path
	int get_size();// Return graph size
	int get_weight();//Return path weight(length)
	void copyPath(int newPath[], int oldPath[]);// Copy oldPath to newPath
	void swap(int path[], int i, int j);// Swap 2 elements in path
	int cost(int path[]);// Return given path weight(length)
	void randomizePath(int tempPath[]);// Mix path by using swap function. It is used to initialize paths
	double random();// Random number from 0 to 1
	void annaeling();// Start algorithm
};


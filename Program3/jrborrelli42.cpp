#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <tuple>
#include <list>
#include <cstddef>
#include <cstring>
#include <math.h>
#include <bits/stl_numeric.h>

typedef std::size_t vertex_t;
typedef std::tuple<vertex_t,vertex_t,double> weighted_edge_t;

/* Get the source of a weighted edge.
 *
 * This function returns the source of a weighted edge.
 *
 * @param edge - the edge
 *
 * @return std::get<0>(edg)
 * */
vertex_t get_source(const weighted_edge_t &edg);

/* Get the destination of a weighted edge.
 *
 * This function returns the destination of a weighted edge.
 *
 * @param edge - the edge
 *
 * @return std::get<1>(edg)
 * */
vertex_t get_destination(const weighted_edge_t &edg);

/* Get the weight of a weighted edge.
 *
 * This function returns the weight of a weighted edge.
 *
 * @param edge - the edge
 *
 * @return std::get<2>(edg)
 * */
vertex_t get_weight(const weighted_edge_t &edg);

/* Determine the number of vertices in the graph.
 *
 * This function determines the number of vertices in the graph represented
 * by a vector of weighted edges. It is assumed that the number of vertices
 * is given by the largest source or destination vertex in the list of edges
 * plus one.
 *
 * @param edges - a vector of weighted edges defining a graph
 *
 * @return the number of vertices in the graph represented by edges
 * */
unsigned int get_vertex_count(const std::vector<weighted_edge_t> &edges);

/* Reads weighted edges from a file.
 *
 * This function reads weighted edges from filename and returns them as a
 * std::vector<weighted_edge_t>. each line of the file is assumed to be of the
 * form "src dst wt" where src is the source vertex, and dst is the destination
 * vertex, and wt is the weight of the edge. All vertices are assumed to be
 * unsigned integers that can be stored as a std::size_t (i.e., in a vertex_t).
 * Duplicate edges found in the file are ignored.
 *
 * @param filename - name of the file to read
 *
 * @return a vector of edges read from filename
 *
 * @throws std::runtime_error - thrown if there is an error reading the file
 * */
std::vector<weighted_edge_t> read_graph(const std::string &filename);

/* Solves the traveling salesman problem by brute force.
 *
 * This function solves the TSP via brute force. It accepts a vector of
 * weighted edges and count of the number of vertices. These two values
 * together define a weighted graph. 
 *
 * @param edges - a vector of weighted edges in the graph
 *
 * @param n_vertices - the number of vertices in the graph;
 *                     the vertices are 0 ... n_vertices-1
 *
 * @return the cost of the minimum Hamiltonian cycle or infinity if none exists
 * */
double TSP(const std::vector<weighted_edge_t> &edges, unsigned int n_vertices);

double TSP(const std::vector<weighted_edge_t> &edges, unsigned int n_vertices) {
	// default value for min_cost is infinity, indicating that there is no hamiltonian cycle.
	double min_cost = std::numeric_limits<double>::infinity();

	// create an adjacency matrix, filled with infinity in all the places because we are trying to find the minimum
	std::vector<std::vector<double>> adjacencyMatrix(n_vertices, std::vector<double>(n_vertices, std::numeric_limits<double>::infinity()));

	// Looping through the entire adjacency matrix
	for(vertex_t i = 0; i < edges.size(); ++i){
		vertex_t source = get_source(edges[i]);
		vertex_t destination = get_destination(edges[i]);
		double weight = get_weight(edges[i]);
		
		// Set weight for edge from destination to source
		adjacencyMatrix[destination][source] = weight;
	}

	// Make a list of all vertices to generate permutations (0...n_vertices)
	std::vector<int> vertices(n_vertices);
	std::iota(vertices.begin(), vertices.end(), 0);

	// Make a list of all the weights corresponding to the hamiltonian circuits
	std::vector<double> cycleWeights;

	// generate permutations and populate cycleWeights
	do{ // loops as long as another permutation can be generated.
		double tempWeight = 0.0;
		bool isHamiltonianCircuit = true;

		// calculate weight and circuit validity for permutation
		for(vertex_t i = 0; i < vertices.size(); ++i){
			int tempSRC = vertices[i];
			int tempDST = vertices[(i + 1) % vertices.size()]; // hits zero again when looped through completely so you don't go out of bounds.

			// if there is an edge between the two vertices
			if(adjacencyMatrix[tempSRC][tempDST] != std::numeric_limits<double>::infinity()){
				// then increment our tempWeight value
				tempWeight += adjacencyMatrix[tempSRC][tempDST];
			}else{
				isHamiltonianCircuit = false;
				break;
			}
		}

		// the this permutation is valid, then add to our lists
		if(isHamiltonianCircuit){ 
			cycleWeights.push_back(tempWeight);
		}
	}while(std::next_permutation(vertices.begin() + 1, vertices.end()));

	// if there was at least one valid circuit, then assign a min_cost
	if(!cycleWeights.empty()){
		min_cost = *std::min_element(cycleWeights.begin(), cycleWeights.end());
	}
	return min_cost;
}

vertex_t get_source(const weighted_edge_t &edg) {
	return std::get<0>(edg);
}

vertex_t get_destination(const weighted_edge_t &edg) {
	return std::get<1>(edg);
}

vertex_t get_weight(const weighted_edge_t &edg) {
	return std::get<2>(edg);
}

unsigned int get_vertex_count(const std::vector<weighted_edge_t> &edges) {
	unsigned int n_vertices = 0;
	if(!edges.empty()) {
		std::set<vertex_t> vertex_set;
		
		/* add the source vertices to vertex_set */
		std::transform(
			edges.begin(),
			edges.end(),
			std::inserter(vertex_set, vertex_set.end()),
			get_source
		);
		
		/* add the destination vertices to vertex_set */
		std::transform(
			edges.begin(),
			edges.end(),
			std::inserter(vertex_set, vertex_set.end()),
			get_destination
		);

		/* get the largest vertex from vertex_set and add 1 */
		n_vertices = *std::max_element(vertex_set.begin(), vertex_set.end()) + 1;
	}
	
	return n_vertices;
}

std::vector<weighted_edge_t> read_graph(const std::string &filename) {
	std::vector<weighted_edge_t> edges;
	std::ifstream file(filename);
	if(file) {
		/* read edges into a set to remove duplicates */
		std::set<weighted_edge_t> edge_set;

		vertex_t src, dst;
		double wt;
		while(file >> src && file >> dst && file >> wt) {
			edge_set.insert( weighted_edge_t(src,dst,wt) );
		}

		/* check if there was an error reading in the file */
		if(file.bad()) { // i/o error
			std::ostringstream oss;
			oss << filename << ": " << strerror(errno);
			throw std::runtime_error(oss.str());
		}
		else if((file.fail() && !file.eof())) { // conversion error
			std::ostringstream oss;
			oss << filename << ": error reading file";
			throw std::runtime_error(oss.str());
		}

		/* copy the edges read to the edges vector */
		std::copy(edge_set.begin(), edge_set.end(), std::back_inserter(edges));
	}
	else { // error opening file
		std::ostringstream oss;
		oss << filename << ": " << strerror(errno);
		throw std::runtime_error(oss.str());
	}

	if(edges.empty()) {
		std::ostringstream oss;
		oss << filename << ": file does not contain any edges";
		throw std::runtime_error(oss.str());

	}
	return edges;
}

void usage(int argc, char *argv[]) {
	if(argc != 2) {
		std::cerr << "Invalid number of command line arguments." << std::endl << std::endl;
	}
	std::cout << "usage: ";
	std::cout << argv[0] << " infile" << std::endl;
	std::cout << "  infile - file containing a list of edges" << std::endl << std::endl;
	std::cout << "It is assumed that each line of <infile> contains an edge of the" <<std::endl;
	std::cout << "form <src> <dst> <wt>." << std::endl;
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		usage(argc, argv);
	}
	else {
		try {
			std::vector<weighted_edge_t> edges = read_graph(argv[1]);
			unsigned int n_vertices = get_vertex_count(edges);
			double min_cost = TSP(edges, n_vertices);
			if(min_cost == std::numeric_limits<double>::infinity()) {
				std::cout << "No Hamiltonian cycle exists." << std::endl;
			}
			else {
				std::cout << min_cost << std::endl;
			}
		}
		catch (std::exception &ex) {
			std::cerr << ex.what() << std::endl;
		}
	}

	return 0;
}
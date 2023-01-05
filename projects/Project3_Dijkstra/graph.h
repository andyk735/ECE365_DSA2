#ifndef _GRAPH_H
#define _GRAPH_H

#include <list>
#include <iostream>
#include <fstream>
#include <climits>
#include <iterator>
#include "hash.h"
#include "heap.h"

class graph{

	public:
        //
        // graph - The constructor will create a new hashTable large enough to fit all nodes and initializes the capacity to zero
        //
        graph();

        //
		// createGraph - Parses through each word/line of the input file, checks and define nodes into nodeList and graph hashTable,
        //               then designates the cost and destination for the corresponding edge as well as the adjacent list
		//
		void createGraph(const std::string &input_file);

		//
		// validNode - Checks if the graphHash contains the user-defined starting node. If it is in graphHash, returns true. If it isn't, returns false.
		//
		bool validNode(const std::string &s);

		//
		// dijkstra - Performs Dijkstra's Algorithm on the created graph for the starting node input
		//
		void dijkstra(const std::string &start);

		//
		// printPaths - Output the minimum distance and path to each node using the data from nodeList and path
		//
		void outputPaths(const std::string &output_file);

	private:
        int capacity; // size of graph
        class edge;
        class node;

        // edge class
        class edge {
        public:
            int cost;       // edge cost
            node *dest;     // destination node
        };

        // node class
        class node {
            public:
                std::string name;               // name of node
                std::list<edge> adj;            // list of adjacent edges from node
                bool known;                     // if node has been found
                int dist;                       // distance of the node
                std::list<std::string> path;    // path to the node
        };

        std::list<node*> nodeList;      // list of nodes
        hashTable * graphHash;          // hashTable of nodes
};

#endif //_GRAPH_H
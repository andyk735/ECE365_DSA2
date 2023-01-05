#include "hash.h"
#include "heap.h"
#include "graph.h"
using namespace std;

//Graph Class Implementation

// The constructor will create a new hashTable large enough to fit all nodes and initializes the capacity to zero
graph::graph() {
    graphHash = new hashTable(500000);
    capacity = 0;
};

// Parses through each line of input file, checks and define nodes into nodeList and graph hashTable,
// then designates the cost and destination for the corresponding edge as well as the adjacent list
void graph::createGraph(const std::string &input_file) {
    node *sourceNode, *destNode;                    // define the nodes that correspond to first two words in input lines
	string line;
    edge tempEdge;

    // open input file and check for failure
    ifstream input;
    input.open(input_file);
    if(!input) {
        cout << "Input file failed to open." << endl;
        exit(EXIT_FAILURE);
    }

    // parse through each "word" of each line, and perform appropriate designations for each edge and corresponding nodes
	while (getline(input, line)){
		int start = 0;
		int end = 0;
		int i = 0;
		vector<string> edgeInput;

		// parse each row into source, destination, and cost and append it to edgeInput vector
		while(end != string::npos){
			end = line.find(" ", start);
			edgeInput.push_back(line.substr(start, end - start));
			start = end + 1;
			i++;
		}

        // check if source already exists, if not add it to nodeList and graphHash
        if (!graphHash->contains(edgeInput[0])) {
            sourceNode = new node;
            sourceNode->name = edgeInput[0];
            sourceNode->known = false;
            sourceNode->dist = INT_MAX;
            nodeList.push_back(sourceNode);
            graphHash->insert(edgeInput[0], sourceNode);
            capacity++;
        }
        else {
            sourceNode = (node *) graphHash->getPointer(edgeInput[0]);
        }

        // check if destination already exists, if not add it to nodeList and graphHash
        if (!graphHash->contains(edgeInput[1])) {
            destNode = new node;
            destNode->name = edgeInput[1];
            destNode->known = false;
            destNode->dist = INT_MAX;
            nodeList.push_back(destNode);
            graphHash->insert(edgeInput[1], destNode);
            capacity++;
        }
        else {
            destNode = (node *) graphHash->getPointer(edgeInput[1]);
        }

        // for the corresponding edge between the two nodes, set the cost and destination, and add the edge to the source's adjacent list
        tempEdge.cost = stoi(edgeInput[2]);
        tempEdge.dest = destNode;
        sourceNode->adj.push_back(tempEdge);
    }
    input.close();
};

// Checks if the graphHash contains the starting node. if it is in graphHash, returns true. if it isn't, returns false.
bool graph::validNode(const std::string &start) {
    return (graphHash->contains(start));
};

// Performs Dijkstra's Algorithm on the created graph for the starting node input
void graph::dijkstra(const std::string &start) {

    // create pointer to begin node and assigns it to the starting node pointer
    node *begin = (node *) graphHash->getPointer(start);

    // initializes begin node parameters, dist = 0 and adds it to path since it's the starting node
    begin->dist = 0;
    begin->path.push_back(start);

    // create minimum distance heap of size capacity for storing each node's minimum distance to the starting node, insert starting node
    heap minDist(capacity);
    minDist.insert(start, begin->dist, begin);

    // insert every node other than the starting node from nodeList into minDist heap
    for (list<node *>::const_iterator it = nodeList.begin(), end = nodeList.end(); it != end; ++it)
    {
        if ((*it)->name != start)
        {
            minDist.insert((*it)->name, (*it)->dist, *it);
        }
    }

    // use temporary node to store and utilize the minimum distance node(deleteMin)
    node *temp;
    for (int i = 0; i < capacity; ++i)
    {
        minDist.deleteMin(nullptr, nullptr, &temp);

        // iterate through tempNode's adjacency list and for each edge destination, evaluate the minimum distance
        for (list<edge>::const_iterator it = temp->adj.begin(), end = temp->adj.end(); it != end && temp->dist != INT_MAX; ++it)
        {
            // check if node is known and check if current distance is less than mininum distance
            if ((!it->dest->known) && (it->dest->dist > (it->cost + temp->dist)))
            {
                it->dest->path.clear();
                it->dest->path.insert(it->dest->path.begin(), temp->path.begin(), temp->path.end());
                it->dest->path.push_back(it->dest->name);               // append the current adjacent node to the path since it has minimum distance

                // set destination's distance to be the edge cost + minimum distance node
                it->dest->dist = (it->cost + temp->dist);
                minDist.setKey(it->dest->name, (it->cost + temp->dist));
            }
        }
        // set tempNode to be known now that the minimum distance has been found
        temp->known = true;
    }
};

// Output the minimum distance and path to each node using the data from nodeList and path
void graph::outputPaths(const std::string &output_file) {
    string line;
    edge tempEdge;

    // open output file and check for failure
    ofstream output;
    output.open(output_file);
    if(!output) {
        cout << "Output file failed to open." << endl;
        exit(EXIT_FAILURE);
    }

    // iterate through nodeList and output the node names, minimum distance, and path
    for (std::list<node*>::const_iterator it = nodeList.begin(), end = nodeList.end(); it != end; ++it) {
        output << (*it)->name << ": ";

        // check if there is no path to the node
        if ((*it)->dist == INT_MAX) {
            output << "NO PATH" << std::endl;
        }

        // proper formatting for assignment
        else {
            output << (*it)->dist << " [";
            // iterature through the path and output with appropriate commas
            for (std::list<std::string>::const_iterator it2 = (*it)->path.begin(), end2 = (*it)->path.end(); it2 != end2; ++it2) {
    output << *it2;
                if (std::next(it2, 1) != end2) output << ", ";
            }
            output << "]" << std::endl;
        }
    }
    output.close();
};
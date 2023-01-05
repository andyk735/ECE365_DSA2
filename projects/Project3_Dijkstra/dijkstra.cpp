/*

DSA 2: Section 2 - Nov. 15
Program 3 - Dijkstra's Algorithm
Andy Kim

*/

#include <time.h>
#include "graph.h"

using namespace std;

// main function
int main()
{
    clock_t begin, stop;                        // used for hash table load time
    string input_file, output_file, start;      // used for filestream
    graph g;                                    // create graph g

    // user input for graph input file
    cout << "Enter name of graph file: ";
    cin >> input_file;

    g.createGraph(input_file);       // build graph from each line of the input_file

    // check if the starting node is a valid node in the graph
    while(!g.validNode(start)) {
        cout << "Enter name of starting vertex: ";
        cin >> start;
    }

    begin = clock();
    g.dijkstra(start);              // execute and time Dijkstra's Algorithm on the created graph
    stop = clock() - begin;

    cout << "Total time (in seconds) to apply Dijkstra's Algorithm: " << ((double)stop)/CLOCKS_PER_SEC << endl;      // output document spell check time

    cout << "Enter name of output file: ";
    cin >> output_file;

    g.outputPaths(output_file);     // output the minimum distances and paths from the starting node to every other nodes

    return 0;
}
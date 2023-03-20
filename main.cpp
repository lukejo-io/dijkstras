/* Dustin Scott <dustin001@live.missouristate.edu>
 * Luke Johnson <edward212@live.missouristate.edu>

   CSC232 Assignment 5
   Dijkstra's Algorithm Assignment;

 Question 1:
 To compute the shortest path from every vertex to every other vertex, we would loop through
 all the vertices, setting each vertex as the start node and calling Dijkstra's again.

 Question 2:
 Running time would be O(n * nm) or O(n^2 * m). In other words, we would be running Dijkstra's
 n times more than we are in this version.

*/


#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

int INFINITE = 1000000;

/*
 * The following function accepts an input file stream
 * by reference and will parse the data in that stream,
 * placing the data from the file input stream into a
 * map-of-vectors-of-pairs and returning the map.
*/
std::map<int, std::vector<std::pair<int, int>>> parseFileStream(std::ifstream& file_stream_name);

/*
 * The following function accepts a map-of-vectors-of-pairs,
 * a vector of bools, and a vector of distances, each of
 * which are passed by reference. The shortest path from
 * vertex 1 to vertex v is stored in distances[v].
 */

void dijkstra(std::map<int, std::vector<std::pair<int, int>>> &map_name, std::vector<bool> &visited_vertices, std::vector<int> &distances);

/*
 * The following helper function will return true if all,
 * vertices in visited_vertices have been visited. This
 * signals the dijkstra function to end processing.
 */
bool allVerticesProcessed(std::map<int, std::vector<std::pair<int, int>>> &map_name, std::vector<bool> &processed_vertices);


//BEGIN MAIN-----------------------------------------------------------------------------------------
int main() {

    //ask for filename:
    std::cout << "Filename for adjacency list:" << std::endl;
    std::string file_name;
    std::cin >> file_name;

    //attempt to create stream from file:
    std::ifstream adjacency_list_stream(file_name);
    if (!adjacency_list_stream.is_open()) {
        std::cout << "\nCould not open the file \"" << file_name << "\". Check spelling/path."
                  << std::endl;
        exit(1);
    }

    std::map<int, std::vector<std::pair<int, int>>> adjacency_list; //create map-of-vector-of-pairs
    adjacency_list = parseFileStream(adjacency_list_stream);        //parse file stream/populate map

    std::vector<bool> processed_vertices{0}; //vector for identifying processed vertices
    std::vector<int> distances {INFINITE};   //vector for keeping track of shortest distance for each vertex

    dijkstra(adjacency_list, processed_vertices, distances); //call dijkstra function to process data

    return 0;
}
//END MAIN-------------------------------------------------------------------------------------------



std::map<int, std::vector<std::pair<int, int>>> parseFileStream(std::ifstream &file_stream_name) {
    std::map<int, std::vector<std::pair<int, int>>> map_name;
    std::string current_line;
    char test_for_comma, test_for_digit;
    int key, destination_vector, weight;

    while (file_stream_name) {

        while (std::getline(file_stream_name, current_line)) {
            std::stringstream current_line_stream(current_line);
            bool another_pair_exists{1};

            current_line_stream >> key;

            while (another_pair_exists) {
                current_line_stream >> destination_vector >> test_for_comma;

                if (test_for_comma != ',') {
                    std::cout << std::endl;
                    std::cout << "The file is not formatted as expected.\nIt should be formatted as follows:"
                              << std::endl;
                    std::cout << "<source_vector> <destination_vector>,<weight> <2nd_destination_vector>,<weight>"
                              << "                                              <3rd_destina...>" << std::endl;
                    exit(1);
                }
                current_line_stream >> weight;

                //add newly found pair to map:
                auto p = std::make_pair(destination_vector, weight);
                map_name[key].push_back(p);

                current_line_stream >> std::ws; //eat up whitespace
                test_for_digit = 'a'; //reset test_for_digit holder.
                current_line_stream >> test_for_digit;

                if (isdigit(test_for_digit)) {
                    another_pair_exists = true;
                    current_line_stream.putback(test_for_digit);
                } else another_pair_exists = false;
            }
        }
    }
    return map_name;
}

void dijkstra(std::map<int, std::vector<std::pair<int, int>>> &map_name, std::vector<bool> &processed_vertices, std::vector<int> &distances) {

    //resize vectors to match number of vertices;
    processed_vertices.resize(map_name.size()+1);
    distances.resize(map_name.size()+1);

    //set all processed vertices to false:
    for(int i = 1; i < processed_vertices.size()+1; i++){
        processed_vertices[i] = false;
    }

    //set all distances to infinite:
    for(int j = 1; j < map_name.size() + 1; j++){
        distances[j] = INFINITE;
    }

    processed_vertices[0] = true; //mark 0 as processed (not using 0)
    processed_vertices[1] = true; //mark 1 as processed
    distances[0] = 0; //unused distance element;
    distances[1] = 0; //distance to source vertex is always 0;

    while (! allVerticesProcessed(map_name, processed_vertices)) { //while not all vertices are processed
         int min = INFINITE;
         int shortest_edge_i_index=0;
         int shortest_edge_j_index=0;

        for(int i = 1; i < map_name.size()+1; i++) { //for each vertex in map
            if(processed_vertices[i]) {//check we are only looking at processed vertices
                for(int j = 0; j < map_name[i].size(); j++) {//for each edge from processed vertices
                    if(!processed_vertices[map_name[i][j].first]) { //if connected vertex has not been processed
                        int dist_to_get_here = map_name[i][j].second + distances[i];
                        if(dist_to_get_here <= min) {//if the this is the shortest edge out of all processed vertices so far
                            shortest_edge_i_index = i;
                            shortest_edge_j_index = j;
                            min = dist_to_get_here;
                        }
                    }
                }
            }
        }
        processed_vertices[map_name[shortest_edge_i_index][shortest_edge_j_index].first] = true;
        distances[map_name[shortest_edge_i_index][shortest_edge_j_index].first] = min;

        //test to see if all vertices have been processed:


    }//end while loop

    for (int i = 1; i < map_name.size()+1; ++i) {
        std::cout << distances[i];
        if (i != map_name.size()) {
            std::cout << ",";
        }
    }
    std::cout << std::endl;
}

bool allVerticesProcessed(std::map<int, std::vector<std::pair<int, int>>> &map_name, std::vector<bool> &processed_vertices) {
    //std::cout << "processed vertices size: " << processed_vertices.size() << std::endl;


    for (int k = 0; k < map_name.size(); ++k) {
        if (processed_vertices[k] == false) {   //if at least 1 vertex hasn't been processed, we are not done.
            return false;
        }
    }
    return true;
}
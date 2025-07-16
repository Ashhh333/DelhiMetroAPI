#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <algorithm>
#include <set>
#include "delhimetro.h"
#include <map>
#include <fstream>      // ✅ for std::ifstream, std::ofstream
#include <sstream>  
using namespace std;
const int INF = numeric_limits<int>::max();
const int NUM_STATIONS = 195;

int Time;
 vector<vector<int>> distanceMatrix(NUM_STATIONS, vector<int>(NUM_STATIONS, INF));
unordered_map<int,int>lineIndexMap;
vector<int>flag(NUM_STATIONS,-1);
unordered_map<string,int>stationIndexMap;
vector<string>Station_name(1,"lodu");
//*************************************DIJSKTRA IMPLEMENTATION******************************************************************************/////////
vector<string> find_shortest_path(string  sour,string dest){
    vector<int>Shortest_Route;
    for (int i = 0; i < NUM_STATIONS; ++i) {
        distanceMatrix[i][i] = 0;
    }
     
       int source=stationIndexMap[sour];
      int destination=stationIndexMap[dest];
   cout<<dest;
priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
vector<int>Distance(NUM_STATIONS,INF);
 vector<int>Path(NUM_STATIONS,0);
pq.push({0,source});
Path[0]=source;
Distance[source]=0;
int chck=0;
int parent_line=0;
while(pq.size()){
       
        int node=pq.top().second;
     
        if(node==destination) break;
        int diss=pq.top().first;
        pq.pop();
        for(int i=0;i<NUM_STATIONS;i++){
        int newn=i;
        int newd=distanceMatrix[node][i];
        if(newd!=INF){
       
          int child_line=lineIndexMap[newn];
           int parent_line=lineIndexMap[Path[node]];
         int posd;
       
            if(parent_line!=child_line&&flag[node]==1){
                
                if(node==116||node==154)
                        posd=newd+diss+5;
                        else posd=newd+diss+10;
            }
      
        else{ posd=newd+diss;}
        
        if(Distance[newn]>posd){
        Distance[newn]=posd;
        pq.push({posd,newn});
        Path[newn]=node;
                     } 
            }  
        }
      
}     

int i=destination;Shortest_Route.push_back(destination);
while(i!=source){
Shortest_Route.push_back(Path[i]);
i=Path[i];
}
reverse(Shortest_Route.begin(),Shortest_Route.end());

Time=Distance[destination];
int len=Shortest_Route.size();
vector<string>path(len);
for(int i=0;i<len;i++){
    path[i]=Station_name[Shortest_Route[i]];
}
return path;


}
void Interchange(){
    cout<<"inte";
     std::ifstream file("interchange.csv");

    if (!file.is_open()) {
        std::cerr << "Error: Could not open interchange_stations.csv" << std::endl;
   
    }

    std::string line;
    std::getline(file, line);  // Read the entire line

    std::stringstream ss(line);
    std::string station;


    while (std::getline(ss, station, ',')) {
        // Trim leading/trailing spaces
        size_t start = station.find_first_not_of(" \t");
        size_t end = station.find_last_not_of(" \t");
        station = (start == std::string::npos) ? "" : station.substr(start, end - start + 1);

        if (!station.empty()) {
            flag[stationIndexMap[station]]=1;
        }
    }

    file.close();
}
//***********************************************************************************************************************************************************//
//**************************************LOADING STATIONS FROM STATIONS.CSV*********************************************************************/////
void  Load_stations(){
    cout<<"load";
        std::ifstream file("metro_lines2.csv");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open metro_lines.csv" << std::endl;
      
    }
        std::string line;
    int stationId = 1;

    // Read header
    std::getline(file, line);
    std::vector<std::string> lineNames;
    std::stringstream headerStream(line);
    std::string column;
    while (std::getline(headerStream, column, ',')) {
        lineNames.push_back(column);
    }

    // Read each line (row-wise stations under each line)
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string station;
        for (int col = 0; col < lineNames.size(); ++col) {
            if (!std::getline(ss, station, ',')) break;

            // Clean extra whitespace
            while (!station.empty() && (station.back() == '\r' || station.back() == ' '))
                station.pop_back();

            if (!station.empty()) {
                stationIndexMap[station] = stationId;
                Station_name.push_back(station);
                lineIndexMap[stationId] = col; 
                stationId++; // Yellow:0, Magenta:1, etc.
            }
        }
    }

    file.close();
}
//******************************************************************************************************************************************************//
///*********************************************LOADING CONNECTIONS AND FORMATION OF DISTANCE_MATRIX*******************************************
void loadConnections(const string& filename ) {
    cout<<"com";
    ifstream inFile(filename);
    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            stringstream ss(line);
            string station1, station2, distanceStr;
            getline(ss, station1, ',');
            getline(ss, station2, ',');
            getline(ss, distanceStr, ',');
             station1.erase(0, station1.find_first_not_of(" \t\n\r\f\v"));
            station1.erase(station1.find_last_not_of(" \t\n\r\f\v") + 1);
            station2.erase(0, station2.find_first_not_of(" \t\n\r\f\v"));
            station2.erase(station2.find_last_not_of(" \t\n\r\f\v") + 1);
            int distance = stoi(distanceStr);
               if (stationIndexMap.find(station1) == stationIndexMap.end()) {
                cerr << "Station " << station1 << " not found in station index map!" << endl;
                continue; // Skip this line
            }
            if (stationIndexMap.find(station2) == stationIndexMap.end()) {
                cerr << "Station " << station2 << " not found in station index map!" << endl;
                continue; // Skip this line
            }
            int idx1 = stationIndexMap.at(station1);
            int idx2 = stationIndexMap.at(station2);
           distanceMatrix[idx1][idx2] = distance;
            distanceMatrix[idx2][idx1] = distance; // bidirectional connections
        }
    }
    inFile.close();
}

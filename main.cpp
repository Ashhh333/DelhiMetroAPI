#define CROW_MAIN
#include "crow_all.h"
#include "delhimetro.h"
#include <json/json.h>
#include <string>

// Add this function somewhere before main()
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

int main() {
    


    crow::SimpleApp app;
     Load_stations();
    loadConnections("connections.csv");
    Interchange();
CROW_ROUTE(app, "/shortest_path")
([](const crow::request& req) {
    std::cout<<"ROute hit";
     // remove if just for testing
auto source_raw = req.url_params.get("source");
auto destination_raw = req.url_params.get("destination");

std::string source = source_raw ? trim(source_raw) : "";
std::string destination = destination_raw ? trim(destination_raw) : "";

crow::json::wvalue response;  // ✅ declare before using

if (source.empty() || destination.empty()) {
    response["error"] = "Missing source or destination parameter";
    return crow::response(400, std::move(response));
}

    std::vector<std::string> path = find_shortest_path(source, destination);
   
    crow::json::wvalue pathArray;
    for (size_t i = 0; i < path.size(); ++i) {
        pathArray[i] = path[i];
    }
   
    response["source"] = source;
    response["destination"] = destination;
    response["path"] = std::move(pathArray);  // ✅ use move if necessary

    return crow::response(200, std::move(response));  // ✅ use std::move
});

    std::cout << "Server running at http://localhost:18080\n";
    app.port(18080).multithreaded().run();
}

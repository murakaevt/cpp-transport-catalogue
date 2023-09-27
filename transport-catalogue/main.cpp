#include "request_handler.h"
#include "json_reader.h"
#include "map_renderer.h"

#include <fstream>

using namespace std::literals;

int main() {
    
    std::ifstream data_in("s12_final_opentest_2.json");
    std::ofstream data_out("s12_final_opentest_2.json_output.json");
    transportcatalogue::TransportCatalogue transport_catalogue;
    json_reader::ParsingOfRequest(transport_catalogue, data_in, data_out);   
}
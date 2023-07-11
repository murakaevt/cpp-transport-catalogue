#include "request_handler.h"
#include "json_reader.h"
#include "map_renderer.h"

#include <fstream>

using namespace std::literals;

int main() {
	std::ifstream data_in("input.json");
	std::ofstream data_out("output.json");
	transportcatalogue::TransportCatalogue transport_catalogue;
	json_reader::ParcingOfRequest(transport_catalogue, data_in, data_out);
}
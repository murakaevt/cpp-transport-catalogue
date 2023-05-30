#include "stat_reader.h"
#include "input_reader.h"
#include "transport_catalogue.h"

#include <fstream>

using namespace std::literals;

int main() {
	//std::ifstream data_in("data_input_1986.txt");
	//std::ifstream data_out("data_output_2000.txt");
	transportcatalogue::TransportCatalogue transport_catalogue;
	transportcatalogue::detail::AddInfoInCatalogue(transport_catalogue, std::cin);
	//std::ostream out(std::cout.rdbuf());
	transportcatalogue::detail::OutputReqFunction(transport_catalogue, std::cin, std::cout);
	
}
#include "stat_reader.h"
#include "input_reader.h"
#include "transport_catalogue.h"

#include <fstream>

using namespace std::literals;

int main() {
	transportcatalogue::TransportCatalogue tran_cat;
	int counter_add = 0;
	std::cin >> counter_add;
	std::cin.ignore();
	std::vector<transportcatalogue::detail::InputData> datas = transportcatalogue::detail::DataMaker(counter_add);
	std::unordered_map<std::tuple<std::string, double, double>, std::unordered_map<std::string, uint32_t>, transportcatalogue::StopsMakerHasher> stops = transportcatalogue::detail::StopsMaker(datas);
	std::unordered_map<std::string, std::vector<std::string>> routes = transportcatalogue::detail::RoutesMaker(datas);
	tran_cat.AddStops(stops);
	tran_cat.AddRoutes(routes);

	int counter_out = 0;
	std::cin >> counter_out;
	std::cin.ignore();
	transportcatalogue::detail::OutputReqFunction(tran_cat, counter_out);

}
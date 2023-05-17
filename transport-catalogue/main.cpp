#include "transport_catalogue.h"
#include "stat_reader.h"
#include <fstream>

using namespace std::literals;

int main() {
	transportcatalogue::TransportCatalogue tran_cat;
	int counter_add = 0;
	std::cin >> counter_add;
	std::cin.ignore();
	tran_cat.Add(counter_add);

	int counter_out = 0;
	std::cin >> counter_out;
	std::cin.ignore();
	transportcatalogue::detail::OutputReqFunction(tran_cat, counter_out);

}
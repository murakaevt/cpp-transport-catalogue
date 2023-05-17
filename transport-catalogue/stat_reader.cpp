#include "stat_reader.h"
using namespace std::literals;
namespace transportcatalogue::detail {
	void OutputReqFunction(const TransportCatalogue& catalogue, int& counter) {
		int t = 1;
		std::string bus_or_stop_req;
		while (t <= counter) {
			t += 1;
			std::string bus_or_stop{};
			int type_of_req_number = 0;
			std::getline(std::cin, bus_or_stop_req);
			const auto type_of_req_begin = bus_or_stop_req.find_first_not_of(' ');
			const auto type_of_req_end = bus_or_stop_req.find_first_of(' ', type_of_req_begin);
			size_t type_of_req_distance = type_of_req_end - type_of_req_begin;
			std::string type_of_req = bus_or_stop_req.substr(type_of_req_begin, type_of_req_distance);
			if (type_of_req == "Bus") {
				type_of_req_number = 1;
				const auto bus_number_begin = bus_or_stop_req.find_first_not_of(' ', type_of_req_end);
				const auto bus_number_end = bus_or_stop_req.find_last_not_of(' ', bus_or_stop_req.npos);
				size_t bus_number_distance = bus_number_end - bus_number_begin + 1;
				bus_or_stop = bus_or_stop_req.substr(bus_number_begin, bus_number_distance);
			}
			else if (type_of_req == "Stop") {
				type_of_req_number = 2;
				const auto stop_name_begin = bus_or_stop_req.find_first_not_of(' ', type_of_req_end);
				const auto stop_name_end = bus_or_stop_req.find_last_not_of(' ', bus_or_stop_req.npos);
				size_t stop_name_distance = stop_name_end - stop_name_begin + 1;
				bus_or_stop = bus_or_stop_req.substr(stop_name_begin, stop_name_distance);
			}
			else {
				throw std::invalid_argument("Wrong!");
			}
			catalogue.Output(bus_or_stop, type_of_req_number);
		}
		return;
	}
}
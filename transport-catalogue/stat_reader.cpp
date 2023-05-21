#include "stat_reader.h"
using namespace std::literals;
namespace transportcatalogue::detail {

	void OutputBus(const std::string& bus, const TransportCatalogue& tran_cat) {
		if (tran_cat.GetBookOfRoutes().find(bus) != tran_cat.GetBookOfRoutes().end()) {

			std::cout << "Bus "s << tran_cat.GetBookOfRoutes().at(bus)->name << ": "s
				<< tran_cat.GetRouteSize(bus) << " stops on route, "s
				<< tran_cat.GetUniqueStopsCount(bus) << " unique stops, "s
				<< tran_cat.ComputeRouteRealDistance(bus) << " route length, "s
				<< tran_cat.ComputeCurvature(bus) << " curvature"s << std::endl;
		}
		else {
			std::cout << "Bus "s << bus << ": "s << "not found"s << std::endl;
		}
	}

	void OutputStop(const std::string& stop, const TransportCatalogue& tran_cat) {

		if (tran_cat.GetStopBuses().find(stop) != tran_cat.GetStopBuses().end()) {
			if (tran_cat.GetStopBuses().at(stop).empty()) {
				std::cout << "Stop "s << stop << ": no buses"s << std::endl;
			}
			else {
				std::cout << "Stop "s << stop << ": buses"s;
				for (const auto& bus : tran_cat.GetStopBuses().at(stop)) {
					std::cout << " "s << bus;
				}
				std::cout << std::endl;
			}
		}
		else {
			std::cout << "Stop "s << stop << ": not found"s << std::endl;
		}
	}

	void OutputReqFunction(const TransportCatalogue& catalogue, int& counter) {
		int t = 1;
		std::string bus_or_stop_req;
		while (t <= counter) {
			t += 1;

			std::string bus_or_stop{};
			int type_of_req_number = 0;
			std::getline(std::cin, bus_or_stop_req);

			size_t pos_for_next = 0;
			std::string type_of_req = StringMaker("  ", bus_or_stop_req, pos_for_next, pos_for_next);

			if (type_of_req == "Bus") {

				size_t name_of_bus_begin = bus_or_stop_req.find_first_not_of(' ', pos_for_next);
				size_t name_of_bus_end = bus_or_stop_req.find_last_not_of(' ', bus_or_stop_req.npos);
				size_t distance_name_of_bus = name_of_bus_end - name_of_bus_begin + 1;
				bus_or_stop = bus_or_stop_req.substr(name_of_bus_begin, distance_name_of_bus);
				OutputBus(bus_or_stop, catalogue);
			}
			else if (type_of_req == "Stop") {

				size_t name_of_stop_begin = bus_or_stop_req.find_first_not_of(' ', pos_for_next);
				size_t name_of_stop_end = bus_or_stop_req.find_last_not_of(' ', bus_or_stop_req.npos);
				size_t distance_name_of_stop = name_of_stop_end - name_of_stop_begin + 1;
				bus_or_stop = bus_or_stop_req.substr(name_of_stop_begin, distance_name_of_stop);
				OutputStop(bus_or_stop, catalogue);
			}
			else {
				throw std::invalid_argument("Wrong type of request!");
			}
		}
		return;
	}
}
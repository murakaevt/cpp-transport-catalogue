#include "stat_reader.h"

using namespace std::literals;
namespace transportcatalogue::detail {

	std::string StringMakerToo(const std::string& simbols, const std::string& inner, const size_t& pos) {
		size_t name_begin = inner.find_first_not_of(simbols, pos);
		size_t name_end = inner.find_last_not_of(simbols, inner.npos);
		size_t distance_of_name = name_end - name_begin + 1;
		std::string bus_or_stop = inner.substr(name_begin, distance_of_name);
		return bus_or_stop;
	}

	void OutputBus(const std::string& bus, const TransportCatalogue& transport_catalogue, std::ostream& out) {
		if (transport_catalogue.FindBus(bus) != nullptr) {
			domain::RouteInfo route_info = transport_catalogue.GetRouteInfo(bus);
			out << "Bus "s << transport_catalogue.FindBus(bus)->name << ": "s
				<< route_info.stops_on_route << " stops on route, "s
				<< route_info.unique_stops << " unique stops, "s
				<< route_info.route_length << " route length, "s
				<< route_info.curvature << " curvature"s << std::endl;
		}
		else {
			out << "Bus "s << bus << ": "s << "not found"s << std::endl;
		}
	}

	void OutputStop(const std::string& stop, const TransportCatalogue& transport_catalogue, std::ostream& out) {

		if (transport_catalogue.FindStop(stop) != nullptr) {
			if (transport_catalogue.FindBuses(stop).empty()) {
				out << "Stop "s << stop << ": no buses"s << std::endl;
			}
			else {
				out << "Stop "s << stop << ": buses"s;
				for (const auto& bus : transport_catalogue.FindBuses(stop)) {
					out << " "s << bus;
				}
				out << std::endl;
			}
		}
		else {
			std::cout << "Stop "s << stop << ": not found"s << std::endl;
		}
	}

	void OutputReqFunction(const TransportCatalogue& catalogue, std::istream& in, std::ostream& out) {
		int counter_out = 0;
		std::cin >> counter_out;
		std::cin.ignore();

		int t = 1;
		std::string bus_or_stop_req;
		while (t <= counter_out) {
			t += 1;

			std::string bus_or_stop{};
			int type_of_req_number = 0;
			std::getline(in, bus_or_stop_req);

			size_t pos_for_next = 0;
			std::string type_of_req = StringMaker("  ", bus_or_stop_req, pos_for_next, pos_for_next);

			if (type_of_req == "Bus") {

				std::string bus_or_stop = StringMakerToo(" ", bus_or_stop_req, pos_for_next);
				OutputBus(bus_or_stop, catalogue, out);
			}
			else if (type_of_req == "Stop") {

				std::string bus_or_stop = StringMakerToo(" ", bus_or_stop_req, pos_for_next);
				OutputStop(bus_or_stop, catalogue, out);
			}
			else {
				throw std::invalid_argument("Wrong type of request!");
			}
		}
		return;
	}
}
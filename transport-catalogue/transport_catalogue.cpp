#include "transport_catalogue.h"

using namespace std::literals;
namespace transportcatalogue {
	void TransportCatalogue::Add(int& counter) {
		std::vector<detail::InputData> datas;
		int t = 1;
		std::string text;
		while (t <= counter) {
			t += 1;
			std::getline(std::cin, text);
			detail::InputData data = detail::ReadInputFunction(text);
			datas.push_back(data);
		}
		for (auto& data : datas) {
			if (data.route_type == 0) {
				if (book_of_stops_.count(data.name) == 0) {
					stops_.push_back({ std::move(data.name), data.latitude, data.longitude });
					stop_buses_.insert({ stops_[stops_.size() - 1].name_of_stop, {} });
					book_of_stops_.insert({ stops_[stops_.size() - 1].name_of_stop, &stops_[stops_.size() - 1] });
				}
			}
			else {
				continue;
			}
		}
		for (auto& data : datas) {
			if (!data.distance_to.empty()) {
				TransportCatalogue::AddStopsDistance(data);
			}
			if (!data.bus.empty() && book_of_routes_.count(data.bus) == 0) {
				std::vector<Stop*> result;
				for (const auto& rou : data.route) {
					result.push_back(book_of_stops_[rou]);
				}
				if (data.route_type == 1) {
					int pos = result.size() - 2;
					while (pos >= 0) {
						result.push_back(result[pos]);
						pos -= 1;
					}
				}
				else if (data.route_type == 2) {

				}
				else {
					throw std::out_of_range("Wrong route type!");
				}
				routes_.push_back({ std::move(data.bus), std::move(result) });
				for (const auto& stop : routes_[routes_.size() - 1].route) {
					stop_buses_[stop->name_of_stop].insert(routes_[routes_.size() - 1].name_of_bus);
				}
				book_of_routes_.insert({ routes_[routes_.size() - 1].name_of_bus, &routes_[routes_.size() - 1] });
			}
		}
	}

	void TransportCatalogue::Output(const std::string& bus_or_stop, int& type_of_req_number) const {
		if (type_of_req_number == 1) {
			if (book_of_routes_.find(bus_or_stop) != book_of_routes_.end()) {
				std::unordered_set<Stop*> uniq;
				for (const auto& r : book_of_routes_.at(bus_or_stop)->route) {
					uniq.insert(r);
				}

				double route_distance = 0;
				for (int i = 0, j = 1; j < book_of_routes_.at(bus_or_stop)->route.size(); i++, j++) {
					double route_distance_buf = ComputeDistance(
						{ book_of_routes_.at(bus_or_stop)->route[i]->latitude,book_of_routes_.at(bus_or_stop)->route[i]->longitude },
						{ book_of_routes_.at(bus_or_stop)->route[j]->latitude,book_of_routes_.at(bus_or_stop)->route[j]->longitude }
					);
					route_distance += route_distance_buf;
				}
				uint32_t route_real_distance = 0;
				for (int i = 0, j = 1; j < book_of_routes_.at(bus_or_stop)->route.size(); i++, j++) {
					std::pair<Stop*, Stop*> para = { book_of_routes_.at(bus_or_stop)->route[i],
						book_of_routes_.at(bus_or_stop)->route[j] };
					route_real_distance += stops_distance_.at(para);
				}
				double curvature = static_cast<double>(route_real_distance) / route_distance;
				std::cout << "Bus "s << book_of_routes_.at(bus_or_stop)->name_of_bus
					<< ": "s << book_of_routes_.at(bus_or_stop)->route.size() << " stops on route, "s << uniq.size() << " unique stops, "s
					<< route_real_distance << " route length, "s << curvature << " curvature"s << std::endl;
			}
			else {
				std::cout << "Bus "s << bus_or_stop
					<< ": "s << "not found"s << std::endl;
			}
		}
		else if (type_of_req_number == 2) {
			if (stop_buses_.find(bus_or_stop) != stop_buses_.end()) {
				if (stop_buses_.at(bus_or_stop).empty()) {
					std::cout << "Stop "s << bus_or_stop << ": no buses"s << std::endl;
				}
				else {
					std::cout << "Stop "s << bus_or_stop << ": buses"s;
					for (const auto& bus : stop_buses_.at(bus_or_stop)) {
						std::cout << " "s << bus;
					}
					std::cout << std::endl;
				}
			}
			else {
				std::cout << "Stop "s << bus_or_stop << ": not found"s << std::endl;
			}
		}
		else {
			throw std::out_of_range("Wrong type_of_req_number!");
		}
	}

	void TransportCatalogue::AddStopsDistance(const detail::InputData& data) { //data.distance_to
		for (const auto& [to_stop, dist] : data.distance_to) {
			stops_distance_[{book_of_stops_.at(data.name), book_of_stops_.at(to_stop)}] = dist;
			if (stops_distance_.find({ book_of_stops_.at(to_stop), book_of_stops_.at(data.name) }) == stops_distance_.end()) {
				stops_distance_[{book_of_stops_.at(to_stop), book_of_stops_.at(data.name)}] = dist;
			}
		}
	}
}
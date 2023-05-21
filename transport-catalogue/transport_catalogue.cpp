#include "transport_catalogue.h"

using namespace std::literals;
namespace transportcatalogue {

	void TransportCatalogue::AddStops(std::unordered_map<std::tuple<std::string, double, double>, std::unordered_map<std::string, uint32_t>, transportcatalogue::StopsMakerHasher>& stops) {
		std::unordered_map<std::string_view, std::unordered_map<std::string, uint32_t>> bufer;
		for (auto& [stop, stops_to] : stops) {
			const auto& [name, lat, lng] = stop;
			stops_.push_back(Stop(name, lat, lng));
			book_of_stops_.insert({ stops_[stops_.size() - 1].name, &stops_[stops_.size() - 1] });
			stop_buses_.insert({ book_of_stops_[name]->name, {} });
			if (!stops_to.empty()) {
				bufer.insert({ book_of_stops_[name]->name , std::move(stops_to) });
			}
		}
		AddStopsDistance(bufer);
	}

	void TransportCatalogue::AddRoutes(std::unordered_map<std::string, std::vector<std::string>>& routes) {
		for (auto& [name, route] : routes) {
			std::vector<Stop*> route_buf;
			for (auto& stop : route) {
				route_buf.push_back(book_of_stops_[stop]);
			}
			routes_.push_back({ name, std::move(route_buf) });
			book_of_routes_.insert({ routes_[routes_.size() - 1].name, &routes_[routes_.size() - 1] });
			for (auto& stop : route) {
				stop_buses_[book_of_stops_[stop]->name].insert(book_of_routes_[name]->name);
			}
		}
	}



	size_t TransportCatalogue::GetRouteSize(std::string_view name_of_bus) const {
		return book_of_routes_.at(name_of_bus)->route.size();
	}

	size_t TransportCatalogue::GetUniqueStopsCount(std::string_view name_of_bus) const {
		std::unordered_set<Stop*> uniq;
		for (const auto& r : book_of_routes_.at(name_of_bus)->route) {
			uniq.insert(r);
		}
		return uniq.size();
	}

	uint32_t TransportCatalogue::GetStopsRealDistance(std::string_view from, std::string_view to) const {
		if (stops_distance_.find({ book_of_stops_.at(from), book_of_stops_.at(to) }) == stops_distance_.end()) {
			return 0;
		}
		return stops_distance_.at({ book_of_stops_.at(from), book_of_stops_.at(to) });
	}

	uint32_t TransportCatalogue::ComputeRouteRealDistance(std::string_view name_of_bus) const {
		uint32_t real_distance = 0;
		for (int i = 0, j = 1; j < book_of_routes_.at(name_of_bus)->route.size(); i++, j++) {
			std::string_view from = book_of_routes_.at(name_of_bus)->route[i]->name;
			std::string_view to = book_of_routes_.at(name_of_bus)->route[j]->name;
			real_distance += GetStopsRealDistance(from, to);
		}
		return real_distance;
	}

	double TransportCatalogue::ComputeGeoDistanceOfRoute(std::string_view name_of_bus) const {
		double route_distance = 0;
		for (int i = 0, j = 1; j < book_of_routes_.at(name_of_bus)->route.size(); i++, j++) {
			double route_distance_buf = ComputeDistance(
				book_of_routes_.at(name_of_bus)->route[i]->coordinates,
				book_of_routes_.at(name_of_bus)->route[j]->coordinates);
			route_distance += route_distance_buf;
		}
		return route_distance;
	}

	double TransportCatalogue::ComputeCurvature(std::string_view name_of_bus) const {
		return static_cast<double>(ComputeRouteRealDistance(name_of_bus)) / ComputeGeoDistanceOfRoute(name_of_bus);
	}

	const std::unordered_map<std::string_view, Bus*>& TransportCatalogue::GetBookOfRoutes() const {
		return book_of_routes_;
	}

	const std::unordered_map<std::string_view, std::set<std::string_view>>& TransportCatalogue::GetStopBuses() const {
		return stop_buses_;
	}

	void TransportCatalogue::AddStopsDistance(std::unordered_map<std::string_view, std::unordered_map<std::string, uint32_t>>& stops_distance) {
		for (auto& [stop, stops_dist] : stops_distance) {
			for (auto& [stop_to, dist] : stops_dist) {
				stops_distance_[{book_of_stops_.at(stop), book_of_stops_.at(stop_to)}] = dist;
				if (stops_distance_.find({ book_of_stops_.at(stop_to), book_of_stops_.at(stop) }) == stops_distance_.end()) {
					stops_distance_[{book_of_stops_.at(stop_to), book_of_stops_.at(stop)}] = dist;
				}
			}
		}
	}
}
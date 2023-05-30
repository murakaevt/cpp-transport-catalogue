#include "transport_catalogue.h"

using namespace std::literals;
namespace transportcatalogue {

	void TransportCatalogue::AddStops(const std::string& stop, const Coordinates& coordinates) {				
			stops_.push_back({stop, coordinates});
			book_of_stops_.insert({ stops_[stops_.size() - 1].name, &stops_[stops_.size() - 1] });
			stop_buses_.insert({ book_of_stops_[stop]->name, {} });						
	}

	void TransportCatalogue::AddStopsDistance(const std::string& name, const std::unordered_map<std::string, uint32_t>& stops_distance) {
		for (auto& [stop_to, dist] : stops_distance) {
				stops_distance_[{book_of_stops_.at(name), book_of_stops_.at(stop_to)}] = dist;
				if (stops_distance_.find({ book_of_stops_.at(stop_to), book_of_stops_.at(name) }) == stops_distance_.end()) {
					stops_distance_[{book_of_stops_.at(stop_to), book_of_stops_.at(name)}] = dist;
				}
			}
	}

	void TransportCatalogue::AddRoutes(const std::string& bus, const std::vector<std::string>& stops) {		
			std::vector<Stop*> route_buf;
			for (auto& stop : stops) {
				route_buf.push_back(book_of_stops_[stop]);
			}
			routes_.push_back({ bus, std::move(route_buf) });
			book_of_routes_.insert({ routes_[routes_.size() - 1].name, &routes_[routes_.size() - 1] });
			for (auto& stop : stops) {
				stop_buses_[book_of_stops_[stop]->name].insert(book_of_routes_[bus]->name);
			}
	}

	RouteInfo TransportCatalogue::GetRouteInfo(std::string_view name_of_bus) const {
		RouteInfo result;
		result.stops_on_route = GetRouteSize(name_of_bus);
		result.unique_stops = GetUniqueStopsCount(name_of_bus);
		result.route_length = ComputeRouteRealDistance(name_of_bus);
		result.curvature = ComputeCurvature(name_of_bus);
		return result;
	}	

	const Bus* TransportCatalogue::FindBus(const std::string& bus) const {
		if (book_of_routes_.find(bus) == book_of_routes_.end()) {
			return nullptr;
		}
		return book_of_routes_.at(bus);
	}

	const Stop* TransportCatalogue::FindStop(const std::string& stop) const{
		if (book_of_stops_.find(stop) == book_of_stops_.end()) {
			return nullptr;
		}
		return book_of_stops_.at(stop);
	}

	const std::set<std::string_view>& TransportCatalogue::FindBuses(const std::string& stop) const {		
		return stop_buses_.at(stop);
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
}
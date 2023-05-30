#pragma once

#include "geo.h"

#include <unordered_map>
#include <string>
#include <unordered_set>
#include <functional>
#include <set>
#include <deque>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <map>

namespace transportcatalogue {

	struct Stop {
		Stop(const std::string& name_t, double latit, double longit) {
			name = name_t;
			coordinates = { latit, longit };
		}
		Stop(const std::string& name_t, const Coordinates& coordinates_) 
		: name(name_t),
		  coordinates(coordinates_) {}

		std::string name;
		Coordinates coordinates;
		bool operator==(const Stop& other) const {
			return name == other.name && coordinates == other.coordinates;
		}
		bool operator!=(const Stop& other) const {
			return !(*this == other);
		}
	};

	struct Bus {
		std::string name;
		std::vector<Stop*> route;
	};

	struct StopsDistanceHasher {
		size_t operator () (const std::pair<Stop*, Stop*>& para) const {
			size_t first = std::hash<const void*>{}(para.first);
			size_t second = std::hash<const void*>{}(para.second);
			return first + second * 37;
		}
	};

	struct RouteInfo {
		size_t stops_on_route = 0;
		size_t unique_stops = 0;
		uint32_t route_length = 0;
		double curvature = 0;
	};

	class TransportCatalogue {
	public:
		TransportCatalogue() = default;

		void AddStops(const std::string& stop, const Coordinates& coordinates);

		void AddStopsDistance(const std::string& name, const std::unordered_map<std::string, uint32_t>& stops_distance);

		void AddRoutes(const std::string& bus, const std::vector<std::string>& stops);

		RouteInfo GetRouteInfo(std::string_view name_of_bus) const;		

		const Bus* FindBus(const std::string& bus) const;

		const Stop* FindStop(const std::string& stop) const;

		const std::set<std::string_view>& FindBuses(const std::string& stop) const;

	private:
		std::deque<Stop> stops_{};
		std::deque<Bus> routes_{};
		std::unordered_map<std::string_view, Stop*> book_of_stops_{};
		std::unordered_map<std::string_view, Bus*> book_of_routes_{};
		std::unordered_map<std::string_view, std::set<std::string_view>> stop_buses_;
		std::unordered_map<std::pair<Stop*, Stop*>, uint32_t, StopsDistanceHasher> stops_distance_;		

		size_t GetRouteSize(std::string_view name_of_bus) const;

		size_t GetUniqueStopsCount(std::string_view name_of_bus) const;

		uint32_t GetStopsRealDistance(std::string_view from, std::string_view to) const;

		uint32_t ComputeRouteRealDistance(std::string_view name_of_bus) const;

		double ComputeGeoDistanceOfRoute(std::string_view name_of_bus) const;

		double ComputeCurvature(std::string_view name_of_bus)const;
	};
}
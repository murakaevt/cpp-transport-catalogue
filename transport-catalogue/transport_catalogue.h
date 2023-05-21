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
			return first * 37 + second * 37 * 37;
		}
	};

	struct StopsMakerHasher {
		size_t operator () (const std::tuple<std::string, double, double>& stop) const {
			const auto& [name_, lat, lng] = stop;
			size_t name = std::hash<std::string>{}(name_);
			size_t coord_lat = std::hash<double>{}(lat);
			size_t coord_lng = std::hash<double>{}(lng);
			return name * 37 + coord_lat * 37 * 37 + coord_lng * 37 * 37 * 37;
		}
	};

	class TransportCatalogue {
	public:
		TransportCatalogue() = default;

		void AddStops(std::unordered_map<std::tuple<std::string, double, double>, std::unordered_map<std::string, uint32_t>, StopsMakerHasher>& stops);

		void AddRoutes(std::unordered_map<std::string, std::vector<std::string>>& routes);

		size_t GetRouteSize(std::string_view name_of_bus) const;

		size_t GetUniqueStopsCount(std::string_view name_of_bus) const;

		uint32_t GetStopsRealDistance(std::string_view from, std::string_view to) const;

		uint32_t ComputeRouteRealDistance(std::string_view name_of_bus) const;

		double ComputeGeoDistanceOfRoute(std::string_view name_of_bus) const;

		double ComputeCurvature(std::string_view name_of_bus)const;

		const std::unordered_map<std::string_view, Bus*>& GetBookOfRoutes() const;

		const std::unordered_map<std::string_view, std::set<std::string_view>>& GetStopBuses() const;

	private:
		std::deque<Stop> stops_{};
		std::deque<Bus> routes_{};
		std::unordered_map<std::string_view, Stop*> book_of_stops_{};
		std::unordered_map<std::string_view, Bus*> book_of_routes_{};
		std::unordered_map<std::string_view, std::set<std::string_view>> stop_buses_;
		std::unordered_map<std::pair<Stop*, Stop*>, uint32_t, StopsDistanceHasher> stops_distance_;

		void AddStopsDistance(std::unordered_map<std::string_view, std::unordered_map<std::string, uint32_t>>& stops_distance);
	};
}
#pragma once

#include "domain.h"

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
	
	struct StopsDistanceHasher {
		size_t operator () (const std::pair<domain::Stop*, domain::Stop*>& para) const {
			size_t first = std::hash<const void*>{}(para.first);
			size_t second = std::hash<const void*>{}(para.second);
			return first + second * 37;
		}
	};	

	class TransportCatalogue {
	public:
		TransportCatalogue() = default;

		void AddStops(const std::string& stop, const geo::Coordinates& coordinates);

		void AddStopsDistance(const std::string& name, const std::unordered_map<std::string, uint32_t>& stops_distance);

		void AddRoutes(const std::string& bus, const std::vector<std::string>& stops);

		domain::RouteInfo GetRouteInfo(std::string_view name_of_bus) const;

		const domain::Bus* FindBus(const std::string& bus) const;

		const domain::Stop* FindStop(const std::string& stop) const;

		const std::set<std::string_view>& FindBuses(const std::string& stop) const;

	private:
		std::deque<domain::Stop> stops_{};
		std::deque<domain::Bus> routes_{};
		std::unordered_map<std::string_view, domain::Stop*> book_of_stops_{};
		std::unordered_map<std::string_view, domain::Bus*> book_of_routes_{};
		std::unordered_map<std::string_view, std::set<std::string_view>> stop_buses_;
		std::unordered_map<std::pair<domain::Stop*, domain::Stop*>, uint32_t, StopsDistanceHasher> stops_distance_;

		size_t GetRouteSize(std::string_view name_of_bus) const;

		size_t GetUniqueStopsCount(std::string_view name_of_bus) const;

		uint32_t GetStopsRealDistance(std::string_view from, std::string_view to) const;

		uint32_t ComputeRouteRealDistance(std::string_view name_of_bus) const;

		double ComputeGeoDistanceOfRoute(std::string_view name_of_bus) const;

		double ComputeCurvature(std::string_view name_of_bus)const;
	};
}
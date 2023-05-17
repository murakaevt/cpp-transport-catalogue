#pragma once
#include "input_reader.h"
#include "geo.h"

#include<unordered_map>
#include<string>
#include<unordered_set>
#include<functional>
#include<set>
#include<deque>
#include<numeric>
#include <algorithm>
#include <fstream>

namespace transportcatalogue {
	struct Stop {
		Stop(const std::string& name, double latit, double longit) {
			name_of_stop = name;
			latitude = latit;
			longitude = longit;
		}
		std::string name_of_stop;
		double latitude = 0;
		double longitude = 0;
	};

	struct Bus {
		std::string name_of_bus;
		std::vector<Stop*> route;
	};

	struct StopsDistanceHasher {
		size_t operator () (const std::pair<Stop*, Stop*>& para) const {
			size_t first = std::hash<const void*>{}(para.first);
			size_t second = std::hash<const void*>{}(para.second);
			return first * 37 + second * 37 * 37;
		}
	};

	class TransportCatalogue {
	public:
		TransportCatalogue() = default;

		void Add(int& counter);

		void Output(const std::string& bus_or_stop, int& type_of_req_number) const;

		uint32_t GetStopsDistance(const std::string& from, const std::string& to) {
			if (stops_distance_.find({ book_of_stops_.at(from), book_of_stops_.at(to) }) == stops_distance_.end()) {
				return 0;
			}
			return stops_distance_.at({ book_of_stops_.at(from), book_of_stops_.at(to) });
		}
	private:
		std::unordered_set<std::string_view> unique_name_of_stops_;
		std::deque<Stop> stops_{};
		std::deque<Bus> routes_{};
		std::unordered_map<std::string_view, Stop*> book_of_stops_{};
		std::unordered_map<std::string_view, Bus*> book_of_routes_{};
		std::unordered_map<std::string_view, std::set<std::string_view>> stop_buses_;
		std::unordered_map<std::pair<Stop*, Stop*>, uint32_t, StopsDistanceHasher> stops_distance_;

		void AddStopsDistance(const detail::InputData& data);
	};
}
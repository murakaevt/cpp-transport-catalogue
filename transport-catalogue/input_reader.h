#pragma once
#include "transport_catalogue.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <unordered_map>
#include <map>
#include <tuple>


namespace transportcatalogue::detail {
	struct InputData {
		std::string name{};
		double latitude = 0;
		double longitude = 0;
		std::string bus{};
		std::vector<std::string> route{};
		int route_type = 0;
		std::unordered_map<std::string, uint32_t> distance_to;
	};

	std::string StringMaker(const std::string& simbols, const std::string& inner, size_t& next_pos, size_t pos);

	InputData ReadInputFunction(const std::string inner);

	std::vector<InputData> DataMaker(int counter);

	std::unordered_map<std::tuple<std::string, double, double>, std::unordered_map<std::string, uint32_t>, transportcatalogue::StopsMakerHasher> StopsMaker(std::vector<InputData>& datas);

	std::unordered_map<std::string, std::vector<std::string>> RoutesMaker(std::vector<InputData>& datas);
}
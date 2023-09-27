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
		std::string route_type{};
		std::unordered_map<std::string, uint32_t> distance_to;
	};

	std::string StringMaker(const std::string& simbols, const std::string& inner, size_t& next_pos, size_t pos);

	InputData ReadInputFunction(const std::string inner);

	std::vector<InputData> DataMaker(int counter, std::istream& in);

	void AddInfoInCatalogue(TransportCatalogue& catalogue, std::istream& in);
} // transportcatalogue::detail
#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include <stdexcept>
#include <iomanip>
#include<unordered_map>
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

	InputData ReadInputFunction(const std::string inner);
}
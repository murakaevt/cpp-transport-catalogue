#pragma once

#include "transport_catalogue.h"

namespace request_handler {
	struct InputData {
		std::string name{};
		double latitude = 0;
		double longitude = 0;
		std::string bus{};
		std::vector<std::string> route{};
		std::string route_type{};
		std::unordered_map<std::string, uint32_t> distance_to;
	};
	
	void AddInfoInCatalogue(transportcatalogue::TransportCatalogue& catalogue, const std::vector<InputData>& data_vector);

	domain::RouteInfo GetInfoAboutBus(transportcatalogue::TransportCatalogue& catalogue, const std::string& name);		
} // namespace request_handler
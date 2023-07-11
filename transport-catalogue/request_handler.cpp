#include "request_handler.h"

namespace request_handler {
	void AddInfoInCatalogue(transportcatalogue::TransportCatalogue& catalogue, const std::vector<InputData>& data_vector) {
		if (!data_vector.empty()) {
			for (const auto& data : data_vector) {
				if (!data.name.empty()) {
					catalogue.AddStops(data.name, { data.latitude, data.longitude });
				}
			}
		}
		for (auto& data : data_vector) {
			if (!data.name.empty() && !data.distance_to.empty()) {
				catalogue.AddStopsDistance(data.name, data.distance_to);
			}
			else if (!data.bus.empty()) {
				std::vector<std::string> route_buf = data.route;
				if (data.route_type == "not ring route") {
					int pos = route_buf.size() - 2;
					while (pos >= 0) {
						route_buf.push_back(route_buf[pos]);
						pos -= 1;
					}
					catalogue.AddRoutes(data.bus, route_buf);
				}
				else if (data.route_type == "ring route") {
					catalogue.AddRoutes(data.bus, route_buf);
				}
			}
		}
	}

	domain::RouteInfo GetInfoAboutBus(transportcatalogue::TransportCatalogue& catalogue, const std::string& name) {
		return catalogue.GetRouteInfo(name);
	}
} // namespace request_handler
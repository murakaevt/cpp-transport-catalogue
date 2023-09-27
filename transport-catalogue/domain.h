#pragma once
#include "geo.h"
#include "graph.h"

#include <string>
#include <vector>

namespace domain {
	struct Stop {
		Stop(const std::string& name_t, double latit, double longit, graph::VertexId vertex_id) {
			name = name_t;
			coordinates = { latit, longit };
			vertex_id_ = vertex_id;
		}

		Stop(const std::string& name_t, const geo::Coordinates& coordinates_, graph::VertexId vertex_id)
			: name(name_t),
			coordinates(coordinates_),
		    vertex_id_(vertex_id){}
	
			
		std::string name;
		geo::Coordinates coordinates;
		graph::VertexId vertex_id_;
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

	struct RouteInfo {
		size_t stops_on_route = 0;
		size_t unique_stops = 0;
		uint32_t route_length = 0;
		double curvature = 0;
	};
} // domain
#include "graph.h"
#include "router.h"
#include "transport_catalogue.h"

#include <vector>

namespace graph {
	struct RoutingSettings {
		size_t wait_time{};
		double velocity{};

		RoutingSettings(size_t bus_wait_time, double bus_velocity)
			: wait_time(bus_wait_time)
			, velocity(bus_velocity) {}
	};

	struct EdgeInfo {
		const domain::Stop* stop;
		const domain::Bus* bus;
		uint32_t count;
	};

	struct CompositeRoute {
		struct Line {
			const domain::Stop* stop;
			const domain::Bus* bus;
			double wait_time;
			double run_time;
			uint32_t count_stops;
		};
		double total_time;
		std::vector<Line> route;
	};

	class TransportRouter {
	public:
		TransportRouter(RoutingSettings settings, transportcatalogue::TransportCatalogue& catalogue)
			: wait_time_(settings.wait_time)
			, velocity_(settings.velocity)
			, catalogue_(catalogue)
			, graph_() {

			FillGraph();
		}

		std::optional<CompositeRoute> PerformRoute(graph::VertexId from, graph::VertexId to);


	private:
		size_t wait_time_;
		double velocity_;
		transportcatalogue::TransportCatalogue& catalogue_;
		graph::DirectedWeightedGraph<double> graph_;
		std::unordered_map<graph::EdgeId, EdgeInfo> edges_;
		std::unique_ptr<graph::Router<double>> router_;

		void FillGraph();
	};
} //namespace graph 
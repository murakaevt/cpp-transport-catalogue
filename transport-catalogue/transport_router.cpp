#include "transport_router.h"

namespace graph {

	void TransportRouter::FillGraph() {
		graph_.SetVertexCount(catalogue_.GetStopsQuantity() * 2);
		const double kmh_to_metrmin = 1000.0 / 60;
		double bus_velocity = velocity_ * kmh_to_metrmin;          //скорость автобуса в метрах/минуту

		for (const auto& bus : catalogue_.AllRoutes()) {						     // итерируемся по всем маршрутам(автобусам)			 
			auto it = bus.route.begin();														 // итератор на первую остановку что он проходит
			if (it == bus.route.end() || it + 1 == bus.route.end()) {						 // если остановок нет или она одна
				continue;																		 // продолжаем
			}
			for (; it + 1 != bus.route.end(); ++it) {										 // итерируемся по остановкам автобуса
				double time = wait_time_ * 1.0;													 // приводим время ожидания

				for (auto next_vertex = it + 1; next_vertex != bus.route.end(); ++next_vertex) {                // итератор на следующую остановку
					time += catalogue_.GetDistBetweenStops(*prev(next_vertex), *next_vertex) / bus_velocity;	   // вычисляем и добавляем время 
					//между соседними остановками к общему времени ребра

					graph::EdgeId id = graph_.AddEdge({ (*it)->vertex_id_,(*next_vertex)->vertex_id_, time });     // добавляем ребро, получаем его номер
					EdgeInfo info = { *it,&bus,static_cast<uint32_t>(next_vertex - it) };
					edges_[id] = info;
				}
			}
		}
		router_ = std::make_unique<graph::Router<double>>(graph_);
	}

	std::optional<CompositeRoute> TransportRouter::PerformRoute(graph::VertexId from, graph::VertexId to) {
		const double eps = 1e-6;

		std::optional<graph::Router<double>::RouteInfo> build_route_ = router_->BuildRoute(from, to);
		if (!build_route_) {
			return std::nullopt;
		}

		if (build_route_->weight < eps) {
			return CompositeRoute({ 0, {} });
		}

		CompositeRoute result;
		result.total_time = build_route_->weight;
		result.route.reserve(build_route_->edges.size());

		for (auto& edge : build_route_->edges)
		{
			EdgeInfo& info = edges_.at(edge);
			double wait_time = static_cast<double>(wait_time_);
			double run_time_ = graph_.GetEdge(edge).weight - wait_time_;
			result.route.push_back(CompositeRoute::Line{ info.stop, info.bus, wait_time, run_time_ ,info.count });

		}
		return result;
	}
}
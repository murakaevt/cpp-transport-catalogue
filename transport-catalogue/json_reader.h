#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "request_handler.h"
#include "map_renderer.h"
#include "json_builder.h"
#include "router.h"
#include "transport_router.h"

#include <sstream>

using namespace std::literals;
namespace json_reader {
	request_handler::InputData MakerInputData(const json::Dict& dict);

	std::vector<request_handler::InputData> ParsingOfBaseRequests(const json::Array& array);

	svg::Color DeterminantTypeOfColor(const json::Node node);

	map_renderer::RenderSettings ParsingRenderSettings (const json::Dict& dict);

	graph::RoutingSettings ParsingRoutingSettings(const json::Dict& dict);

	json::Node ParsingStatRequests (const json::Document& document, transportcatalogue::TransportCatalogue& catalogue, std::string& map_str, graph::TransportRouter& transport_router);

	void RenderRoute(std::map<std::string_view, std::pair<std::string, std::string>>& buses, transportcatalogue::TransportCatalogue& catalogue, map_renderer::RenderSettings& render_settings,
		map_renderer::SphereProjector& sphere_projector, svg::Document& objects_for_paint);

	std::map<std::string_view, std::pair<std::string, std::string>> MakerNamesOfBusesAndCoordinates (std::vector<request_handler::InputData>& input_data,
		transportcatalogue::TransportCatalogue& catalogue, std::vector<geo::Coordinates>& coordinates);

	void MakerTextBusBackgroundAndTitle (svg::Text& background, svg::Text& title, map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, std::string_view bus, geo::Coordinates coordinates, int counter_for_color);

	void InputerTextBusInSvgDocument (transportcatalogue::TransportCatalogue& catalogue, map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, std::map<std::string_view, std::pair<std::string, std::string>>& buses, svg::Document& objects_for_paint);

	std::vector<domain::Stop*> MakerVectorStops (transportcatalogue::TransportCatalogue& catalogue, std::map<std::string_view,
		std::pair<std::string, std::string>>& buses);

	void MakerCircleOfStops (svg::Circle& circle, map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, domain::Stop* stop);

	void InputerCircleInSvgDocument (map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, std::vector<domain::Stop*>& stops, svg::Document& objects_for_paint);

	void MakerTextStopBackgroundAndTitle (svg::Text& background, svg::Text& title, map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, domain::Stop* stop, geo::Coordinates& coordinates);

	void InputerTextStopInSvgDocument (map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, std::vector<domain::Stop*>& stops, svg::Document& objects_for_paint);

	void ParsingOfRequest(transportcatalogue::TransportCatalogue& catalogue, std::istream& in, std::ostream& out);
} // json_reader
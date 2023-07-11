#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "request_handler.h"
#include "map_renderer.h"

#include <sstream>

using namespace std::literals;
namespace json_reader {
	request_handler::InputData MakerInputData(const json::Dict& dict);

	std::vector<request_handler::InputData> ParcingOfBaseRequests(const json::Array& array);

	svg::Color DeterminantTypeOfColor(const json::Node node);

	map_renderer::RenderSettings RenderSettingsParcing(const json::Dict& dict);

	json::Array StatRequestsParcing(const json::Document& document, transportcatalogue::TransportCatalogue& catalogue, std::string& map_str);

	void RouteRender(std::map<std::string_view, std::pair<std::string, std::string>>& buses, transportcatalogue::TransportCatalogue& catalogue, map_renderer::RenderSettings& render_settings,
		map_renderer::SphereProjector& sphere_projector, svg::Document& objects_for_paint);

	std::map<std::string_view, std::pair<std::string, std::string>> NamesOfBusesAndCoordinatesMaker(std::vector<request_handler::InputData>& input_data,
		transportcatalogue::TransportCatalogue& catalogue, std::vector<geo::Coordinates>& coordinates);

	void TextBusBackgroundAndTitleMaker(svg::Text& background, svg::Text& title, map_renderer::SphereProjector& sphere_projector, 
		map_renderer::RenderSettings& render_settings, std::string_view bus, geo::Coordinates coordinates, int counter_for_color);

	void TextBusInputerInSvgDocument(transportcatalogue::TransportCatalogue& catalogue, map_renderer::SphereProjector& sphere_projector, 
		map_renderer::RenderSettings& render_settings, std::map<std::string_view, std::pair<std::string, std::string>>& buses, svg::Document& objects_for_paint);

	std::vector<domain::Stop*> VectorStopsMaker(transportcatalogue::TransportCatalogue& catalogue, std::map<std::string_view, 
		std::pair<std::string, std::string>>& buses);

	void CircleOfStopsMaker(svg::Circle& circle, map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, domain::Stop* stop);

	void CircleInputerInSvgDocument(map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, std::vector<domain::Stop*>& stops, svg::Document& objects_for_paint);

	void TextStopBackgroundAndTitleMaker(svg::Text& background, svg::Text& title, map_renderer::SphereProjector& sphere_projector, 
		map_renderer::RenderSettings& render_settings, domain::Stop* stop, geo::Coordinates& coordinates);

	void TextStopInputerInSvgDocument(map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, std::vector<domain::Stop*>& stops, svg::Document& objects_for_paint);

	void ParcingOfRequest(transportcatalogue::TransportCatalogue& catalogue, std::istream& in, std::ostream& out);
} // json_reader
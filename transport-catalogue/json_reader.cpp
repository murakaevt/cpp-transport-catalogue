#include "json_reader.h"

namespace json_reader {
	request_handler::InputData MakerInputData(const json::Dict& dict) {
		request_handler::InputData data{};
		if (dict.at("type").AsString() == "Stop") {
			data.name = dict.at("name").AsString();
			data.latitude = dict.at("latitude").AsDouble();
			data.longitude = dict.at("longitude").AsDouble();
			if (!dict.at("road_distances").AsMap().empty()) {
				for (const auto& [key, value] : dict.at("road_distances").AsMap()) {
					data.distance_to.insert({ key, value.AsInt() });
				}
			}
		}
		else if (dict.at("type").AsString() == "Bus") {
			data.bus = dict.at("name").AsString();
			if (!dict.at("stops").AsArray().empty()) {
				for (const auto& stop : dict.at("stops").AsArray()) {
					data.route.push_back(stop.AsString());
				}
			}
			if (dict.at("is_roundtrip").AsBool()) {
				data.route_type = "ring route";
			}
			else {
				data.route_type = "not ring route";
			}
		}
		return data;
	}

	std::vector<request_handler::InputData> ParcingOfBaseRequests(const json::Array& array) {
		std::vector<request_handler::InputData> data_vector;
		for (const auto& ar : array) {
			data_vector.push_back(MakerInputData(ar.AsMap()));
		}
		return data_vector;
	}

	svg::Color DeterminantTypeOfColor(const json::Node node) {
		if (node.IsString()) {
			return node.AsString();
		}
		else if (node.IsArray()) {
			if (node.AsArray().size() == 3) {
				return svg::Rgb{ node.AsArray()[0].AsInt(), node.AsArray()[1].AsInt(), node.AsArray()[2].AsInt() };
			}
			else {
				return svg::Rgba{ node.AsArray()[0].AsInt(), node.AsArray()[1].AsInt(), node.AsArray()[2].AsInt(), node.AsArray()[3].AsDouble() };
			}
		}
		return "none";
	}

	map_renderer::RenderSettings RenderSettingsParcing(const json::Dict& dict) {
		map_renderer::RenderSettings render_settings;
		render_settings.width = dict.at("width").AsDouble();
		render_settings.height = dict.at("height").AsDouble();
		render_settings.padding = dict.at("padding").AsDouble();
		render_settings.line_width = dict.at("line_width").AsDouble();
		render_settings.stop_radius = dict.at("stop_radius").AsDouble();
		render_settings.bus_label_font_size = dict.at("bus_label_font_size").AsInt();
		render_settings.bus_label_offset = {dict.at("bus_label_offset").AsArray()[0].AsDouble(), dict.at("bus_label_offset").AsArray()[1].AsDouble() };
		render_settings.stop_label_font_size = dict.at("stop_label_font_size").AsInt();
		render_settings.stop_label_offset = { dict.at("stop_label_offset").AsArray()[0].AsDouble(), dict.at("stop_label_offset").AsArray()[1].AsDouble() };
		render_settings.underlayer_color = DeterminantTypeOfColor(dict.at("underlayer_color"));
		render_settings.underlayer_width = dict.at("underlayer_width").AsDouble();
		for (const auto& color : dict.at("color_palette").AsArray()) {
			render_settings.color_palette.emplace_back(DeterminantTypeOfColor(color));
		}
		return render_settings;
	}

	json::Array StatRequestsParcing(const json::Document& document, transportcatalogue::TransportCatalogue& catalogue, std::string& map_str) {
		json::Array array;
		for (const auto& value : document.GetRoot().AsMap().at("stat_requests").AsArray()) {
			json::Dict info{};
			const auto& val = value.AsMap();
			info["request_id"] = json::Node{ val.at("id").AsInt() };
			if (val.at("type").AsString() == "Bus") {
				domain::RouteInfo info_of_bus = request_handler::GetInfoAboutBus(catalogue, val.at("name").AsString());
				if (info_of_bus.stops_on_route == 0) {
					info["error_message"] = json::Node{ static_cast<std::string>("not found") };
				}
				else {
					info["curvature"] = json::Node{ info_of_bus.curvature };
					info["route_length"] = json::Node{ static_cast<int>(info_of_bus.route_length) };
					info["stop_count"] = json::Node{ static_cast<int>(info_of_bus.stops_on_route) };
					info["unique_stop_count"] = json::Node{ static_cast<int>(info_of_bus.unique_stops) };
				}
			}
			else if (val.at("type").AsString() == "Stop") {
				json::Array buses;
				if (catalogue.FindStop(val.at("name").AsString()) == nullptr) {
					info["error_message"] = json::Node{ static_cast<std::string>("not found") };
				}
				else {
					for (const auto& bus : catalogue.FindBuses(val.at("name").AsString())) {
						buses.push_back(json::Node{ std::string{bus} });
					}
					info["buses"] = json::Node{ buses };
				}
			}
			else if (val.at("type").AsString() == "Map") {
				info["map"] = json::Node{ map_str };
			}
			array.push_back(json::Node{ info });
		}
		return array;
	}

	void RouteRender(std::map<std::string_view, std::pair<std::string, std::string>>& buses, transportcatalogue::TransportCatalogue& catalogue, map_renderer::RenderSettings& render_settings,
		map_renderer::SphereProjector& sphere_projector, svg::Document& objects_for_paint) {
		int counter_for_color{ 0 };
		for (const auto& [name, type_and_stop] : buses) {
			svg::Polyline route;
			for (const auto& stop : catalogue.FindBus(std::string(name))->route) {
				route.AddPoint(sphere_projector(stop->coordinates));
			}
			route.SetFillColor("none");
			route.SetStrokeColor(render_settings.color_palette[counter_for_color]);
			route.SetStrokeWidth(render_settings.line_width);
			route.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
			route.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
			objects_for_paint.Add(route);
			if (counter_for_color >= static_cast<int>(render_settings.color_palette.size() - 1)) {
				counter_for_color = 0;
			}
			else {
				++counter_for_color;
			}
		}
	}

	std::map<std::string_view, std::pair<std::string, std::string>> NamesOfBusesAndCoordinatesMaker(std::vector<request_handler::InputData>& input_data, 
		transportcatalogue::TransportCatalogue& catalogue, std::vector<geo::Coordinates>& coordinates) {
		std::map<std::string_view, std::pair<std::string, std::string>> buses;
		for (const auto& data : input_data) {
			if (!data.bus.empty()) {
				for (const auto& stop : catalogue.FindBus(data.bus)->route) {
					coordinates.emplace_back(stop->coordinates);
				}				
				buses.insert({ catalogue.FindBus(data.bus)->name, {data.route_type, data.route[data.route.size()-1]}});
			}
		}		
		return buses;
	}

	void TextBusBackgroundAndTitleMaker(svg::Text& background, svg::Text& title, map_renderer::SphereProjector& sphere_projector, 
		map_renderer::RenderSettings& render_settings, std::string_view bus, geo::Coordinates coordinates, int counter_for_color) {
		background.SetPosition(sphere_projector(coordinates));
		background.SetOffset({ render_settings.bus_label_offset[0], render_settings.bus_label_offset[1] });
		background.SetFontSize(render_settings.bus_label_font_size);
		background.SetFontFamily("Verdana");
		background.SetFontWeight("bold");
		background.SetData(std::string(bus));
		background.SetFillColor(render_settings.underlayer_color);
		background.SetStrokeColor(render_settings.underlayer_color);
		background.SetStrokeWidth(render_settings.underlayer_width);
		background.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
		background.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

		title.SetPosition(sphere_projector(coordinates));
		title.SetOffset({ render_settings.bus_label_offset[0], render_settings.bus_label_offset[1] });
		title.SetFontSize(render_settings.bus_label_font_size);
		title.SetFontFamily("Verdana");
		title.SetFontWeight("bold");
		title.SetData(std::string(bus));
		title.SetFillColor(render_settings.color_palette[counter_for_color]);
	}

	void TextBusInputerInSvgDocument(transportcatalogue::TransportCatalogue& catalogue, map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, std::map<std::string_view, std::pair<std::string, std::string>>& buses, svg::Document& objects_for_paint) {
		int counter_for_color{ 0 };
		for (const auto& [name, type_of_route_and_stop] : buses) {
			svg::Text background;
			svg::Text title;
			geo::Coordinates coordinates = catalogue.FindBus(std::string(name))->route[0]->coordinates;
			TextBusBackgroundAndTitleMaker(background, title, sphere_projector, render_settings, name, coordinates, counter_for_color);
			objects_for_paint.Add(background);
			objects_for_paint.Add(title);
			if (type_of_route_and_stop.first == "not ring route" && catalogue.FindBus(std::string(name))->route[0]->name !=
				catalogue.FindStop(type_of_route_and_stop.second)->name) {
				coordinates = catalogue.FindStop(type_of_route_and_stop.second)->coordinates;
			TextBusBackgroundAndTitleMaker(background, title, sphere_projector, render_settings, name, coordinates, counter_for_color);
			objects_for_paint.Add(background);
			objects_for_paint.Add(title);
			}
			if (counter_for_color >= static_cast<int>(render_settings.color_palette.size() - 1)) {
				counter_for_color = 0;
			}
			else {
				++counter_for_color;
			}
		}		
	}

	std::vector<domain::Stop*> VectorStopsMaker(transportcatalogue::TransportCatalogue& catalogue, 
		std::map<std::string_view, std::pair<std::string, std::string>>& buses) {
		std::vector<domain::Stop*> stops;
		for (const auto& [name, type_of_route_and_stop] : buses) {
			for (const auto& stop : catalogue.FindBus(std::string(name))->route) {
				if (std::find(stops.begin(), stops.end(), stop) == stops.end()) {
					stops.emplace_back(stop);
				}
			}
		}
		std::sort(stops.begin(), stops.end(), [](domain::Stop* lhs, domain::Stop* rhs) {return lhs->name < rhs->name;});
		return stops;
	}

	void CircleOfStopsMaker (svg::Circle& circle, map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, domain::Stop* stop) {
		circle.SetCenter(sphere_projector(stop->coordinates));
		circle.SetRadius(render_settings.stop_radius);
		circle.SetFillColor("white");
	}

	void CircleInputerInSvgDocument(map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, std::vector<domain::Stop*>& stops, svg::Document& objects_for_paint) {
		svg::Circle circle;		
		for (const auto& stop : stops) {
			CircleOfStopsMaker(circle, sphere_projector, render_settings, stop);
			objects_for_paint.Add(circle);
		}
	}

	void TextStopBackgroundAndTitleMaker(svg::Text& background, svg::Text& title, map_renderer::SphereProjector& sphere_projector, 
		map_renderer::RenderSettings& render_settings, domain::Stop* stop, geo::Coordinates& coordinates) {
		background.SetPosition(sphere_projector(coordinates));
		background.SetOffset({ render_settings.stop_label_offset[0], render_settings.stop_label_offset[1]});
		background.SetFontSize(render_settings.stop_label_font_size);
		background.SetFontFamily("Verdana");		
		background.SetData(stop->name);
		background.SetFillColor(render_settings.underlayer_color);
		background.SetStrokeColor(render_settings.underlayer_color);
		background.SetStrokeWidth(render_settings.underlayer_width);
		background.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
		background.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

		title.SetPosition(sphere_projector(coordinates));
		title.SetOffset({ render_settings.stop_label_offset[0], render_settings.stop_label_offset[1] });
		title.SetFontSize(render_settings.stop_label_font_size);
		title.SetFontFamily("Verdana");		
		title.SetData(stop->name);
		title.SetFillColor("black");
	}

	void TextStopInputerInSvgDocument(map_renderer::SphereProjector& sphere_projector,
		map_renderer::RenderSettings& render_settings, std::vector<domain::Stop*>& stops, svg::Document& objects_for_paint) {
		svg::Text background;
		svg::Text title;
		geo::Coordinates coordinates;
		for (const auto& stop : stops) {
			coordinates = stop->coordinates;
			TextStopBackgroundAndTitleMaker(background, title, sphere_projector, render_settings, stop, coordinates);
			objects_for_paint.Add(background);
			objects_for_paint.Add(title);
		}
	}

	void ParcingOfRequest(transportcatalogue::TransportCatalogue& catalogue, std::istream& in, std::ostream& out) {
		json::Document document = json::Load(in);

		if (!document.GetRoot().IsMap()) {
			throw json::ParsingError("Wrong structure of json file");
		}
		std::vector<request_handler::InputData> input_data = ParcingOfBaseRequests(document.GetRoot().AsMap().at("base_requests").AsArray());
		request_handler::AddInfoInCatalogue(catalogue, input_data);
		
		map_renderer::RenderSettings render_settings = RenderSettingsParcing(document.GetRoot().AsMap().at("render_settings").AsMap());		
		std::vector<geo::Coordinates> coordinates;
		std::map<std::string_view, std::pair<std::string, std::string>> buses = NamesOfBusesAndCoordinatesMaker(input_data, catalogue, coordinates);
		std::vector<domain::Stop*> stops = VectorStopsMaker(catalogue, buses);
		map_renderer::SphereProjector sphere_projector(coordinates.begin(), coordinates.end(), render_settings.width, render_settings.height, render_settings.padding);		
		svg::Document objects_for_paint;
		RouteRender(buses, catalogue, render_settings, sphere_projector, objects_for_paint);
		TextBusInputerInSvgDocument(catalogue, sphere_projector, render_settings, buses, objects_for_paint);
		CircleInputerInSvgDocument(sphere_projector, render_settings, stops, objects_for_paint);
		TextStopInputerInSvgDocument(sphere_projector, render_settings, stops, objects_for_paint);
		std::ostringstream map{};
		objects_for_paint.Render(map);
		std::string map_str = map.str();
		
		if (document.GetRoot().AsMap().at("stat_requests").AsArray().size() != 0) {
		json::Array responses_to_queries = StatRequestsParcing(document, catalogue, map_str);
		json::Document output_info{ json::Node{responses_to_queries} };
		json::Print(output_info, out);
	}
		
	}
}// json_reader
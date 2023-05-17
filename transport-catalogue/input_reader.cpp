#include "input_reader.h"


namespace transportcatalogue::detail {
	InputData ReadInputFunction(const std::string inner) {
		int route_type = 0;
		const auto type_of_req_begin = inner.find_first_not_of(' ');
		const auto type_of_req_end = inner.find_first_of(' ', type_of_req_begin);
		size_t type_of_req_distance = type_of_req_end - type_of_req_begin;
		std::string type_of_req = inner.substr(type_of_req_begin, type_of_req_distance);

		if (type_of_req == "Stop") {
			const auto name_begin = inner.find_first_not_of(' ', type_of_req_end);
			const auto name_end = inner.find_first_of(':', name_begin);
			size_t name_distance = name_end - name_begin;
			std::string name_of_req = inner.substr(name_begin, name_distance);

			const auto latitude_begin = inner.find_first_not_of(' ', name_end + 1);
			const auto latitude_end = inner.find_first_of(',', latitude_begin);
			size_t latitude_distance = latitude_end - latitude_begin;
			double latitude = stod(inner.substr(latitude_begin, latitude_distance));

			const auto longitude_begin = inner.find_first_not_of(' ', latitude_end + 1);
			const auto longitude_end = inner.find_first_of(',', longitude_begin);
			size_t longitude_distance = longitude_end - longitude_begin;
			double longitude = stod(inner.substr(longitude_begin, longitude_distance));
			if (longitude_end == inner.npos) {
				return { name_of_req, latitude, longitude, {}, {}, route_type, {} };
			}
			std::unordered_map<std::string, uint32_t> distance_to{};
			auto pos_of_comma = longitude_end;
			while (pos_of_comma != inner.npos) {
				const auto pos_of_distance_begin = inner.find_first_not_of({ ' ', ',' }, pos_of_comma);
				const auto pos_of_m = inner.find_first_of('m', pos_of_distance_begin);
				size_t distance_distance = pos_of_m - pos_of_distance_begin;
				const auto pos_of_to = inner.find_first_of('t', pos_of_m);
				const auto pos_of_name_stop_to_begin = inner.find_first_not_of(' ', pos_of_to + 2);
				const auto pos_of_name_stop_to_end = inner.find_first_of(',', pos_of_name_stop_to_begin);
				size_t name_stop_to_distance = pos_of_name_stop_to_end - pos_of_name_stop_to_begin;
				distance_to.insert({ inner.substr(pos_of_name_stop_to_begin, name_stop_to_distance),
					stoi(inner.substr(pos_of_distance_begin, distance_distance)) });
				pos_of_comma = pos_of_name_stop_to_end;
			}
			return { name_of_req, latitude, longitude, {}, {}, route_type, distance_to };
		}
		else if (type_of_req == "Bus") {
			std::vector<std::string>stops;

			std::string bus{};
			const auto name_begin = inner.find_first_not_of(' ', type_of_req_end);
			const auto name_end = inner.find_first_of(':', name_begin);
			size_t name_distance = name_end - name_begin;
			bus = inner.substr(name_begin, name_distance);

			auto pos = inner.find_first_not_of(' ', name_end + 1);
			const auto pos_end = inner.npos;
			if (std::count(inner.begin(), inner.end(), '-') != 0) {
				route_type = 1;
				while (pos != pos_end) {
					auto pos_simbol = inner.find_first_of('-', pos);
					auto pos_next = inner.find_last_not_of({ '-',' ' }, pos_simbol);
					size_t distance = pos_next - pos + 1;
					stops.push_back(inner.substr(pos, distance));
					pos = inner.find_first_not_of({ '-',' ' }, pos_next + 1);
				}
			}
			else {
				route_type = 2;
				while (pos != pos_end) {
					auto pos_simbol = inner.find_first_of('>', pos);
					auto pos_next = inner.find_last_not_of({ '>',' ' }, pos_simbol);
					size_t distance = pos_next - pos + 1;
					stops.push_back(inner.substr(pos, distance));
					pos = inner.find_first_not_of({ '>', ' ' }, pos_next + 1);
				}
			}
			return { {}, 0, 0, bus, stops, route_type, {} };
		}
		else {
			throw std::invalid_argument("Error request");
		}
	}
}
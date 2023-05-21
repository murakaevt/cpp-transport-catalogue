#include "input_reader.h"

namespace transportcatalogue::detail {

	std::string StringMaker(const std::string& simbols, const std::string& inner, size_t& next_pos, size_t pos = 0) {
		size_t begin = 0;
		size_t end = 0;
		if (simbols.size() == 2) {
			begin = inner.find_first_not_of(simbols[0], pos);
			end = inner.find_first_of(simbols[1], begin);
		}
		else if (simbols.size() == 3) {
			begin = inner.find_first_not_of({ simbols[0], simbols[1] }, pos);
			end = inner.find_first_of(simbols[2], begin);
		}
		size_t distance = end - begin;
		next_pos = end;
		return inner.substr(begin, distance);
	}

	InputData ReadInputFunction(const std::string inner) {
		int route_type = 0;
		size_t pos_for_next = 0;
		std::string type_of_req = StringMaker("  ", inner, pos_for_next, pos_for_next);

		if (type_of_req == "Stop") {
			std::string name_of_req = StringMaker(" :", inner, pos_for_next, pos_for_next);

			std::string latitude_str = StringMaker(" ,", inner, pos_for_next, pos_for_next + 1);
			double latitude = stod(latitude_str);

			std::string longitude_str = StringMaker(" ,", inner, pos_for_next, pos_for_next + 1);
			double longitude = stod(longitude_str);

			if (pos_for_next == inner.npos) {
				return { name_of_req, latitude, longitude, {}, {}, route_type, {} };
			}

			std::unordered_map<std::string, uint32_t> distance_to{};
			while (pos_for_next != inner.npos) {
				std::string name_of_distance = StringMaker(" ,m", inner, pos_for_next, pos_for_next);

				const auto pos_of_to = inner.find_first_of('t', pos_for_next);

				std::string name_of_stop_to = StringMaker(" ,", inner, pos_for_next, pos_of_to + 2);

				distance_to.insert({ name_of_stop_to, stoi(name_of_distance) });
			}
			return { name_of_req, latitude, longitude, {}, {}, route_type, distance_to };
		}
		else if (type_of_req == "Bus") {
			std::vector<std::string>stops;
			std::string name_of_bus = StringMaker(" :", inner, pos_for_next, pos_for_next);

			auto pos = inner.find_first_not_of(' ', pos_for_next + 1);
			if (std::count(inner.begin(), inner.end(), '-') != 0) {
				route_type = 1;
				while (pos != inner.npos) {
					auto pos_simbol = inner.find_first_of('-', pos);
					auto pos_next = inner.find_last_not_of({ '-',' ' }, pos_simbol);
					size_t distance = pos_next - pos + 1;
					stops.push_back(inner.substr(pos, distance));
					pos = inner.find_first_not_of({ '-',' ' }, pos_next + 1);
				}
			}
			else {
				route_type = 2;
				while (pos != inner.npos) {
					auto pos_simbol = inner.find_first_of('>', pos);
					auto pos_next = inner.find_last_not_of({ '>',' ' }, pos_simbol);
					size_t distance = pos_next - pos + 1;
					stops.push_back(inner.substr(pos, distance));
					pos = inner.find_first_not_of({ '>', ' ' }, pos_next + 1);
				}
			}
			return { {}, 0, 0, name_of_bus, stops, route_type, {} };
		}
		else {
			throw std::invalid_argument("Error request");
		}
	}

	std::vector<InputData> DataMaker(int counter) {
		std::vector<InputData> result;
		int t = 1;
		std::string text;
		while (t <= counter) {
			t += 1;
			std::getline(std::cin, text);
			InputData data = ReadInputFunction(text);
			result.push_back(data);
		}
		return result;
	}

	std::unordered_map<std::tuple<std::string, double, double>, std::unordered_map<std::string, uint32_t>, transportcatalogue::StopsMakerHasher> StopsMaker(std::vector<InputData>& datas) {
		std::unordered_map<std::tuple<std::string, double, double>, std::unordered_map<std::string, uint32_t>, transportcatalogue::StopsMakerHasher> result;
		for (auto& data : datas) {
			if (!data.name.empty()) {
				result.insert({ { data.name, data.latitude, data.longitude }, data.distance_to });
			}
		}
		return result;
	}

	std::unordered_map<std::string, std::vector<std::string>> RoutesMaker(std::vector<InputData>& datas) {
		std::unordered_map<std::string, std::vector<std::string>> result;
		for (auto& data : datas) {
			if (data.bus.empty()) {
				continue;
			}
			std::vector<std::string> route_buf = data.route;
			if (data.route_type == 1) {
				int pos = route_buf.size() - 2;
				while (pos >= 0) {
					route_buf.push_back(route_buf[pos]);
					pos -= 1;
				}
				result.insert({ std::move(data.bus), std::move(route_buf) });
			}
			else if (data.route_type == 2) {
				result.insert({ std::move(data.bus), std::move(route_buf) });
			}
		}
		return result;
	}
}
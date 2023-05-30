#pragma once
#include "transport_catalogue.h"
#include "input_reader.h"

#include <iostream>

namespace transportcatalogue::detail {

	std::string StringMakerToo(const std::string& simbols, const std::string& inner, const size_t& pos);

	void OutputBus(const std::string& bus, const TransportCatalogue& tran_cat, std::ostream& out);

	void OutputStop(const std::string& bus, const TransportCatalogue& tran_cat, std::ostream& out);

	void OutputReqFunction(const TransportCatalogue& catalogue, std::istream& in, std::ostream& out);
}
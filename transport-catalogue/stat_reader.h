#pragma once
#include "transport_catalogue.h"
#include "input_reader.h"

#include <iostream>

namespace transportcatalogue::detail {

	void OutputBus(const std::string& bus, const TransportCatalogue& tran_cat);

	void OutputStop(const std::string& bus, const TransportCatalogue& tran_cat);

	void OutputReqFunction(const TransportCatalogue& catalogue, int& counter);
}
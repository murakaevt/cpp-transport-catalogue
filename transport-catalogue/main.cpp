#include "request_handler.h"
#include "json_reader.h"
#include "map_renderer.h"

#include <fstream>

using namespace std::literals;

int main() {
    std::ifstream data_in("input.json");
    std::ofstream data_out("output.json");
    transportcatalogue::TransportCatalogue transport_catalogue;
    json_reader::ParsingOfRequest(transport_catalogue, data_in, data_out);
    /*
    json::Print(
       json::Document{
            json::Builder{}
            .StartDict()
                .Key("key1"s).Value(123)
                .Key("key2"s).Value("value2"s)
                .Key("key3"s).StartArray()
                    .Value(456)
                    .StartDict().EndDict()
                    .StartDict()
                        .Key(""s)
                        .Value(nullptr)
                    .EndDict()
                    .Value(""s)
                .EndArray()
            .EndDict()
            .Build()
        },
        std::cout
    );
    std::cout << std::endl;

    json::Print(
        json::Document{
            json::Builder{}
            .Value("just a string"s)
            .Build()
        },
        std::cout
    );
    std::cout << std::endl;
    */
}
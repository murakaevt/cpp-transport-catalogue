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
    json::Builder builder;
    builder.StartDict();
    builder.Key("key1"s);
    builder.Value(123);
    builder.Key("key2"s);
    builder.Value("value2"s);
    builder.Key("key3"s);
    builder.StartArray();
    builder.Value(456);
    builder.StartDict();
    builder.EndDict();
    builder.StartDict();
    builder.Key(""s);
    builder.Value(nullptr)
        .EndDict();
    builder.Value(""s)
        .EndArray()
        .EndDict();
    json::Print(
        json::Document{
            builder.Build()
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
#include "json_builder.h"

namespace json {

    Builder::Builder()
    {
        nodes_stack_.push_back(&root_);
    }

    DictItemContext Builder::StartDict()
    {
        if (nodes_stack_.back()->IsArray())
        {
            const_cast<Array&>(nodes_stack_.back()->AsArray()).push_back(Dict());
            Node* node = &const_cast<Array&>(nodes_stack_.back()->AsArray()).back();
            nodes_stack_.push_back(node);
        }
        else
        {
            *nodes_stack_.back() = Dict();
        }
        return *this;
    }

    ArrayItemContext Builder::StartArray()
    {
        if (nodes_stack_.back()->IsArray())
        {
            const_cast<Array&>(nodes_stack_.back()->AsArray()).push_back(Array());
            Node* node = &const_cast<Array&>(nodes_stack_.back()->AsArray()).back();
            nodes_stack_.push_back(node);
        }
        else
        {
            *nodes_stack_.back() = Array();
        }
        return *this;
    }

    Builder& Builder::EndDict()
    {
        if (nodes_stack_.empty()) {
            throw std::logic_error("unable to close as without opening");
        }
        Node node = *nodes_stack_.back();

        if (!node.IsMap()) {
            throw std::logic_error("object isn't dictionary");
        }

        nodes_stack_.erase(nodes_stack_.end() - 1);
        return *this;
    }

    Builder& Builder::EndArray()
    {
        if (nodes_stack_.empty()) {
            throw std::logic_error("unable to close without opening");
        }

        Node node = *nodes_stack_.back();

        if (!node.IsArray()) {
            throw std::logic_error("object isn't array");
        }
        nodes_stack_.erase(nodes_stack_.end() - 1);
        return *this;
    }

    KeyContext Builder::Key(std::string key)
    {
        if (nodes_stack_.empty()) {
            throw std::logic_error("unable to create Key");
        }

        nodes_stack_.emplace_back(&const_cast<Dict&>(nodes_stack_.back()->AsMap())[key]);
        return *this;
    }

    Builder& Builder::Value(Node value)
    {
        if (nodes_stack_.back()->IsArray())
        {
            const_cast<Array&>(nodes_stack_.back()->AsArray()).push_back(value);
        }
        else
        {
            *nodes_stack_.back() = value;
            nodes_stack_.erase(nodes_stack_.end() - 1);
        }
        return *this;
    }

    Node Builder::Build()
    {
        if (root_.IsNull()) {
            throw std::logic_error("empty json");
        }

        if (!nodes_stack_.empty()) {
            throw std::logic_error("invalid json");
        }

        return root_;
    }

    //------------FamContext------------------
    Builder& FamContext::EndDict() { return builder_.EndDict(); }

    Builder& FamContext::EndArray() { return builder_.EndArray(); }

    ArrayItemContext FamContext::StartArray() { return builder_.StartArray(); }

    DictItemContext FamContext::StartDict() { return builder_.StartDict(); }

    KeyContext FamContext::Key(std::string key) { return builder_.Key(key); }

    Builder& FamContext::Value(json::Node& value) { return builder_.Value(value); }
    FamContext::FamContext(json::Builder& builder) : builder_(builder) {}

    //----------------KeyContext----------------

    KeyContext::KeyContext(Builder& builder)
        : FamContext(builder) {}

    DictItemContext KeyContext::Value(Node& value) {
        return FamContext::Value(value);
    }

    //------------------DictItemContext------------------

    DictItemContext::DictItemContext(Builder& builder)
        : FamContext(builder) {}


    //-----------------ArrayItemContext------------------------

    ArrayItemContext::ArrayItemContext(Builder& builder)
        : FamContext(builder) {}

    ArrayItemContext ArrayItemContext::Value(json::Node& value) {
        return FamContext::Value(value);
    }


}// namespace json
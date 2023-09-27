#pragma once

#include "json.h"
#include <stack>
#include <string>
#include <memory>



namespace json {

    class KeyContext;
    class DictItemContext;
    class ArrayItemContext;

    //---------Builder----------------

    class Builder
    {
    public:

        Builder();

        DictItemContext StartDict();
        ArrayItemContext StartArray();

        Builder& EndDict();
        Builder& EndArray();

        KeyContext Key(std::string key);
        Builder& Value(Node value);

        Node Build();

    private:

        Node root_;
        std::vector<Node*> nodes_stack_;
    };
    //------------FamContext------------------
    class FamContext {
    public:
        FamContext(Builder& builder);
        DictItemContext StartDict();
        ArrayItemContext StartArray();

        Builder& EndDict();
        Builder& EndArray();

        KeyContext Key(std::string key);
        Builder& Value(Node& value);

    private:

        Builder& builder_;
    };

    //------------KeyContext------------------

    class KeyContext : public FamContext
    {
    public:

        KeyContext(Builder& builder);

        KeyContext Key(const std::string& key) = delete;

        FamContext EndDict() = delete;
        FamContext EndArray() = delete;

        DictItemContext Value(Node& value);

    };

    //---------------DictItemContext-------------------

    class DictItemContext : public FamContext
    {
    public:

        DictItemContext(Builder& builder);

        KeyContext Key(const std::string& key) = delete;

        DictItemContext StartDict() = delete;

        FamContext EndArray() = delete;
        FamContext StartArray() = delete;

        DictItemContext Value(const Node& value) = delete;

    };

    //-----------ArrayItemContext----------------------

    class ArrayItemContext : public FamContext
    {
    public:

        ArrayItemContext(Builder& builder);

        ArrayItemContext Value(Node& value);

        DictItemContext EndDict() = delete;

        KeyContext Key(const std::string& key) = delete;
    };

}// namespace json
#include "svg.h"

namespace svg {

    using namespace std::literals;

    void Object::Render(const RenderContext& context) const {
        context.RenderIndent();

        // Делегируем вывод тега своим подклассам
        RenderObject(context);

        context.out << std::endl;
    }

    // ---------- Circle ------------------

    Circle& Circle::SetCenter(Point center) {
        center_ = center;
        return *this;
    }

    Circle& Circle::SetRadius(double radius) {
        radius_ = radius;
        return *this;
    }

    void Circle::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << std::endl;
        out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
        out << "r=\""sv << radius_ << "\""sv;
        RenderAttrs(out);
        out << "/>"s;
    }

    Polyline& Polyline::AddPoint(Point point) {
        points_.push_back(point);
        return *this;
    }

    void Polyline::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << std::endl;
        out << "<polyline points=\""s;
        bool is_first = true;
        for (const auto& point : points_) {
            if (is_first) {
                out << point.x << ","s << point.y;
                is_first = false;
            }
            else {
                out << " "s << point.x << ","s << point.y;
            }
        }
        out << "\""s;
        RenderAttrs(out);
        out << "/>"s;
    }

    Text& Text::SetPosition(Point pos) {
        position_ = pos;
        return *this;
    }

    Text& Text::SetOffset(Point offset) {
        off_set_ = offset;
        return *this;
    }

    Text& Text::SetFontSize(uint32_t size) {
        font_size_ = size;
        return *this;
    }

    Text& Text::SetFontFamily(std::string font_family) {
        font_family_ = font_family;
        return *this;
    }

    Text& Text::SetFontWeight(std::string font_weight) {
        font_weight_ = font_weight;
        return *this;
    }

    Text& Text::SetData(std::string data) {
        data_ = data;
        return *this;
    }

    void Text::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<text"s;
        RenderAttrs(out);
        out << " x=\""s << position_.x << "\" y=\""s << position_.y
            << "\" dx=\""s << off_set_.x << "\" dy=\""s << off_set_.y
            << "\" font-size=\""s << font_size_ << "\""s;
        if (!font_family_.empty()) {
            out << " font-family=\""s << font_family_ << "\""s;
        }
        if (!font_weight_.empty()) {
            out << " font-weight=\""s << font_weight_ << "\""s;
        }
        out << ">"s;
        for (const auto& l : data_) {
            switch (l)
            {
            case '"':
                out << "&quot;"s;
                break;
            case '\'':
                out << "&apos;"s;
                break;
            case '<':
                out << "&lt;"s;
                break;
            case '>':
                out << "&gt;"s;
                break;
            case '&':
                out << "&amp;"s;
                break;
            default:
                out << l;
                break;
            }
        }
        out << "</text>"s;
    }

    void Document::AddPtr(std::unique_ptr<Object>&& obj) {
        objects_.emplace_back(std::move(obj));
    }

    void Document::Render(std::ostream& out) const {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"s << std::endl;
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"s;
        if (objects_.empty()) {
            out << std::endl;
        }
        else {
            for (const auto& object : objects_) {
                object.get()->Render(out);
            }
        }
        out << "</svg>"s << std::endl;
    }

    std::ostream& operator<<(std::ostream& out, StrokeLineCap line_cap) {
        if (line_cap == StrokeLineCap::BUTT) {
            out << "butt";
        }
        else if (line_cap == StrokeLineCap::ROUND) {
            out << "round";
        }
        else if (line_cap == StrokeLineCap::SQUARE) {
            out << "square";
        }
        else if (line_cap == StrokeLineCap::DEFAULT) {
            out << "default";
        }
        return out;
    }
    std::ostream& operator<<(std::ostream& out, StrokeLineJoin line_join) {
        if (line_join == StrokeLineJoin::ARCS) {
            out << "arcs";
        }
        else if (line_join == StrokeLineJoin::BEVEL) {
            out << "bevel";
        }
        else if (line_join == StrokeLineJoin::MITER) {
            out << "miter";
        }
        else if (line_join == StrokeLineJoin::MITER_CLIP) {
            out << "miter-clip";
        }
        else if (line_join == StrokeLineJoin::ROUND) {
            out << "round";
        }
        else if (line_join == StrokeLineJoin::DEFAULT) {
            out << "default";
        }
        return out;
    }

    void RenderColor(std::ostream& out, std::monostate) {
        out << "none"sv;
    }

    void RenderColor(std::ostream& out, const int color) {
        out << color;
    }

    void RenderColor(std::ostream& out, const double color) {
        out << color;
    }

    void RenderColor(std::ostream& out, const std::string& color) {
        out << color;
    }

    void RenderColor(std::ostream& out, const Rgb& color) {
        out << "rgb("s << color.red << ","s << color.green << ","s << color.blue << ")"s;
    }

    void RenderColor(std::ostream& out, const Rgba& color) {
        out << "rgba("s << color.red << ","s << color.green << ","s << color.blue << ","s << color.opacity << ")"s;
    }

    std::ostream& operator<<(std::ostream& out, const Color& color) {
        std::visit(
            [&out](const auto& value) {
                RenderColor(out, value);
            },
            color);
        return out;
    }
}  // namespace svg

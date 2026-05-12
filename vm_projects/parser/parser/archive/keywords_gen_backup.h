#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

struct keyword_node
{
    enum { SIZE = 256, BOUNDARY_CHAR = SIZE };

    std::array<int16_t, SIZE + 1> data;
    std::string id;
    
    inline keyword_node() : data({ 0 }) {}
    
    inline size_t count(bool include_boundary = true) const
    {
        size_t sz = 0;
        for (size_t i = 0; i < data.size() - (include_boundary ? 0 : 1); i++) {
            sz += data[i] ? 1 : 0;
        }
        return sz;
    }

    std::ostream &print(size_t row, std::ostream &os) const
    {
        os << std::setw(5) << row << ": ";
        for (int i = 0; i < data.size(); i++) {
            if (data[i]) {
                (i == BOUNDARY_CHAR ? (os << "\\b") : (os << char(i))) << ":" << data[i] << " ";
            }
        }
        return id.empty() ? (os << "#" << id) : os;
    }
};

struct keyword_context
{
    size_t                           start, end, ahead;
    bool                             completed;
    bool                             updated;
    const std::vector<keyword_node> &rows;
    
    inline keyword_context(const std::vector<keyword_node> &rows, size_t start = 0) :
        rows(rows), start(start), end(start), ahead(start), completed(false), updated(false) {}
    
    inline bool next(int ch)
    {
        size_t r = rows[end].data[ch];
        return (bool)(end = (r ? r : end));
    }
    
    inline bool next_boundary()
    {
        return next(keyword_node::BOUNDARY_CHAR);
    }
    
    inline bool has_next(int ch) const
    {
        return (bool)rows[end].data[ch];
    }
    
    inline size_t count(bool include_boundary) const
    {
        return node().count(include_boundary);
    }
    
    inline bool has_next_boundary() const
    {
        return has_next(keyword_node::BOUNDARY_CHAR);
    }
    
    inline const keyword_node &node() const
    {
        return rows[end];
    }
    
    inline int first_child_from(int start = 0) const
    {
        const auto &k = rows[end];
        for (int i = start; i < keyword_node::SIZE; i++) {
            if (k.data[i]) {
                return i;
            }
        }
        return -1;
    }

    inline std::string_view expr_check_boundary() const
    {
        return "!_word(i, e)";
    }
    
    inline std::string expr_check_first_character() const
    {
        return expr_check_character(first_child_from());
    }
    
    inline std::string expr_check_character(int ch) const
    {
        return std::string("*i++ == '") + char(ch) + "'";
    }
    
    inline keyword_context expr_linear_match() const
    {
        keyword_context kc = *this;
        if (kc.has_next_boundary()) {
            return kc;
        }
        if (kc.count(true) != 1) {
            return kc;
        }
        return kc;
    }
    
    inline std::string_view keyword_id() const
    {
        const auto &r = rows[end];
        if (r.id.empty()) {
            size_t i = rows[end].data[keyword_node::BOUNDARY_CHAR];
            if (i) {
                const auto &r = rows[i];
                if (!r.id.empty()) {
                    return r.id;
                }
            }
        }
        return r.id;
    }

    std::ostream &print(std::ostream &os) const
    {
        os << "// " << " start " << start << " end " << end << " next " << ahead << " [";
        return node().print(end, os) << "]" << std::endl;
    }
};

struct keywords
{
    typedef std::pair<std::string, std::string> keyword_entry_t;

    std::vector<keyword_node>    rows;
    std::vector<keyword_entry_t> keywords;
    
    void add_keyword(std::string_view s, std::string_view id, bool case_insensitive = false)
    {
        keywords.push_back(keyword_entry_t(s, id));
    }
    
    std::ostream &print(std::ostream &os) const
    {
        size_t n = 0;
        for (const auto &kn : rows) {
            kn.print(n++, os) << std::endl;
        }
        return os << std::endl;
    }
    
    std::string single_expr_full(std::string s, keyword_context &context, size_t row, bool include_boundary = true, size_t depth = 0) const
    {
        const auto &kn = rows[row];
        size_t count = kn.count(include_boundary);
        if (count == 1) {
            for (int i = 0; i < kn.data.size(); i++) {
                if (kn.data[i]) {
                    context.ahead = kn.data[i];
                    context.updated = true;
                    context.end = std::max(context.end, row);
                    s += s.empty() ? "" : " && ";
                    if (i == keyword_node::BOUNDARY_CHAR) {
                        context.completed = true;
                        std::ostringstream ss;
                        ss << "(e - i) >= " << depth << " && " << s << "!_word(i, e)";
                        return ss.str();
                    }
                    return single_expr(s + "*i++ == '" + char(i) + "'", context, kn.data[i], true,/*include_boundary,*/ depth + 1);
                }
            }
        }
        return s;
    }

    std::string single_expr(std::string s, keyword_context &context, size_t row, bool include_boundary = true, size_t depth = 0) const
    {
        const auto &kn = rows[row];
        size_t count = kn.count(include_boundary);
        if (count == 1) {
            for (int i = 0; i < kn.data.size(); i++) {
                if (kn.data[i]) {
                    context.ahead = kn.data[i];
                    context.updated = true;
                    context.end = std::max(context.end, row);
                    s += s.empty() ? "" : " && ";
                    if (i == keyword_node::BOUNDARY_CHAR) {
                        context.completed = true;
                        std::ostringstream ss;
                        ss << s << "!_word(i, e)";
                        return ss.str();
                    }
                    return single_expr(s + "*i++ == '" + char(i) + "'", context, kn.data[i], true,/*include_boundary,*/ depth + 1);
                }
            }
        }
        return s;
    }
    
    std::string generate(std::string_view name)
    {
        std::ostringstream ss;
        size_t min = _min_length(), max = _max_length();
        std::string unk = _return_value(_unknown_id());
        const char *el("\n");
        ss << "#include <algorithm>" << el
           << "#include <cctype>" << el
           << "#include <iostream>" << el
           << "#include <sstream>" << el
           << el
           << "template <typename IterT> struct " << name << el
           << "{" << el
           << "    enum Id { " << _symbol_names() << " };" << el
           << el
           << "    typedef std::pair<IterT, Id> val_t;" << el
           << el
           << "    static val_t read(IterT b, IterT e)" << el
           << "    {" << el
           << "        auto len = std::distance(b, e);" << el
           << "        if (len < " << min << ") {" << el
           << "            return _unknown(b);" << el
           << "        }" << el
           << "        auto i = b;" << el
           << "        if (len > " << _max_length() << ") {"
           << _generate_length_limited(0, 0, 2) << el
           << "        }" << el
           << "        switch (len) {" << el;
        for (size_t i = min; i <= max; ++i) {
            std::string current = _generate_length_limited(i, 0, 2),
                        next    = i == max ? "" : _generate_length_limited(i + 1, 0, 2);
            ss << "        case " << i << ": " << (current.size() == next.size() ? el : current);
        }
        ss << "        default: return " << unk << ";" << el
           << "        }" << el
           << "    }" << el
           << el
           << "private:" << el
           << el
           << "    inline static bool _word(const IterT b, const IterT e)" << el
           << "    {" << el
           << "        return b < e && (std::isalnum(*b) || *b == '_');" << el
           << "    }" << el
           << el
           << "    inline static val_t _unknown(IterT b)" << el
           << "    {" << el
           << "        return val_t(b, ID_UNKNOWN);" << el
           << "    }" << el
           << "};" << el;
        return ss.str();
    }

private:

    size_t _max_length() const
    {
        size_t max = 0;
        for (const auto &ke : keywords) {
            max = std::max(ke.first.size(), max);
        }
        return max;
    }
    
    size_t _min_length() const
    {
        size_t min = ~0ul;
        for (const auto &ke : keywords) {
            min = std::min(ke.first.size(), min);
        }
        return min;
    }
    
    void _compute(size_t max_length = 0)
    {
        rows.clear();
        rows.push_back(keyword_node());
        for (const auto &ke : keywords) {
            if (max_length == 0 || ke.first.size() <= max_length) {
                _add_keyword(ke.first, ke.second);
            }
        }
    }
    
    std::string _symbol_names() const
    {
        std::ostringstream ss;
        std::vector<std::string> kw;
        for (const auto &ke : keywords) {
            kw.push_back(ke.second);
        }
        auto e = std::unique(kw.begin(), kw.end());
        kw.resize(std::distance(kw.begin(), e));
        ss << _unknown_id();
        for (const auto &k : kw) {
            ss << ", " << k;
        }
        return ss.str();
    }
    
    const char *_unknown_id() const { return "ID_UNKNOWN"; }
    
    std::string _return_value(std::string_view sv) const
    {
        return std::string("val_t(i, ").append(sv) + ")";
    }
    
    std::string _generate_full(size_t row = 0, size_t depth = 0)
    {
        _compute();
        const auto &kn = rows[row];
        std::ostringstream ss;
        std::string ind = _indent(depth + 1), ind_1 = _indent(depth + 2);
        keyword_context kc(rows, row);
        size_t count = kn.count(true), count_nb = kn.count(false);
        std::string unk = _return_value(_unknown_id());
        if (count == 1) {
            std::string s = single_expr_full("", kc, row, true);
            if (!kc.completed) {
                ss << std::endl
                   << ind << "if (" << s << ") {" << std::endl
                   << _generate_full(kc.ahead, depth + 1)
                   << ind << "}" << std::endl
                   << ind << "return " << unk << ";" << std::endl;
            } else {
                kc.next_boundary();
                ss << "return (" << s << ") ? " << _return_value(kc.node().id) << " : " << unk << ";" << std::endl;
            }
        } else if (count > 0) {
            ss << std::endl;
            if (count_nb != count) {
                ss << ind   << "if (!_word(i, e)) {" << std::endl
                   << ind_1 << "return " << _return_value(rows[kn.data[keyword_node::BOUNDARY_CHAR]].id) << ";" << std::endl
                   << ind   << "}" << std::endl;
            }
            if (count_nb == 1 && count == 1) {
                std::string s = single_expr_full("", kc, row, false);
                ss << ind;
                kc.next_boundary();
                ss << ind << "return (" << s << ") ? " << _return_value(kc.node().id) << " : " << unk << ";" << std::endl;
            } else if (count_nb == 1 && count == 2) {
                for (size_t i = 0; i < 256; i++) {
                    if (kn.data[i]) {
                        ss  << ind << "if (*i++ == '" << char(i) << "') {" << std::endl
                            << ind_1 << _generate_full(kn.data[i], depth + 1)
                            << ind << "}" << std::endl
                            << ind << "return " << unk << ";" << std::endl;
                        break;
                    }
                }
            } else {
                ss << ind << "switch (*i++) {" << std::endl;
                for (size_t i = 0; i < 256; i++) {
                    if (kn.data[i]) {
                        ss << ind << "case '" << char(i) << "': " << _generate_full(kn.data[i], depth + 1);
                    }
                }
                ss << ind << "default: return " << unk << ";" << std::endl;
                ss << ind << "}" << std::endl;
            }
        }
        return ss.str();
    }
    
    std::string _generate_length_limited(size_t length, size_t row = 0, size_t depth = 0)
    {
        if (length > _max_length()) {
            length = 0; // no need to worry about it
        }
        _compute(length);
        const auto &kn = rows[row];
        std::ostringstream ss;
        std::string ind = _indent(depth + 1), ind_1 = _indent(depth + 2);
        keyword_context kc(rows, row);
        size_t count = kn.count(true), count_nb = kn.count(false);
        std::string unk = "_unknown(i)";
        if (count == 1) {
            std::string s = single_expr("", kc, row, true);
            if (!kc.completed) {
                ss << std::endl
                   << ind << "if (" << s << ") {" << std::endl
                   << _generate_length_limited(length, kc.ahead, depth + 1)
                   << ind << "}" << std::endl
                   << ind << "return " << unk << ";" << std::endl;
            } else {
                kc.next_boundary();
                ss << "return " << s << " ? " << _return_value(kc.node().id) << " : " << unk << ";" << std::endl;
            }
        } else if (count > 0) {
            ss << std::endl;
            if (count_nb != count) {
                ss << ind   << "if (!_word(i, e)) {" << std::endl
                   << ind_1 << "return " << _return_value(rows[kn.data[keyword_node::BOUNDARY_CHAR]].id) << ";" << std::endl
                   << ind   << "}" << std::endl;
            }
            if (count_nb == 1 && count == 1) {
                // I don't think this ever gets executed
                std::string s = single_expr("", kc, row, false);
                ss << ind;
                kc.next_boundary();
                ss << ind << "return (" << s << ") ? " << _return_value(kc.node().id) << " : " << unk << ";" << std::endl;
            } else if (count_nb == 1 && count == 2) {
                for (size_t i = 0; i < 256; i++) {
                    if (kn.data[i]) {
                        ss  << ind << "if (*i++ == '" << char(i) << "') {" << std::endl
                            << ind_1 << _generate_length_limited(length, kn.data[i], depth + 1)
                            << ind << "}" << std::endl
                            << ind << "return " << unk << ";" << std::endl;
                        break;
                    }
                }
            } else {
                ss << ind << "switch (*i++) {" << std::endl;
                for (size_t i = 0; i < 256; i++) {
                    if (kn.data[i]) {
                        ss << ind << "case '" << char(i) << "': " << _generate_length_limited(length, kn.data[i], depth + 1);
                    }
                }
                ss << ind << "default: return " << unk << ";" << std::endl;
                ss << ind << "}" << std::endl;
            }
        }
        return ss.str();
    }
    
    inline static std::string _indent(size_t depth)
    {
        std::string indent;
        for (size_t i = 0; i < depth; i++) {
            indent += "    ";
        }
        return indent;
    }
    
    void _add_keyword(std::string_view s, std::string_view id)
    {
        uint16_t n = 0;
        for (auto c : s) {
            if (rows[n].data[c] != 0) {
                n = rows[n].data[c];
                continue;
            }
            n = _add_row_for(n, c);
        }
        _add_row_for(n, keyword_node::BOUNDARY_CHAR);
        auto &kn = rows.back();
        kn.id = id;
    }
    
    inline int16_t _add_row_for(int n, int ch)
    {
        int16_t rs = int16_t(rows.size());
        rows[n].data[ch] = rs;
        rows.push_back(keyword_node());
        return rs;
    }
};

void keywords_gen();

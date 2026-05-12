#include <algorithm>
#include <vector>
#include <array>

struct keyword
{
    std::string key;
    size_t      id;
    bool        case_sensitive;
    
    inline keyword(std::string_view key, size_t id, bool case_sensitive = true) : key(key), id(id), case_sensitive(case_sensitive) {}
    inline keyword(const keyword &o) : key(o.key), id(o.id), case_sensitive(o.case_sensitive) {}
    
    inline keyword &operator=(const keyword &o)
    {
        if (&o != this) {
            key = o.key;
            id = o.id;
            case_sensitive = o.case_sensitive;
        }
        return *this;
    }
    
    inline bool operator<(const keyword &k) const
    {
        return &k == this ? false : key < k.key;
    }
    
    inline std::ostream &print(std::ostream &os) const
    {
        return os << "key " << key << " id " << id << " cs " << (case_sensitive ? "y" : "n");
    }
};

struct keyword_set
{
    typedef std::vector<keyword>        list_t;
    typedef list_t::const_iterator      iter_t;
    typedef std::string::const_iterator str_iter_t;
    typedef std::pair<iter_t, iter_t>   range_t;
    typedef std::array<range_t, 256>    ranges_t;
    
    typedef std::pair<iter_t, size_t> val_t;
    
    list_t   keywords;
    ranges_t ranges;

    void add_keyword(std::string_view key, size_t id, bool case_sensitive = true)
    {
        keywords.push_back(keyword(key, id, case_sensitive));
    }
    
    void complete_add()
    {
        std::sort(keywords.begin(), keywords.end());
        const auto &e = keywords.end();
        ranges = ranges_t({ range_t(e, e) });
        int ch = 0;
        iter_t b = keywords.begin();
        range_t r(b, b);
        for (iter_t i = r.first, e = keywords.end(); i != e; i++) {
            if ((*i).key[0] != ch) {
                if (r.second != r.first) {
                    ranges[ch] = r;
                }
                r.first = r.second;
                r.second = i;
            } else {
                r.second = i;
            }
            ch = (*i).key[0];
        }
    }
    /*
    inline std::pair<range_t, str_iter_t> upper(size_t index, str_iter_t i, const range_t &r) const
    {
        auto d = std::distance(r.first, r.second), da = d / 2, db = d - d / 2;
        if ((*(r.first + da)).key[index] > *(i + index)) {
            
        }

        auto b = d / 2 + r.first;

    }

    val_t read(str_iter_t b, str_iter_t e) const
    {
        const auto &r = ranges[*b++];
        //std::upper_bound(r.first, r.second, );
        
        
        return val_t(b, 0);
    }
*/
    std::ostream &print(std::ostream &os) const
    {
        for (const auto &k : keywords) {
            k.print(os) << std::endl;
        }
        return os << std::endl;
    }
};

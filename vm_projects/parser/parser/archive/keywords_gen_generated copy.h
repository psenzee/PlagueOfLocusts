#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

template <typename IterT> struct identifier_reader
{
    enum Id { ID_UNKNOWN, ID_TRUE, ID_TRUNK, ID_TRUER, ID_TRUERY, ID_TRUERYO, ID_TRUERYA, ID_FALSE, ID_NULL, ID_NONE };

    typedef std::pair<IterT, Id> val_t;

    static val_t read(IterT b, IterT e)
    {
        auto len = std::distance(b, e);
        if (len < 1) {
            return _unknown(b);
        }
        auto i = b;
        if (len > 7) {
            switch (*i++) {
            case 'f':
                if (!_word(i, e)) {
                    return val_t(i, ID_FALSE);
                }
                if (*i++ == 'a') {
                    return _match(i, e, ID_FALSE, 'l', 's', 'e');
                }
                return _unknown(i);
            case 'n':
                switch (*i++) {
                case 'o': return _match(i, e, ID_NONE, 'n', 'e');
                case 'u': return _match(i, e, ID_NULL, 'l', 'l');
                default: return _unknown(i);
                }
            case 't':
                if (*i++ == 'r' && *i++ == 'u') {
                    switch (*i++) {
                    case 'e':
                        if (!_word(i, e)) {
                            return val_t(i, ID_TRUE);
                        }
                        if (*i++ == 'r') {
                            if (!_word(i, e)) {
                                return val_t(i, ID_TRUER);
                            }
                            if (*i++ == 'y') {
                                if (!_word(i, e)) {
                                    return val_t(i, ID_TRUERY);
                                }
                                switch (*i++) {
                                case 'a': return _match(i, e, ID_TRUERYA);
                                case 'o': return _match(i, e, ID_TRUERYO);
                                default: return _unknown(i);
                                }
                            }
                            return _unknown(i);
                        }
                        return _unknown(i);
                    case 'n': return _match(i, e, ID_TRUNK, 'k');
                    default: return _unknown(i);
                    }
                }
                return _unknown(i);
            default: return _unknown(i);
            }

        }
        switch (len) {
        case 1:
        case 2:
        case 3: return _match(i, e, ID_FALSE, 'f');
        case 4:
            switch (*i++) {
            case 'f': return _match(i, e, ID_FALSE);
            case 'n':
                switch (*i++) {
                case 'o': return _match(i, e, ID_NONE, 'n', 'e');
                case 'u': return _match(i, e, ID_NULL, 'l', 'l');
                default: return _unknown(i);
                }
            case 't': return _match(i, e, ID_TRUE, 'r', 'u', 'e');
            default: return _unknown(i);
            }
        case 5:
            switch (*i++) {
            case 'f':
                if (!_word(i, e)) {
                    return val_t(i, ID_FALSE);
                }
                if (*i++ == 'a') {
                    return _match(i, e, ID_FALSE, 'l', 's', 'e');
                }
                return _unknown(i);
            case 'n':
                switch (*i++) {
                case 'o': return _match(i, e, ID_NONE, 'n', 'e');
                case 'u': return _match(i, e, ID_NULL, 'l', 'l');
                default: return _unknown(i);
                }
            case 't':
                if (*i++ == 'r' && *i++ == 'u') {

                    switch (*i++) {
                    case 'e':
                        if (!_word(i, e)) {
                            return val_t(i, ID_TRUE);
                        }
                        if (*i++ == 'r') {
                            return _match(i, e, ID_TRUER);
                        }
                        return _unknown(i);
                    case 'n': return _match(i, e, ID_TRUNK, 'k');
                    default: return _unknown(i);
                    }
                }
                return _unknown(i);
            default: return _unknown(i);
            }
        case 6:
            switch (*i++) {
            case 'f':
                if (!_word(i, e)) {
                    return val_t(i, ID_FALSE);
                }
                if (*i++ == 'a') {
                    return _match(i, e, ID_FALSE, 'l', 's', 'e');
                }
                return _unknown(i);
            case 'n':
                switch (*i++) {
                case 'o': return _match(i, e, ID_NONE, 'n', 'e');
                case 'u': return _match(i, e, ID_NULL, 'l', 'l');
                default: return _unknown(i);
                }
            case 't':
                if (*i++ == 'r' && *i++ == 'u') {

                    switch (*i++) {
                    case 'e':
                        if (!_word(i, e)) {
                            return val_t(i, ID_TRUE);
                        }
                        if (*i++ == 'r') {
                            if (!_word(i, e)) {
                                return val_t(i, ID_TRUER);
                            }
                            if (*i++ == 'y') {
                                return _match(i, e, ID_TRUERY);
                            }
                            return _unknown(i);
                        }
                        return _unknown(i);
                    case 'n': return _match(i, e, ID_TRUNK, 'k');
                    default: return _unknown(i);
                    }
                }
                return _unknown(i);
            default: return _unknown(i);
            }
        case 7:
            switch (*i++) {
            case 'f':
                if (!_word(i, e)) {
                    return val_t(i, ID_FALSE);
                }
                if (*i++ == 'a') {
                    return _match(i, e, ID_FALSE, 'l', 's', 'e');
                }
                return _unknown(i);
            case 'n':
                switch (*i++) {
                case 'o': return _match(i, e, ID_NONE, 'n', 'e');
                case 'u': return _match(i, e, ID_NULL, 'l', 'l');
                default: return _unknown(i);
                }
            case 't':
                if (*i++ == 'r' && *i++ == 'u') {
                    switch (*i++) {
                    case 'e':
                        if (!_word(i, e)) {
                            return val_t(i, ID_TRUE);
                        }
                        if (*i++ == 'r') {
                            if (!_word(i, e)) {
                                return val_t(i, ID_TRUER);
                            }
                            if (*i++ == 'y') {
                                if (!_word(i, e)) {
                                    return val_t(i, ID_TRUERY);
                                }
                                switch (*i++) {
                                case 'a': return _match(i, e, ID_TRUERYA);
                                case 'o': return _match(i, e, ID_TRUERYO);
                                default: return _unknown(i);
                                }
                            }
                            return _unknown(i);
                        }
                        return _unknown(i);
                    case 'n': return _match(i, e, ID_TRUNK, 'k');
                    default: return _unknown(i);
                    }
                }
                return _unknown(i);
            default: return _unknown(i);
            }
        default: return val_t(i, ID_UNKNOWN);
        }
    }

private:
    
    
    inline static val_t _match(IterT &i, const IterT e, Id id)
    {
        return !_word(i, e) ? val_t(i, id) : _unknown(i);
    }
    
    inline static val_t _match(IterT &i, const IterT e, Id id, char a)
    {
        return *i++ == a && !_word(i, e) ? val_t(i, id) : _unknown(i);
    }
    
    inline static val_t _match(IterT &i, const IterT e, Id id, char a, char b)
    {
        return *i++ == a && *i++ == b && !_word(i, e) ? val_t(i, id) : _unknown(i);
    }
    
    inline static val_t _match(IterT &i, const IterT e, Id id, char a, char b, char c)
    {
        return *i++ == a && *i++ == b && *i++ == c && !_word(i, e) ? val_t(i, id) : _unknown(i);
    }
    
    inline static val_t _match(IterT &i, const IterT e, Id id, char a, char b, char c, char d)
    {
        return *i++ == a && *i++ == b && *i++ == c && *i++ == d && !_word(i, e) ? val_t(i, id) : _unknown(i);
    }

    inline static val_t _match(IterT &i, const IterT e, Id id, char a, char b, char c, char d, char f)
    {
        return *i++ == a && *i++ == b && *i++ == c && *i++ == d && *i++ == f && !_word(i, e) ? val_t(i, id) : _unknown(i);
    }

    inline static bool _word(const IterT b, const IterT e)
    {
        return b < e && (std::isalnum(*b) || *b == '_');
    }

    inline static val_t _unknown(IterT b)
    {
        return val_t(b, ID_UNKNOWN);
    }
};

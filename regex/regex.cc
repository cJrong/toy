template <typename LeftExpr, typename RightExpr>
struct concat;

template <typename LeftExpr, typename RightExpr>
struct alter;

template <typename SubExpr>
struct closure;

template <typename SubExpr>
struct optional;

template <char c>
struct matched;

template <typename Regex>
struct match;

template <typename LeftExpr, typename RightExpr>
struct match<concat<LeftExpr, RightExpr>> {
    template <typename Continuation>
    static bool apply(const char *s, const Continuation& cont) {
        return match<LeftExpr>::apply(s, [cont](const char *ss) {
            return match<RightExpr>::apply(ss, cont);
        });
    }
};

template <typename LeftExpr, typename RightExpr>
struct match<alter<LeftExpr, RightExpr>> {
    template <typename Continuation>
    static bool apply(const char *s, const Continuation& cont) {
        return match<LeftExpr>::apply(s, cont) ||
               match<RightExpr>::apply(s, cont);
    }
};

template <typename SubExpr>
struct match<closure<SubExpr>> {
    template <typename Continuation>
    static bool apply(const char *s, const Continuation& cont) {
        return match<SubExpr>::apply(s, [cont](const char *ss) {
            return match<closure<SubExpr>>::apply(ss, cont) || cont(ss);
        }) || cont(s);
    }
};

template <typename SubExpr>
struct match<optional<SubExpr>> {
    template <typename Continuation> 
    static bool apply(const char *s, const Continuation& cont) {
        return match<SubExpr>::apply(s, cont) || cont(s);
    }
};

template <char c>
struct match<matched<c>> {
    template <typename Continuation>
    static bool apply(const char *s, const Continuation& cont) {
        return *s && *s == c && cont(s + 1);
    }
};

template <typename Regex>
bool regex_match(const char *s)
{
    return match<Regex>::apply(s, [](const char* ss) {return *ss == '\0';});
}

template <typename Regex>
bool regex_search(const char *s)
{
    return match<Regex>::apply(s, [](const char* ss) {return true;}) || 
        (*s && regex_search<Regex>(++s));
}

#include <iostream>

int main(int argc, char** args)
{
    std::cout << std::boolalpha;
    std::cout << regex_match <concat<matched<'a'>, matched<'b'>>>("ab") << std::endl;
    std::cout << regex_match <alter<matched<'a'>, matched<'b'>>>("a") << std::endl;
    std::cout << regex_match <alter<matched<'a'>, matched<'b'>>>("b") << std::endl;
    std::cout << regex_match <closure<matched<'a'>>>("aaaa") << std::endl;
    std::cout << regex_match <optional<matched<'a'>>>("a") << std::endl;
    std::cout << regex_match <optional<matched<'a'>>>("") << std::endl;
    std::cout << regex_search <closure<matched<'a'>>>("bcczsaaa") << std::endl;
    return 0;
}
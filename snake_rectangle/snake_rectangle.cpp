#include <iostream>
#include <array>
#include <cstring>
#include <cstdio>

struct LEFT{};
struct DOWN{};
struct RIGHT{};
struct UP{};

struct true_type{};
struct false_type{};

template <bool expr>
struct cmpr_expr {
};

template <> 
struct cmpr_expr<true> {
    typedef true_type result;
};

template <>
struct cmpr_expr<false> {
    typedef false_type result;
};

/* 步进 */
template <typename, typename recursive, int i, int j, int n, int m, int k, int left, int right, int down, int up>
struct step {
    step(std::array<std::array<int, n>, n>& map){}
};

template <int i, int j, int n, int m, int k, int left, int right, int down, int up>
struct step<RIGHT, false_type, i, j, n, m, k, left, right, down, up> {
    step(std::array<std::array<int, n>, n>& map){}
};

template <int i, int j, int n, int m, int k, int left, int right, int down, int up>
struct step<DOWN, false_type, i, j, n, m, k, left, right, down, up> {
    step(std::array<std::array<int, n>, n>& map){}
};

template <int i, int j, int n, int m, int k, int left, int right, int down, int up>
struct step<UP, false_type, i, j, n, m, k, left, right, down, up> {
    step(std::array<std::array<int, n>, n>& map){}
};

template <int i, int j, int n, int m, int k, int left, int right, int down, int up>
struct step<LEFT, false_type, i, j, n, m, k, left, right, down, up> {
    step(std::array<std::array<int, n>, n>& map){}
};

/* 向右走 */
template <int i, int j, int n, int m, int k, int left, int right, int down, int up>
struct step<RIGHT, true_type, i, j, n, m, k, left, right, down, up> {
    step(std::array<std::array<int, n>, n>& map)
    {
        map[i][j] = k;
        step<RIGHT, typename cmpr_expr<j + 1 < right && k <= m>::result, i, j + 1, n, m, k + 1, left, right, down, up>a(map);
        step<DOWN, typename cmpr_expr<j + 1 == right && k <= m>::result, i, j, n, m, k, left, right - 1, down, up>b(map);
    }
};

/* 向下走 */
template <int i, int j, int n, int m, int k, int left, int right, int down, int up>
struct step<DOWN, true_type, i, j, n, m, k, left, right, down, up> {
    step(std::array<std::array<int, n>, n>& map)
    {
        map[i][j] = k;
        step<DOWN, typename cmpr_expr<i + 1 < down && k <= m>::result, i + 1, j, n, m, k + 1, left, right, down, up>a(map);
        step<LEFT, typename cmpr_expr<i + 1 == down && k <= m>::result, i, j, n, m, k, left, right, down - 1, up>b(map);
    }
};

/* 向左走 */
template <int i, int j, int n, int m, int k, int left, int right, int down, int up>
struct step<LEFT, true_type, i, j, n, m, k, left, right, down, up> {
    step(std::array<std::array<int, n>, n>& map)
    {
        map[i][j] = k;
        step<LEFT, typename cmpr_expr<left <= j - 1 && k <= m>::result, i, j - 1, n, m, k + 1, left, right, down, up>a(map);
        step<UP, typename cmpr_expr<j == left && k <= m>::result, i, j, n, m, k, left + 1, right, down, up>b(map);
    }
};

/* 向上走 */
template <int i, int j, int n, int m, int k, int left, int right, int down, int up>
struct step<UP, true_type, i, j, n, m, k, left, right, down, up> {
    step(std::array<std::array<int, n>, n>& map)
    {
        map[i][j] = k;
        step<UP, typename cmpr_expr<i - 1 >= up && k <= m>::result, i - 1, j, n, m, k + 1, left, right, down, up>a(map);
        step<RIGHT, typename cmpr_expr<i == up && k <= m>::result, i, j, n, m, k, left, right, down, up + 1>b(map);
    }
};

template <int n, int m>
std::array<std::array<int, n>, n> get_result()
{
    std::array<std::array<int, n>, n> map;
    step<RIGHT, true_type, 0, 0, 10, 120, 1, 0, 10, 10, 1> stp(map);
    return std::move(map);
}

int main(void) 
{
    auto map = get_result<10, 100>();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%4d", map[i][j]);
        }
        std::cout << std::endl;
    }

    return 0;
}
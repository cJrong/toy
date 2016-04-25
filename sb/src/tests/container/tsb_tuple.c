
#include <sb/container/sb_tuple.h>
#include <sb/io/sb_console.h>

sb_tuple1_id(TUPLE1, sb_char_t*);
sb_tuple2_id(TUPLE2, sb_char_t*, sb_int_t);
sb_tuple3_id(TUPLE3, sb_char_t*, sb_int_t, sb_int_t);
sb_tuple4_id(TUPLE4, sb_char_t*, sb_int_t, sb_int_t, sb_int_t);
sb_tuple5_id(TUPLE5, sb_char_t*, sb_int_t, sb_int_t, sb_int_t, sb_int_t);
sb_tuple6_id(TUPLE6, sb_char_t*, sb_int_t, sb_int_t, sb_int_t, sb_int_t, sb_int_t);

sb_tuple_t(TUPLE1, 1) func1()
{
    sb_tuple1_make(TUPLE1, v, "tuple1");
    sb_console_fmt("tuple1: %s\n", sb_tuple_get(TUPLE1, v, 0));
    return_tuple1(TUPLE1, "func1");
}

sb_tuple_t(TUPLE2, 2) func2()
{
    sb_tuple2_make(TUPLE2, v, "tuple2", 1);
    sb_console_fmt("tuple2: %s, %d\n", sb_tuple_get(TUPLE2, v, 0), sb_tuple_get(TUPLE2, v, 1));
    return_tuple2(TUPLE2, "func2", 1);
}

sb_tuple_t(TUPLE3, 3) func3()
{
    sb_tuple3_make(TUPLE3, v, "tuple3", 1, 2);
    sb_console_fmt("tuple3: %s, %d, %d\n", sb_tuple_get(TUPLE3, v, 0), sb_tuple_get(TUPLE3, v, 1), sb_tuple_get(TUPLE3, v, 2));
    return_tuple3(TUPLE3, "func3", 1, 2);
}

sb_tuple4_t(TUPLE4) func4()
{
    sb_tuple4_make(TUPLE4, v, "tuple4", 1, 2, 3);
    sb_console_fmt("tuple4: %s, %d, %d, %d\n", sb_tuple_get(TUPLE4, v, 0), sb_tuple_get(TUPLE4, v, 1), sb_tuple_get(TUPLE4, v, 2), sb_tuple_get(TUPLE4, v, 3));
    return_tuple4(TUPLE4, "func4", 1, 2, 3);
}

sb_tuple5_t(TUPLE5) func5()
{
    sb_tuple5_make(TUPLE5, v, "tuple5", 1, 2, 3, 4);
    sb_console_fmt("tuple5: %s, %d, %d, %d, %d\n", sb_tuple_get(TUPLE5, v, 0), sb_tuple_get(TUPLE5, v, 1), sb_tuple_get(TUPLE5, v, 2), sb_tuple_get(TUPLE5, v, 3), sb_tuple_get(TUPLE5, v, 4));
    return_tuple5(TUPLE5, "func5", 1, 2, 3, 4);
}

sb_tuple6_t(TUPLE6) func6()
{
    sb_tuple6_make(TUPLE6, v, "tuple6", 1, 2, 3, 4, 5);
    sb_console_fmt("tuple6: %s, %d, %d, %d, %d, %d\n", sb_tuple_get(TUPLE6, v, 0), sb_tuple_get(TUPLE6, v, 1), sb_tuple_get(TUPLE6, v, 2), sb_tuple_get(TUPLE6, v, 3), sb_tuple_get(TUPLE6, v, 4), sb_tuple_get(TUPLE6, v, 5));
    return_tuple6(TUPLE6, "func6", 1, 2, 3, 4, 5);
}

int main(void)
{
    sb_tuple1_t(TUPLE1) t1 = func1();
    sb_tuple2_t(TUPLE2) t2 = func2();
    sb_tuple3_t(TUPLE3) t3 = func3();
    sb_tuple4_t(TUPLE4) t4 = func4();
    sb_tuple5_t(TUPLE5) t5 = func5();
    sb_tuple6_t(TUPLE6) t6 = func6();

    sb_console_fmt("tuple1: %s\n", sb_tuple_get(TUPLE1, t1, 0));
    sb_console_fmt("tuple2: %s, %d\n", sb_tuple_get(TUPLE2, t2, 0), sb_tuple_get(TUPLE2, t2, 1));
    sb_console_fmt("tuple3: %s, %d, %d\n", sb_tuple_get(TUPLE3, t3, 0), sb_tuple_get(TUPLE3, t3, 1), sb_tuple_get(TUPLE3, t3, 2));
    sb_console_fmt("tuple4: %s, %d, %d, %d\n", sb_tuple_get(TUPLE4, t4, 0), sb_tuple_get(TUPLE4, t4, 1), sb_tuple_get(TUPLE4, t4, 2), sb_tuple_get(TUPLE4, t4, 3));
    sb_console_fmt("tuple5: %s, %d, %d, %d, %d\n", sb_tuple_get(TUPLE5, t5, 0), sb_tuple_get(TUPLE5, t5, 1), sb_tuple_get(TUPLE5, t5, 2), sb_tuple_get(TUPLE5, t5, 3), sb_tuple_get(TUPLE5, t5, 4));
    sb_console_fmt("tuple6: %s, %d, %d, %d, %d, %d\n", sb_tuple_get(TUPLE6, t6, 0), sb_tuple_get(TUPLE6, t6, 1), sb_tuple_get(TUPLE6, t6, 2), sb_tuple_get(TUPLE6, t6, 3), sb_tuple_get(TUPLE6, t6, 4), sb_tuple_get(TUPLE6, t6, 5));

    return 0;
}
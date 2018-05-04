#include "unity.h"
#include "small.c"

void test_addq_1(void)
{
    rational r1 = {.p = 2, .q = 1};
    rational r2 = {.p = 4, .q = 1};
    rational res = addq(r1, r2);

    rational expected_res = {.p = 6, .q = 1};
    TEST_ASSERT(res.p == expected_res.p);
    TEST_ASSERT(res.q == expected_res.q);
}

void test_addq_2(void)
{
    rational r1 = {.p = -2, .q = 1};
    rational r2 = {.p = 4, .q = 1};
    rational res = addq(r1, r2);

    rational expected_res = {.p = 2, .q = 1};
    TEST_ASSERT(res.p == expected_res.p);
    TEST_ASSERT(res.q == expected_res.q);
}

void test_reduce_zero(void)
{
    rational r1 = {.p = 0, .q = 1};
    r1 = reduce(r1);

    rational expected_res = {.p = 0, .q = 1};
    TEST_ASSERT(r1.p == expected_res.p);
    TEST_ASSERT(r1.q == expected_res.q);
}

void test_sign(void)
{
    rational r1 = {.p = 2, .q = 1};
    rational r2 = {.p = -3, .q = 1};
    rational r3 = {.p = 0, .q = 3};

    TEST_ASSERT(sign(r1) == 1);
    TEST_ASSERT(sign(r2) == -1);
    TEST_ASSERT(sign(r3) == 0);
}

void test_div_zero(void)
{
    rational r1 = {.p = 2, .q = 1};
    rational r2 = {.p = 0, .q = 1};

    rational res = divq(r2, r1);

    rational expected_res = {.p = 0, .q = 1};
    TEST_ASSERT(res.p == expected_res.p);
}

void test_big_div(void)
{
    rational r1 = {.p = 343548, .q = 46297};
    rational r2 = {.p = -102708, .q = 601861};
    rational sol = {.p = -41353, .q = 951};

    rational res = divq(r1, r2);

    TEST_ASSERT(res.p == sol.p);
    TEST_ASSERT(res.q == sol.q);
}

test_group_sort()
{
    rational r1 = {.p = 0, .q = 1};
    rational r2 = {.p = 1, .q = 1};
    rational r3 = {.p = -2, .q = 1};
    rational r4 = {.p = 3, .q = 1};
    rational r5 = {.p = -4, .q = 1};
    rational r6 = {.p = 0, .q = 1};
    rational r7 = {.p = 5, .q = 1};

    rational a[3][1] = {r1, r2, r3};
    rational c[3] = {r4, r5, r6};

    group_sort(3, 1, a, c);
    
    print_rational(a[0][0]);
    print_rational(a[1][0]);
    print_rational(a[2][0]);
    TEST_ASSERT(compare(a[0][0], r2) == 0);
    TEST_ASSERT(compare(a[1][0], r3) == 0);
    TEST_ASSERT(compare(a[2][0], r1) == 0);
}

void test_gcd_zero(void)
{
    TEST_ASSERT(gcd(0, 2) == 1);
}
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_addq_1);
    RUN_TEST(test_addq_2);
    RUN_TEST(test_reduce_zero);
    RUN_TEST(test_sign);
    RUN_TEST(test_div_zero);
    RUN_TEST(test_big_div);
    RUN_TEST(test_group_sort);
    return UNITY_END();
}

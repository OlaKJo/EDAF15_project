#include "unity.h"
#include "small.c"

void test_addq_1(void)
{
    rational r1 = {.p = 2, .q = 1};
    rational r2 = {.p = 4, .q = 1};
    rational res = addq(r1, r2);

    rational expected_res = {.p = 6, .q = 1};
    print_rational(res);
    print_rational(expected_res);
    TEST_ASSERT(res.p == expected_res.p);
    TEST_ASSERT(res.q == expected_res.q);
}

void test_addq_2(void)
{
    rational r1 = {.p = -2, .q = 1};
    rational r2 = {.p = 4, .q = 1};
    rational res = addq(r1, r2);

    rational expected_res = {.p = 2, .q = 1};
    print_rational(res);
    print_rational(expected_res);
    TEST_ASSERT(res.p == expected_res.p);
    TEST_ASSERT(res.q == expected_res.q);
}

void test_reduce_zero(void)
{
    rational r1 = {.p = 0, .q = 1};
    reduce(&r1);

    rational expected_res = {.p = 0, .q = 1};
    print_rational(r1);
    print_rational(expected_res);
    TEST_ASSERT(r1.p == expected_res.p);
    TEST_ASSERT(r1.q == expected_res.q);
}

void test_sign(void)
{
    rational r1 = {.p = 2, .q = 1};
    rational r2 = {.p = -3, .q = 1};
    rational r3 = {.p = 0, .q = 3};

    print_rational(r1);
    TEST_ASSERT(sign(r1) == 1);
    
    print_rational(r2);
    TEST_ASSERT(sign(r2) == -1);

    print_rational(r3);
    TEST_ASSERT(sign(r3) == 0);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_addq_1);
    RUN_TEST(test_addq_2);
    RUN_TEST(test_reduce_zero);
    RUN_TEST(test_sign);
    return UNITY_END();
}
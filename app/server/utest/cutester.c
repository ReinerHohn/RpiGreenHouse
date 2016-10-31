#include "socket/utest/socketTest.h"

#include <CUnit/CUnit.h>

void runTests(void);

int main()
{
    runTests();
}

void runTests()
{
    CU_pSuite pSuite;

    CU_initialize_registry();

    pSuite = CU_add_suite("Sockettest", NULL, NULL);
    CU_add_test(pSuite, "TransferTest", sockTransferTest);

    CU_automated_run_tests();

    CU_cleanup_registry();
}



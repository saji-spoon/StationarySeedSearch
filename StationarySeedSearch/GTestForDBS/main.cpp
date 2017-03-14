
#include<Windows.h>
#include<thread>
#include"../gtest_inc/gtest/gtest.h"
#include"../DB-CUI/db-sort.hpp"

#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )

TEST(test_case, DBSortBasicTest)
{
    try
    {
        DBSort dbs;
        std::thread th(std::ref(dbs));
        th.detach();

        for (;;)
        {
            Sleep(500);

            const double perProgress = 100.0*dbs.getProgress();
            printf("%.2lf %%\n", perProgress);

            if (dbs.getState() == DBSort::Finished)
            {
                 std::cout << "DBCreate finished.\n";
                 break;
            }
            else if (dbs.getState() == DBSort::Error)
            {
                ADD_FAILURE() << "Error";
                break;
            }
        }


    }
    catch (std::exception& ex)
    {
        ADD_FAILURE() << L"ExceptionFail";

        std::cout << ex.what() << "\n";
    }
}
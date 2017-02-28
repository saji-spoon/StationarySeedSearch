
#include<Windows.h>
#include"../gtest_inc/gtest/gtest.h"
#include"../DB-CUI/db-create.hpp"

#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )

TEST(test_case, test_name)
{
    try
    {
        DBCreator dbc;
        std::thread th(std::ref(dbc));
        th.detach();

        for (;;)
        {
            Sleep(500);

            const double perProgress = 100.0*dbc.getProgress();
            printf("%.2lf %%\n", perProgress);


            if (perProgress > 0.0001)
            {
                dbc.abort();
            }

            if (dbc.getState() == DBCreator::Finished)
            {
                ADD_FAILURE() << "DBCreate finished without abortion.";
            }
            else if (dbc.isAborted())
            {
                break;
            }
            else if (dbc.getState() == DBCreator::Error)
            {
                ADD_FAILURE() << dbc.getErrString();
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
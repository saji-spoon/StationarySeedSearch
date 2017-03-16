
#include<Windows.h>
#include"../gtest_inc/gtest/gtest.h"
#include"../DB-CUI/db-create.hpp"
#include"../DB-CUI/db-sort.hpp"
#include"../DB-CUI/db-search.hpp"

#pragma comment( lib, "gtestd.lib" )
//#pragma comment( lib, "gtest_maind.lib" )


TEST(DB_Search, Call)
{
    std::string testcase = "0BEEFBAG";

    std::vector<uint64_t> ticks;

    for (size_t i = 0; i<testcase.length(); ++i)
    {
        ticks.push_back(HepttoNum(testcase[i]));
    }

    std::vector<TickSeed> result = searchDB(ticks);

    if (result.size() == 0)
    {
        printf("Not found.\n");
    }
    else
    {
        for (const auto& res : result)
        {
            std::cout << tickToStr(seedToTick(res.seed, 10)) << ":" << std::hex << res.seed << std::dec << "\n";
        }
    }


}

TEST(DB_Sort, Basic) 
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
                std::cout << "DBSort finished.\n";
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

TEST(DB_Create, Basic)
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
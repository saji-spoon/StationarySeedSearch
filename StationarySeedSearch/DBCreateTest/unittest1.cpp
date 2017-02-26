#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DBCreateTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(DBCreate1)
		{
            try
            {
                DBCreator dbc;
                std::thread th(std::ref(dbc));

                th.detach();
                for (;;)
                {
                    Sleep(500);

                    printf("%.2lf %%\n", 100.0*dbc.getProgress());

                    if (dbc.getState() == DBCreator::Finished)
                    {
                        break;
                    }
                    else if (dbc.getState() == DBCreator::Error)
                    {
                        std::stringstream ss;
                        size_t ret;

                        wchar_t wcs[300];
                        mbstowcs_s(&ret, wcs, 100, dbc.getErrString().c_str(), _TRUNCATE);
                        Assert::Fail(wcs, LINE_INFO());
                        break;
                    }
                }


            }
            catch (std::exception& ex)
            {
                Assert::Fail(L"ExceptionFail", LINE_INFO());

                std::cout << ex.what() << "\n";
            }



			// TODO: テスト コードをここに挿入します
		}

	};
}
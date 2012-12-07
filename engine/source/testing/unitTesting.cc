//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// We don't want tests in a shipping version.
#ifndef TORQUE_SHIPPING

#ifndef _CONSOLEINTERNAL_H_
#include "console/consoleInternal.h"
#endif

#ifndef _UNIT_TESTING_H_
#include "testing/unitTesting.h"
#endif

#include "src/gtest.cc"
#include "src/gtest-death-test.cc"
#include "src/gtest-filepath.cc"
#include "src/gtest-port.cc"
#include "src/gtest-printers.cc"
#include "src/gtest-test-part.cc"
#include "src/gtest-typed-test.cc"

//-----------------------------------------------------------------------------

class TorqueUnitTestListener : public ::testing::EmptyTestEventListener
{
    // Called before a test starts.
    virtual void OnTestStart( const ::testing::TestInfo& testInfo )
    {
        Con::printf("> Starting Test '%s.%s'",
                testInfo.test_case_name(), testInfo.name());
    }

    // Called after a failed assertion or a SUCCEED() invocation.
    virtual void OnTestPartResult( const ::testing::TestPartResult& testPartResult )
    {
        if ( testPartResult.failed() )
        {
            Con::warnf(">> Failed with '%s' in '%s' at (line:%d)",
                    testPartResult.summary(),
                    testPartResult.file_name(),
                    testPartResult.line_number()
                    );
        }
        else
        {
            Con::printf(">> Passed with '%s' in '%s' at (line:%d)",
                    testPartResult.summary(),
                    testPartResult.file_name(),
                    testPartResult.line_number()
                    );
        }
    }

    // Called after a test ends.
    virtual void OnTestEnd( const ::testing::TestInfo& testInfo )
    {
        Con::printf("> Ending Test '%s.%s'",
                testInfo.test_case_name(), testInfo.name());
        Con::printBlankLine();
    }
};

//-----------------------------------------------------------------------------

ConsoleFunction( runAllUnitTests, S32, 1, 1, "() - Runs all the registered unit tests." )
{
    // Set-up some empty arguments.
    S32 testArgc = 0;
    char** testArgv = NULL;

    // Initialize Google Test.
    testing::InitGoogleTest( &testArgc, testArgv );

    // Fetch the unit test instance.
    testing::UnitTest& unitTest = *testing::UnitTest::GetInstance();

    // Fetch the unit test event listeners.
    testing::TestEventListeners& listeners = unitTest.listeners();

    // Release the default listener.
    delete listeners.Release( listeners.default_result_printer() );

    // Add the Torque unit test listener.
    listeners.Append( new TorqueUnitTestListener );

    Con::printBlankLine();
    Con::printSeparator();
    Con::printf( "Unit Tests Starting..." );
    Con::printBlankLine();

    const S32 result RUN_ALL_TESTS();

    Con::printBlankLine();
    Con::printf( "... Unit Tests Ended." );
    Con::printSeparator();
    Con::printBlankLine();

    return result;
}

#endif // TORQUE_SHIPPING
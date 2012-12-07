//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// We don't want tests in a shipping version.
#ifndef TORQUE_SHIPPING

#ifndef _UNIT_TESTING_H_
#include "testing/unitTesting.h"
#endif

#ifndef _PLATFORM_H_
#include "platform/platform.h"
#endif

//-----------------------------------------------------------------------------

#define PLATFORM_UNITTEST_MEMORY_BUFFERSIZE     16384

//-----------------------------------------------------------------------------

TEST( PlatformMemoryTests, dMallocrAnddFreeTest )
{
    // Allocate some memory.
    void* pResult = dMalloc_r( PLATFORM_UNITTEST_MEMORY_BUFFERSIZE, __FILE__, __LINE__ );

    // Check.
    ASSERT_NE( (void*)0, pResult ) << "Memory not allocated.";

    // Set memory (ensure no exceptions).
    for( U32 index = 0; index < PLATFORM_UNITTEST_MEMORY_BUFFERSIZE; ++index )
    {
        *((U8*)pResult) = index % 255;
    }

    // Free memory (ensure no exceptions).
    dFree( pResult );
}

//-----------------------------------------------------------------------------

TEST( PlatformMemoryTests, dReallocrAnddFreeTest )
{
    // Allocate some memory.
    void* pResult = dMalloc_r( PLATFORM_UNITTEST_MEMORY_BUFFERSIZE, __FILE__, __LINE__ );

    // Check.
    ASSERT_NE( (void*)0, pResult ) << "Memory not allocated.";

    // Reallocate some memory.
    pResult = dRealloc_r( pResult, PLATFORM_UNITTEST_MEMORY_BUFFERSIZE * 2, __FILE__, __LINE__ );

    // Check.
    ASSERT_NE( (void*)0, pResult ) << "Memory not allocated.";

    // Set memory (ensure no exceptions).
    for( U32 index = 0; index < (PLATFORM_UNITTEST_MEMORY_BUFFERSIZE*2); ++index )
    {
        *((U8*)pResult) = index % 255;
    }

    // Free memory (ensure no exceptions).
    dFree( pResult );
}

//-----------------------------------------------------------------------------

TEST( PlatformMemoryTests, dMemcpyTest )
{
    U8 source[] = { 0,1,2,3,4,5,6,7,8,9 };
    U8 destination[sizeof(source)];

    // Copy.
    void* result = dMemcpy( destination, source, sizeof(source) );

    // Check.
    ASSERT_EQ( result, destination ) << "Destination buffer incorrect.";

    // Check,
    for( U32 index = 0; index < sizeof(source); ++index )
    {
        ASSERT_EQ( index, source[index] ) << "Source memory value is incorrect.";
        ASSERT_EQ( index, destination[index] ) << "Destination memory value is incorrect.";
    }
}

//-----------------------------------------------------------------------------

TEST( PlatformMemoryTests, dMemmoveTest )
{
    U8 destination[] = { 0,1,2,3,4,5,6,7,8,9 };

    // Move.
    void* result = dMemmove( destination, destination+1, sizeof(destination)-1 );

    // Check.
    ASSERT_EQ( result, destination ) << "Destination buffer incorrect.";

    // Check,
    for( U32 index = 0; index < (sizeof(destination)-1); ++index )
    {
        ASSERT_EQ( index+1, destination[index] ) << "Destination memory value is incorrect.";
    }
}

//-----------------------------------------------------------------------------

TEST( PlatformMemoryTests, dMemsetTest )
{
    U8 destination[] = { 0,1,2,3,4,5,6,7,8,9 };

    const U8 magic = 128;

    // Set.
    void* result = dMemset( destination, magic, sizeof(destination)-1 );

    // Check.
    ASSERT_EQ( result, destination ) << "Destination buffer incorrect.";

    // Check,
    for( U32 index = 0; index < (sizeof(destination)-1); ++index )
    {
        ASSERT_EQ( magic, destination[index] ) << "Destination memory value is incorrect.";
    }

    // Check.
    ASSERT_EQ( 9, destination[sizeof(destination)-1] ) << "Destination memory value is incorrect.";
}

//-----------------------------------------------------------------------------

TEST( PlatformMemoryTests, dMemcmpTest )
{
    U8 source1[] = { 0,1,2,3,4,5,6,7,8,10 };
    U8 source2[] = { 0,1,2,3,4,5,6,7,8,9 };
    U8 source3[] = { 0,1,2,3,4,5,6,7,8,0 };

    U8 destination[] = { 0,1,2,3,4,5,6,7,8,9 };

    // Compare.
    const S32 result1 = dMemcmp( source1, destination, sizeof(destination) );
    const S32 result2 = dMemcmp( source2, destination, sizeof(destination) );
    const S32 result3 = dMemcmp( source3, destination, sizeof(destination) );

    // Check.
    ASSERT_LT( 0, result1 ) << "Memory compare is incorrect.";
    ASSERT_EQ( 0, result2 ) << "Memory compare is incorrect.";
    ASSERT_GT( 0, result3 ) << "Memory compare is incorrect.";
}


#endif // TORQUE_SHIPPING
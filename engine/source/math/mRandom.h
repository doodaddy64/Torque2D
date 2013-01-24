//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _MRANDOM_H_
#define _MRANDOM_H_

#ifndef _PLATFORM_H_
#include "platform/platform.h"
#endif

//-----------------------------------------------------------------------------

class RandomGeneratorBase
{
protected:
    RandomGeneratorBase() {}
    S32  mSeed;

public:
    void resetSeed( void );
    inline S32 getSeed( void ) const { return mSeed; }

    inline F32 randRangeF( const F32 from, const F32 to ) { AssertFatal( from <= to, "RandomGeneratorBase::randF() - Inverted range." ); return (from + (to - from) * randF()); }
    inline S32 randRangeI( const S32 from, const S32 to ) { AssertFatal( from <= to, "RandomGeneratorBase::randI() - Inverted range." ); return (S32)(from + (randI() % (to - from + 1)) ); }

    virtual F32 randF( void ) { return F32(randI()) * F32(1.0/2147483647.0); }
    virtual U32 randI( void ) = 0;
    virtual void setSeed( const S32 seed ) = 0;
};

//-----------------------------------------------------------------------------
/// Linear Congruential Method, the "minimal standard generator"
///
/// Fast, fairly good random numbers (better than using rand)
///
/// @author Park & Miller, 1988, Comm of the ACM, 31(10), pp. 1192-1201
//-----------------------------------------------------------------------------
class RandomLCG : public RandomGeneratorBase
{
protected:
   static const S32 msQuotient;
   static const S32 msRemainder;

public:
    RandomLCG();
    RandomLCG( const S32 seed );

    static void setGlobalRandSeed( const U32 seed );

    void setSeed( const S32 seed );
    U32 randI( void );
};


//-----------------------------------------------------------------------------
/// Fast, very good random numbers
///
/// Period = 2^249
///
/// Kirkpatrick, S., and E. Stoll, 1981; A Very Fast Shift-Register
///       Sequence Random Number Generator, Journal of Computational Physics,
/// V. 40.
///
/// Maier, W.L., 1991; A Fast Pseudo Random Number Generator,
/// Dr. Dobb's Journal, May, pp. 152 - 157
//-----------------------------------------------------------------------------
class RandomR250 : public RandomGeneratorBase
{
private:
   U32 mBuffer[250];
   S32 mIndex;

public:
   RandomR250();
   RandomR250( const S32 seed );

   void setSeed( const S32 seed );
   U32 randI( void );
};

//-----------------------------------------------------------------------------

extern RandomLCG gRandGen;

#endif //_MRANDOM_H_

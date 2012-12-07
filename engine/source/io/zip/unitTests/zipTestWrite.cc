//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------
#ifdef TORQUE_ALLOW_UNIT_TESTS

#include "platform/platform.h"

#include "unit/test.h"
#include "unit/memoryTester.h"

#include "io/zip/zipArchive.h"
#include "io/zip/unitTests/zipTest.h"

#include "string/stringTable.h"

using namespace UnitTesting;
using namespace Zip;

CreateUnitTest(ZipTestWrite, "Zip/Write")
{
private:
   StringTableEntry mWriteFilename;
   StringTableEntry mBaselineFilename;
   StringTableEntry mWorkingFilename;

public:
   /// Number of files to write for testing large numbers of files in a zip
   static const U32 mTons = 5000;

   ZipTestWrite()
   {
      mWriteFilename = makeTestPath(ZIPTEST_WRITE_FILENAME);
      mBaselineFilename = makeTestPath(ZIPTEST_BASELINE_FILENAME);
      mWorkingFilename = makeTestPath(ZIPTEST_WORKING_FILENAME);
   }

   void run()
   {
      MemoryTester m;
      m.mark();

      // Clean up from a previous run
      cleanup();
      
      // Test writing to zip files without the zip file existing
      testWriting(mWriteFilename, ZipArchive::Read);
      testWriting(mWriteFilename, ZipArchive::ReadWrite);

      // Cleanup to try write without existing file
      cleanup();
      testWriting(mWriteFilename, ZipArchive::Write);

      // Now use previous file to test everything again with an existing file
      testWriting(mWriteFilename, ZipArchive::ReadWrite);
      testWriting(mWriteFilename, ZipArchive::Write);
      testWriting(mWriteFilename, ZipArchive::Read);

      testWritingTons(makeTestPath("WriteTons.zip"));

      test(m.check(), "Zip write test leaked memory!");
   }

private:

   //////////////////////////////////////////////////////////////////////////

   void cleanup()
   {
      if(Platform::isFile(mWriteFilename))
         Platform::fileDelete(mWriteFilename);
      if(Platform::isFile(mWorkingFilename))
         Platform::fileDelete(mWorkingFilename);
   }

   //////////////////////////////////////////////////////////////////////////

   bool writeFile(ZipArchive *zip, const char *filename, char *fileBuf, U32 bufSize, bool mustNotExist = false, const char *contents = NULL)
   {
      if(mustNotExist && fileBuf && bufSize > 0)
      {
         // Find a unique filename
         U32 count = 1;
         dStrcpy(fileBuf, filename);
         
         while(zip->findFileInfo(fileBuf))
         {
            dSprintf(fileBuf, bufSize, "%s.%04x", filename, count++);

            if(count == 0)
            {
               fail("writeFile - got stuck in an infinite loop looking for a unique filename");
               return false;
            }
         }
      }
      else if(fileBuf && bufSize > 0)
         dStrcpy(fileBuf, filename);

      // Try and write to the file
      Stream *stream;
      if(stream = zip->openFile(fileBuf ? fileBuf : filename, ZipArchive::Write))
      {
         stream->writeLine(contents ? (U8 *)contents : (U8 *)"This is a test of writing to a file.");
         zip->closeFile(stream);

         return true;
      }

      return false;
   }

   //////////////////////////////////////////////////////////////////////////

   bool testWriting(const char *zipfile, ZipArchive::AccessMode mode)
   {
      ZipArchive *zip = new ZipArchive;

      if(! zip->openArchive(zipfile, mode))
      {
         delete zip;
         
         // This is only an error if we're not trying to open as read

         if(mode != ZipArchive::Read)
            fail("Unable to open zip file");
         
         return mode == ZipArchive::Read;
      }

      char fileBuf[1024];

      // Write to file that doesn't already exist
      if(!writeFile(zip, "testWriteNew.txt", fileBuf, sizeof(fileBuf), true))
      {
         fail("Couldn't write to a file that didn't already exist");
         goto bail;
      }

      // Write to file that we've already written to
      if(!writeFile(zip, fileBuf, NULL, 0, false, "This is a test of overwriting the file."))
      {
         if(mode != ZipArchive::Read)
            fail("Couldn't write to a file that we've already written to");
         goto bail;
      }

      // Write to file that already exists, but we haven't written to
      // To do this, we need to close and re-open the zipfile

      zip->closeArchive();
      delete zip;

      zip = new ZipArchive;
      if(! zip->openArchive(zipfile, mode))
      {
         delete zip;
         fail("Unable to re-open zip file. Strange!");
         return false;
      }
      
      // Use the file we already overwrote since we are sure of it's filename
      if(!writeFile(zip, fileBuf, NULL, 0, false, "This is a test of overwriting the file again."))
      {
         if(mode != ZipArchive::Read)
            fail("Couldn't write to a file that already existed");
         goto bail;
      }

      // Attempt to open a file for write that's already open for write (should fail)
      Stream *stream1, *stream2;
      if(stream1 = zip->openFile("writeLockTest.txt", ZipArchive::Write))
      {
         stream1->writeLine((U8 *)"Test text to make a valid file");

         // This next open should fail
         if(stream2 = zip->openFile("writeLockTest.txt", ZipArchive::Write))
         {
            if(mode != ZipArchive::Read)
               fail("Opening a file for write multiple times should have failed");
            zip->closeFile(stream2);
         }
         zip->closeFile(stream1);
      }
      
bail:
      zip->closeArchive();
      delete zip;
      
      return true;
   }

   //////////////////////////////////////////////////////////////////////////

   bool testWritingTons(const char *zipfile)
   {
      ZipArchive zip;

      if(! zip.openArchive(zipfile, ZipArchive::Write))
      {
         fail("Unable to open zip file");
         return false;
      }

      bool ret = true;

      for(U32 i = 0;i < mTons;++i)
      {
         char fname[256];
         dSprintf(fname, sizeof(fname), "file%04x.txt", i);

         if(! writeFile(&zip, fname, NULL, 0))
         {
            fail("Failed to write file");
            fail(fname);

            ret = false;

            break;
         }
      }

      zip.closeArchive();
      return ret;
   }
};

#endif //TORQUE_ALLOW_UNIT_TESTS
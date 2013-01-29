# Update all T2D copyright headers

import os
import glob

readOnly = False

# dirs = ['engine/source']
# dirs = ['modules']
dirs = ['engine/source']

donottouch = ['cmdgram.cpp', 'cmdgram.h', 'CMDscan.cpp', 'VFSRes.h']

# C++ style comments: //
chandled = ['.h', '.cpp', '.cc', '.mm', '.cs']
# asm style comments: ;
ahandled = ['.asm']
unhandled = []

filecount = 0
dircount = 0

companies = ['GarageGames', 'InstantAction']

# Our console file to hold all issue files
cf = None

# A file to hold all of the 'valid' headers, for checking
hf = None

header = [	'//-----------------------------------------------------------------------------', \
			'// Copyright (c) 2013 GarageGames, LLC', \
			'//', \
			'// Permission is hereby granted, free of charge, to any person obtaining a copy', \
			'// of this software and associated documentation files (the "Software"), to', \
			'// deal in the Software without restriction, including without limitation the', \
			'// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or', \
			'// sell copies of the Software, and to permit persons to whom the Software is', \
			'// furnished to do so, subject to the following conditions:', \
			'//', \
			'// The above copyright notice and this permission notice shall be included in', \
			'// all copies or substantial portions of the Software.', \
			'//', \
			'// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR', \
			'// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,', \
			'// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE', \
			'// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER', \
			'// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING', \
			'// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS', \
			'// IN THE SOFTWARE.', \
			'//-----------------------------------------------------------------------------',]

aheader = [	';-----------------------------------------------------------------------------', \
			'; Copyright (c) 2013 GarageGames, LLC', \
			';', \
			'; Permission is hereby granted, free of charge, to any person obtaining a copy', \
			'; of this software and associated documentation files (the "Software"), to', \
			'; deal in the Software without restriction, including without limitation the', \
			'; rights to use, copy, modify, merge, publish, distribute, sublicense, and/or', \
			'; sell copies of the Software, and to permit persons to whom the Software is', \
			'; furnished to do so, subject to the following conditions:', \
			';', \
			'; The above copyright notice and this permission notice shall be included in', \
			'; all copies or substantial portions of the Software.', \
			';', \
			'; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR', \
			'; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,', \
			'; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE', \
			'; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER', \
			'; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING', \
			'; FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS', \
			'; IN THE SOFTWARE.', \
			';-----------------------------------------------------------------------------',]

def scandirs(path):
    global dircount, filecount
    for currentFile in glob.glob( os.path.join(path, '*') ):
        if os.path.isdir(currentFile):
            #print 'got a directory: ' + currentFile
            dircount += 1
            scandirs(currentFile)
            continue

        filecount += 1
        basename, ext = os.path.splitext(currentFile)

        base = os.path.basename(currentFile)
        if base in donottouch:
            # Don't process this file
            continue;
        
        if ext in chandled:
            processCFile(currentFile)
        elif ext in ahandled:
            processAFile(currentFile)
        else:
            if not ext in unhandled:
                unhandled.append(ext)

def processCFile(path):
    global cf
    
    f = open(path, 'r')
    lines = f.readlines()
    f.close();
    
    # Check that we have the correct signature at the top of
    # the file.
    if len(lines) < 4 or \
       lines[0][0:3] != '//-' or lines[1][0:2] != '//' or \
       lines[2][0:2] != '//' or lines[3][0:3] != '//-':
        print 'Missing comment header: ' + path

        # Output the problem file to the console log
        cf.write(path + '\n')
        if len(lines) > 0 and lines[0][0:2] == '//':
            # Handle files that have // comments but differ
            # in other ways
            count = 0;
            for line in lines:
                count += 1
                if line[0:2] == '//':
                    cf.write(line)
                    if count >= 10:
                        # 10 lines is enough
                        cf.write('...\n')
                        break
                else:
                    break
                
        elif len(lines) > 0 and lines[0][0:2] == '/*':
            # Handle files with /**/ style comments
            for line in lines:
                cf.write(line)
                if line.rfind('*/') != -1:
                    break;

        else:
            # Write out the top four line, if possible
            count = 0
            for line in lines:
                cf.write(line)
                count += 1
                if count >= 4:
                    cf.write('...\n')
                    break;
                
        cf.write('\n')

    else:
        # Top header has the correct formating.  Check that it
        # has one of the known companies.
        global companies
        check = False
        for company in companies:
            if lines[2].find(company):
                check = True
                break

        if check == True:
            # One of the known companies
            hf.write(path + '\n')
            count = 0;
            for line in lines:
                count += 1
                if line[0:2] == '//':
                    hf.write(line)
                    if count >= 10:
                        # 10 lines is enough
                        hf.write('...\n')
                        break
                else:
                    break

            hf.write('\n')

            updateProperFormatCFile(path, lines)
            
        else:
            # Not one of the known companies, so is an issue
            cf.write(path + '\n')
            count = 0;
            for line in lines:
                count += 1
                if line[0:2] == '//':
                    cf.write(line)
                    if count >= 10:
                        # 10 lines is enough
                        cf.write('...\n')
                        break
                else:
                    break

            cf.write('\n')

# Update the header on a properly formatted C file
def updateProperFormatCFile(path, lines):
    global readOnly
    if readOnly is True:
        return
    
    print 'Correcting ' + path
    
    f = open(path, 'w')

    count = 0;
    for line in lines:
        if count == 0:
            # Write out all of the header lines at once
            for hl in header:
                f.write(hl + '\n')
        elif count >= 4:
            # Start writing out the file's code past the 4 lines of previous header
            f.write(line)
        count += 1
    
    f.close()

def processAFile(path):
    global cf
    
    f = open(path, 'r')
    lines = f.readlines()
    f.close();
    
    # Check that we have the correct signature at the top of
    # the file.
    if lines[0][0:2] != ';-' or lines[1][0:1] != ';' or \
       lines[2][0:1] != ';' or lines[3][0:2] != ';-':
        print 'Missing comment header: ' + path

        # Output the problem file to the console log
        cf.write(path + '\n')
        # Handle files that have ; comments but differ
        # in other ways
        count = 0;
        for line in lines:
            count += 1
            cf.write(line)
            if count >= 10:
                # 10 lines is enough
                cf.write('...\n')
                break
            
    else:
        # Top header has the correct formating.  Check that it
        # has one of the known companies.
        global companies
        check = False
        for company in companies:
            if lines[2].find(company):
                check = True
                break

        if check == True:
            # One of the known companies
            hf.write(path + '\n')
            count = 0;
            for line in lines:
                count += 1
                if line[0:1] == ';':
                    hf.write(line)
                    if count >= 10:
                        # 10 lines is enough
                        hf.write('...\n')
                        break
                else:
                    break

            hf.write('\n')

            updateProperFormatAFile(path, lines)
            
        else:
            # Not one of the known companies, so is an issue
            cf.write(path + '\n')
            count = 0;
            for line in lines:
                count += 1
                if line[0:2] == '//':
                    cf.write(line)
                    if count >= 10:
                        # 10 lines is enough
                        cf.write('...\n')
                        break
                else:
                    break

            cf.write('\n')

# Update the header on a properly formatted C file
def updateProperFormatAFile(path, lines):
    global readOnly
    if readOnly is True:
        return
    
    print 'Correcting ' + path
    
    f = open(path, 'w')

    count = 0;
    for line in lines:
        if count == 0:
            # Write out all of the header lines at once
            for ahl in aheader:
                f.write(ahl + '\n')
        elif count >= 4:
            # Start writing out the file's code past the 4 lines of previous header
            f.write(line)
        count += 1
    
    f.close()

if __name__ == "__main__":
    cf = open(os.path.abspath('copyrightissues.txt'), 'w')
    hf = open(os.path.abspath('copyrightheaders.txt'), 'w')
    
    print "Starting file scan"
    for directory in dirs:
        start = os.path.abspath(directory)
        print 'Processing ' + start
        scandirs( start )
        
    print 'Processed ' + str(filecount) + ' files.'
    print 'Processed ' + str(dircount) + ' directories.'
    print 'Found ' + str(len(unhandled)) + ' unhandled file extensions.'

    print 'Unhandled extensions:'    
    for ext in unhandled:
        print ext

    hf.close()
    cf.close()

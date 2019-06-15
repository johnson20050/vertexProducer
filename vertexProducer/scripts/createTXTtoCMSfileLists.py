#!/usr/bin/env python2
#./createTXTtoCMSfileLists.py -i $HOME/path -o myFile -d myDir -D

import os
import argparse
parser=argparse.ArgumentParser(description='Utility to separate multiple line files, you need to setup each file with N lines')
parser.add_argument('--outputTag', '-o',type=str,help='tag to create outputFileName, "tag" inputed will goes to "tag_(inputFileName)_cfi.py" ', default='tmpTag')
parser.add_argument('--inputFileName', '-i', type=str, help='input file to be separate', default=None)
parser.add_argument('--outputDirectory', '-d', type=str, help='select where to store output file', default='.')
parser.add_argument('--deleteSource', '-D', help='delete input TXT file after finished', action="store_true")

args=parser.parse_args()

if not args.inputFileName:
    parser.print_help()
    exit()

# abstract code to output file.
fileContent='''import FWCore.ParameterSet.Config as cms
readFiles = cms.untracked.vstring()
readFiles.extend( [
{}
] )
'''

namelists=None
with open(args.inputFileName) as ipfile:
    # handle input file path to file name. /a/b/c.txt -> c
    sepIFName=args.inputFileName.split('/')
    sepedIFName=sepIFName[ len(sepIFName)-1 ]
    finalIFName=sepedIFName.split('.') [0]

    totlists=ipfile.readlines()

    # processing content with extra words. /a/b/c.root ->"file:///a/b/c.root"
    fileFormatList=[ '"file://{}"'.format(line.split()[0]) for line in totlists ]

    outputFile='{}/{}_{}_cfi.py'.format(args.outputDirectory, args.outputTag, finalIFName)
    fOut=open(outputFile, 'w')
    fOut.write(fileContent.format(',\n'.join(fileFormatList)))
    fOut.close()

    # delete input source
    if args.deleteSource:
        os.system(args.inputFileName)

    print 'your file is stored at {}'.format(outputFile)

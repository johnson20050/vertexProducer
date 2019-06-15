#!/usr/bin/env python2
#./separateFileToTXT.py -N 100 -o tmpFile -i ${HOME}/path // if $HOME/path exist and ready to be separated

from DataFormats.FWLite import Events, Handle
import argparse
parser=argparse.ArgumentParser(description='Utility to separate multiple line files, you need to setup each file with N lines')
parser.add_argument('--sepNumber', '-N',type=int,help='how much lines per file?', default=50)
parser.add_argument('--outputName', '-o',type=str,help='general list file name, like tmpFile_1.txt with tmpFile', default='tmpFile')
parser.add_argument('--inputFileName', '-i', type=str, help='input file to be separate', default=None)

args=parser.parse_args()
NUM_EACH_JOB=args.sepNumber

if not args.inputFileName:
    parser.print_help()
    exit()


namelists=None
with open(args.inputFileName) as ipfile:
    totlists=ipfile.readlines()
    namelists=[ totlists[x:x+NUM_EACH_JOB] for x in xrange(0, len(totlists), NUM_EACH_JOB) ]

    idx=0
    for namelist in namelists:
        fOut=open('{}_{}.txt'.format(args.outputName,idx), 'w')
        for name in namelist:
            fOut.write(name)
        fOut.close()
        idx+=1

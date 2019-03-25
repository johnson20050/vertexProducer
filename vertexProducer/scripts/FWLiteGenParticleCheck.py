#!/usr/bin/env python
#./FWLiteGenParticleCheck.py --idx=3 --listName=tmpFileList; // if there is tmpFileList_)3.txt at certain directory
# this file is recommended to work with separateFileToTXT.py
def passTrig(val, idx):
    return (abs(val)>>idx)%2
import ROOT
from DataFormats.FWLite import Events, Handle
import argparse
parser=argparse.ArgumentParser(description='Process to calculate efficiency')
parser.add_argument('--idx', type=int, help='number to indicate index', default=-1)
parser.add_argument('--listName',type=str,help='general list file name, like tmpFile_1.txt with tmpFile')
args=parser.parse_args()

if args.idx == -1:
    parser.print_help()
    exit()
elif not args.listName:
    parser.print_help()
    exit()


fileList=None
with open('{}_{}.txt'.format(args.listName, args.idx), 'r') as ipFile:
    fileList=[ x.rstrip('\n') for x in ipFile.readlines() ]

# don't pop up canvases
ROOT.gROOT.SetBatch()
rootFile=ROOT.TFile('recordGPContent_{}.root'.format(args.idx),'recreate')
hGenRec=ROOT.TH1I('genRec','genRec',30,-10,20)

import array
varNames=['numDaugs', 'selfID', 'motherID' ]
NUM=11
varNames.extend( [ 'dau{}ID'.format(x) for x in range(0,NUM) ] )
ntuple=ROOT.TNtuple('exotics', 'exotics', ':'.join(varNames))

for fName in fileList:
    events=Events(fName)
    handle=Handle('vector<reco::GenParticle>')
    label=("genParticles::HLT")

    # don't pop up canvases
    ROOT.gROOT.SetBatch()

    # set white background
    ROOT.gROOT.SetStyle('Plain')

    for event in events:
        event.getByLabel(label,handle)
        genList=handle.product()

        nParticles=0
        for gp in genList:
            if gp.pdgId() != 5122 and gp.pdgId() != -5122:
                continue
            nParticles+=1
            if gp.daughter(0).pdgId() == 4414:
                hGenRec.Fill(-2)
            elif gp.daughter(0).pdgId() == -4414:
                hGenRec.Fill(-3)
            else:
                varlist=[
                        gp.numberOfDaughters(),
                        gp.pdgId(),
                        gp.mother().pdgId(),
                        ]
                varlist.extend( [0 for x in range(0,NUM)] )
                for x in range(0,NUM):
                    if x < gp.numberOfDaughters():
                        varlist[3+x]=gp.daughter(x).pdgId()
                ntuple.Fill(array.array('f',varlist))
            if gp.daughter(0).pdgId() == 3122:
                hGenRec.Fill(-5)
            elif gp.daughter(0).pdgId() == -3122:
                hGenRec.Fill(-5)
            elif gp.daughter(1).pdgId() == 3122:
                hGenRec.Fill(-6)
            elif gp.daughter(1).pdgId() == -3122:
                hGenRec.Fill(-6)
            if gp.numberOfDaughters() > 2:
                if gp.daughter(2).pdgId() == 3122:
                    hGenRec.Fill(-7)
                elif gp.daughter(2).pdgId() == -3122:
                    hGenRec.Fill(-7)
        hGenRec.Fill(nParticles)
    print fName+' done'

rootFile.Write()
rootFile.Close()

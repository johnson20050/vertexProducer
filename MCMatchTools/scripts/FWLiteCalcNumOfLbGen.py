#!/usr/bin/env python
#./FWLiteGenParticleCheck.py --idx=3 --listName=tmpFileList; // if there is tmpFileList_)3.txt at certain directory
# this file is recommended to work with separateFileToTXT.py
def passTrig(val, idx):
    return (abs(val)>>idx)%2
def listAllDaugs(mcCand):
    for dIdx in range(mcCand.numberOfDaughters()):
        print "mc candidate {} with daughter {} = {}".format(mcCand.pdgId(), dIdx, mcCand.daughter(dIdx).pdgId())
    print ""
import ROOT

from DataFormats.FWLite import Events, Handle
import argparse
parser=argparse.ArgumentParser(description='Process to calculate efficiency')
parser.add_argument('--idx', type=int,default=None,                     help='number to indicate index')
parser.add_argument('--listName',type=str,default=None,                 help='general list file name, like tmpFile_1.txt with tmpFile')
parser.add_argument('--testFile',type=str,default=None,                 help='input test file, if use this option, [--idx] and [--listName] will not be used')
parser.add_argument('--outputFile','-o', type=str,default='genInfoRec', help='set output file name. output will append the result at the end of file')
parser.add_argument('--ptonTrackCut','-cp',type=float,default=0.8,      help='set proton track pt cut')
parser.add_argument('--ntrkTrackCut','-ck',type=float,default=0.3,      help='set ntrk track pt cut')
parser.add_argument('--muonTrackCut','-cm',type=float,default=4.0,      help='set muon track pt cut')
args=parser.parse_args()



fileList=None
if args.testFile:
    fileList=[]
    fileList.append(args.testFile)
else:
    if not args.idx:
        parser.print_help()
        exit()
    elif not args.listName:
        parser.print_help()
        exit()
    with open('{}_{}.txt'.format(args.listName, args.idx), 'r') as ipFile:
        fileList=[ x.rstrip('\n') for x in ipFile.readlines() ]


nPassedCand=0
nTotalCands=0
for fName in fileList:
    events=Events(fName)
    handle=Handle('vector<reco::GenParticle>')
    label=("genParticles::HLT")

    for event in events:
        event.getByLabel(label,handle)
        genList=handle.product()

        for gp in genList:
            if abs(gp.pdgId()) != 5122: continue
            # for Lb->pQ + K, pQ->Jpsi + p
            #penQ=gp.daughter(0)
            #if abs(penQ.pdgId()) != 4414: continue
            #ntrk=gp.daughter(1)
            #if abs(ntrk.pdgId()) != 321: continue
            #ptrk=gp.daughter(0).daughter(1)
            #if abs(ptrk.pdgId()) != 2212: continue
            #jpsi=gp.daughter(0).daughter(0)
            #if abs(jpsi.pdgId()) != 443: continue


            # for Lb->Lam0+jpsi
            lam0=gp.daughter(0)
            jpsi=gp.daughter(1)
            if abs(jpsi.pdgId()) != 443: continue
            if abs(lam0.pdgId()) != 3122: continue
            ptrk=lam0.daughter(0)
            if lam0.numberOfDaughters() < 2:
                listAllDaugs(lam0)
                continue
            if abs(ptrk.pdgId()) != 2212:
                listAllDaugs(lam0)
                continue
            ntrk=lam0.daughter(1)
            if abs(ntrk.pdgId()) != 211:
                listAllDaugs(lam0)
                continue


            p_mu=jpsi.daughter(0)
            n_mu=jpsi.daughter(1)


            nTotalCands+=1
            # set cuts:
            if ptrk.pt() < args.ptrkTrackCut: continue #proton
            if ntrk.pt() < args.ntrkTrackCut: continue #ntrk
            if p_mu.pt() < args.muonTrackCut: continue #muon
            if n_mu.pt() < args.muonTrackCut: continue #muon
            if ptrk.eta()> 2.5 or ptrk.eta()<-2.5: continue
            if ntrk.eta()> 2.5 or ntrk.eta()<-2.5: continue
            if p_mu.eta()> 2.5 or p_mu.eta()<-2.5: continue
            if n_mu.eta()> 2.5 or n_mu.eta()<-2.5: continue


            nPassedCand+=1

    print fName+' done'

fOUT=open('{}.txt'.format(args.outputFile), mode='a+')
fOUT.write('{}/{}\n'.format(nPassedCand,nTotalCands))
fOUT.close()

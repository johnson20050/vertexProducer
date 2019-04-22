#!/usr/bin/env python
#./effReaderSplit.py --idx=3 --listName=tmpFileList; // if there is tmpFileList_)3.txt at certain directory
# this file is recommended to work with separateFileToTXT.py
def passTrig(val, idx):
    return (abs(val)>>idx)%2
import ROOT
from DataFormats.FWLite import Events, Handle
import argparse
parser=argparse.ArgumentParser(description='Process to calculate efficiency')
parser.add_argument('--idx', type=int, help='number to indicate index')
parser.add_argument('--listName',type=str,help='general list file name, like tmpFile_1.txt with tmpFile')
args=parser.parse_args()

fileList=None
with open('{}_{}.txt'.format(args.listName, args.idx), 'r') as ipFile:
    fileList=[ x.rstrip('\n') for x in ipFile.readlines() ]

# don't pop up canvases
ROOT.gROOT.SetBatch()
rootFile=ROOT.TFile('result_efficiency_{}.root'.format(args.idx),'recreate')

# set white background
ROOT.gROOT.SetStyle('Plain')

nameLabel={
        'preSel_mu'  :"selectedMuons:MuonPreselectionEfficiencyBoolInt:myVertexingProcedure",
        'preSel_tk'  :"selectedTracks:TrackPreselectionEfficiencyBoolInt:myVertexingProcedure",
        'vtxEff_LbL0':"fourTracksFromVCCProducer:fourTkVertexingEfficiencyBoolIntLbL0:myVertexingProcedure",
        'vtxEff_LbLo':"fourTracksFromVCCProducer:fourTkVertexingEfficiencyBoolIntLbLo:myVertexingProcedure",
        'vtxEff_nL0B':"fourTracksFromVCCProducer:fourTkVertexingEfficiencyBoolIntnL0B:myVertexingProcedure",
        'vtxEff_pL0B':"fourTracksFromVCCProducer:fourTkVertexingEfficiencyBoolIntpL0B:myVertexingProcedure",
        'vtxEff_mumu':"mumuVertexingProducer:mumuVertexingEfficiencyBoolInt:myVertexingProcedure",
        'vtxEff_tktk':"tktkVertexingProducer:tktkVertexingEfficiencyBoolInt:myVertexingProcedure",
        }
histLabel={}
for name, label in nameLabel.iteritems():
    histLabel.update( {ROOT.TH1I(name,name,70,0,35) : label} )
histLabelMatched={}
for name, label in nameLabel.iteritems():
    histLabelMatched.update( {ROOT.TH1I('matched_'+name,name+'_matched',70,0,35) : label} )
hTot=ROOT.TH1I('totFourVtxEfficiency','totFourVtxEfficiency',8,0,8)

for fName in fileList:
    events=Events(fName)
    #events=Events('ta.root')

    for event in events:
        # event without MC truth matching
        for hist, label in histLabel.iteritems():
            handle=Handle('std::vector<int>')
            evtlabel=(label)
            event.getByLabel(evtlabel,handle)
            encodedEffvalueList=handle.product()

            for effValue in encodedEffvalueList:
                if effValue < 0:
                    effValue *= -1
                for idx in range(1,30):
                    if passTrig(effValue,idx):
                        hist.Fill(idx)
                if passTrig(effValue,0):
                    hist.Fill(31)

        # event with MC truth matching
        for hist, label in histLabelMatched.iteritems():
            handle=Handle('std::vector<int>')
            evtlabel=(label)
            event.getByLabel(evtlabel,handle)
            encodedEffvalueList=handle.product()

            for effValue in encodedEffvalueList:
                if effValue > 0:
                    continue;
                effValue *= -1
                for idx in range(1,30):
                    if passTrig(effValue,idx):
                        hist.Fill(idx)
                if passTrig(effValue,0):
                    hist.Fill(31)

        handleINT=Handle('int')
        evtlabelINT=("fourTracksFromVCCProducer:fourTracksTotallyVertexingEfficiency:myVertexingProcedure")
        event.getByLabel(evtlabelINT,handleINT)
        effval=handleINT.product()[0]
        if effval<0:
            effval*=-1
        for idx in range(0,5):
            if passTrig(effval,idx):
                hTot.Fill(idx)
    print fName+' done'

rootFile.Write()
rootFile.Close()

#!/usr/bin/env python
def passTrig(val, idx):
    return (abs(val)>>idx)%2
import ROOT
from DataFormats.FWLite import Events, Handle

#events=Events('recoBPHanalysis_withFilter_Data.root')
#events=Events('reco_fourTracksVertexing.root')
events=Events('reco_final.root')
#events=Events('ta.root')
handle=Handle('std::vector<int>')
#label=('selectedMuons:MuonPreselectionBoolInt:myVertexingProcedure')
#label=('selectedTracks:TrackPreselectionBoolInt:myVertexingProcedure')
#label=("mumuVertexingProducer:mumuVertexingEfficiencyBoolInt:myVertexingProcedure")
#label=("tktkVertexingProducer:tktkVertexingEfficiencyBoolInt:myVertexingProcedure")
label=("fourTracksFromVCCProducer:fourTkVertexingEfficiencyBoolIntpL0B:myFourTracksVertexingProcedure")

# don't pop up canvases
ROOT.gROOT.SetBatch()

# set white background
ROOT.gROOT.SetStyle('Plain')

h_eff=ROOT.TH1I('eff','efficiency', 40, 0, 20)

for event in events:
    event.getByLabel(label,handle)
    encodedEffvalueList=handle.product()
    for effValue in encodedEffvalueList:
        if effValue < 0:
            pass
            # storeInAnotherHisto
        for idx in range(1,15):
            if passTrig(effValue,idx):
                h_eff.Fill(idx)
        if passTrig(effValue,0):
            h_eff.Fill(16)

#h_eff.SetMaximum(20000)
c1=ROOT.TCanvas()
h_eff.Draw()
c1.SaveAs('eff.png')

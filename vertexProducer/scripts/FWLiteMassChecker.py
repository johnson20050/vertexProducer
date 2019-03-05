#!/usr/bin/env python
def passTrig(val, idx):
    return (abs(val)>>idx)%2
import ROOT
from DataFormats.FWLite import Events, Handle

#events=Events('reco_fourTracksVertexing.root')
#events=Events('reco_final.root')
events=Events('reco_fourTracksVertexing_9.root')
#events=Events('reco_test.root')
#events=Events('ta.root')
handle=Handle('vector<reco::VertexCompositeCandidate>')
label=('fourTracksFromVCCProducer:pL0B:myVertexingProcedure')

# don't pop up canvases
ROOT.gROOT.SetBatch()

# set white background
ROOT.gROOT.SetStyle('Plain')

hMass=ROOT.TH1D('hMass','massDist',40,5.4,5.8)

for event in events:
    event.getByLabel(label,handle)
    candList=handle.product()
    encodedEffvalueList=handle.product()
    for cand in candList:
        hMass.Fill(cand.mass())
    #for effValue in encodedEffvalueList:
    #    if effValue < 0:
    #        pass
    #        # storeInAnotherHisto
    #    for idx in range(1,15):
    #        if passTrig(effValue,idx):
    #            h_eff.Fill(idx)
    #    if passTrig(effValue,0):
    #        h_eff.Fill(16)

#h_eff.SetMaximum(20000)
c1=ROOT.TCanvas()
hMass.Draw()
c1.SaveAs('mass.png')

#!/usr/bin/env python
def passTrig(val, idx):
    return (abs(val)>>idx)%2
import ROOT
from ROOT import TLorentzVector
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
        mu1=cand.daughter(0).daughter(0)
        mu2=cand.daughter(0).daughter(1)
        tk1=cand.daughter(1).daughter(0)
        tk2=cand.daughter(1).daughter(1)
        tlmu1=TLorentzVector(mu1.px(),mu1.py(),mu1.pz(),mu1.energy())
        tlmu2=TLorentzVector(mu2.px(),mu2.py(),mu2.pz(),mu2.energy())
        tltk1=TLorentzVector(tk1.px(),tk1.py(),tk1.pz(),tk1.energy())
        tltk2=TLorentzVector(tk2.px(),tk2.py(),tk2.pz(),tk2.energy())
        tltk1.SetE((tltk1.P()**2+0.493677**2)**0.5)
        tltk2.SetE((tltk2.P()**2+0.9382720813**2)**0.5)
        reassignMass=(tlmu1+tlmu2+tltk1+tltk2).Mag()
        distFromCent1=abs(cand.mass()-5.619)
        distFromCent2=abs(reassignMass-5.619)

        if distFromCent1<0.015 and distFromCent2<0.015:
            pass
        #elif distFromCent1<distFromCent2:
        #    hMass.Fill(cand.mass())
        #elif distFromCent1>distFromCent2:
        #    hMass.Fill(reassignMass)
        elif tk1.pt() > tk2.pt() and distFromCent1<distFromCent2:
            hMass.Fill(cand.mass())
        elif tk1.pt() < tk2.pt() and distFromCent1>distFromCent2:
            hMass.Fill(reassignMass)


#h_eff.SetMaximum(20000)
c1=ROOT.TCanvas()
hMass.Draw()
#c1.SaveAs('mass_assign_test.png')
c1.SaveAs('mass_assign.png')

import FWCore.ParameterSet.Config as cms
readFiles = cms.untracked.vstring()
readFiles.extend( [
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1088.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1089.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_109.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1090.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1091.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1092.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1093.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1094.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1095.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1096.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1097.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1098.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1099.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_11.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_110.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1100.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1101.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1102.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1103.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1104.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1105.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1106.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1107.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1108.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1109.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_111.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1110.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1111.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1112.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1113.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1114.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1115.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1116.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1117.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1118.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1119.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_112.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1120.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1121.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1122.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1123.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1124.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1125.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1126.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1127.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1128.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1129.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_113.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1130.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1131.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1132.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1133.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1134.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1135.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1136.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1137.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1138.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1139.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_114.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1140.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1141.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1142.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1143.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1144.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1145.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1146.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1147.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1148.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1149.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_115.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1150.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1151.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1152.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1153.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1154.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1155.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1156.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1157.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1158.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1159.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_116.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1160.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1161.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1162.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1163.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1164.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1165.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1166.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1167.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1168.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1169.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_117.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1170.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1171.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1172.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1173.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1174.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1175.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1176.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_19426/noPreselection_vertexedpL0B/reco_myTESTfourTracksVertexingMC_1177.root"
] )
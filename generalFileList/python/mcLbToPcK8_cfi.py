import FWCore.ParameterSet.Config as cms
readFiles = cms.untracked.vstring()
readFiles.extend( [
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_8.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_80.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_800.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_801.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_802.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_803.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_804.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_805.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_806.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_807.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_808.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_809.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_81.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_810.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_811.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_812.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_813.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_814.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_815.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_816.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_817.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_818.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_819.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_82.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_820.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_821.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_822.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_823.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_824.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_825.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_826.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_827.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_828.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_829.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_83.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_830.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_831.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_832.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_833.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_834.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_835.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_836.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_837.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_838.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_839.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_84.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_840.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_841.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_842.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_843.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_844.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_845.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_846.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_847.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_848.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_849.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_85.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_850.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_851.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_852.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_853.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_854.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_855.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_856.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_857.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_858.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_859.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_86.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_860.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_861.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_862.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_863.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_864.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_865.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_866.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_867.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_868.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_869.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_87.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_870.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_871.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_872.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_873.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_874.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_875.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_876.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_877.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_878.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_879.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_88.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_880.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_881.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_882.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_883.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_884.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_885.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_886.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_887.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_888.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_889.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_89.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_890.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_891.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_892.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_893.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_894.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_895.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_896.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_897.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_898.root",
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3_899.root",
] )
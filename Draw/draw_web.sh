
plots_folder="plots_vRun3_04/Muon/"
web_path="/eos/user/w/wjun/www/Run2023_MuonHLT/240108_MuonHLT_forDPv3/"
rm -rf $web_path
mkdir $web_path

#root -l -b -q 'drawtnpCompEffL3wrtOff.C("hltPixelTracks")'
root -l -b -q 'drawtnpCompEffL3wrtOff_ratio.C("L1sSingleMu22")'
root -l -b -q 'drawtnpCompEffL3wrtOff_ratio.C("IsoMu24")'
root -l -b -q 'drawtnpCompEffL3wrtOff_ratio.C("Mu50")'
root -l -b -q 'drawtnpCompEffL3wrtOff_ratio.C("Mu50OrOldMu100OrTkMu100")'

root -l -b -q 'drawtnpCompEffL3wrtL1_ratio.C("L2Muon")'
root -l -b -q 'drawtnpCompEffL3wrtL1_ratio.C("hltOI")'
root -l -b -q 'drawtnpCompEffL3wrtL1_ratio.C("hltIter0FromL1")'
root -l -b -q 'drawtnpCompEffL3wrtL1_ratio.C("hltL3FromL2Merged")'
root -l -b -q 'drawtnpCompEffL3wrtL1_ratio.C("hltL3Merged")'
root -l -b -q 'drawtnpCompEffL3wrtL1_ratio.C("hltIterL3MuonNoIDTrack")'
root -l -b -q 'drawtnpCompEffL3wrtL1_ratio.C("hltIterL3Muon")'
root -l -b -q 'drawtnpCompEffL3wrtL1_ratio.C("IsoMu24")'
root -l -b -q 'drawtnpCompEffL3wrtL1_ratio.C("Mu50")'
#root -l -b -q 'drawtnpCompEffL3wrtL1_ratio.C("Mu50OrOldMu100OrTkMu100")'


:<<END
#plots_folder="plots_trackQual/"
plots_folder="plots_vRun3_03_DR/"
web_path="/eos/user/w/wjun/www/Run2023_MuonHLT/231116_MuonHLT_DR1/"
rm -rf $web_path
mkdir $web_path
#root -l -b -q 'drawTrackQuals.C'
root -l -b -q drawtnpCompEffL3wrtL1_DR.C
END
for dir in `find $plots_folder -type d` ; do cp /afs/cern.ch/user/f/fernanpe/public/for_Won/index.php $dir/ ; done

cp -r $plots_folder/* $web_path

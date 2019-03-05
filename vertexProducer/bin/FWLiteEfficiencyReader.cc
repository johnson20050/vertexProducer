#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <math.h>

#include <TH1D.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>
#include <TCanvas.h>

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

// create histograms from CMSSW based data.
// use FWLIte to load data.
// this file just write down:
// 1. the argument you should or can input.
// 2. apply general cuts known.
// 3. loop the event.
//
// And the content in the loop is written in histProduce/histProduce/src/hMain????.cc
// every hMain??? decides what to do in each event.
// Normally, create a histogram and fill it. Save it into root file to be ploted.
//
// usage:
//     hCreate filelist=fileList

// initialize static member
//std::vector<generalCutList*>* histMain::_cutLists = NULL;
//std::map<std::string, TH1D*> histMain::hMap;

int main(int argc, char* argv[])
{
    // ----------------------------------------------------------------------
    // First Part:
    //
    //  * enable the AutoLibraryLoader
    //  * book the histograms of interest
    //  * open the input files
    // ----------------------------------------------------------------------

    // load framework libraries
    gSystem->Load( "libFWCoreFWLite" );
    AutoLibraryLoader::enable();

    // check default path {{{
    std::string sysPath = std::string( getenv("CMSSW_BASE") );
    try
    {
        if ( sysPath == "" )
            throw "----- error, you need to setup CMSSW envirnment! -----";
    }
    catch ( const char* e )
    { printf( "%s", e ); }
    // check default path end }}}
//
//    // parser setting {{{
//    // initialize command line parser
//    optutl::CommandLineParser parser ("Analyze FWLite Histograms");
//
//    parser.addOption("testFile",optutl::CommandLineParser::kString,"test input file, recommend to use fileList_cfi.py to put files","");
//    parser.addOption("fileList",optutl::CommandLineParser::kString,"input the python file records file list.","fileList");
//    // set defaults for testFile is assigned
//    parser.stringValue  ("outputFile" ) = "efficiencyResult.root";
//
//    // parse arguments
//    parser.parseArguments (argc, argv);
//
//    // deal with python file path
//    const std::string configFile = parser.stringValue("configFile");
//    const std::string listFile   = parser.stringValue("fileList");
//    const std::string dPath      = "/src/vertexProducer/vertexProducer/python/";
//    const std::string confPython = sysPath+dPath+configFile+"_cfi.py";
//    const std::string fOptPython = sysPath+dPath+listFile+"_cfi.py";
//    // load IO option from python file. default is fileList_cfi.py
//    const edm::ParameterSet& ioOption = edm::readPSetsFrom(fOptPython.c_str())->getParameter<edm::ParameterSet>("process");
//    const edm::ParameterSet& runSetting = ioOption.getParameter<edm::ParameterSet>("runSetting");
//    const edm::ParameterSet& inputFileNames = ioOption.getParameter<edm::ParameterSet>("inputFiles");
//    const std::vector<std::string>& fileNames = inputFileNames.getParameter< std::vector<std::string> >("fileNames");
//    unsigned outputEvery_   = runSetting.getParameter<unsigned>("outEvery");
//    std::string outputFile_ = runSetting.getParameter<std::string>("outName");
//
//    // load cut configuration from python file. default is histogramCutParameter_cfi.py
//    //const edm::ParameterSet& cutOption = edm::readPSetsFrom(confPython.c_str())->getParameter<edm::ParameterSet>("process");
//    //const std::vector<edm::ParameterSet>& runSetting_ = cutOption.getParameter< std::vector<edm::ParameterSet> >("cutSet");
//    // parser setting end }}}
//
//    // input files
//    std::vector<std::string> inputFiles_;
//
//    // if inputFile not set, use python file : fileList_cfi.py
//    if ( parser.stringValue("testFile").empty() )
//        inputFiles_ = fileNames;
//    else
//    {
//        // only if testFile is set, parameters from parser valid
//        inputFiles_.push_back( parser.stringValue("testFile") );
//        outputFile_ = parser.stringValue("outputFile");
//        maxEvents_ = parser.integerValue("maxEvents");
//        outputEvery_ = parser.integerValue("outputEvery");
//    }

    // book a set of histograms
    TCanvas* c1 = new TCanvas("c1", "", 1600,1600);
    TH1D* h = new TH1D("eff", "", 20, 0.,10.);



    std::vector<std::string> inputFiles_;
    inputFiles_.push_back("reco_fourTracksVertexing_9.root");

    int ievt=0;
    for ( const auto& file : inputFiles_ )
    {
        TFile* inFile = TFile::Open( ("file://"+file).c_str() );
        if( !inFile )
        { printf("file : %s not found!\n", file.c_str()); continue; }

        fwlite::Event ev(inFile);
        for(ev.toBegin(); !ev.atEnd(); ++ev, ++ievt)
        {
            fwlite::Handle<int> handle;
            handle.getByLabel( ev, "fourTracksFromVCCProducer", "fourTracksTotallyVertexingEfficiency", "myVertexingProcedure" );
            int vtxEffVal = *(handle.product());
            for(int i=0;i<9;++i)
                if ( (vtxEffVal>>i)%2 )
                    h->Fill(i);
        }
        inFile->Close();
        // break loop if maximal number of events is reached:
        // this has to be done twice to stop the file loop as well
    }
    h->Draw();
    c1->SaveAs("h_vtxEff.root");

    return 0;
}

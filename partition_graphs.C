#include <TChain.h>
#include <TChainElement.h>
#include <TFile.h>
#include <TKey.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TStopwatch.h>
#include <TString.h>
#include <TAxis.h>

#include <iostream>
#include <fstream>

std::vector<TString> parse_config(TString filename){

    std::string line;

    std::ifstream reader(filename);

    if (!reader.is_open()){
        std::cout << "Config file " << filename << " can't be opened" << std::endl;
    }   

    std::vector<TString> files;
    while (std::getline(reader, line)){
        if (line.length() == 0) continue;
        char *cstr = new char[line.length() + 1]; 
        strcpy(cstr, line.c_str());
        char *p = strtok(cstr, ",");
        std::vector<TString> tokenized;
        while (p) {
            tokenized.push_back(p);
            p = strtok(NULL, ",");
        }   

        files.push_back(tokenized[0]);
    }   
    reader.close();

    return files;  
}

// start copied stuff from SiStripDetid
enum SubDetector { UNKNOWN=0, TIB=3, TID=4, TOB=5, TEC=6 };
//end copied stuff from SiStripDetid

// start copied stuff from Detid
static const int kDetOffset          = 28;
static const int kSubdetOffset       = 25;

int subdetId(unsigned int id_)  { return ((id_>>kSubdetOffset)&0x7); }
// end copied stuff from Detid


// start: copied stuff from TIDDetid
static const unsigned int TIDringStartBit_=         9;
static const unsigned int TIDringMask_=          0x3;
unsigned int TIDring(unsigned int id_) 
    { return ((id_>>TIDringStartBit_) & TIDringMask_) ;}
// end: copied stuff from TIDDetid

// start: copied stuff from TIBDetid - identical to TOB layer bit and mask
static const unsigned int layerStartBit_= 14;
static const unsigned int layerMask_    = 0x7;

unsigned int layer(unsigned int id_) {
    return int((id_>>layerStartBit_) & layerMask_);
}
// end: copied stuff from TIBDetid

// start: copied stuff from TECDetid
static const unsigned int TECringStartBit_ = 5;
static const unsigned int TECringMask_ = 0x7;

unsigned int TECring(unsigned int id_) { return ((id_>>TECringStartBit_) & TECringMask_) ;}
// end: copied stuff from TECDetid


void make_graphs(std::vector<TString> filenames)
{
    Ssiz_t ref_namestart = filenames[0].Last('/')+1;
    TString ref_filename( filenames[0](ref_namestart,filenames[0].Length()));

    // extract the run number from the filename 
    Int_t position = 17; // the position after the second "GAINSCAN_" in the default filename produced by tkCommissioner
    Ssiz_t first = ref_filename.Index('_',position);
    Ssiz_t last = ref_filename.Last('_');
    Ssiz_t period = ref_filename.Last('.'); // the ref number is the one right before ".root" in the default filename 
    TString ref_number_string( ref_filename(first+1,last-first-1) );  
    unsigned int ref_number = std::stoi(ref_number_string.Data());
    
    TStopwatch watch;
    watch.Start();

    TChain *chain = new TChain;

    std::vector<TString>::iterator filenames_it = filenames.begin();

    for (filenames_it; filenames_it != filenames.end(); filenames_it++) {
        chain->Add(*filenames_it); }
        

    TObjArray *fileElements=chain->GetListOfFiles();
    TIter next(fileElements);
    TChainElement *chEl=0;

    // outer map: key = std::pair< detid, i2c address > -> allows us to later iterate over devices
    //
    // inner map: runnumber -> value; as std::map sorts on insertion,
    // we can even loop over the trees in any order and will get the
    // correct order when later iterating through this map
    //
    std::map< std::pair<unsigned int, unsigned int>, std::map<unsigned int, float> > valueMap;

    while (( chEl=(TChainElement*)next() )) {
        TFile *f = TFile::Open(chEl->GetTitle());

        if( ! f ) {
            std::cerr << "File not found" << std::endl;
            continue;
        }

        TKey *key = static_cast<TKey*>( f->GetListOfKeys()->At(0) ); // assumes that there is only a single object in the file

        if( ! key ) {
            std::cerr << "No key found" << std::endl;
            continue;
        }

        TTree *tree = static_cast<TTree*>(key->ReadObj());

        if( ! tree ) {
            std::cerr << "No tree found" << std::endl;
            continue;
        }

        double Detid;
        double I2CAddress;
        double Threshold0;

        tree->SetBranchAddress("Detid",&Detid);
        tree->SetBranchAddress("I2CAddress",&I2CAddress);
        tree->SetBranchAddress("Threshold0",&Threshold0);

        TString treeName = tree->GetName();

        // get the run number from the tree name. This assumes that the
        // run number is the last field in the tree name and that the
        // elements are separated by '_'
        TString run = treeName(treeName.Last('_')+1,treeName.Length()-(treeName.Last('_')+1));

        std::cout << "Processing run " << run << std::endl;

        unsigned int iRun = run.Atoi();
        Int_t badcounter = 0;
        for( Long64_t entry = 0; entry < tree->GetEntries(); ++entry ) {
            tree->GetEntry(entry);

            // skip invalid devices
            if( fabs( Threshold0 - 0xFFFF ) < 1e-6 ) {
                badcounter++;
                continue; }
   //         if (entry % 1000 == 0) {
                //std::cout << std::setprecision(10);
                //std::cout << "DetID is " << Detid << " and I2C is " << I2CAddress << std::endl; }
            valueMap[ std::make_pair( Detid, I2CAddress ) ][iRun] = Threshold0;

        }
        std::cout << "Tree " << treeName << " contained " << badcounter << " skipped entries out of " << tree->GetEntries() << " total." << std::endl;

    }

    std::map< std::pair<unsigned int, unsigned int>, std::map<unsigned int, float> >::iterator it = valueMap.begin();

    std::vector<std::pair< std::vector<double>, std::vector<double > > > vValues;
    std::map<std::pair<unsigned int, unsigned int>, TGraph*> vGraphs;

    unsigned int count = 0;

    for( ; it != valueMap.end(); ++it, ++count ) {

        if( count % 1000 == 0 ) {

            std::cout << "Processing entry " << count << " of " << valueMap.size() << std::endl;
        }


        std::map<unsigned int, float> &innerMap = it->second;
        if( innerMap.find( ref_number ) == innerMap.end() ) {
            //std::cerr << "No value found for detid " << it->first.first << " I2C Address " << it->first.second << std::endl;
            //std::cerr << "Found a value for detid " << it->first.first << " I2C Address " << it->first.second << std::endl;
            continue;
        }
        double refValue = innerMap[ref_number];
        if( refValue < 1e-6 ) {
            std::cerr << "Reference value is very small ( " << refValue << " ) " << std::endl;
            continue;
        }

        std::vector<double> x;
        std::vector<double> y;    
        for( std::map<unsigned int, float>::iterator innerIt = innerMap.begin(); innerIt != innerMap.end(); ++innerIt ) {
            x.push_back(innerIt->first);
            y.push_back( (innerIt->second-refValue)/refValue );
        }
        vValues.push_back( std::make_pair( x, y ) );
        vGraphs[ std::make_pair( it->first.first, it->first.second ) ] =  new TGraph( x.size(), &x[0], &y[0] ) ;
        vGraphs[ std::make_pair( it->first.first, it->first.second ) ]->SetName(TString("g_")+TString::Itoa(it->first.first,10)+TString("_")+TString::Itoa(it->first.second,10));

    }

    TCanvas *canvas = new TCanvas("canvas","Canvas",1500,500);

    // make sure we have the right x and y range without relying on the
    // TGraph content
    canvas->DrawFrame(230000,-0.9,280000,0.9);

    TCanvas *canvas_layers = new TCanvas("canvas_layers","Canvas Layers",1500,1000);

    canvas_layers->Divide(2,2);

    TCanvas *canvas_rings = new TCanvas("canvas_rings","Canvas Rings",1500,500);

    canvas_rings->Divide(3,1);

    TCanvas *tob_layers = new TCanvas("tob_layers","TOB Layers",1500,1000);

    tob_layers->Divide(3,2);
    
    TCanvas *tec_rings = new TCanvas("tec_rings","TEC Rings",2000,2000);

    tec_rings->Divide(3,3);

    for( Int_t i = 1; i <= 4; ++i ) {
        TVirtualPad *pad = canvas_layers->cd(i);
        pad->DrawFrame(230000,-0.9,280000,0.9,TString("TIB Layer ")+TString::Itoa(i,10) );
    }
    for( Int_t i = 1; i <= 3; ++i ) {
        TVirtualPad *pad = canvas_rings->cd(i);
        pad->DrawFrame(230000,-0.9,280000,0.9,TString("TID Ring ")+TString::Itoa(i,10) );
    }
    for( Int_t i = 1; i <= 6; ++i ) {
        TVirtualPad *pad = tob_layers->cd(i);
        pad->DrawFrame(230000,-0.9,280000,0.9,TString("TOB Layer ")+TString::Itoa(i,10) );
    }
    for( Int_t i = 1; i <= 7; ++i ) {
        TVirtualPad *pad = tec_rings->cd(i);
        pad->DrawFrame(230000,-0.9,280000,0.9,TString("TEC Ring ")+TString::Itoa(i,10) );
    }
    
    canvas->cd();
    std::map<std::pair<unsigned int, unsigned int>, TGraph*>::iterator graphIt = vGraphs.begin();
    for( ; graphIt != vGraphs.end(); ++graphIt ) { 
        graphIt->second->Draw( "LPsame");
    }

    // reset iterator
    graphIt = vGraphs.begin();
    for( ; graphIt != vGraphs.end(); ++graphIt ) { 

        unsigned int detid = graphIt->first.first;
        if( subdetId( detid ) == TIB ) {

            unsigned int iLayer = layer( detid );

            if( iLayer < 1 || iLayer > 4 ) continue;

            canvas_layers->cd( iLayer );
            graphIt->second->Draw( "LPsame");
        }
        if( subdetId( detid ) == TID ) {

            unsigned int iRing = TIDring( detid );

            if( iRing < 1 || iRing > 3 ) continue;

            canvas_rings->cd( iRing );
            graphIt->second->Draw( "LPsame");
        }

        if( subdetId( detid ) == TOB ) {

            unsigned int iLayer = layer( detid );

            if( iLayer < 1 || iLayer > 6 ) continue;

            tob_layers->cd( iLayer );
            graphIt->second->GetXaxis()->SetTitle("Run number");
            graphIt->second->GetYaxis()->SetTitle("% Diff from Ref");
            graphIt->second->Draw( "LPsame");
        }
    
        if( subdetId( detid ) == TEC ) {

            unsigned int iRing = TECring( detid );

            if( iRing < 1 || iRing > 7 ) continue;

            tec_rings->cd( iRing );
            graphIt->second->Draw( "LPsame");
        }
    }

    canvas->Print("pngs/overview.png");
    canvas_layers->Print("pngs/layer_plots.png");
    canvas_rings->Print("pngs/ring_plots.png");
    tob_layers->Print("pngs/tob_layer_plots.png");
    tec_rings->Print("pngs/tec_ring_plots.png");

    std::cout << "Real time : " << watch.RealTime() << std::endl;
    std::cout << "Cpu time  : " << watch.CpuTime() << std::endl;

}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        std::cerr << "Usage: ./partition_graphs <config_path>" << std::endl;
        exit(0);
    }
    
    std::vector<TString> filenames = parse_config(argv[1]);
    make_graphs(filenames);
}
    

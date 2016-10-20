#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TString.h"

vector<TString> parse_config(TString filename){

   string line;

   std::ifstream reader(filename);

   if (!reader.is_open()){
      cout << "Config file " << filename << " can't be opened" << endl;
   }
  
   vector<TString> files;
   while (std::getline(reader, line)){
      if (line.length() == 0) continue;
      char *cstr = new char[line.length() + 1]; 
      strcpy(cstr, line.c_str());
      char *p = strtok(cstr, ",");
      vector<TString> tokenized;
      while (p) {
        tokenized.push_back(p);
        p = strtok(NULL, ",");
      }

      files.push_back(tokenized[0]);
   } 
   reader.close();

   return files;  
}



void process_single_file(const TString run_filepath, const TString ref_filepath, const TString output_filepath)
{

   // extract the filename from the filepath 
   Ssiz_t run_namestart = run_filepath.Last('/')+1;
   Ssiz_t ref_namestart = ref_filepath.Last('/')+1;
   TString run_filename( run_filepath(run_namestart,run_filepath.Length()));
   TString ref_filename( ref_filepath(ref_namestart,ref_filepath.Length()));
   
   // extract the run number from the filename 
   Int_t position = 17; // the position after the second "GAINSCAN_" in the default filename produced by tkCommissioner
   Ssiz_t first = run_filename.Index('_',position);
   Ssiz_t last = run_filename.Last('_');
   Ssiz_t period = run_filename.Last('.'); // the ref number is the one right before ".root" in the default filename 
   TString run_number( run_filename(first+1, last-first-1) );
   TString ref_number( ref_filename(first+1,last-first-1) ); 

   // put the run number into an int to use on the TGraph
   std::stringstream run_stream(run_number.Data());
   Long64_t run_number_int;
   run_stream >> run_number_int;

   // get filename and tree name for run data 
   TString run_treename("Tree_04_" + run_number);
   TString ref_treename("Tree_04_" + ref_number);
  
   TFile *run = new TFile(run_filepath);
   TTree *run_tree = (TTree*)run->Get(run_treename);
  
   // set up run variables
   Double_t detid;
   Double_t i2caddress;
   Double_t run_threshold;
   Double_t run_isvalid;  
 
   // book the run branches 
   run_tree->SetBranchAddress("Detid",&detid);
   run_tree->SetBranchAddress("I2CAddress",&i2caddress);
   run_tree->SetBranchAddress("Threshold0",&run_threshold);
   run_tree->SetBranchAddress("Isvalid",&run_isvalid);

   // get the reference tree and build an index for it
   TFile *ref = new TFile(ref_filepath);
   TTree *ref_tree = (TTree*)ref->Get(ref_treename);
   ref_tree->BuildIndex("Detid","I2CAddress");

   // set up ref variables
   Double_t ref_threshold;
   Double_t ref_isvalid;
   
   // book the ref branches 
   ref_tree->SetBranchAddress("Threshold0",&ref_threshold);
   ref_tree->SetBranchAddress("Isvalid",&ref_isvalid);

   //read all run entries and fill the histograms
   Long64_t numentries = run_tree->GetEntries();
   Long64_t refentries = ref_tree->GetEntries();
   Long64_t bad_entry_counter = 0;
   for (Long64_t i=0;i<numentries;i++) {
      run_tree->GetEntry(i);
   
      if (!run_isvalid) continue;
     
      Int_t ref_outcome = ref_tree->GetEntryWithIndex(detid,i2caddress);
      if (ref_outcome==-1) continue;

      if (!ref_isvalid) continue;
      
      if (i % 1000 == 0) {
        cout << i << " rows completed" << endl;}
       
      Double_t percentage_threshold = 100*((run_threshold - ref_threshold) / ref_threshold);
     
      std::ostringstream detstream;
      detstream << std::setprecision(10);
      detstream << detid;
      TString det_string = detstream.str();
      
      std::ostringstream i2cstream;
      i2cstream << i2caddress;
      TString i2c_string = i2cstream.str();

      TString key = det_string + "_" + i2c_string;

      TFile *output = new TFile(output_filepath,"UPDATE");
      output->cd();
      TGraph *graph = (TGraph*)output->Get(key);
      if (graph) {
        graph->SetPoint(graph->GetN(),run_number_int,percentage_threshold);
        graph->Write("",TObject::kOverwrite);}
      else {
        TGraph *new_graph = new TGraph();
        new_graph->SetPoint(0,run_number_int,percentage_threshold);
        new_graph->SetName(key);
        new_graph->Write("",TObject::kOverwrite);}
      output->Close();
        
   }
     
}   

 void make_tgraphs(const TString config, const TString output_filepath)
{
    filenames = parse_config(config);
    for (Int_t i=1; i<filenames.size(); i++)
    {
        cout << "Processing " << filenames[i] << endl;
        process_single_file(filenames[i], filenames[0], output_filepath);
    }
}   

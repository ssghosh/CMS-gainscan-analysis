#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TString.h"

void make_histograms(const TString run_filepath, const TString ref_filepath, const TString output_filename, const TDatime da, const Int_t offset, const Int_t end, const Int_t numtimebins, const TString yaxisparamfile)
{
   vector<vector<Int_t>> yaxis_params;
   std::string line;

   std::ifstream yaxis_reader(yaxisparamfile);

   if (!yaxis_reader.is_open()){
      cout << "y-axis parameter file can't be opened" << endl;
   }   

   while (std::getline(yaxis_reader, line)){
      if (line.length() == 0) continue;
      // tokenize the line, store in a list, and push the list onto the params vector
      char *c_line = new char[line.length()+1];
      strcpy(c_line, line.c_str());
      char *current;
      current = strtok(c_line," ");
      yaxis_params.push_back(vector<Int_t>());
      while (current != NULL) {
         yaxis_params.back().push_back(std::stoi(current));
         current = strtok(NULL," ");
      }   
   }   
   yaxis_reader.close();
   Int_t this_run = da.Convert()-offset;
   //cout << "Run number is " << this_run << endl; 
 
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

   // get filename and tree name for run data 
   TString run_treename("Tree_04_" + run_number);
   TString ref_treename("Tree_04_" + ref_number);
  
   TFile *run = new TFile(run_filepath);
   TTree *run_tree = (TTree*)run->Get(run_treename);
  
   // set up run variables
   Double_t detid;
   Double_t i2caddress;
   Double_t run_measgain;
   Double_t run_bias;
   Double_t run_liftoff;
   Double_t run_baselineslop;
   Double_t run_threshold;
   Double_t run_tickheight;
   Double_t run_linknoise;
   Double_t run_zerolight;
   Double_t run_isvalid;  
 
   // book the run branches 
   run_tree->SetBranchAddress("Detid",&detid);
   run_tree->SetBranchAddress("I2CAddress",&i2caddress);
   run_tree->SetBranchAddress("Measgain0",&run_measgain);
   run_tree->SetBranchAddress("Bias0",&run_bias);
   run_tree->SetBranchAddress("Liftoff0",&run_liftoff);
   run_tree->SetBranchAddress("Threshold0",&run_threshold);
   run_tree->SetBranchAddress("Baselineslop0",&run_baselineslop);
   run_tree->SetBranchAddress("Tickheight0",&run_tickheight);
   run_tree->SetBranchAddress("Linknoise0",&run_linknoise);
   run_tree->SetBranchAddress("Zerolight0",&run_zerolight);
   run_tree->SetBranchAddress("Isvalid",&run_isvalid);

   // get the reference tree and build an index for it
  
   TFile *ref = new TFile(ref_filepath);
   TTree *ref_tree = (TTree*)ref->Get(ref_treename);
   ref_tree->BuildIndex("Detid","I2CAddress");

   // set up ref variables
   Double_t ref_measgain;
   Double_t ref_bias;
   Double_t ref_liftoff;
   Double_t ref_baselineslop;
   Double_t ref_threshold;
   Double_t ref_tickheight;
   Double_t ref_linknoise;
   Double_t ref_zerolight;
   Double_t ref_isvalid;
   
   // book the ref branches 
   ref_tree->SetBranchAddress("Measgain0",&ref_measgain);
   ref_tree->SetBranchAddress("Bias0",&ref_bias);
   ref_tree->SetBranchAddress("Liftoff0",&ref_liftoff);
   ref_tree->SetBranchAddress("Threshold0",&ref_threshold);
   ref_tree->SetBranchAddress("Baselineslop0",&ref_baselineslop);
   ref_tree->SetBranchAddress("Tickheight0",&ref_tickheight);
   ref_tree->SetBranchAddress("Linknoise0",&ref_linknoise);
   ref_tree->SetBranchAddress("Zerolight0",&ref_zerolight);
   ref_tree->SetBranchAddress("Isvalid",&ref_isvalid);

   // create the histograms
   TH2D *th_diffmeasgain = new TH2D("th_diffmeasgain",
      "DiffMeasgain0/Ref;Run date;Difference (%)",
      numtimebins,0,end,yaxis_params[0][0], yaxis_params[0][1], yaxis_params[0][2]);
   TH2D *th_diffbias = new TH2D("th_diffbias",
      "DiffBias0/Ref;Run date;Difference (%)",
      numtimebins,0,end,yaxis_params[1][0], yaxis_params[1][1], yaxis_params[1][2]);
   TH2D *th_diffliftoff = new TH2D("th_diffliftoff",
      "DiffLiftoff0/Ref;Run date;Difference (%)",
      numtimebins,0,end,yaxis_params[2][0], yaxis_params[2][1], yaxis_params[2][2]);
   TH2D *th_diffthreshold = new TH2D("th_diffthreshold",
      "DiffThreshold0/Ref;Run date;Difference (%)",
      numtimebins,0,end,yaxis_params[3][0], yaxis_params[3][1], yaxis_params[3][2]);
   TH2D *th_diffbaselineslop = new TH2D("th_diffbaselineslop",
      "DiffBaselineslope0/Ref;Run date;Difference (%)",
      numtimebins,0,end,yaxis_params[4][0], yaxis_params[4][1], yaxis_params[4][2]);
   TH2D *th_difftickheight = new TH2D("th_difftickheight",
      "DiffTickheight0/Ref;Run date;Difference (%)",
      numtimebins,0,end,yaxis_params[5][0], yaxis_params[5][1], yaxis_params[5][2]);
   TH2D *th_difflinknoise = new TH2D("th_difflinknoise",
      "DiffLinknoise0/Ref;Run date;Difference (%)",
      numtimebins,0,end,yaxis_params[6][0], yaxis_params[6][1], yaxis_params[6][2]);
   TH2D *th_diffzerolight = new TH2D("th_diffzerolight",
      "DiffZerolight0/Ref;Run date;Difference (%)",
      numtimebins,0,end,yaxis_params[7][0], yaxis_params[7][1], yaxis_params[7][2]);


   //read all run entries and fill the histograms
   Long64_t numentries = run_tree->GetEntries();
   Long64_t refentries = ref_tree->GetEntries();
   Long64_t bad_entry_counter = 0;
   for (Long64_t i=0;i<numentries;i++) {
      run_tree->GetEntry(i);
   
      if (!run_isvalid) {
         bad_entry_counter++;
         continue;
      }
     
      if (ref_tree->GetEntryWithIndex(detid,i2caddress)==-1) {
         bad_entry_counter++;
         continue;
      }  

      if (!ref_isvalid) {
         bad_entry_counter++;
         continue;
      }
       
      Double_t percentage_measgain = 100*((run_measgain - ref_measgain) / ref_measgain);
      Double_t percentage_bias = 100*((run_bias - ref_bias) / ref_bias);
      Double_t percentage_liftoff = 100*((run_liftoff - ref_liftoff) / ref_liftoff);
      Double_t percentage_threshold = 100*((run_threshold - ref_threshold) / ref_threshold);
      Double_t percentage_baselineslop = 100*((run_baselineslop - ref_baselineslop) / ref_baselineslop);
      Double_t percentage_tickheight = 100*((run_tickheight - ref_tickheight) / ref_tickheight);
      Double_t percentage_linknoise = 100*((run_linknoise - ref_linknoise) / ref_linknoise);
      Double_t percentage_zerolight = 100*((run_zerolight - ref_zerolight) / ref_zerolight);
      
      th_diffmeasgain->Fill(this_run,percentage_measgain);
      //if (i % 1000 == 0) cout << "measgain percentage is " << percentage_measgain << endl;
      th_diffbias->Fill(this_run,percentage_bias);
      th_diffliftoff->Fill(this_run,percentage_liftoff);
      th_diffthreshold->Fill(this_run,percentage_threshold);
      th_diffbaselineslop->Fill(this_run,percentage_baselineslop);
      th_difftickheight->Fill(this_run,percentage_tickheight);
      th_difflinknoise->Fill(this_run,percentage_linknoise);
      th_diffzerolight->Fill(this_run,percentage_zerolight);
   }
   if (bad_entry_counter > 0) {
      cout << run_filename << " contained " << to_string(bad_entry_counter) << " excluded entries out of " << to_string(numentries) << " total entries" << endl;
   }
   
   // scale all of the hists
   //cout << "prescaling diffmeasgain integral is " << to_string(th_diffmeasgain->Integral()) << endl;
   //cout << "prescaling diffbias integral is " << to_string(th_diffbias->Integral()) << endl;
   //cout << "prescaling diffliftoff integral is " << to_string(th_diffliftoff->Integral()) << endl;
   //cout << "prescaling diffthreshold integral is " << to_string(th_diffthreshold->Integral()) << endl;
   //cout << "prescaling diffbaselineslop integral is " << to_string(th_diffbaselineslop->Integral()) << endl;
   //cout << "prescaling difftickheight integral is " << to_string(th_difftickheight->Integral()) << endl;
   //cout << "prescaling difflinknoise integral is " << to_string(th_difflinknoise->Integral()) << endl;
   //cout << "prescaling diffzerolight integral is " << to_string(th_diffzerolight->Integral()) << endl;
   
   th_diffmeasgain->Scale(refentries / th_diffmeasgain->Integral());
   th_difflinknoise->Scale(refentries / th_difflinknoise->Integral());
   th_difftickheight->Scale(refentries / th_difftickheight->Integral());
   th_diffbaselineslop->Scale(refentries / th_diffbaselineslop->Integral());
   th_diffzerolight->Scale(refentries / th_diffzerolight->Integral());
   th_diffthreshold->Scale(refentries / th_diffthreshold->Integral());
   th_diffliftoff->Scale(refentries / th_diffliftoff->Integral());
   th_diffbias->Scale(refentries / th_diffbias->Integral());
   
   // cout << "scaled diffmeasgain integral is " << to_string(th_diffmeasgain->Integral()) << endl;
   //cout << "scaled diffbias integral is " << to_string(th_diffbias->Integral()) << endl;
   //cout << "scaled diffliftoff integral is " << to_string(th_diffliftoff->Integral()) << endl;
   //cout << "scaled diffthreshold integral is " << to_string(th_diffthreshold->Integral()) << endl;
   //cout << "scaled diffbaselineslop integral is " << to_string(th_diffbaselineslop->Integral()) << endl;
   //cout << "scaled difftickheight integral is " << to_string(th_difftickheight->Integral()) << endl;
   //cout << "scaled difflinknoise integral is " << to_string(th_difflinknoise->Integral()) << endl;
   TFile *output_file = new TFile(output_filename,"recreate");
   output_file->cd();
   th_diffmeasgain->Write();
   th_diffbias->Write();
   th_diffliftoff->Write();
   th_diffthreshold->Write();
   th_diffbaselineslop->Write();
   th_difftickheight->Write();
   th_difflinknoise->Write();
   th_diffzerolight->Write();

   output_file->Write();
   output_file->Close(); 
}   


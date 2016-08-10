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
   // extract the filenames from the filepath 
   Ssiz_t run_namestart = run_filepath.Last('/')+1;
   Ssiz_t ref_namestart = ref_filepath.Last('/')+1;
   TString run_filename( run_filepath(run_namestart,run_filepath.Length()));
   TString ref_filename( ref_filepath(ref_namestart,ref_filepath.Length()) );
   // extract the run and ref numbers from the filename 
   Ssiz_t first = 17; //the index of the '_' after the second "GAINSCAN" in the default filename produced by tkCommissioner
   Ssiz_t last = run_filename.Last('_'); //left this adaptable in case the position changes due to longer/shorter run numbers
   TString run_number( run_filename(first+1, last-first-1) );
   TString ref_number( ref_filename(first+1,last-first-1) ); 

   // get filename and tree name for run data 
   TString tree_name("Tree_04_" + run_number);
   TFile *run = new TFile(run_filepath.Data());
   TTree *run_tree = (TTree*)run->Get(tree_name.Data());
   
   // set up run variables
   Double_t measgain;
   Double_t bias;
   Double_t liftoff;
   Double_t baselineslop;
   Double_t threshold;
   Double_t tickheight;
   Double_t linknoise;
   Double_t zerolight;
   Double_t run_isvalid;  
  
   // book the run branches 
   run_tree->SetBranchAddress("Measgain0",&measgain);
   run_tree->SetBranchAddress("Bias0",&bias);
   run_tree->SetBranchAddress("Liftoff0",&liftoff);
   run_tree->SetBranchAddress("Threshold0",&threshold);
   run_tree->SetBranchAddress("Baselineslop0",&baselineslop);
   run_tree->SetBranchAddress("Tickheight0",&tickheight);
   run_tree->SetBranchAddress("Linknoise0",&linknoise);
   run_tree->SetBranchAddress("Zerolight0",&zerolight);
   run_tree->SetBranchAddress("Isvalid",&run_isvalid);

   // Construct the filename of the reference run
   TString ref_treename("Tree_04_" + ref_number);
   TFile *ref = new TFile(ref_filepath.Data());
   TTree *ref_tree = (TTree*)ref->Get(ref_treename.Data());

   // set up ref variables
   Double_t refmeasgain;
   Double_t refbias;
   Double_t refliftoff;
   Double_t refbaselineslop;
   Double_t refthreshold;
   Double_t reftickheight;
   Double_t reflinknoise;
   Double_t refzerolight;
   Double_t ref_isvalid;
   
   // book the ref branches 
   ref_tree->SetBranchAddress("Measgain0",&refmeasgain);
   ref_tree->SetBranchAddress("Bias0",&refbias);
   ref_tree->SetBranchAddress("Liftoff0",&refliftoff);
   ref_tree->SetBranchAddress("Threshold0",&refthreshold);
   ref_tree->SetBranchAddress("Baselineslop0",&refbaselineslop);
   ref_tree->SetBranchAddress("Tickheight0",&reftickheight);
   ref_tree->SetBranchAddress("Linknoise0",&reflinknoise);
   ref_tree->SetBranchAddress("Zerolight0",&refzerolight);
   ref_tree->SetBranchAddress("Isvalid",&ref_isvalid);

   // create the histograms
   //cout << "y-axis params are " << to_string(yaxis_params[0][0]) << ", " << to_string(yaxis_params[0][1]) << ", " << to_string(yaxis_params[0][2]) << endl;

   //cout << "y-axis params are " << to_string(yaxis_params[3][0]) << ", " << to_string(yaxis_params[3][1]) << ", " << to_string(yaxis_params[3][2]) << endl;
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
   Long64_t minentries = ref_tree->GetEntries();
   Long64_t refentries = ref_tree->GetEntries();
   if (run_tree->GetEntries() < minentries){
      minentries = run_tree->GetEntries();} 
   for (Long64_t i=0;i<minentries;i++) {
      run_tree->GetEntry(i);
      ref_tree->GetEntry(i);
   
      if (!run_isvalid){
         continue;}
      if (!ref_isvalid){
         continue;}

 
      Double_t percentage_measgain = 100*((measgain - refmeasgain) / refmeasgain);
      Double_t percentage_bias = 100*((bias - refbias) / refbias);
      Double_t percentage_liftoff = 100*((liftoff - refliftoff) / refliftoff);
      Double_t percentage_threshold = 100*((threshold - refthreshold) / refthreshold);
      Double_t percentage_baselineslop = 100*((baselineslop - refbaselineslop) / refbaselineslop);
      Double_t percentage_tickheight = 100*((tickheight - reftickheight) / reftickheight);
      Double_t percentage_linknoise = 100*((linknoise - reflinknoise) / reflinknoise);
      Double_t percentage_zerolight = 100*((zerolight - refzerolight) / refzerolight);
      
      th_diffmeasgain->Fill(this_run,percentage_measgain);
      th_diffbias->Fill(this_run,percentage_bias);
      th_diffliftoff->Fill(this_run,percentage_liftoff);
      th_diffthreshold->Fill(this_run,percentage_threshold);
      th_diffbaselineslop->Fill(this_run,percentage_baselineslop);
      th_difftickheight->Fill(this_run,percentage_tickheight);
      th_difflinknoise->Fill(this_run,percentage_linknoise);
      th_diffzerolight->Fill(this_run,percentage_zerolight);
   }
   
   // scale all of the hists
   th_diffmeasgain->Scale(refentries / th_diffmeasgain->Integral());
   th_diffbias->Scale(refentries / th_diffbias->Integral());
   th_diffliftoff->Scale(refentries / th_diffliftoff->Integral());
   th_diffthreshold->Scale(refentries / th_diffthreshold->Integral());
   th_diffbaselineslop->Scale(refentries / th_diffbaselineslop->Integral());
   th_difftickheight->Scale(refentries / th_difftickheight->Integral());
   th_difflinknoise->Scale(refentries / th_difflinknoise->Integral());
   th_diffzerolight->Scale(refentries / th_diffzerolight->Integral());

   // Now open the output file...
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


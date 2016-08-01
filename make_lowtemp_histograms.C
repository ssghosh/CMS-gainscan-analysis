#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TString.h"

void make_lowtemp_histograms(const TString run_filename, const TString output_filename, const TDatime da, const Int_t offset, const Int_t end, const Int_t numtimebins)
{
   Int_t this_run = da.Convert()-offset;
   // extract the run number from the filename to use in the tree name
   Int_t position = 9; // the position after the first "GAINSCAN_" in the default filename produced by tkCommissioner
   Ssiz_t first = run_filename.Index('_',position);
   Ssiz_t last = run_filename.Last('_');
   Ssiz_t period = run_filename.Last('.'); // the ref number is the one right before ".root" in the default filename 
   TString run_number( run_filename(first+1, last-first-1) );
   TString ref_number( run_filename(last+1,period-last-1) ); 

   // get filename and tree name for run data 
   TString tree_name("Tree_04_" + run_number);
   TFile *run = new TFile(run_filename.Data());
   TTree *run_tree = (TTree*)run->Get(tree_name.Data());
   
   // set up run variables
   Double_t diffmeasgain;
   Double_t diffbias;
   Double_t diffliftoff;
   Double_t diffbaselineslop;
   Double_t diffthreshold;
   Double_t difftickheight;
   Double_t difflinknoise;
   Double_t diffzerolight;
   Double_t run_isvalid;  
  
   // book the run branches 
   run_tree->SetBranchAddress("DiffMeasgain0",&diffmeasgain);
   run_tree->SetBranchAddress("DiffBias0",&diffbias);
   run_tree->SetBranchAddress("DiffLiftoff0",&diffliftoff);
   run_tree->SetBranchAddress("DiffThreshold0",&diffthreshold);
   run_tree->SetBranchAddress("DiffBaselineslop0",&diffbaselineslop);
   run_tree->SetBranchAddress("DiffTickheight0",&difftickheight);
   run_tree->SetBranchAddress("DiffLinknoise0",&difflinknoise);
   run_tree->SetBranchAddress("DiffZerolight0",&diffzerolight);
   run_tree->SetBranchAddress("Isvalid",&run_isvalid);

   // Construct the filename of the reference run
   TString ref_filename("GAINSCAN_GAINSCAN_" + ref_number + "_" + ref_number + ".root");
   TString ref_treename("Tree_04_" + ref_number);
   TFile *ref = new TFile(ref_filename.Data());
   TTree *ref_tree = (TTree*)ref->Get(ref_treename.Data());

   // set up ref variables
   Double_t measgain;
   Double_t bias;
   Double_t liftoff;
   Double_t baselineslop;
   Double_t threshold;
   Double_t tickheight;
   Double_t linknoise;
   Double_t zerolight;
   Double_t ref_isvalid;
   
   // book the ref branches 
   ref_tree->SetBranchAddress("Measgain0",&measgain);
   ref_tree->SetBranchAddress("Bias0",&bias);
   ref_tree->SetBranchAddress("Liftoff0",&liftoff);
   ref_tree->SetBranchAddress("Threshold0",&threshold);
   ref_tree->SetBranchAddress("Baselineslop0",&baselineslop);
   ref_tree->SetBranchAddress("Tickheight0",&tickheight);
   ref_tree->SetBranchAddress("Linknoise0",&linknoise);
   ref_tree->SetBranchAddress("Zerolight0",&zerolight);
   ref_tree->SetBranchAddress("Isvalid",&ref_isvalid);

   // create the histograms
   TH2D *th_diffmeasgain = new TH2D("th_diffmeasgain",
      "DiffMeasgain0/Ref;Run date;Difference (%)",
      numtimebins,0,end,70, -10, 30);
   TH2D *th_diffbias = new TH2D("th_diffbias",
      "DiffBias0/Ref;Run date;Difference (%)",
      numtimebins,0,end,60,-30,0);
   TH2D *th_diffliftoff = new TH2D("th_diffliftoff",
      "DiffLiftoff0/Ref;Run date;Difference (%)",
      numtimebins,0,end,60,-40,-10);
   TH2D *th_diffthreshold = new TH2D("th_diffthreshold",
      "DiffThreshold0/Ref;Run date;Difference (%)",
      numtimebins,0,end,70,-50,-10);
   TH2D *th_diffbaselineslop = new TH2D("th_diffbaselineslop",
      "DiffBaselineslope0/Ref;Run date;Difference (%)",
      numtimebins,0,end,60,-15,25);
   TH2D *th_difftickheight = new TH2D("th_difftickheight",
      "DiffTickheight0/Ref;Run date;Difference (%)",
      numtimebins,0,end,70,-10,30);
   TH2D *th_difflinknoise = new TH2D("th_difflinknoise",
      "DiffLinknoise0/Ref;Run date;Difference (%)",
      numtimebins,0,end,70,-20,20);
   TH2D *th_diffzerolight = new TH2D("th_diffzerolight",
      "DiffZerolight0/Ref;Run date;Difference (%)",
      numtimebins,0,end,60,-15,15);


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

 
      Double_t percentage_measgain = 100*(diffmeasgain / measgain);
      Double_t percentage_bias = 100*(diffbias / bias);
      Double_t percentage_liftoff = 100*(diffliftoff / liftoff);
      Double_t percentage_threshold = 100*(diffthreshold / threshold);
      Double_t percentage_baselineslop = 100*(diffbaselineslop / baselineslop);
      Double_t percentage_tickheight = 100*(difftickheight / tickheight);
      Double_t percentage_linknoise = 100*(difflinknoise / linknoise);
      Double_t percentage_zerolight = 100*(diffzerolight / zerolight);
      
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


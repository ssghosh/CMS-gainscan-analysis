void debug_tob_trees(const TString run_filepath, const TString ref_filepath){
   Ssiz_t run_namestart = run_filepath.Last('/')+1;
   Ssiz_t ref_namestart = ref_filepath.Last('/')+1;
   TString run_filename( run_filepath(run_namestart,run_filepath.Length()));
   TString ref_filename( ref_filepath(ref_namestart,ref_filepath.Length()) );
   Ssiz_t first = 17;
   Ssiz_t last = run_filename.Last('_');
   TString run_number( run_filename(first+1, last-first-1) );
   TString ref_number( ref_filename(first+1,last-first-1) );

   TString tree_name("Tree_04_" + run_number);
   TFile *run = new TFile(run_filepath.Data());
   TTree *run_tree = (TTree*)run->Get(tree_name.Data());

   Double_t measgain;
   Double_t diffmeasgain;
   Double_t run_isvalid;

   run_tree->SetBranchAddress("Measgain0",&measgain);
   run_tree->SetBranchAddress("DiffMeasgain0",&diffmeasgain);
   run_tree->SetBranchAddress("Isvalid",&run_isvalid);

   TString ref_treename("Tree_04_" + ref_number);
   TFile *ref = new TFile(ref_filepath.Data());
   TTree *ref_tree = (TTree*)ref->Get(ref_treename.Data());

   Double_t refmeasgain;
   Double_t ref_isvalid;

   ref_tree->SetBranchAddress("Measgain0",&refmeasgain);
   ref_tree->SetBranchAddress("Isvalid",&ref_isvalid);

   Long64_t minentries = ref_tree->GetEntries();
   Long64_t refentries = ref_tree->GetEntries();
   if (run_tree->GetEntries() < minentries){
      minentries = run_tree->GetEntries();
   }
   for (Long64_t i=0;i<minentries;i++) {
      run_tree->GetEntry(i);
      ref_tree->GetEntry(i);

      if (!run_isvalid) continue;
      if (!ref_isvalid) continue;

      if (i <= 10){
         cout << "run measgain " << to_string(i) << " is " << to_string(measgain) << endl;
         cout << "ref measgain " << to_string(i) << " is " << to_string(refmeasgain) << endl;
         cout << "(measgain - refmeasgain) " << to_string(i) << " is " << to_string(measgain - refmeasgain) << endl;
         cout << "diff measgain " << to_string(i) << " is  " << to_string(diffmeasgain) << endl; 
      }
   }
}

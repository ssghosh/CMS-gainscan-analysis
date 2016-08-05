void draw(const TString tob_filename, const TString tob_refnum, const TString tib_filename, const TString tib_refnum, const TString tecp_filename, const TString tecp_refnum, const TString tecm_filename, const TString tecm_refnum, const Int_t offsetdate) {
   TFile *tob = new TFile(tob_filename);
   TFile *tib = new TFile(tib_filename);
   TFile *tecp = new TFile(tecp_filename);
   TFile *tecm = new TFile(tecm_filename); 

   offset = TDatime(offsetdate,000000);
   gStyle->SetTimeOffset(offset.Convert());
   gStyle->SetPalette(1);
   gStyle->SetPadGridY(1);
 
   Int_t number_of_hists = 8;
   TString *hist_names = new TString[number_of_hists];
   hist_names[0] = "th_diffmeasgain";
   hist_names[1] = "th_diffbias";
   hist_names[2] = "th_diffliftoff";
   hist_names[3] = "th_diffthreshold";
   hist_names[4] = "th_diffbaselineslop";
   hist_names[5] = "th_difftickheight";
   hist_names[6] = "th_difflinknoise";
   hist_names[7] = "th_diffzerolight";

   TCanvas *canvases = new TCanvas[number_of_hists];
   canvases[0].SetTitle("diffmeasgain");
   canvases[1].SetTitle("diffbias");
   canvases[2].SetTitle("diffliftoff");
   canvases[3].SetTitle("diffthreshold");
   canvases[4].SetTitle("diffbaselineslope");
   canvases[5].SetTitle("difftickheight");
   canvases[6].SetTitle("difflinknoise");
   canvases[7].SetTitle("diffzerolight");

   TH2D **tob_hists = new TH2D*[number_of_hists];
   for (Int_t i = 0; i < number_of_hists; i++) {
      tob_hists[i] = (TH2D*)tob->Get(hist_names[i]);
      tob_hists[i]->GetXaxis()->SetTimeDisplay(1);
      tob_hists[i]->GetXaxis()->SetTimeFormat("%m/%Y");
      tob_hists[i]->SetStats(0);
      
      TString title = tob_hists[i]->GetTitle();
      TString newtitle = "TOB " + title;
      tob_hists[i]->SetTitle(newtitle);

      TString xtitle = "Run date (Ref = " + tob_refnum + ")";
      tob_hists[i]->SetXTitle(xtitle);
   }

   TH2D **tib_hists = new TH2D*[number_of_hists];
   for (Int_t i = 0; i < number_of_hists; i++) { 
      tib_hists[i] = (TH2D*)tib->Get(hist_names[i]);
      tib_hists[i]->GetXaxis()->SetTimeDisplay(1);
      tib_hists[i]->GetXaxis()->SetTimeFormat("%m/%Y");
      tib_hists[i]->SetStats(0);
      
      TString title = tib_hists[i]->GetTitle();
      TString newtitle = "TIB " + title;
      tib_hists[i]->SetTitle(newtitle);

      TString xtitle = "Run date (Ref = " + tib_refnum + ")";
      tib_hists[i]->SetXTitle(xtitle);   
   }

   TH2D **tecp_hists = new TH2D*[number_of_hists];
   for (Int_t i = 0; i < number_of_hists; i++) {
      tecp_hists[i] = (TH2D*)tecp->Get(hist_names[i]);
      tecp_hists[i]->GetXaxis()->SetTimeDisplay(1);
      tecp_hists[i]->GetXaxis()->SetTimeFormat("%m/%Y");
      tecp_hists[i]->SetStats(0);
      
      TString title = tecp_hists[i]->GetTitle();
      TString newtitle = "TECP " + title;
      tecp_hists[i]->SetTitle(newtitle);

      TString xtitle = "Run date (Ref = " + tecp_refnum + ")";
      tecp_hists[i]->SetXTitle(xtitle);   
   }

   TH2D **tecm_hists = new TH2D*[number_of_hists];
   for (Int_t i = 0; i < number_of_hists; i++) {
      tecm_hists[i] = (TH2D*)tecm->Get(hist_names[i]);
      tecm_hists[i]->GetXaxis()->SetTimeDisplay(1);
      tecm_hists[i]->GetXaxis()->SetTimeFormat("%m/%Y");
      tecm_hists[i]->SetStats(0);
      
      TString title = tecm_hists[i]->GetTitle();
      TString newtitle = "TECM " + title;
      tecm_hists[i]->SetTitle(newtitle);

      TString xtitle = "Run date (Ref = " + tecm_refnum + ")";
      tecm_hists[i]->SetXTitle(xtitle);   
   }

   for (Int_t i = 0; i < number_of_hists; i++){
      canvases[i].Divide(2,2);
      canvases[i].cd();
      canvases[i].cd(1);
      tob_hists[i]->DrawClone("colz");
      canvases[i].cd(2);
      tib_hists[i]->DrawClone("colz");
      canvases[i].cd(3);
      tecp_hists[i]->DrawClone("colz");
      canvases[i].cd(4);
      tecm_hists[i]->DrawClone("colz"); 
   }

}


struct draw_objects {
    vector<*TH2D> hists;
    vector<*TProfile> profiles;
}

draw_objects setup(TFile input, TString partition, TString refnum){ 
   draw_objects result;
    for (Int_t i = 0; i < number_of_hists; i++) {
      result.hists.push_back((TH2D*)tob->Get(hist_names[i]));
      result.hists[i]->GetXaxis()->SetTimeDisplay(1);
      result.hists[i]->GetXaxis()->SetTimeFormat("%m/%Y");
      result.hists[i]->SetStats(0);
      //cout << "axes have been set for tob_hist " << to_string(i) << endl; 
      
      TString title = result.hists[i]->GetTitle();
      TString newtitle = partition + title;
      result.hists[i]->SetTitle(newtitle);
      
      TString xtitle = "Run date (Ref = " + refnum + ")";
      result.hists[i]->SetXTitle(xtitle);
      
      result.profiles[i] = tob_hists[i]->ProfileX("_pfx",1,-1,"s");
      tob_profiles[i]->GetXaxis()->SetTimeDisplay(1);
      tob_profiles[i]->GetXaxis()->SetTimeFormat("%m/%Y");
      tob_profiles[i]->SetStats(0);
      
      tob_profiles[i]->SetTitle("");
      
      tob_profiles[i]->SetXTitle(xtitle);
      tob_profiles[i]->SetYTitle("Difference (%)");
   }
void draw(const TString filename, const TString refnum, const Int_t offsetdate, const TString outputdir) {
   // uncomment the following line to prevent the draw script from creating windows
   gROOT->SetBatch();

   TFile *tob = new TFile(tob_filename);
  
   TDatime offset = TDatime(offsetdate,000000);
   Int_t converted_offset = offset.Convert();
   gStyle->SetTimeOffset(converted_offset);
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


   for (Int_t i = 0; i < number_of_hists; i++){
      canvases[i].Divide(2,2);
      canvases[i].cd(); canvases[i].cd(1);
      tob_hists[i]->DrawClone("colz");
      canvases[i].cd(2);
      tib_hists[i]->DrawClone("colz");
      canvases[i].cd(3);
      tecp_hists[i]->DrawClone("colz");
      canvases[i].cd(4);
      tecm_hists[i]->DrawClone("colz"); 
      TString title = canvases[i].GetTitle();
      TString pdfname = "4c_pdfs/" + title + ".pdf";
      canvases[i].Print(pdfname);
   }
   
   TCanvas *test_canvas = new TCanvas;
   test_canvas->Divide(1,2);
   test_canvas->cd();
   test_canvas->cd(1);
   tob_hists[3]->DrawClone("colz");
   tob_profiles[3]->DrawClone("sameE1");
   test_canvas->cd(2);
   tob_profiles[3]->DrawClone("E1");
   
   test_canvas->Print("profile_test.pdf");
  
   gStyle->SetPaintTextFormat(".0f"); 
   TCanvas *text_canvas = new TCanvas;
   text_canvas->cd();
   tob_hists[0]->SetMarkerSize(0.8);
   tob_hists[0]->DrawClone("TEXT");
   text_canvas->Print("text_test.pdf");
   
}

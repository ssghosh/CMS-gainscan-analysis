void draw_partition(const TString tob_filename, const TString tob_refnum, const TString tib_filename, const TString tib_refnum, const TString tecp_filename, const TString tecp_refnum, const TString tecm_filename, const TString tecm_refnum, const Int_t offsetdate, const TString pdf_dir) {
    // uncomment the following line to prevent the draw script from creating windows
    gROOT->SetBatch();

    TFile *tob = new TFile(tob_filename);
    TFile *tib = new TFile(tib_filename);
    TFile *tecp = new TFile(tecp_filename);
    TFile *tecm = new TFile(tecm_filename); 

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

    vector<TH2D*> tob_hists;
    //vector<TProfile> tob_profiles;
    for (Int_t i = 0; i < number_of_hists; i++) {
        tob_hists.push_back((TH2D*)tob->Get(hist_names[i]));
        tob_hists[i]->GetXaxis()->SetTimeDisplay(1);
        tob_hists[i]->GetXaxis()->SetTimeFormat("%m/%Y");
        tob_hists[i]->SetStats(0);
        //cout << "axes have been set for tob_hist " << to_string(i) << endl; 

        TString title = tob_hists[i]->GetTitle();
        TString newtitle = "TOB " + title;
        tob_hists[i]->SetTitle(newtitle);

        TString xtitle = "Run date (Ref = " + tob_refnum + ")";
        tob_hists[i]->SetXTitle(xtitle);

        //tob_profiles.push_back(tob_hists[i]->ProfileX("_pfx",1,-1,"s"));
        //tob_profiles[i].GetXaxis()->SetTimeDisplay(1);
        //tob_profiles[i].GetXaxis()->SetTimeFormat("%m/%Y");
        //tob_profiles[i].SetStats(0);
        //
        //tob_profiles[i].SetTitle("");
        //
        //tob_profiles[i].SetXTitle(xtitle);
        //tob_profiles[i].SetYTitle("Difference (%)");
    }

    vector<TH2D*> tib_hists;
    for (Int_t i = 0; i < number_of_hists; i++) {
        tib_hists.push_back((TH2D*)tib->Get(hist_names[i]));
        tib_hists[i]->GetXaxis()->SetTimeDisplay(1);
        tib_hists[i]->GetXaxis()->SetTimeFormat("%m/%Y");
        tib_hists[i]->SetStats(0);
        //cout << "axes have been set for tib_hist " << to_string(i) << endl; 

        TString title = tib_hists[i]->GetTitle();
        TString newtitle = "TIB " + title;
        tib_hists[i]->SetTitle(newtitle);

        TString xtitle = "Run date (Ref = " + tib_refnum + ")";
        tib_hists[i]->SetXTitle(xtitle);

    }

    vector<TH2D*> tecp_hists;
    for (Int_t i = 0; i < number_of_hists; i++) {
        tecp_hists.push_back((TH2D*)tecp->Get(hist_names[i]));
        tecp_hists[i]->GetXaxis()->SetTimeDisplay(1);
        tecp_hists[i]->GetXaxis()->SetTimeFormat("%m/%Y");
        tecp_hists[i]->SetStats(0);
        //cout << "axes have been set for tecp_hist " << to_string(i) << endl; 

        TString title = tecp_hists[i]->GetTitle();
        TString newtitle = "TECP " + title;
        tecp_hists[i]->SetTitle(newtitle);

        TString xtitle = "Run date (Ref = " + tecp_refnum + ")";
        tecp_hists[i]->SetXTitle(xtitle);
    }

    vector<TH2D*> tecm_hists;
    for (Int_t i = 0; i < number_of_hists; i++) {
        tecm_hists.push_back((TH2D*)tecm->Get(hist_names[i]));
        tecm_hists[i]->GetXaxis()->SetTimeDisplay(1);
        tecm_hists[i]->GetXaxis()->SetTimeFormat("%m/%Y");
        tecm_hists[i]->SetStats(0);
        //cout << "axes have been set for tecm_hist " << to_string(i) << endl; 

        TString title = tecm_hists[i]->GetTitle();
        TString newtitle = "TECM " + title;
        tecm_hists[i]->SetTitle(newtitle);

        TString xtitle = "Run date (Ref = " + tecm_refnum + ")";
        tecm_hists[i]->SetXTitle(xtitle);

    }

    for (Int_t i = 0; i < number_of_hists; i++){
        TCanvas *output_canvas = new TCanvas("canvas", "Canvas", 1500, 800);;
        output_canvas->Divide(2,2);
        output_canvas->cd();
        output_canvas->cd(1);
        tob_hists[i]->DrawClone("colz");
        TProfile *tob_profile = tob_hists[i]->ProfileX("_pfx",1,-1,"s");
        tob_profile->DrawClone("sameE1");
        output_canvas->cd(2);
        tib_hists[i]->DrawClone("colz");
        TProfile *tib_profile = tib_hists[i]->ProfileX("_pfx",1,-1,"s");
        tib_profile->DrawClone("sameE1");
        output_canvas->cd(3);
        tecp_hists[i]->DrawClone("colz");
        TProfile *tecp_profile = tecp_hists[i]->ProfileX("_pfx",1,-1,"s");
        tecp_profile->DrawClone("sameE1");
        output_canvas->cd(4);
        tecm_hists[i]->DrawClone("colz");
        TProfile *tecm_profile = tecm_hists[i]->ProfileX("_pfx",1,-1,"s");
        tecm_profile->DrawClone("sameE1");

        TString filename = hist_names[i] + ".pdf";
        output_canvas->Print(pdf_dir + "/" + filename);
        output_canvas->Print(pdf_dir + "/" + hist_names[i] + ".png");
    }
}

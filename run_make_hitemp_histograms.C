#include "make_hitemp_histograms.C"
#include <string>

void run_make_hitemp_histograms(){
   Int_t num_files = 11;	
   
   TString *tob_filenames = new TString[num_files];
   tob_filenames[0] = "GAINSCAN_GAINSCAN_164729_146016.root";
   tob_filenames[1] = "GAINSCAN_GAINSCAN_177365_146016.root";
   tob_filenames[2] = "GAINSCAN_GAINSCAN_193248_146016.root";
   tob_filenames[3] = "GAINSCAN_GAINSCAN_203103_146016.root";
   tob_filenames[4] = "GAINSCAN_GAINSCAN_209193_146016.root";
   tob_filenames[5] = "GAINSCAN_GAINSCAN_209423_146016.root";
   tob_filenames[6] = "GAINSCAN_GAINSCAN_211976_146016.root";
   tob_filenames[7] = "GAINSCAN_GAINSCAN_215430_146016.root";
   tob_filenames[8] = "GAINSCAN_GAINSCAN_223737_146016.root";
   tob_filenames[9] = "GAINSCAN_GAINSCAN_231872_146016.root";
   tob_filenames[10] = "GAINSCAN_GAINSCAN_245736_146016.root";


   TString *tib_filenames = new TString[num_files];
   tib_filenames[0] = "GAINSCAN_GAINSCAN_164706_126252.root";
   tib_filenames[1] = "GAINSCAN_GAINSCAN_177358_126252.root";
   tib_filenames[2] = "GAINSCAN_GAINSCAN_193246_126252.root";
   tib_filenames[3] = "GAINSCAN_GAINSCAN_203101_126252.root";
   tib_filenames[4] = "GAINSCAN_GAINSCAN_209172_126252.root";
   tib_filenames[5] = "GAINSCAN_GAINSCAN_209413_126252.root";
   tib_filenames[6] = "GAINSCAN_GAINSCAN_211975_126252.root";
   tib_filenames[7] = "GAINSCAN_GAINSCAN_215039_126252.root";
   tib_filenames[8] = "GAINSCAN_GAINSCAN_223549_126252.root";
   tib_filenames[9] = "GAINSCAN_GAINSCAN_231873_126252.root";
   tib_filenames[10] = "GAINSCAN_GAINSCAN_245734_126252.root";


   TString *tecp_filenames = new TString[num_files];
   tecp_filenames[0] = "GAINSCAN_GAINSCAN_164700_131014.root";
   tecp_filenames[1] = "GAINSCAN_GAINSCAN_177362_131014.root";
   tecp_filenames[2] = "GAINSCAN_GAINSCAN_193273_131014.root";
   tecp_filenames[3] = "GAINSCAN_GAINSCAN_203104_131014.root";
   tecp_filenames[4] = "GAINSCAN_GAINSCAN_209173_131014.root";
   tecp_filenames[5] = "GAINSCAN_GAINSCAN_209429_131014.root";
   tecp_filenames[6] = "GAINSCAN_GAINSCAN_211977_131014.root";
   tecp_filenames[7] = "GAINSCAN_GAINSCAN_215428_131014.root";
   tecp_filenames[8] = "GAINSCAN_GAINSCAN_223548_131014.root";
   tecp_filenames[9] = "GAINSCAN_GAINSCAN_231874_131014.root";
   tecp_filenames[10] = "GAINSCAN_GAINSCAN_245737_131014.root";
  
 
   TString *tecm_filenames = new TString[num_files];
   tecm_filenames[0] = "GAINSCAN_GAINSCAN_164714_131015.root";
   tecm_filenames[1] = "GAINSCAN_GAINSCAN_177359_131015.root";
   tecm_filenames[2] = "GAINSCAN_GAINSCAN_193247_131015.root";
   tecm_filenames[3] = "GAINSCAN_GAINSCAN_203102_131015.root";
   tecm_filenames[4] = "GAINSCAN_GAINSCAN_209180_131015.root";
   tecm_filenames[5] = "GAINSCAN_GAINSCAN_209416_131015.root";
   tecm_filenames[6] = "GAINSCAN_GAINSCAN_212046_131015.root";
   tecm_filenames[7] = "GAINSCAN_GAINSCAN_215593_131015.root";
   tecm_filenames[8] = "GAINSCAN_GAINSCAN_223536_131015.root";
   tecm_filenames[9] = "GAINSCAN_GAINSCAN_231875_131015.root";
   tecm_filenames[10] = "GAINSCAN_GAINSCAN_245735_131015.root";
     

   TDatime *datimes = new TDatime[num_files];
   datimes[0] = TDatime(20110512,000000);
   datimes[1] = TDatime(20110928,000000);
   datimes[2] = TDatime(20120503,000000);
   datimes[3] = TDatime(20120920,000000);
   datimes[4] = TDatime(20121217,000000);
   datimes[5] = TDatime(20130107,000000);
   datimes[6] = TDatime(20130216,000000);
   datimes[7] = TDatime(20131021,000000);
   datimes[8] = TDatime(20140722,000000);
   datimes[9] = TDatime(20150105,000000);
   datimes[10] = TDatime(20150526,000000);


   TDatime begin_date(20110501,000000);
   TDatime end_date(20150601,000000);
   Int_t offset = begin_date.Convert();
   gStyle->SetTimeOffset(offset);
   Int_t end = end_date.Convert()-offset;
   Int_t numtimebins = 49;


   // Loop over the files and produce all histograms, putting them in individual output files 
   for (Int_t i = 0; i < num_files; i++) {
      TString tob_output_filename = "tob_hitemp_output_" + to_string(i) + ".root";
      make_hitemp_histograms(tob_filenames[i],tob_output_filename,datimes[i],offset,end,numtimebins);
      TString tib_output_filename = "tib_hitemp_output_" + to_string(i) + ".root"; 
      make_hitemp_histograms(tib_filenames[i],tib_output_filename,datimes[i],offset,end,numtimebins);
      TString tecp_output_filename = "tecp_hitemp_output_" + to_string(i) + ".root"; 
      make_hitemp_histograms(tecp_filenames[i],tecp_output_filename,datimes[i],offset,end,numtimebins);
      TString tecm_output_filename = "tecm_hitemp_output_" + to_string(i) + ".root"; 
      make_hitemp_histograms(tecm_filenames[i],tecm_output_filename,datimes[i],offset,end,numtimebins);
   }


      
}

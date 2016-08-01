#include "make_lowtemp_histograms.C"
#include <string>

void run_make_lowtemp_histograms(){
   Int_t num_files = 9;	
   
   TString *tob_filenames = new TString[num_files];
   tob_filenames[0] = "GAINSCAN_GAINSCAN_216398_146016.root";
   tob_filenames[1] = "GAINSCAN_GAINSCAN_218830_146016.root";
   tob_filenames[2] = "GAINSCAN_GAINSCAN_231907_146016.root";
   tob_filenames[3] = "GAINSCAN_GAINSCAN_243523_146016.root";
   tob_filenames[4] = "GAINSCAN_GAINSCAN_250493_146016.root";
   tob_filenames[5] = "GAINSCAN_GAINSCAN_264332_146016.root";
   tob_filenames[6] = "GAINSCAN_GAINSCAN_268831_146016.root";
   tob_filenames[7] = "GAINSCAN_GAINSCAN_273868_146016.root";
   tob_filenames[8] = "GAINSCAN_GAINSCAN_274648_146016.root";

   TString *tib_filenames = new TString[num_files];
   tib_filenames[0] = "GAINSCAN_GAINSCAN_216430_126252.root";
   tib_filenames[1] = "GAINSCAN_GAINSCAN_218831_126252.root";
   tib_filenames[2] = "GAINSCAN_GAINSCAN_231906_126252.root";
   tib_filenames[3] = "GAINSCAN_GAINSCAN_243521_126252.root";
   tib_filenames[4] = "GAINSCAN_GAINSCAN_250485_126252.root";
   tib_filenames[5] = "GAINSCAN_GAINSCAN_264326_126252.root";
   tib_filenames[6] = "GAINSCAN_GAINSCAN_268824_126252.root";
   tib_filenames[7] = "GAINSCAN_GAINSCAN_273867_126252.root";
   tib_filenames[8] = "GAINSCAN_GAINSCAN_274640_126252.root";


   TString *tecp_filenames = new TString[num_files];
   tecp_filenames[0] = "GAINSCAN_GAINSCAN_216399_131014.root";
   tecp_filenames[1] = "GAINSCAN_GAINSCAN_218837_131014.root";
   tecp_filenames[2] = "GAINSCAN_GAINSCAN_231904_131014.root";
   tecp_filenames[3] = "GAINSCAN_GAINSCAN_243524_131014.root";
   tecp_filenames[4] = "GAINSCAN_GAINSCAN_250487_131014.root";
   tecp_filenames[5] = "GAINSCAN_GAINSCAN_264342_131014.root";
   tecp_filenames[6] = "GAINSCAN_GAINSCAN_268826_131014.root";
   tecp_filenames[7] = "GAINSCAN_GAINSCAN_273865_131014.root";
   tecp_filenames[8] = "GAINSCAN_GAINSCAN_274639_131014.root";
  
 
   TString *tecm_filenames = new TString[num_files];
   tecm_filenames[0] = "GAINSCAN_GAINSCAN_216436_131015.root";
   tecm_filenames[1] = "GAINSCAN_GAINSCAN_218838_131015.root";
   tecm_filenames[2] = "GAINSCAN_GAINSCAN_231905_131015.root";
   tecm_filenames[3] = "GAINSCAN_GAINSCAN_243522_131015.root";
   tecm_filenames[4] = "GAINSCAN_GAINSCAN_250488_131015.root";
   tecm_filenames[5] = "GAINSCAN_GAINSCAN_264331_131015.root";
   tecm_filenames[6] = "GAINSCAN_GAINSCAN_268825_131015.root";
   tecm_filenames[7] = "GAINSCAN_GAINSCAN_273853_131015.root";
   tecm_filenames[8] = "GAINSCAN_GAINSCAN_274650_131015.root";

   TDatime *datimes = new TDatime[num_files];
   datimes[0] = TDatime(20131107,000000);
   datimes[1] = TDatime(20140221,000000);
   datimes[2] = TDatime(20150108,000000);
   datimes[3] = TDatime(20150506,000000);
   datimes[4] = TDatime(20150701,000000);
   datimes[5] = TDatime(20160210,000000);
   datimes[6] = TDatime(20160407,000000);
   datimes[7] = TDatime(20160526,000000);
   datimes[8] = TDatime(20160608,000000);

   TDatime begin_date(20131101,000000);
   TDatime end_date(20160701,000000);
   Int_t offset = begin_date.Convert();
   gStyle->SetTimeOffset(offset);
   Int_t end = end_date.Convert()-offset;
   Int_t numtimebins = 32;


   // Loop over the files and produce all histograms, putting them in individual output files 
   for (Int_t i = 0; i < num_files; i++) {
      TString tob_output_filename = "tob_lowtemp_output_" + to_string(i) + ".root";
      make_lowtemp_histograms(tob_filenames[i],tob_output_filename,datimes[i],offset,end,numtimebins);
      TString tib_output_filename = "tib_lowtemp_output_" + to_string(i) + ".root"; 
      make_lowtemp_histograms(tib_filenames[i],tib_output_filename,datimes[i],offset,end,numtimebins);
      TString tecp_output_filename = "tecp_lowtemp_output_" + to_string(i) + ".root"; 
      make_lowtemp_histograms(tecp_filenames[i],tecp_output_filename,datimes[i],offset,end,numtimebins);
      TString tecm_output_filename = "tecm_lowtemp_output_" + to_string(i) + ".root"; 
      make_lowtemp_histograms(tecm_filenames[i],tecm_output_filename,datimes[i],offset,end,numtimebins);
   }


      
}

#include "draw.C"

void run_lowtemp_draw() {
   TString tob_filename = "tob_lowtemp_added.root";
   TString tib_filename = "tib_lowtemp_added.root";
   TString tecp_filename = "tecp_lowtemp_added.root";
   TString tecm_filename = "tecm_lowtemp_added.root";
   
   TDatime offset(20131101,000000);

   draw(tob_filename,tib_filename,tecp_filename,tecm_filename,offset);
}

    

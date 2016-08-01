#include "draw.C"

void run_hitemp_draw() {
   TString tob_filename = "tob_hitemp_added.root";
   TString tib_filename = "tib_hitemp_added.root";
   TString tecp_filename = "tecp_hitemp_added.root";
   TString tecm_filename = "tecm_hitemp_added.root";
   
   TDatime offset(20110501,000000);

   draw(tob_filename,tib_filename,tecp_filename,tecm_filename,offset);
}

    

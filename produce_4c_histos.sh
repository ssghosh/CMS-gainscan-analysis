root -l -q run_make_histograms.C'("tob_filenames.txt","tib_filenames.txt","tecp_filenames.txt","tecm_filenames.txt","rundates.txt", "4c")'

hadd -f tob_4c_added.root tob_4c_output_*.root
hadd -f tib_4c_added.root tib_4c_output_*.root
hadd -f tecp_4c_added.root tecp_4c_output_*.root
hadd -f tecm_4c_added.root tecm_4c_output_*.root

root -l -q 

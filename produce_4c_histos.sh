#!/bin/bash
root -l -q run_make_partition_hists.C'(  "config/tob_4c_filenames.txt",
                                    "config/tib_4c_filenames.txt",
                                    "config/tecp_4c_filenames.txt",
                                    "config/tecm_4c_filenames.txt",
                                    "output",
                                    "4c_output", 
                                    "config/4c_yaxis_params.txt")'

hadd -f output/tob_4c_added.root output/tob_4c_output_*.root
hadd -f output/tib_4c_added.root output/tib_4c_output_*.root
hadd -f output/tecp_4c_added.root output/tecp_4c_output_*.root
hadd -f output/tecm_4c_added.root output/tecm_4c_output_*.root

#root -l draw.C'("tob_4c_added.root", "158944", "tib_4c_added.root", "160769", "tecp_4c_added.root", "158943", "tecm_4c_added.root", "158941", 20110501, "4c_pdfs")'
root -l draw_partition.C'("output/tob_4c_added.root", "158944", "output/tib_4c_added.root", "160769", "output/tecp_4c_added.root", "158943", "output/tecm_4c_added.root", "158941", 20110501, "pdfs_4c")'

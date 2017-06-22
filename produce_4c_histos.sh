#!/bin/bash
root -l -q run_make_histograms.C'(  "config/tob_4c_filenames.txt",
                                    "config/tib_4c_filenames.txt",
                                    "config/tecp_4c_filenames.txt",
                                    "config/tecm_4c_filenames.txt",
                                    "4c_output", 
                                    "config/4c_yaxis_params.txt")'

hadd -f tob_4c_added.root tob_4c_output_*.root
hadd -f tib_4c_added.root tib_4c_output_*.root
hadd -f tecp_4c_added.root tecp_4c_output_*.root
hadd -f tecm_4c_added.root tecm_4c_output_*.root

root -l draw.C'("tob_4c_added.root", "158944", "tib_4c_added.root", "160769", "tecp_4c_added.root", "158943", "tecm_4c_added.root", "158941", 20110501, "4c_pdfs")'

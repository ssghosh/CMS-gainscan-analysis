#root -l -q run_make_partition_hists.C'("config/tob_-15c_filenames.txt","config/tib_-15c_filenames.txt","config/tecp_-15c_filenames.txt","config/tecm_-15c_filenames.txt","output", "-15c_output", "config/yaxis_params_-15c.txt")'
##
#hadd -f output/tob_-15c_added.root output/tob_-15c_output_*.root
#hadd -f tib_-15c_added.root tib_-15c_output_*.root
#hadd -f tecp_-15c_added.root tecp_-15c_output_*.root
#hadd -f tecm_-15c_added.root tecm_-15c_output_*.root

root -l draw.C'("output/tob_-15c_added.root", "219662", "output/tib_-15c_added.root", "219716", "output/tecp_-15c_added.root", "219661", "output/tecm_-15c_added.root", "219659", 20150101)'

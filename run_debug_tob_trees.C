#include "debug_tob_trees.C"
void run_debug_tob_trees(const TString run_filelist){
   vector<TString> run_files;   

   string line;

   std::ifstream run_reader(run_filelist);

   if (!run_reader.is_open()){
      cout << "TOB file can't be opened" << endl;
   }   

   while (std::getline(run_reader, line)){
      if (line.length() == 0) continue;
      cout << line << endl;
      run_files.push_back(line);
   }   
   run_reader.close();

   cout << "Using ref file " << run_files[0] << endl;
   for (Int_t i = 1; i < run_files.size(); i++){
      cout << "For TOB file " << run_files[i] << ":" << endl;
      debug_tob_trees(run_files[i], run_files[0]);
   } 


}

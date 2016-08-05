#include "make_hitemp_histograms.C"
#include <string>

void run_make_histograms(TString tob_filelist, TString tib_filelist, TString tecp_filelist, TString tecm_filelist, TString datelist, TString output_prefix){
   vector<TString> tob_files;
   vector<TString> tib_files;
   vector<TString> tecp_files;
   vector<TString> tecm_files;
   vector<TDatime> rundates;

   string line;

   std::ifstream tob_reader(tob_filelist);

   if (!tob_reader.is_open()){
      cout << "TOB file can't be opened" << endl;
   }

   while (std::getline(tob_reader, line)){
      if (line.length() == 0) continue;
      cout << line << endl;
      tob_files.push_back(line);
   }
   tob_reader.close();

   std::ifstream tib_reader(tib_filelist);
   
   if (!tib_reader.is_open()){
      cout << "TIB file can't be opened" << endl;
   }

   while (std::getline(tib_reader, line)){
      if (line.length() == 0) continue;
      cout << line << endl;
      tib_files.push_back(line);
   }
   tib_reader.close();

   std::ifstream tecp_reader(tecp_filelist);
   
   if (!tecp_reader.is_open()){
      cout << "TECP file can't be opened" << endl;
   }

   while (std::getline(tecp_reader, line)){
      if (line.length() == 0) continue;
      cout << line << endl;
      tecp_files.push_back(line);
   }
   tecp_reader.close();

   std::ifstream tecm_reader(tecm_filelist);
   
   if (!tecm_reader.is_open()){
      cout << "TECM file can't be opened" << endl;
   }

   while (std::getline(tecm_reader, line)){
      if (line.length() == 0) continue;
      cout << line << endl;
      tecm_files.push_back(line);
   }
   tecm_reader.close();

   std::ifstream datime_reader(datelist);
   
   if (!datime_reader.is_open()){
      cout << "TDatime file can't be opened" << endl;
   }

   
   while (std::getline(datime_reader, line)){
      if (line.length() == 0) continue;
      cout << line << endl;
      rundates.push_back(TDatime(line.c_str()));
   }
   datime_reader.close();

   TString begin_date_string = to_string(rundates.front().GetYear()) + "-" 
                        + to_string(rundates.front().GetMonth()) + "-"
                        + to_string(1) + " 00:00:00";
 
   Int_t year;
   Int_t month;
   year = (rundates.back().GetMonth() == 12) ? rundates.back().GetYear()+1 : rundates.back().GetYear();
   month = (rundates.back().GetMonth() == 12) ? 1 : rundates.back().GetMonth()+1;
   TString end_date_string = to_string(year) + "-" 
                        + to_string(month) + "-"
                        + to_string(1) + " 00:00:00"; 
   TDatime begin_date(begin_date_string);
   TDatime end_date(end_date_string);
   Int_t offset = begin_date.Convert();
   gStyle->SetTimeOffset(offset);
   Int_t end = end_date.Convert()-offset;
   // the number of time bins is the number of months
   // this calculation assumes that the rundates span multiple years
   Int_t numtimebins = ((end_date.GetYear()-1) - (begin_date.GetYear()+1))*12 + begin_date.GetMonth() + (11 - end_date.GetMonth());

   Int_t num_files = rundates.size();
   // Loop over the files and produce all histograms, putting them in individual output files 
   for (Int_t i = 0; i < num_files; i++) {
      TString tob_output_filename = "tob_" + output_prefix + "_" + to_string(i) + ".root";
      make_hitemp_histograms(tob_files[i],tob_output_filename,rundates[i],offset,end,numtimebins);
      TString tib_output_filename = "tib_" + output_prefix + "_" + to_string(i) + ".root"; 
      make_hitemp_histograms(tib_files[i],tib_output_filename,rundates[i],offset,end,numtimebins);
      TString tecp_output_filename = "tecp_" + output_prefix + "_" + to_string(i) + ".root"; 
      make_hitemp_histograms(tecp_files[i],tecp_output_filename,rundates[i],offset,end,numtimebins);
      TString tecm_output_filename = "tecm_" + output_prefix + "_" + to_string(i) + ".root"; 
      make_hitemp_histograms(tecm_files[i],tecm_output_filename,rundates[i],offset,end,numtimebins);
   }


      
}

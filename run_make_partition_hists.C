#include "make_histograms.C"
#include <string>

struct parsed_config{
    vector<TString> filenames;
    vector<TDatime> rundates;
};

parsed_config parse_config(TString filename){

    string line;

    std::ifstream reader(filename);

    if (!reader.is_open()){
        cout << "Config file " << filename << " can't be opened" << endl;
    }

    vector<TString> files;
    vector<TDatime> rundates; 
    while (std::getline(reader, line)){
        if (line.length() == 0 || line.substr(0,1) == "$") continue;
        char *cstr = new char[line.length() + 1];
        strcpy(cstr, line.c_str());
        char *p = strtok(cstr, ",");
        vector<TString> tokenized;
        while (p) {
            tokenized.push_back(p);
            p = strtok(NULL, ",");
        }
        //cout << "filename is " << tokenized[0] << endl;
        //cout << "datestamp is " << tokenized[1] << endl;
        files.push_back(tokenized[0]);
        cout << "rundate is " << tokenized[1] << std::endl;
        rundates.push_back(TDatime(tokenized[1]));
    }
    reader.close();

    parsed_config result = {files, rundates};
    return result; 
}

vector<TDatime> get_begin_end_dates(vector<TDatime> rundates){

    TString begin_date_string = to_string(rundates[1].GetYear()) + "-" 
        + to_string(rundates[1].GetMonth()) + "-"
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
    vector<TDatime> result;
    result.push_back(begin_date);
    result.push_back(end_date);

    return result;
}

Int_t get_num_time_bins(vector<TDatime> begin_end_dates){
    Int_t numtimebins = (begin_end_dates[1].GetYear() - begin_end_dates[0].GetYear() + 1)*12 - begin_end_dates[0].GetMonth() - (12 - begin_end_dates[1].GetMonth());
    return numtimebins;
}

void produce_hists(TString output_dir, TString output_name_base, parsed_config partition_info, TString yaxisparamfile){
    vector<TDatime> begin_end_dates = get_begin_end_dates(partition_info.rundates);
    Int_t offset = begin_end_dates[0].Convert();
    std::cout << "begin date is " << begin_end_dates[0].AsSQLString() << std::endl;
    std::cout << "end date is " << begin_end_dates[1].AsSQLString() << std::endl;
    gStyle->SetTimeOffset(offset);
    Int_t end = begin_end_dates[1].Convert()-offset;

    Int_t numtimebins = get_num_time_bins(begin_end_dates);
    std::cout << "num time bins is " << numtimebins << std::endl;
    Int_t num_files = partition_info.rundates.size();

    for (Int_t i = 1; i < num_files; i++) {
        TString output_filename = output_dir + "/" + output_name_base + "_" + to_string(i) + ".root";
        cout << "Processing " << partition_info.filenames[i] << endl;
        make_histograms(partition_info.filenames[i],
                partition_info.filenames[0],
                output_filename,
                partition_info.rundates[i],
                offset,
                end,
                numtimebins,
                yaxisparamfile);
    }
}

void run_make_partition_hists(TString tob_filelist, TString tib_filelist, TString tecp_filelist, TString tecm_filelist, TString output_dir, TString output_prefix, TString yaxisparamfile){

    cout << "\n ===== Processing TOB files =====\n" << endl;
    parsed_config tob_info = parse_config(tob_filelist);
    produce_hists(output_dir, "tob_" + output_prefix, tob_info, yaxisparamfile);

    cout << "\n ===== Processing TIB files =====\n" << endl;
    parsed_config tib_info = parse_config(tib_filelist);
    produce_hists(output_dir, "tib_" + output_prefix, tib_info, yaxisparamfile);

    parsed_config tecp_info = parse_config(tecp_filelist);
    cout << "\n ===== Processing TECP files =====\n" << endl;
    produce_hists(output_dir, "tecp_" + output_prefix, tecp_info, yaxisparamfile);

    parsed_config tecm_info = parse_config(tecm_filelist);
    cout << "\n ===== Processing TECM files =====\n" << endl;
    produce_hists(output_dir, "tecm_" + output_prefix, tecm_info, yaxisparamfile);
}

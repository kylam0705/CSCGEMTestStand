#include "Set.h"
#include "eth_lib.h"
#include "commands.h"
#include "utils.h"
//
//#include "CSCConstants.h"

namespace cw {

const std::string default_dir = "/home/cscdev/Sasha/pats/tmp/";
const std::string default_ps_output_file = "/home/cscdev/Sasha/pattern_studies/paramScan.log";

const int tmb_counters[2] = {
					29,	// clct0 sent to TMB matching section
					30	// clct1 sent to TMB matching section

				      };

using namespace std;
using namespace emu;
using namespace pc;

int patFile_to_pageID[11] = {// id	 .pat file
				1,	// CFEB 0
				2,	// CFEB 1
				3,	// CFEB 2
				8,	// CFEB 3
				9,	// CFEB 4
				10,	// CFEB 5
				11,	// CFEB 6
				4,	// GEM 1
				5,	// GEM 2
				6,	// GEM 3
				7	// GEM 4
			    };


//~~~~~~~~~ TMB Readout Manager ~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TMBresponse::TMBresponse(int clct_0, int clct_1, int delta_clct0, int delta_clct1, int N_occr){
	CLCT_0 = clct_0;
	CLCT_1 = clct_1;
	delta_CLCT0 = delta_clct0;
	delta_CLCT1 = delta_clct1;
	occurCount = N_occr;
}

void TMBresponse::operator++(){
	occurCount += 1;
	return;
}

bool operator==(const TMBresponse& a, const TMBresponse& b){
	return ((a.CLCT_0 == b.CLCT_0) && (a.CLCT_1 == b.CLCT_1) && (a.delta_CLCT0 == b.delta_CLCT0) && (a.delta_CLCT1 == b.delta_CLCT1));
}

std::ostream& operator<<(std::ostream& oss, const TMBresponse& tmbr_o){
	oss << "( " << std::hex << tmbr_o.CLCT_0 << " " << std::hex << tmbr_o.CLCT_1 << " " << std::dec << tmbr_o.delta_CLCT0 << " " << tmbr_o.delta_CLCT1 << " | " << std::dec << tmbr_o.occurCount << " )";
	return oss;
}

std::istream& operator>>(std::istream& iss, TMBresponse& tmbr_i){
	char tmp;
	int  clct_0, clct_1, delta_clct0, delta_clct1, n_occr;
	iss >> tmp >> clct_0 >> clct_1 >> delta_clct0 >> tmp >> delta_clct1 >> tmp >> n_occr >> tmp;
	tmbr_i = TMBresponse(clct_0, clct_1, delta_clct0, delta_clct1, n_occr);
	return iss;
}


//~~~~~~~~~ TMB Readout Manager LONG ~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TMBresponse_long::TMBresponse_long (int clct_nhit_0, int clct_pid_0, int clct_key_0, int clct_nhit_1, int clct_pid_1, int clct_key_1, int delta_clct0, int delta_clct1, int N_occr){
        CLCT_nhit_0 = clct_nhit_0;
	CLCT_pid_0 = clct_pid_0;
	CLCT_key_0 = clct_key_0;
        CLCT_nhit_1 = clct_nhit_1;
	CLCT_pid_1 = clct_pid_1;
	CLCT_key_1 = clct_key_1;
        delta_CLCT0 = delta_clct0;
	delta_CLCT1 = delta_clct1;
        occurCount = N_occr;
}

void TMBresponse_long::operator++(){
        occurCount += 1;
        return;
}

bool operator==(const TMBresponse_long& a, const TMBresponse_long& b){
        bool tf = ((a.CLCT_nhit_0 == b.CLCT_nhit_0) && (a.CLCT_pid_0 == b.CLCT_pid_0) && (a.CLCT_key_0 == b.CLCT_key_0));
	tf = tf && ((a.CLCT_nhit_1 == b.CLCT_nhit_1) && (a.CLCT_pid_1 == b.CLCT_pid_1) && (a.CLCT_key_1 == b.CLCT_key_1));
	tf = tf && ((a.delta_CLCT0 == b.delta_CLCT0) && (a.delta_CLCT1 == b.delta_CLCT1));
}

std::ostream& operator<<(std::ostream& oss, const TMBresponse_long& tmbr_o){
        oss << "( " << tmbr_o.CLCT_nhit_0 << " " << tmbr_o.CLCT_pid_0 << " " << tmbr_o.CLCT_key_0
	    << "  " << tmbr_o.CLCT_nhit_1 << " " << tmbr_o.CLCT_pid_1 << " " << tmbr_o.CLCT_key_1
	    << "; " << tmbr_o.delta_CLCT0 << " " << tmbr_o.delta_CLCT1 << " | " << std::dec << tmbr_o.occurCount << " )";
        return oss;
}

//~~~~~~~~~ Pattern Sets ~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

std::ostream& operator<<(std::ostream& oss, const RangeParam& rp){
	oss << "clct" << rp.clct << " : ";
	switch(rp.param){
	case 0:
		oss << "BX ";
		break;
	case 1:
		oss << "KEY";
		break;
	case 2:
		oss << "PID";
		break;
	case 3:
		oss << "NHIT";
		break;
	defalut:
		oss << "INVALID";
		break;
	}

	oss <<" Range [" << rp.min << " , " << rp.max << "] Step Size : " << rp.step_size;
	return oss;
}

std::ostream& operator<<(std::ostream& oss, const RangeParamCCLUT& rp){
	oss << "cclut" << rp.cclut << " : ";
	switch(rp.param){
	case 0:
		oss << "BX ";
		break;
	case 1:
		oss << "KEY";
		break;
	case 2:
		oss << "PID";
		break;
	case 3:
		oss << "CC_CODE";
		break;
	defalut:
		oss << "INVALID";
		break;
	}

	oss <<" Range [" << rp.min << " , " << rp.max << "] Step Size : " << rp.step_size;
	return oss;
}


bool Increment(std::vector<CLCT>& muons, std::vector<RangeParam>& free_pars){
	int ind = 0;
	while(true){

		int max_val = free_pars[ind].max;
		int curr_val = muons[free_pars[ind].clct].GetParam(free_pars[ind].param);

		cout << "Current Val = " << curr_val << endl;
		cout << "Maximum Val = " << max_val << endl;
		cout << "clct" << free_pars[ind].clct << "Parameter : " << free_pars[ind].param << endl;
		if(curr_val < max_val){
			muons[free_pars[ind].clct].SetParam(free_pars[ind].param, (curr_val + free_pars[ind].step_size) );	// Increment value
			muons[free_pars[ind].clct].RegenHits();									// Regnerate Triad Data
			return true;												// Continue Stepping
		}
		else if( (curr_val >= max_val) && (ind != (free_pars.size()-1)) ){	// reach max value AND not the last parameter
			muons[free_pars[ind].clct].SetParam(free_pars[ind].param, free_pars[ind].min );
			++ind;
		}
		else{
			return false;
		}

	}
	return false;
}



bool Increment(std::vector<CCLUT>& muons, std::vector<RangeParamCCLUT>& free_pars){
	int ind = 0;
	while(true){

		int max_val = free_pars[ind].max;
		int curr_val = muons[free_pars[ind].cclut].GetParam(free_pars[ind].param);

		cout << "Current Val = " << curr_val << endl;
		cout << "Maximum Val = " << max_val << endl;
		cout << "cclut" << free_pars[ind].cclut << "Parameter : " << free_pars[ind].param << endl;
		if(curr_val < max_val){
			muons[free_pars[ind].cclut].SetParam(free_pars[ind].param, (curr_val + free_pars[ind].step_size) );	// Increment value
			muons[free_pars[ind].cclut].RegenHits();									// Regnerate Triad Data
			return true;												// Continue Stepping
		}
		else if( (curr_val >= max_val) && (ind != (free_pars.size()-1)) ){	// reach max value AND not the last parameter
			muons[free_pars[ind].cclut].SetParam(free_pars[ind].param, free_pars[ind].min );
			++ind;
		}
		else{
			return false;
		}

	}
	return false;
}

bool Increment_new(std::vector<std::vector<int> > parameters, std::vector<std::vector<int> > &CombinationCounter){
	    std::cout << "We are in increment" << '\n';
	    for (int i = 0; i < CombinationCounter.size(); i++) {
	      std::cout << parameters[i][CombinationCounter[i][0]] << ' ';
	    }
	    std::cout << '\n';
	    CombinationCounter[CombinationCounter.size()-1][0] += 1;
	    for (int i = CombinationCounter.size()-1; i > 0; i--) {
	      if(CombinationCounter[i][0] == CombinationCounter[i][1]){
	        CombinationCounter[i-1][0] += 1;
	        CombinationCounter[i][0] = 0;
	      }
	    }
	  if (CombinationCounter[0][0] == CombinationCounter[0][1]) return 0;
	  else return 1;
}
bool DoesScanParamersSizeBad(std::vector<std::vector<std::vector<int> > > parameters){
  if(parameters.size() >= 2){
		for (size_t i = 0; i < parameters[0].size(); i++) {
			for (size_t j = 0; j < parameters.size(); j++) {
				if(parameters[j][i].size() != parameters[0][i].size()) return 1;
			}
		}
	}
	return 0;
}
bool Shold_we_Swap(std::vector<CCLUT> Send_0, std::vector<CCLUT> Send_1, std::vector<CCLUT> &Read_0, std::vector<CCLUT> &Read_1){
	std::vector<std::vector<int> > VisualMapSend_temp;
	std::vector<std::vector<int> > VisualMapRead_temp;
	Plot_And_Compare_Hits(Send_0[0],VisualMapSend_temp);
	Plot_And_Compare_Hits(Read_1[0],VisualMapRead_temp);
	if(abs(Send_0[0].bx - Read_1[0].bx) < 4 || abs(Send_1[0].bx - Read_0[0].bx) < 4){
		Read_0.swap(Read_1);
		return 1;
	}
	return 0;
}


namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

void Print_run2(CCLUT &Send, std::vector<CCLUT> &Read, CCLUT Expect, int n_tries, std::fstream &fout, bool total ) {

	int n_out = 1;
	if(!total) n_out = Read.size();

  for (size_t k = 0; k < n_out; k++) {
		std::vector<std::vector<std::string> > output;
		std::vector<std::string> out_line0, out_line1, out_line2, out_line3, out_line4, collum;
		out_line0.push_back("                ");
		out_line0.push_back("         cc code");
		out_line0.push_back("        eightres");
		out_line0.push_back("         bending");
		out_line0.push_back("              lr");
		out_line0.push_back("             key");

		output.push_back(out_line0);

		collum.push_back("            Send");
		collum.push_back("          Expect");
		collum.push_back("            Read");
		collum.push_back("           Fails");


	// Filling second line
		out_line1.push_back(collum[0]);
		for (size_t i = 0; i < out_line0.size() - 1; i++) {
			out_line1.push_back(out_line0[0]);
		}
		std::string tmp_string = patch::to_string(Send.LUT_code);
		out_line1[1].replace(out_line1[1].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Send.eightres);
		out_line1[2].replace(out_line1[2].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Send.bending);
		out_line1[3].replace(out_line1[3].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Send.lr);
		out_line1[4].replace(out_line1[4].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Send.hs);
		out_line1[5].replace(out_line1[5].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		std::vector<std::string> binary_rep;
		int cc = Send.LUT_code;
		for (int i = 0; i < 6; i++) {
			 if (cc % 4 == 0) binary_rep.push_back(" 00");
			 else if (cc % 4 == 1) binary_rep.push_back(" 01");
			 else if (cc % 4 == 2) binary_rep.push_back(" 10");
			 else if (cc % 4 == 3) binary_rep.push_back(" 11");
			 cc /= 4;
		}

		out_line1.push_back(" |(");
		for (int i = binary_rep.size()-1; i >=0; i--) {
			out_line1.push_back(binary_rep[i]);
		}
		out_line1.push_back(")");


		output.push_back(out_line1);


		//line 2 expect

		out_line2.push_back(collum[1]);
		for (size_t i = 0; i < out_line0.size() - 1; i++) {
			out_line2.push_back(out_line0[0]);
		}
		tmp_string = patch::to_string(Expect.LUT_code);
		out_line2[1].replace(out_line2[1].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Expect.eightres);
		out_line2[2].replace(out_line2[2].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Expect.bending);
		out_line2[3].replace(out_line2[3].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Expect.lr);
		out_line2[4].replace(out_line2[4].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Expect.hs);
		out_line2[5].replace(out_line2[5].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		binary_rep.clear();
		cc = Expect.LUT_code;
		for (int i = 0; i < 6; i++) {
			 if (cc % 4 == 0) binary_rep.push_back(" 00");
			 else if (cc % 4 == 1) binary_rep.push_back(" 01");
			 else if (cc % 4 == 2) binary_rep.push_back(" 10");
			 else if (cc % 4 == 3) binary_rep.push_back(" 11");
			 cc /= 4;
		}

		out_line2.push_back(" |(");
		for (int i = binary_rep.size()-1; i >=0; i--) {
			out_line2.push_back(binary_rep[i]);
		}
		out_line2.push_back(")");

		output.push_back(out_line2);



	 	//line 3 read
		out_line3.push_back(collum[2]);
		for (size_t i = 0; i < out_line0.size() - 1; i++) {
			out_line3.push_back(out_line0[0]);
		}
		tmp_string = patch::to_string(Read[k].LUT_code);
		out_line3[1].replace(out_line3[1].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Read[k].eightres);
		out_line3[2].replace(out_line3[2].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Read[k].bending);
		out_line3[3].replace(out_line3[3].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Read[k].lr);
		out_line3[4].replace(out_line3[4].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		tmp_string = patch::to_string(Read[k].hs);
		out_line3[5].replace(out_line3[5].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
		tmp_string.clear();

		binary_rep.clear();
		cc = Read[k].LUT_code;
		for (int i = 0; i < 6; i++) {
			 if (cc % 4 == 0) binary_rep.push_back(" 00");
			 else if (cc % 4 == 1) binary_rep.push_back(" 01");
			 else if (cc % 4 == 2) binary_rep.push_back(" 10");
			 else if (cc % 4 == 3) binary_rep.push_back(" 11");
			 cc /= 4;
		}

		out_line3.push_back(" |(");
		for (int i = binary_rep.size()-1; i >=0; i--) {
			out_line3.push_back(binary_rep[i]);
		}
		out_line3.push_back(")");

		output.push_back(out_line3);

		if (!total) {
			out_line4.push_back("--------------");
			for (size_t i = 0; i < out_line0.size() - 1; i++) {
				out_line4.push_back("--------------");
			}
			output.push_back(out_line4);
		}
		else{
			out_line4.push_back(collum[3]);
			for (size_t i = 0; i < out_line0.size() - 1; i++) {
				out_line4.push_back(out_line0[0]);
			}
		}



		if (total) {
			CCLUT Miss_match;
			for (size_t i = 0; i < Read.size(); i++) {
				for (size_t j = 0; j < 7; j++) {
					if(Read[i].GetParam(j) != Expect.GetParam(j)) Miss_match.SetParam(j, Miss_match.GetParam(j) + 1);
				}
			}
			tmp_string = patch::to_string(Miss_match.LUT_code);
			tmp_string.append("/");
			tmp_string.append(patch::to_string(n_tries));
			out_line4[1].replace(out_line4[1].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
			tmp_string.clear();

			tmp_string = patch::to_string(Miss_match.eightres);
			tmp_string.append("/");
			tmp_string.append(patch::to_string(n_tries));
			out_line4[2].replace(out_line4[2].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
			tmp_string.clear();

			tmp_string = patch::to_string(Miss_match.bending);
			tmp_string.append("/");
			tmp_string.append(patch::to_string(n_tries));
			out_line4[3].replace(out_line4[3].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
			tmp_string.clear();

			tmp_string = patch::to_string(Miss_match.lr);
			tmp_string.append("/");
			tmp_string.append(patch::to_string(n_tries));
			out_line4[4].replace(out_line4[4].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
			tmp_string.clear();

			tmp_string = patch::to_string(Miss_match.hs);
			tmp_string.append("/");
			tmp_string.append(patch::to_string(n_tries));
			out_line4[5].replace(out_line4[5].size() - 1 - tmp_string.size(), tmp_string.size(), tmp_string);
			tmp_string.clear();

			output.push_back(out_line4);
		}


		for (size_t i = 0; i < output.size(); i++) {
			for (size_t j = 0; j < output[i].size(); j++) {
				fout << output[i][j];
			}
			fout << '\n';
		}

		std::vector<std::vector<int> > VisualMapExpect;
		std::vector<std::vector<int> > VisualMapRead;
		cw::Plot_And_Compare_Hits(Expect, VisualMapExpect);
		cw::Plot_And_Compare_Hits(Read[k],VisualMapRead);
		for (int i = 0; i < VisualMapExpect.size(); i++) {
			for (int j = 0; j < VisualMapExpect[i].size(); j++) {
				if(VisualMapExpect[i][j] == 0) fout << "~";
				else if(VisualMapExpect[i][j] == 1) fout << "X";
				else fout << "?";
			}
			fout << "        " ;
			for (int j = 0; j < VisualMapRead[i].size(); j++) {
				if(VisualMapRead[i][j] == 0) fout << "~";
				else if(VisualMapRead[i][j] == 1) fout << "X";
				else fout << "?";
			}
			fout << std::endl;
		}
  }
}

bool Compare_run2(CCLUT Send, CCLUT Read, CCLUT Expect){
	bool fail = 0;
		if(Read.LUT_code != Expect.LUT_code){
			fail = 1;
		}
		if(Read.bending != Expect.bending){
			fail = 1;
		}
		if(Read.lr != Expect.lr){
			fail = 1;
		}
		if(Read.eightres != Expect.eightres){
			fail = 1;
		}
	return fail;
}



bool Shold_we_Swap_run2(CCLUT &Expect_0, CCLUT &Expect_1, CCLUT Read_0, CCLUT Read_1){
	bool shold_swap = 0;
	if(abs(Expect_0.eightres - Read_1.eightres) < 4 || abs(Expect_1.eightres - Read_0.eightres) < 4){
		CCLUT tmp(Expect_0) ;
		Expect_0 = Expect_1;
		Expect_1 = tmp;
		shold_swap =  1;
	}
	return shold_swap;
}

/*
std::istream& operator>>(std::istream& iss, TMBresponse_long& tmbr_i){
        char tmp;
        int  clct_0, clct_1, delta_lct, n_occr;
        iss >> tmp >> clct_0 >> clct_1 >> delta_lct >> tmp >> n_occr >> tmp;
        tmbr_i = TMBresponse_long(clct_0, clct_1, delta_lct, n_occr);
        return iss;
}
*/

//~~~~~~~~~ Overall Sets ~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Set::Set(void){
	for(int i=0; i < 5; i++){
		Stats.push_back(0);
	}
	//Stats = {0, 0, 0, 0, 0};
	N_trials = 0;
	//Tmb_type = COMPILE_TYPE - 0xa + 'a';
	Tmb_type = 'c';//default
	Firmware = "XXX";
	Prefix = "default";
}

Set::Set(std::string& path)
{
	for(int i=0; i < 5; i++){
		Stats.push_back(0);
	}
	//Stats = {0, 0, 0, 0, 0};
	N_trials = 0;
	//Tmb_type = COMPILE_TYPE - 0xa + 'a';
	Tmb_type = 'c';//default
	Prefix = ReadTxt(path, CSC, GEM);
  sort(CSC.begin(), CSC.end(), compareCLCT);
}

bool Set::AddCLCT(CLCT clct_in)
{
	CSC.push_back(clct_in);
	return true;
}
bool Set::AddGEM(Cluster clust_in)
{
	GEM.push_back(clust_in);
	return true;
}
bool Set::AddCCLUT(CCLUT lut_in)
{
	LUT.push_back(lut_in);
	return true;
}

bool Set::RemoveCLCT(int i)
{
	if(i >=0 && i < CSC.size()){ return false; }
	CSC.erase(CSC.begin() + i);
	return true;
}
bool Set::RemoveGEM(int i)
{
	if(i>=0 && i < GEM.size()){ return false; }
	GEM.erase(GEM.begin() + i);
	return true;
}
bool Set::RemoveCCLUT(int i)
{
	if(i>=0 && i < LUT.size()){ return false; }
	LUT.erase(LUT.begin() + i);
	return true;
}


void Set::Clear(void)
{
	CSC.clear();
	GEM.clear();
	LUT.clear();
	Results.clear();
	Results_l.clear();
	Stats.clear();
	return;
}

bool Set::WritePatterns(std::string opt_path)
{
	std::cout << "Wrong pattern tool  *************************************************************************************************" << endl;
	std::string full_path;
	if(opt_path.empty()){
		full_path = default_dir + Prefix;
	}
	else{
		full_path = opt_path + Prefix;
	}
	return (WritePat(full_path, CSC)) && (WritePat(full_path, GEM));
}
bool Set::WritePatterns_LUT(std::string opt_path)
{
	std::cout << "Correct pattern tool  *************************************************************************************************" << endl;
	std::string full_path;
	if(opt_path.empty()){
		full_path = default_dir + Prefix;
	}
	else{
		full_path = opt_path + Prefix;
	}
	return (WritePat(full_path, LUT)) && (WritePat(full_path, GEM));

/*
	if (CSC.size() == 0 and ComparatorHit.size() > 0 )                     //Need futher analusis to add this
		return (WritePat(full_path, ComparatorHit)) && (WritePat(full_path, GEM));
	else
		return (WritePat(full_path, CSC)) && (WritePat(full_path, GEM));
	*/
}


bool Set::LoadEmuBoard(std::string opt_path, int Mute)
{
	std::cout << Mute << "************************* Is it Muted *****************" << std::endl;
	if(opt_path.empty()) opt_path = default_dir;
	std::cout << " send on cmslab1, path "<< opt_path << " tmb type "<< Tmb_type << std::endl;
	char block[RAMPAGE_SIZE];
	std::vector<FILE*> pat_files;
	pat_files.clear();
	for (int i = 0; i < CSCConstants::NUM_DCFEBS; i++)
        {
        	std::stringstream ss;
        	ss << opt_path << Prefix << "_cfeb" << i << "_tmb" << Tmb_type << ".pat";
          pat_files.push_back( fopen(ss.str().c_str(), "r") );
					if (pat_files.back() != NULL)
					{
						if (Mute == 0) std::cout << "Opening File: " << ss.str() << std::endl;
				  }
					else
					{
						if (Mute == 0)std::cout << "==== Error!!====, Failed to open File: " << ss.str() << std::endl;
					}
         	//oss.push_back(new std::fstream(ss.str().c_str(), std::ios_base::out) );
        }
	for (int i=0; i < GEM_FIBERS; i++)
				{
					std::stringstream ss;
					ss << opt_path << Prefix << "_GEM" << i << ".pat"; // no type is required for GEM
					pat_files.push_back( fopen(ss.str().c_str(), "r") );
					if (pat_files.back() != NULL)
					{
						if (Mute == 0) std::cout << "Opening File: " << ss.str() << std::endl;
					}
					else
					{
						if (Mute == 0) std::cout << "==== Error!!====, Failed to open File: " << ss.str() << std::endl;
					}


				}
	// Open device (i.e. Emulator Board)
	if(eth_open_device() == -1) {
		std::cout<<"Returning without closing files properly"<<std::endl;
		/*
		for (int i = 0; i < pat_files.size(); ++i) {
			fclose(pat_files[i]);
		}
		*/
		return false;

	}

	eth_reset();

	for (int i=0; i < pat_files.size(); i++)
	{
		if (Mute == 0) std::cout << "pageid = " << patFile_to_pageID[i] << std::endl;
		fread(block, sizeof(char), RAMPAGE_SIZE, pat_files[i]);
		memcpy(wdat,block,RAMPAGE_SIZE);
		if (Mute == 0) std::cout << " block = " << block << endl;

		int e = write_command(7,patFile_to_pageID[i], block, Mute);
		if (Mute == 0) std::cout << "write command 7 status = " << e << std::endl;

		char* pkt;
		e = read_command(&pkt, Mute);
		if (Mute == 0) {std::cout << "read command 7 status = " << e << std::endl;}
		if(false){
		char tmp[2];
		tmp[0] = patFile_to_pageID[i] & 0x00ff;
		tmp[1] = (patFile_to_pageID[i] & 0xff00)>>8;
		if (Mute == 0) std::cout << "DEBUG[commands.cpp] send_RAMPage, compare address bytes: "<<std::endl;
		if (Mute == 0) std::cout << HEX(tmp[0])<<" "<<HEX(pkt[2]&0xff)<<std::endl;
		if (Mute == 0) std::cout << HEX(tmp[1])<<" "<<HEX(pkt[3]&0xff - 0xd0)<<std::endl;
		if (Mute == 0) std::cout << DEC();
		}

		fclose(pat_files[i]);

	}

	pat_files.clear();




	eth_close();

	return true;		// ****************************************************
}

void Set::ReadEmuBoard()
{
	return;
}

void Set::DeleteCurrentSet()
{
	for (int i = 0; i < CSCConstants::NUM_DCFEBS; i++)
        {
        	std::stringstream ss;
                ss << default_dir << Prefix << "_cfeb" << i << "_tmb" << Tmb_type << ".pat";
                //pat_files.push_back( fopen(ss.str().c_str(), "r") );
                if(std::remove(ss.str().c_str())) std::cout << "Deleting File: " << ss.str() << std::endl;
	}
	for (int i=0; i < GEM_FIBERS; i++)
        {
                std::stringstream ss;
                ss << default_dir << Prefix << "_GEM"<<i<< "_tmb" << Tmb_type << ".pat";
                //pat_files.push_back( fopen(ss.str().c_str(), "r") );
                if(std::remove(ss.str().c_str())) std::cout << "Deleting File: " << ss.str() << std::endl;
        }


	return;
}

void Set::ClearEmuBoard(){
	return;
}


void Set::Dump(int Mute, char opt)
{
	char mode = 0x0;
	if(opt == 'c'){
		mode = 0xc;
	}
	else{
		mode = 0x0;
	}

	// open Device
	eth_open_device();
	eth_reset();

	int e = write_command(0xd, Mute);
	if (Mute == 0) std::cout << "write command d status = " << e << std::endl;

	char* pkt;
	e = read_command(&pkt, Mute);
	if (Mute == 0) cout << "read command d status = " << e << std::endl;

	if (Mute == 0) dumphex(e, pkt, &std::cout);

	eth_close();

	return;					// ****************************************************
}

void Set::ViewContents(std::ostream& oss)
{
  oss << "******************************************************\n"
            << "                   Muon Tracks in Set\n"
            << "******************************************************\n";
  for(int i=0; i < CSC.size(); i++){
    oss << '\n';
    PrintCLCT(CSC[i],oss,false);
  }

	oss << "\n" << "LUT DEFINED CLCT's::" << "\n";
	for(int i=0; i < LUT.size(); i++){
    oss << '\n';
    PrintCCLUT(LUT[i],oss,false);
  }

  oss << "\n******************************************************\n"
            << "******************************************************\n";
  return;
}

void Set::SaveResults(std::string opt_path)
{
	if(opt_path.empty()){
		opt_path = Prefix + ".log";
	}

	std::fstream ofs(opt_path.c_str(), std::ios_base::out);

	// Header
	{	// Record Time of Experiment
		time_t rawtime;
		struct tm * timeinfo;
		time( &rawtime );
		timeinfo = localtime( &rawtime );

		ofs << "Date: " << asctime(timeinfo);
		ofs << "TMB Compile Type: " << Tmb_type << std::endl;
		ofs << "Firmware Vers.: " << Firmware << std::endl << std::endl;
	}

	ofs << "# Trials  : \t\t" << N_trials << std::endl;
	ofs << "# Success : \t\t" << Stats[0] << std::endl;
	ofs << "# Errors  : \t\t" << N_trials - Stats[0] << std::endl << std::endl;

	int N_err = 0;
	for(int i=1; i < Stats.size(); i++){
		ofs << "Error " << i << ": " << Stats[i] << std::endl;
		N_err += Stats[i];
	}

	if(N_err != (N_trials - Stats[0])){
		ofs << "\nSomething Went Wrong!\nCorrect # Errors = " << N_err << std::endl;
	}

	ofs << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	ofs << "~~~~~~~~~~Original Pattern Set~~~~~~~~~~~~~~~~\n";
	ofs << "prefix: " << Prefix << std::endl;
	ofs << "\nCSC\nbx\tkey\tpid\tNhit\t\t{ (bx,ly,hs) }\n";

	for(int i=0; i < CSC.size(); i++){
    		CSC[i].RelativeSpaceTime();
		ofs << CSC[i] << std::endl;
	}

	ofs << "\nGEM\nbx\troll\tpad\tsize\tlayer\n";
	for(int i=0; i < GEM.size(); i++){
		ofs << GEM[i] << std::endl;
	}

	ofs << "\nLUT\nbx\tkey\tpid\tCCODE\n";
	for(int i=0; i < LUT.size(); i++){
		LUT[i].RelativeSpaceTime();
		ofs << LUT[i] << std::endl;
	}

	ofs.close();
	return;
}

std::ostream& operator<<(std::ostream& oss, const Set& set){

	for(int i=0; i < set.CSC.size(); i++){
		oss << "<C " << set.CSC[i] << ">";	//	C ==> clct
	}                       //Need further investigation
	/*
	for(unsigned int i=0; i < set.ComparatorHit.size(); i++){
		oss << "<H " << set.ComparatorHit[i] << ">";	//	H ==> Hit
	}
	*/
	for(int i=0; i < set.GEM.size(); i++){		//	G ==> gem cluster
		oss << "<G " << set.GEM[i] << ">";
	}
	for(int i=0; i < set.LUT.size(); i++){		//	G ==> gem cluster
		oss << "<G " << set.LUT[i] << ">";
	}
	for(int i=0; i < set.Results.size(); i++){	//	R ==> Expr. Results
		oss << "<R-" << i << " " << set.Results[i] << ">";
	}
	return oss;
}

}

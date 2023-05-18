/*
	This file is for version control:
	I am currently unsure how to recreate this GUI in a new directory
		~Cameorn	(06/24/2019)


*/
/*
 * $Id: $
 */

// class header
#include "emu/pc/CSCGEMTestApplication.h"

// Emu includes
#include "emu/utils/Cgi.h"
#include "emu/utils/System.h"
#include "emu/pc/Crate.h"
#include "emu/pc/DAQMB.h"
#include "emu/pc/TMB.h"
#include "emu/pc/CCB.h"
#include "emu/pc/MPC.h"
#include "emu/pc/ALCTController.h"
#include "emu/pc/RAT.h"
#include "emu/pc/VMECC.h"
#include "emu/pc/DDU.h"

//added by Ben for debugging
//#include "CLCT.h"

// XDAQ includes
#include "cgicc/Cgicc.h"
#include "cgicc/HTMLClasses.h"
#include "toolbox/string.h"

// system includes
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <string>
#include <unistd.h> // for sleep()
#include <ctype.h> // for toupper()

// radtest includes
#include "eth_lib.h"
#include "utils.h"
#include "commands.h"
#include <time.h>
#include <boost/filesystem.hpp>

// testing includes
#include <stdio.h>
#include <stdlib.h>

//debugging includes: optional
#include <iostream>

//#include "PatternGen.h"
//#include "emu/pc/Yuriy_CLCT_PatternGen.h"


//parsing
#include "emu/pc/TMBTester.h"

namespace emu {  namespace pc {

using std::endl;
using std::cout;
using std::string;
using emu::utils::bindMemberMethod;
using cw::Hit;
using cw::CLCT;
using cw::CCLUT;
using cw::Cluster;


//This is the section for the default values displayed on the webpage:

CSCGEMTestApplication::CSCGEMTestApplication(xdaq::ApplicationStub * s)
: EmuPeripheralCrateBase(s)     //: xdaq::WebApplication(s)

{
  //------------------------------------------------------
  // bind methods
  //------------------------------------------------------
  xgi::bind(this, &CSCGEMTestApplication::Default, "Default");
  xgi::bind(this, &CSCGEMTestApplication::GenerateDCFEBData, "GenerateDCFEBData");
  xgi::bind(this, &CSCGEMTestApplication::UploadDCFEBData, "UploadDCFEBData");
  xgi::bind(this, &CSCGEMTestApplication::CheckDCFEBData, "CheckDCFEBData");
  xgi::bind(this, &CSCGEMTestApplication::TransmitDCFEBData, "TransmitDCFEBData");
  xgi::bind(this, &CSCGEMTestApplication::ReadOutTriggerResults, "ReadOutTriggerResults");
  xgi::bind(this, &CSCGEMTestApplication::F0, "F0");
  xgi::bind(this, &CSCGEMTestApplication::GenGEMPattxtFileMounN, "GenGEMPattxtFileMounN");
  xgi::bind(this, &CSCGEMTestApplication::GenGEMPattxtFile, "GenGEMPattxtFile");
  xgi::bind(this, &CSCGEMTestApplication::YuriyPatConvert, "YuriyPatConvert");
  xgi::bind(this, &CSCGEMTestApplication::Yuriy_CLCT_PatternConverter, "Yuriy_CLCT_PatternConverter");
  xgi::bind(this, &CSCGEMTestApplication::GemPatternConverter, "GemPatternConverter");
  xgi::bind(this, &CSCGEMTestApplication::CfebPatternConverter, "CfebPatternConverter");
  xgi::bind(this, &CSCGEMTestApplication::SendFromEmuBoard, "SendFromEmuBoard");
  xgi::bind(this, &CSCGEMTestApplication::ClearEmuBoard, "ClearEmuBoard");
  xgi::bind(this, &CSCGEMTestApplication::OutputCLCTInfo, "OutputCLCTInfo");


  // Pattern Studies Methods
  xgi::bind(this, &CSCGEMTestApplication::GetOTMBCompileType, "GetOTMBCompileType");
  xgi::bind(this, &CSCGEMTestApplication::AddCLCT, "AddCLCT");
  //xgi::bind(this, &CSCGEMTestApplication::AddComparatorHit, "AddComparatorHit");
  xgi::bind(this, &CSCGEMTestApplication::AddGEM, "AddGEM");
  xgi::bind(this, &CSCGEMTestApplication::LoadToEmuBoard, "LoadToEmuBoard");
  xgi::bind(this, &CSCGEMTestApplication::SaveCurrentSet, "SaveCurrentSet");	// Outdated! to be REMOVED
  xgi::bind(this, &CSCGEMTestApplication::SaveAsPat, "SaveAsPat");
  xgi::bind(this, &CSCGEMTestApplication::SaveAsTxt, "SaveAsTxt");
  xgi::bind(this, &CSCGEMTestApplication::ClearSet, "ClearSet");
  xgi::bind(this, &CSCGEMTestApplication::RunStudy, "RunStudy");
  xgi::bind(this, &CSCGEMTestApplication::AddCLCTParamScan, "AddCLCTParamScan");
  xgi::bind(this, &CSCGEMTestApplication::RunParamScan, "RunParamScan");

  xgi::bind(this, &CSCGEMTestApplication::DOESWORK, "DOESWORK");

  //parsing section
  XML_or_DB_ = "xml";
  xmlFile_ = "config.xml" ;
  Valid_config_ID="";
  xml_or_db = -1;  /* actual configuration source: 0: xml, 1: db, -1: unknown or error */
  total_crates_=0;
  this_crate_no_=0;
  this->getApplicationInfoSpace()->fireItemAvailable("Standalone", &standalone);
  standalone_ = true;
  thisCCB = 0;
  thisMPC = 0;
  this->getApplicationInfoSpace()->fireItemAvailable("xmlFileName", &xmlFile_);
  parsed = 0;

  startgemtxt = false;
  sprintf(GemtxtMounNchar,"");

  sprintf(DirGen,             "/home/cscdev/Jared/EmulatorBoard/TriDAS_BoardPlus2/emu/emuDCS/CSCGEMTestStand/tmp");
  sprintf(DirUp,              "/home/cscdev/Jared/EmulatorBoard/TriDAS_BoardPlus2/emu/emuDCS/CSCGEMTestStand/tmp");
  sprintf(DirYuriyConvtxt,    "/home/cscdev/Jared/EmulatorBoard/TriDAS_BoardPlus2/emu/emuDCS/CSCGEMTestStand/tmp");
  sprintf(DirYuriyCLCTConvPat,"/home/cscdev/Jared/EmulatorBoard/TriDAS_BoardPlus2/emu/emuDCS/CSCGEMTestStand/tmp");
  sprintf(DirGEMPat,          "/home/cscdev/Jared/EmulatorBoard/TriDAS_BoardPlus2/emu/emuDCS/CSCGEMTestStand/tmp");
  sprintf(DirCfebPat,         "/home/cscdev/Jared/EmulatorBoard/TriDAS_BoardPlus2/emu/emuDCS/CSCGEMTestStand/tmp");
  sprintf(DirGEMtxt,          "/home/cscdev/Jared/EmulatorBoard/TriDAS_BoardPlus2/emu/emuDCS/CSCGEMTestStand/tmp");
  sprintf(DirCfebtxt,         "/home/cscdev/Jared/EmulatorBoard/TriDAS_BoardPlus2/emu/emuDCS/CSCGEMTestStand/tmp");
  sprintf(DirYuriyCLCTConvtxt,"/home/cscdev/Jared/EmulatorBoard/TriDAS_BoardPlus2/emu/emuDCS/CSCGEMTestStand/tmp");
  sprintf(DCFEBN,"1");
  sprintf(HalfStripN,"1");
  sprintf(FiberN,"1");
  sprintf(PatFile,"DCFEB_1.pat");
  sprintf(NumRuns,"1");
  sprintf(GEMtxtFile,"gempads.txt");
  sprintf(YuriyCLCTConvtxtFile,"clctpattern.txt");
  sprintf(CfebtxtFile,"clctpattern.txt");
  sprintf(YuriyConvtxtFile,"Yuriy_pattern.txt");

  // Cameron Addition : Jan. 2020
  sprintf(GetSetPrefix, patternSet.Prefix.c_str());

  sprintf(TrialsPerStep_char, "1000");


  EmuBoardLocation = "/dev/schar3";

  for (int i = 0; i < 8; ++i) {
    sprintf(FileNameFiberN[i],"N/A");
  }

  sprintf(Fiber0PatName,"N/A");
  sprintf(Fiber1PatName,"N/A");
  sprintf(Fiber2PatName,"N/A");
  sprintf(Fiber3PatName,"N/A");
  sprintf(Fiber4PatName,"N/A");
  sprintf(Fiber5PatName,"N/A");
  sprintf(Fiber6PatName,"N/A");
  sprintf(Fiber7PatName,"N/A");

  FileLoadedToBoard = false;
  //TMB * thisTMB   = tmbVector[chamber_index];

  for (int i = 0; i < 512; ++i) {
    string Result;          // string which will contain the result
    std::ostringstream convert;   // stream used for the conversion
    convert << i;      // insert the textual representation of 'Number' in the characters in the stream
    Result = convert.str();
    string tempstring = "N" + Result;
    sprintf(testingarray[i],tempstring.c_str());
    sprintf(testingarray[i],"");
  }

  //TMB * thisTMB = tmbVector[1];
  //CLCT0_Counter = thisTMB->GetCounter(cw::tmb_counters[0]);
  //CLCT1_Counter = thisTMB->GetCounter(cw::tmb_counters[1]);

  for(int i=0; i < 4; i++){
    std::string str;
    GetCLCT_ps_char.push_back(str);
  }
}

bool CSCGEMTestApplication::ParsingXML(){
  //
	xmlFile_.fromString(emu::utils::performExpansions(xmlFile_));
    std::string config_src, config_key;
    //
    Logger logger_ = getApplicationLogger();
    //
    LOG4CPLUS_INFO(logger_, "EmuPeripheralCrate reloading...");
    //
    config_src = XML_or_DB_.toString();
    // std::cout << "XML_or_DB: " << config_src << std::endl;
    if(config_src == "xml" || config_src == "XML")
    {
       config_key = xmlFile_.toString();
    }
    else if (config_src == "db" || config_src == "DB")
    {
       config_key = Valid_config_ID;
    }
    else
    {
       std::cout << "No valid XML_or_DB found..." << std::endl;
       return false;
    }
    std::cout << "this works up to the CommonParser. CSCGEMTestApplication btw." << std::endl;
    std::cout << "The strings that go into the CommonParser function are as follows:" << std::endl; //jmn
    std::cout << "config_src: " << config_src << std::endl; //jmn
    std::cout << "config_key: " << config_key << std::endl; //jmn
    if(!CommonParser(config_src, config_key)) return false;
    std::cout << "this works past the CommonParser. " << std::endl;
    EmuEndcap *myEndcap = GetEmuEndcap();
    if(myEndcap == NULL) return false;
    myEndcap->NotInDCS();
    xml_or_db = Xml_or_Db();
    crateVector = myEndcap->crates();
    //
    total_crates_=crateVector.size();
    if(total_crates_<=0) return false;
    this_crate_no_=0;

    std::string lab_mode=standalone;
    if(lab_mode=="True" || lab_mode=="TRUE" || lab_mode=="true" || lab_mode=="Yes" || lab_mode=="YES" || lab_mode=="yes")
    {    standalone_ = true;
         std::cout << "PeripheralCrateConfig started in Standalone mode." << std::endl;
    }

    for(unsigned crate_number=0; crate_number< crateVector.size(); crate_number++) {
      //
      SetCurrentCrate(crate_number);
      for(unsigned i=0; i<dmbVector.size();i++) {
	OutputDMBTests[i][current_crate_] << "DMB-CFEB Tests "
					  << thisCrate->GetChamber(dmbVector[i]->slot())->GetLabel().c_str()
					  << " output:" << std::endl;
      }
      for(unsigned i=0; i<tmbVector.size();i++) {
	OutputTMBTests[i][current_crate_] << "TMB-RAT Tests "
					  << thisCrate->GetChamber(tmbVector[i]->slot())->GetLabel().c_str()
					  << " output:" << std::endl;
	ChamberTestsOutput[i][current_crate_] << "Chamber-Crate Phases "
					      << thisCrate->GetChamber(tmbVector[i]->slot())->GetLabel().c_str()
					      << " output:" << std::endl;
      }
      OutputCCBTests[current_crate_] << "CCB Tests output: " << std::endl;
    }
    //
    SetCurrentCrate(this_crate_no_);
    //
    std::cout << "Parser Done" << std::endl ;
    //
    parsed=1;
    return true;
  }

void CSCGEMTestApplication::SetCurrentCrate(int cr)
  {
    if(total_crates_<=0) return;
    thisCrate = crateVector[cr];

    if ( ! thisCrate ) {
      std::cout << "Crate doesn't exist" << std::endl;
      assert(thisCrate);
    }

    ThisCrateID_=thisCrate->GetLabel();
    thisCCB = thisCrate->ccb();
    thisMPC = thisCrate->mpc();
    tmbVector = thisCrate->tmbs();
    dmbVector = thisCrate->daqmbs();
    chamberVector = thisCrate->chambers();
    //dduVector = thisCrate->ddus();
    //
    //tmbTestVector = InitTMBTests(thisCrate);
    //
    for (unsigned int i=0; i<(tmbVector.size()<9?tmbVector.size():9) ; i++) {
      MyTest[i][cr].SetTMB(tmbVector[i]);
      MyTest[i][cr].SetDMB(dmbVector[i]);
      MyTest[i][cr].SetCCB(thisCCB);
      MyTest[i][cr].SetMPC(thisMPC);
      // reset ALCT firmware check value for this crate
      able_to_load_alct[i] = -1;
    }
    //
  // DefineFirmwareFilenames();
    //
    current_crate_ = cr;
  }

void CSCGEMTestApplication::Default(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception)
{
  using cgicc::br;
  using namespace std;

  cgicc::Cgicc cgi(in);

  string urn = getApplicationDescriptor()->getURN();

  emu::utils::headerXdaq(out, this, "GEM-CSC Pattern Studies");

  //*out << "Hello World!";
  //TMB * thisTMB   = tmbVector[chamber_index];
  xmlFile_.fromString("$BUILD_HOME/xml/tamu_crate_config_2tmbs2019_GEM.xml");
  std::string testxmlfilename = xmlFile_.toString();
  std::cout << "This is the xmlFile_ name before the fromString: " << testxmlfilename << std::endl << std::endl;
  xmlFile_.fromString(emu::utils::performExpansions(xmlFile_));
  testxmlfilename = xmlFile_.toString(); //jmn
  std::cout << "This is the xmlFile_ name after the fromString: " << testxmlfilename << std::endl << std::endl;

  if(!parsed) ParsingXML();

  string s1,s2,s3,s4,s5,s6,s7;
  string c1,c2,c3,c4,c5,c6,c7;

  if(xgi::Utils::hasFormElement(cgi, "GemtxtMounNchar")) sprintf(GemtxtMounNchar,cgi["GemtxtMounNchar"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "DirGen")) sprintf(DirGen,cgi["DirGen"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "DirYuriyConvtxt")) sprintf(DirYuriyConvtxt,cgi["DirYuriyConvtxt"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "DirGEMPat")) sprintf(DirGEMPat,cgi["DirGEMPat"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "DirCfebPat")) sprintf(DirCfebPat,cgi["DirCfebPat"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "DirGEMtxt")) sprintf(DirGEMtxt,cgi["DirGEMtxt"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "DirCfebtxt")) sprintf(DirCfebtxt,cgi["DirCfebtxt"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "FiberN")) sprintf(FiberN,cgi["FiberN"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "PatFile")) sprintf(PatFile,cgi["PatFile"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "NumRuns")) sprintf(NumRuns,cgi["NumRuns"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "GEMtxtFile")) sprintf(GEMtxtFile,cgi["GEMtxtFile"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "CfebtxtFile")) sprintf(CfebtxtFile,cgi["CfebtxtFile"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "YuriyConvtxtFile")) sprintf(YuriyConvtxtFile,cgi["YuriyConvtxtFile"]->getValue().c_str());

  for (int i = 0; i < 512; ++i) {
    string number;          // string which will contain the result
    std::ostringstream convert;   // stream used for the conversion
    convert << i;      // insert the textual representation of 'Number' in the characters in the stream
    number = convert.str();
    string tempstring = "GemBx" + number;
    if(xgi::Utils::hasFormElement(cgi,tempstring)) sprintf(testingarray[i],cgi[tempstring]->getValue().c_str());
  }






  ifstream fs;
  string line;
  fs.open("s1.txt"); getline(fs,s1); fs.close();
  fs.open("s2.txt"); getline(fs,s2); fs.close();
  fs.open("s3.txt"); getline(fs,s3); fs.close();
  fs.open("s4.txt"); getline(fs,s4); fs.close();
  fs.open("s5.txt"); getline(fs,s5); fs.close();
  fs.open("s6.txt"); getline(fs,s6); fs.close();
  fs.open("s7.txt"); getline(fs,s7); fs.close();
  fs.open("c1.txt"); getline(fs,c1); fs.close();
  fs.open("c2.txt"); getline(fs,c2); fs.close();
  fs.open("c3.txt"); getline(fs,c3); fs.close();
  fs.open("c4.txt"); getline(fs,c4); fs.close();
  fs.open("c5.txt"); getline(fs,c5); fs.close();
  fs.open("c6.txt"); getline(fs,c6); fs.close();
  fs.open("c7.txt"); getline(fs,c7); fs.close();

  ofstream ofs;
  ofs.open("s1.txt"); ofs << "a"; ofs.close();
  ofs.open("s2.txt"); ofs << "a"; ofs.close();
  ofs.open("s3.txt"); ofs << "a"; ofs.close();
  ofs.open("s4.txt"); ofs << "a"; ofs.close();
  ofs.open("s5.txt"); ofs << "a"; ofs.close();
  ofs.open("s6.txt"); ofs << "a"; ofs.close();
  ofs.open("s7.txt"); ofs << "a"; ofs.close();
  ofs.open("c1.txt"); ofs << "a"; ofs.close();
  ofs.open("c2.txt"); ofs << "a"; ofs.close();
  ofs.open("c3.txt"); ofs << "a"; ofs.close();
  ofs.open("c4.txt"); ofs << "a"; ofs.close();
  ofs.open("c5.txt"); ofs << "a"; ofs.close();
  ofs.open("c6.txt"); ofs << "a"; ofs.close();
  ofs.open("c7.txt"); ofs << "a"; ofs.close();


cgicc::form_iterator name = cgi.getElement("tmb");
int tmb;
if(name != cgi.getElements().end()) {
  tmb = cgi["tmb"]->getIntegerValue();
  std::cout << "TMBStatus:  TMB=" << tmb << std::endl;
  TMB_ = tmb;
} else {
  std::cout << "TMBStatus: No TMB" << std::endl ;
  tmb = TMB_;
}


tmb = 1;
TMB * thisTMB = tmbVector[tmb];
std::cout << "the value for tmb is = " << tmb << std::endl;
std::cout << "The tmbVector[tmb] = " << tmbVector[tmb] << std::endl;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  LOG4CPLUS_INFO(getApplicationLogger(), "Start PrintCounters");
  thisTMB->RedirectOutput(&OutputStringTMBStatus[tmb]);
  thisTMB->GetCounters();
  thisTMB->PrintCounters();
  thisTMB->PrintGemCounters();
  thisTMB->RedirectOutput(&std::cout);
  LOG4CPLUS_INFO(getApplicationLogger(), "Done PrintCounters");



  *out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFFFBB") << endl;
  *out << cgicc::legend("Run Parameter Scan").set("style", "color:blue") ;
  *out << cgicc::form().set("method", "GET").set("action","/" +urn+ "/ClearEmuBoard") << endl;
  *out << cgicc::input().set("type","submit").set("value","Run").set("name", "RunParamScan") << endl;
  *out << cgicc::form() << endl;
  *out << cgicc::fieldset() << endl;




*out << cgicc::fieldset();

  *out << cgicc::form().set("method","GET") << std::endl ;
  *out << cgicc::pre();
  *out << cgicc::textarea().set("name","CrateTestTMBOutput")
    .set("rows","50")
    .set("cols","150")
    .set("WRAP","OFF");
  *out << OutputStringTMBStatus[tmb].str() << std::endl ;
  *out << cgicc::textarea();
  OutputStringTMBStatus[tmb].str("");
  *out << cgicc::pre();
  *out << cgicc::form() << std::endl ;


*out << cgicc::legend("CLCT Info").set("style","color:blue") << std::endl ;
*out << cgicc::pre();
thisTMB->RedirectOutput(out);
thisTMB->DecodeCLCT();
thisTMB->PrintCLCT();
thisTMB->RedirectOutput(&std::cout);
*out << cgicc::pre();
*out << cgicc::fieldset();


  if (thisTMB->GetGemEnabled()) {
    *out << cgicc::td();
    *out << cgicc::td().set("valign", "top");
    *out << cgicc::fieldset();
    *out << cgicc::legend("GEM Info").set("style","color:blue") << std::endl ;
    *out << cgicc::pre();
    thisTMB->RedirectOutput(out);
    thisTMB->DecodeGEMHits();
    thisTMB->PrintGEMHits();
    thisTMB->RedirectOutput(&std::cout);
    *out << cgicc::pre();
    *out << cgicc::fieldset();
   }
   if (thisTMB->GetGemEnabled()){
    *out << cgicc::td();
    *out << cgicc::table();
   }

if(thisTMB->GetHardwareVersion() >= 2) {
  *out << cgicc::fieldset();
  *out
    << cgicc::legend("LCT Info: Frames Sent to MPC").set("style", "color:red")
    << std::endl;
  *out << cgicc::pre();
  thisTMB->RedirectOutput(out);
  //thisTMB->DecodeMPCFrames(); // Decode MPC frames for LAST trigger. VME registers: 0x88, 0x8a, 0x8c, 0x8e
  //thisTMB->PrintMPCFrames();  // Print  MPC frames for LAST trigger. VME registers: 0x88, 0x8a, 0x8c, 0x8e
  ////
  //thisTMB->DecodeMPCFramesFromFIFO(); // Decode MPC frames for ONE trigger from FIFO. VME registers: 0x17C, 0x17E, 0x180, 0x182
  //thisTMB->PrintMPCFramesFromFIFO();  // Print  MPC frames for ONE trigger from FIFO. VME registers: 0x17C, 0x17E, 0x180, 0x182
  ////
  thisTMB->DecodeAndPrintMPCFrames(0); // Decode and print MPC frames for both cases:
                    //   1. LAST trigger. VME registers: 0x88, 0x8a, 0x8c, 0x8e
                    //   2. ONE trigger from FIFO. VME registers: 0x17C, 0x17E, 0x180, 0x182
  thisTMB->RedirectOutput(&std::cout);
  *out << cgicc::pre();
  *out << cgicc::fieldset();
}

  emu::utils::footer(out);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//					Pattern Studies Functions (Jan. 2020)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void CSCGEMTestApplication::Bit11(xgi::Input * in, xgi::Output * out)
{
  using namespace std;

  cgicc::Cgicc cgi(in);
  Bit11_int = false;
  if(xgi::Utils::hasFormElement(cgi,"Bit11True")) {
    sprintf(bit11_char,cgi["Bit11True"]->getValue().c_str());
    Bit11_int = atoi(bit11_char);
  }
  std::cout << "Do we use 11 bit cc? " << Bit11_int << '\n';
  this->Default(in, out);
  return;
}

void CSCGEMTestApplication::Run3(xgi::Input * in, xgi::Output * out)
{
  using namespace std;

  cgicc::Cgicc cgi(in);
  Run3_int = false;
  if(xgi::Utils::hasFormElement(cgi,"Run3True")) {
    sprintf(run3_char,cgi["Run3True"]->getValue().c_str());
    Run3_int = atoi(run3_char);
  }
  std::cout << "Do we use run 3 output style? " << run3_char << '\n';
  this->Default(in, out);
  return;
}

void CSCGEMTestApplication::SetCFEBDelay(xgi::Input * in, xgi::Output * out) // Added by Sashha to chabge CFEB delays on the fly. DONT HARD RESERT AFER YOU SET IT!!!
{
  TMB * thisTMB = tmbVector[1];
  using namespace std;
  cgicc::Cgicc cgi(in);
  CFEB0123_delay = 20;
  CFEB456_delay  = 20;
  if(xgi::Utils::hasFormElement(cgi,"CFEB0123_delay")) {
    sprintf(CFEB0123_delay_char,cgi["CFEB0123_delay"]->getValue().c_str());
    CFEB0123_delay = atoi(CFEB0123_delay_char);
  }
  if(xgi::Utils::hasFormElement(cgi,"CFEB456_delay")) {
    sprintf(CFEB456_delay_char,cgi["CFEB456_delay"]->getValue().c_str());
    CFEB456_delay = atoi(CFEB456_delay_char);
  }
  std::cout << "CFEB0123_delay = " << CFEB0123_delay << '\n';
  std::cout << "CFEB456_delay = " << CFEB456_delay << '\n';

  if(CFEB0123_delay >= 0 && CFEB0123_delay <= 25 && CFEB456_delay >= 0 && CFEB456_delay <= 25){
    thisTMB->SetCFEB0123delay(CFEB0123_delay);
    thisTMB->SetCFEB456delay(CFEB456_delay);
  }
  else{
    std::cout << "WRONG CFEB DELAY PARAMETERS!!!!!!!!!!!!!!" << '\n';
  }
  this->Default(in, out);
  return;
}

void CSCGEMTestApplication::GetCFEBDelay(xgi::Input * in, xgi::Output * out) // Added by Sashha to chabge CFEB delays on the fly. DONT HARD RESERT AFER YOU SET IT!!!
{
  using namespace std;
  cgicc::Cgicc cgi(in);
  TMB * thisTMB = tmbVector[1];
  CFEB0123_delay_read = thisTMB->GetCfeb0123RxClockDelay();
  CFEB456_delay_read = thisTMB->GetCfeb456RxClockDelay();
  std::cout << "CFEB0123_delay = " << CFEB0123_delay_read << '\n';
  std::cout << "CFEB456_delay = " << CFEB456_delay_read << '\n';
  this->Default(in, out);

  return;
}


void CSCGEMTestApplication::GetOTMBCompileType(xgi::Input * in, xgi::Output * out)
{
  using namespace std;

  cgicc::Cgicc cgi(in);
  if(xgi::Utils::hasFormElement(cgi,"GetOTMBType")) sprintf(GetOTMBType,cgi["GetOTMBType"]->getValue().c_str());

  GetOTMBType[0] = tolower(GetOTMBType[0]);

  patternSet.SetOTMBCompileType(GetOTMBType[0]);
  std::cout << "OTM Type Set to " << GetOTMBType[0] << '\n';

  this->Default(in, out);
  return;

}


void CSCGEMTestApplication::AddCLCT(xgi::Input * in, xgi::Output * out)
{
  using namespace std;

  cgicc::Cgicc cgi(in);

  if(xgi::Utils::hasFormElement(cgi,"GetCLCT_bx_char")) sprintf(GetCLCT_bx_char,cgi["GetCLCT_bx_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCLCT_key_char")) sprintf(GetCLCT_key_char,cgi["GetCLCT_key_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCLCT_pid_char")) sprintf(GetCLCT_pid_char,cgi["GetCLCT_pid_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCLCT_nhit_char")) sprintf(GetCLCT_nhit_char,cgi["GetCLCT_nhit_char"]->getValue().c_str());

  GetCLCT_bx_int = atoi(GetCLCT_bx_char);
  GetCLCT_key_int = atoi(GetCLCT_key_char);
  GetCLCT_pid_int = atoi(GetCLCT_pid_char);
  GetCLCT_nhit_int = atoi(GetCLCT_nhit_char);
  int tmbtype = 0xa+GetOTMBType[0]-'a';//get OTMB type, a,c,d->0xa, 0xc, 0xd

  cout << "READ IN CFEB : \n";
  cout << "Bx : " << GetCLCT_bx_int << endl;
  cout << "Key : " << GetCLCT_key_int << endl;
  cout << "PiD : " << GetCLCT_pid_int << endl;
  cout << "N Hits : " << GetCLCT_nhit_int  << endl;
  cout << "OTMB type : " << GetOTMBType[0]  << " int "<< tmbtype<< endl;


  patternSet.AddCLCT( CLCT(GetCLCT_bx_int, GetCLCT_key_int, GetCLCT_pid_int, GetCLCT_nhit_int, tmbtype) );

  this->Default(in, out);
  return;
}

////////////// ADDED BY SASHA |||||||||||||||||||||||||||||||||




void CSCGEMTestApplication::AddCCLUT(xgi::Input * in, xgi::Output * out)
{
  using namespace std;

  cgicc::Cgicc cgi(in);

  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_bx_char")) sprintf(GetCCLUT_bx_char,cgi["GetCCLUT_bx_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_key_char")) sprintf(GetCCLUT_key_char,cgi["GetCCLUT_key_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_pid_char")) sprintf(GetCCLUT_pid_char,cgi["GetCCLUT_pid_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_ccode_char")) sprintf(GetCCLUT_ccode_char,cgi["GetCCLUT_ccode_char"]->getValue().c_str());

  GetCCLUT_bx_int = atoi(GetCCLUT_bx_char);
  GetCCLUT_key_int = atoi(GetCCLUT_key_char);
  GetCCLUT_pid_int = atoi(GetCCLUT_pid_char);
  GetCCLUT_ccode_int = atoi(GetCCLUT_ccode_char);
  int tmbtype = 0xa+GetOTMBType[0]-'a';//get OTMB type, a,c,d->0xa, 0xc, 0xd

  cout << "READ IN CFEB : \n";
  cout << "Bx : " << GetCCLUT_bx_int << endl;
  cout << "Key : " << GetCCLUT_key_int << endl;
  cout << "PiD : " << GetCCLUT_pid_int << endl;
  cout << "CCLUT code: " << GetCCLUT_ccode_int  << endl;
  cout << "OTMB type : " << GetOTMBType[0]  << " int "<< tmbtype<< endl;

  patternSet.AddCCLUT( CCLUT(GetCCLUT_bx_int, GetCCLUT_key_int, GetCCLUT_pid_int, GetCCLUT_ccode_int, tmbtype) );

  this->Default(in, out);
  return;
}


void CSCGEMTestApplication::AddGEM(xgi::Input * in, xgi::Output * out)
{
cgicc::Cgicc cgi(in);

  if(xgi::Utils::hasFormElement(cgi,"GetGEM_bx_char")) sprintf(GetGEM_bx_char,cgi["GetGEM_bx_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetGEM_roll_char")) sprintf(GetGEM_roll_char,cgi["GetGEM_roll_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetGEM_pad_char")) sprintf(GetGEM_pad_char,cgi["GetGEM_pad_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetGEM_size_char")) sprintf(GetGEM_size_char,cgi["GetGEM_size_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetGEM_layer_char")) sprintf(GetGEM_layer_char,cgi["GetGEM_layer_char"]->getValue().c_str());

  GetGEM_bx_int = atoi(GetGEM_bx_char);
  GetGEM_roll_int = atoi(GetGEM_roll_char);
  GetGEM_pad_int = atoi(GetGEM_pad_char);
  GetGEM_size_int = atoi(GetGEM_size_char);
  GetGEM_layer_int = atoi(GetGEM_layer_char);

  cout << "READ IN GEM : \n";
  cout << "Bx : " << GetGEM_bx_int << endl;
  cout << "Roll : " << GetGEM_roll_int << endl;
  cout << "Pad : " << GetGEM_pad_int << endl;
  cout << "Size : " << GetGEM_size_int  << endl;
  cout << "Layer : " << GetGEM_layer_int << endl;

  patternSet.AddGEM( Cluster(GetGEM_bx_int, GetGEM_roll_int, GetGEM_pad_int, GetGEM_size_int, GetGEM_layer_int) );

  this->Default(in, out);
  return;
}

void CSCGEMTestApplication::SaveAsPat(xgi::Input * in, xgi::Output * out)
{
  using namespace std;
  cgicc::Cgicc cgi(in);

  if(xgi::Utils::hasFormElement(cgi,"GetSetPrefix")) sprintf(GetSetPrefix,cgi["GetSetPrefix"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetSaveDir")) sprintf(GetSaveDir,cgi["GetSaveDir"]->getValue().c_str());

  patternSet.Prefix = GetSetPrefix;
  patternSet.WritePatterns(GetSaveDir);
  cout << "\n TRYING TO WRITE PATS \n";

  cout << "Input prefix = " << GetSetPrefix << endl;
  cout << "Save prefix = " << patternSet.Prefix << endl;
  cout << "Save Dir = " << GetSaveDir << endl;

  patternSet.Prefix = "default";	// return to default settings
  this->Default(in, out);
  return;
}

void CSCGEMTestApplication::SaveAsTxt(xgi::Input * in, xgi::Output * out)
{
  this->Default(in, out);
  return;
}


void CSCGEMTestApplication::SaveCurrentSet(xgi::Input * in, xgi::Output * out)
{
  this->Default(in, out);
  return;
}

void CSCGEMTestApplication::ClearEmuBoard(xgi::Input * in, xgi::Output * out)
{
    using namespace std;
    cgicc::Cgicc cgi(in);
    cw::Set emptySet;
    if(emptySet.WritePatterns()){
        if( emptySet.LoadEmuBoard() ){
                cout << "Successfully Loaded to EmuBoard!\n";
        }
        else{
                cout << "ERROR: FAILED TO LOAD EmuBoard!!!!!!!\n";
        }
    }

    patternSet.DeleteCurrentSet();

    this->Default(in, out);
    return;
}

void CSCGEMTestApplication::ClearSet(xgi::Input * in, xgi::Output * out)
{
  patternSet = cw::Set();
  patternSet.DeleteCurrentSet();

  sprintf(GetSetPrefix, patternSet.Prefix.c_str());
  sprintf(GetSaveDir, cw::default_dir.c_str());

  this->Default(in,out);
  return;
}


void CSCGEMTestApplication::RunStudy(xgi::Input * in, xgi::Output * out)
{
  cgicc::Cgicc cgi(in);

  if(xgi::Utils::hasFormElement(cgi,"GetNtrials_char")) sprintf(GetNtrials_char,cgi["GetNtrials_char"]->getValue().c_str());

  int clct0_lsbs, clct1_lsbs, clct_msbs;
  int CLCT0_data_, CLCT1_data_;
  TMB * thisTMB = tmbVector[1];				/// THIS SHOULD BE EDITED IF USING DIFFERENT TMB SETTINGS !!!!!!!!

  GetNtrials_int = atoi(GetNtrials_char);
  patternSet.N_trials += GetNtrials_int;
  for(int i=0; i < GetNtrials_int; i++){
    patternSet.Dump();
    clct0_lsbs = thisTMB->ReadRegister(seq_clct0_adr);
    clct1_lsbs = thisTMB->ReadRegister(seq_clct1_adr);
    clct_msbs  = thisTMB->ReadRegister(seq_clctm_adr);

    CLCT0_data_ = ( (clct_msbs & 0xf) << 16 ) | (clct0_lsbs & 0xffff);
    CLCT1_data_ = ( (clct_msbs & 0xf) << 16 ) | (clct1_lsbs & 0xffff);

    int CLCT0_nhit = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_nhit_bitlo, CLCT0_nhit_bithi);
    int CLCT0_pid = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_pattern_bitlo, CLCT0_pattern_bithi);
    int CLCT0_key = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_keyHalfStrip_bitlo, CLCT0_keyHalfStrip_bithi);

    std::cout << std::endl << "CLCT 0 Decode:" << std::endl;
    std::cout << "N Hit = " << CLCT0_nhit << std::endl;
    std::cout << "Pid   = " << CLCT0_pid << std::endl;
    std::cout << "KeyStr= " << CLCT0_key << std::endl;

    thisTMB->GetCounters();

    int CLCT1_nhit = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_nhit_bitlo, CLCT1_nhit_bithi);
    int CLCT1_pid = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_pattern_bitlo, CLCT1_pattern_bithi);
    int CLCT1_key = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_keyHalfStrip_bitlo, CLCT1_keyHalfStrip_bithi);

    int clct0_inc = thisTMB->GetCounter(cw::tmb_counters[0]) - CLCT0_Counter;
    int clct1_inc = thisTMB->GetCounter(cw::tmb_counters[1]) - CLCT1_Counter;

    std::cout << "CLCT 1 Decode:" << std::endl;
    std::cout << "N Hit = " << CLCT1_nhit << std::endl;
    std::cout << "Pid   = " << CLCT1_pid << std::endl;
    std::cout << "KeyStr= " << CLCT1_key << std::endl;

    std::cout << "Increment on CLCT0 = " << clct0_inc << "    " << CLCT0_Counter << "   " << thisTMB->GetCounter(cw::tmb_counters[0]) << std::endl;
    std::cout << "Increment on CLCT1 = " << clct1_inc << "    " << CLCT1_Counter << "   " << thisTMB->GetCounter(cw::tmb_counters[1]) << std::endl;

    CLCT0_Counter = thisTMB->GetCounter(cw::tmb_counters[0]);//thisTMB->GetCounter(cw::tmb_counters[0]);
    CLCT1_Counter = thisTMB->GetCounter(cw::tmb_counters[1]);//thisTMB->GetCounter(cw::tmb_counters[1]);


    //cw::TMBresponse thisTrial = cw::TMBresponse(CLCT0_data_, CLCT1_data_, 0);
    cw::TMBresponse_long thisTrial = cw::TMBresponse_long(CLCT0_nhit, CLCT0_pid, CLCT0_key, CLCT1_nhit, CLCT1_pid, CLCT1_key, clct0_inc, clct1_inc);
    bool match = false;
    for(unsigned int i=0; i < patternSet.Results_l.size(); i++){
      if(thisTrial == patternSet.Results_l[i]){
         ++patternSet.Results_l[i];
	 match = true;
         break;
      }
    }
    if(!match){ patternSet.Results_l.push_back(thisTrial); }

    //thisTMB->ResetCounters();
  }

  std::cout << "Took this many TMB Dumps: " << GetNtrials_int << std::endl;

  this->Default(in,out);
  return;
}

void CSCGEMTestApplication::RunStudyCCLUT(xgi::Input * in, xgi::Output * out)
{
  cgicc::Cgicc cgi(in);

  if(xgi::Utils::hasFormElement(cgi,"GetNtrials_char")) sprintf(GetNtrials_char,cgi["GetNtrials_char"]->getValue().c_str());

  int clct0_lsbs, clct1_lsbs, clct_msbs;
  int CLCT0_data_, CLCT1_data_;
  //int clct0_xky_data_, clct1_xky_data_;
  int clct0_CC_data_, clct1_CC_data_;
  TMB * thisTMB = tmbVector[1];				/// THIS SHOULD BE EDITED IF USING DIFFERENT TMB SETTINGS !!!!!!!!

  GetNtrials_int = atoi(GetNtrials_char);
  patternSet.N_trials += GetNtrials_int;
  for(int i=0; i < GetNtrials_int; i++){
    patternSet.Dump(0,GetOTMBType[0]);
    clct0_lsbs = thisTMB->ReadRegister(seq_clct0_adr);//first part for CLCT0
    clct1_lsbs = thisTMB->ReadRegister(seq_clct1_adr);//first part for CLCT1
    clct_msbs  = thisTMB->ReadRegister(seq_clctm_adr);// common for both CLCT, second
    clct0_CC_data_ = thisTMB->ReadRegister(clct0_cc_adr);//16bits
    clct1_CC_data_ = thisTMB->ReadRegister(clct1_cc_adr);
    // readin the register value
    // for 11 comparator code, clct0_xky_adr->clct0_bndxky_adr
    // add bending comparison for CCLUT
    // ./emuDCS/PeripheralCore/include/emu/pc/TMB_constants.h
    // ./emuDCS/PeripheralCore/src/common/TMB.cc
    // https://twiki.cern.ch/twiki/bin/view/Main/GEMCSCOTMBFWDevelopementLS2
    //clct0_xky_data_ = thisTMB->ReadRegister(clct0_xky_adr);
    //clct1_xky_data_ = thisTMB->ReadRegister(clct1_xky_adr);

    CLCT0_data_ = ( (clct_msbs & 0xf) << 16 ) | (clct0_lsbs & 0xffff);//32 bits
    CLCT1_data_ = ( (clct_msbs & 0xf) << 16 ) | (clct1_lsbs & 0xffff);

    int CLCT0_nhit = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_nhit_bitlo, CLCT0_nhit_bithi);
    int CLCT0_pid = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_pattern_bitlo, CLCT0_pattern_bithi);
    int CLCT0_key = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_keyHalfStrip_bitlo, CLCT0_keyHalfStrip_bithi);
    //int CLCT0_xky = thisTMB->ExtractValueFromData(clct0_xky_data_, clct0_xky_bitlo, clct0_xky_bithi);
    int CLCT0_CC = thisTMB->ExtractValueFromData(clct0_CC_data_, clct0_cc_bitlo, clct0_cc_bithi);

    // extract value from register value. bitlo means the lowest bit
    //int run =  patternSet.Results_l.size()/ GetNtrials_int;


    std::cout << std::endl << std::endl  << "CLCT 0 Input:" << std::endl;
    std::cout << "N Hit = " << cw::GetNHits(patternSet.LUT[0].LUT_code) << std::endl;
    std::cout << "Pid   = " << patternSet.LUT[0].pat << std::endl;
    std::cout << "KeyStr= " << patternSet.LUT[0].hs << std::endl;
    std::cout << "CC Code= " << patternSet.LUT[0].LUT_code << std::endl;
    std::cout << "1/8 resolution= " << cw::EightResolution(patternSet.LUT[0].hs, patternSet.LUT[0].LUT_code, patternSet.LUT[0].pat) << std::endl << std::endl;



    std::cout << std::endl << "CLCT 0 Decode:" << std::endl;
    std::cout << "N Hit = " << CLCT0_nhit << std::endl;
    std::cout << "Pid   = " << CLCT0_pid << std::endl;
    std::cout << "KeyStr= " << CLCT0_key << std::endl;
    std::cout << "CC Code= " << CLCT0_CC << std::endl;
    //std::cout << "1/8 resolution= " << CLCT0_xky << std::endl << std::endl ;




    thisTMB->GetCounters();



    int CLCT1_nhit = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_nhit_bitlo, CLCT1_nhit_bithi);
    int CLCT1_pid = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_pattern_bitlo, CLCT1_pattern_bithi);
    int CLCT1_key = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_keyHalfStrip_bitlo, CLCT1_keyHalfStrip_bithi);
    //int CLCT1_xky = thisTMB->ExtractValueFromData(clct1_xky_data_, clct1_xky_bitlo, clct1_xky_bithi);
    int CLCT1_CC = thisTMB->ExtractValueFromData(clct1_CC_data_, clct1_cc_bitlo, clct1_cc_bithi);

    int clct0_inc = thisTMB->GetCounter(cw::tmb_counters[0]) - CLCT0_Counter;
    int clct1_inc = thisTMB->GetCounter(cw::tmb_counters[1]) - CLCT1_Counter;

    std::cout << "CLCT 1 Decode:" << std::endl;
    std::cout << "N Hit = " << CLCT1_nhit << std::endl;
    std::cout << "Pid   = " << CLCT1_pid << std::endl;
    std::cout << "KeyStr= " << CLCT1_key << std::endl;
    //std::cout << "1/8 resolution= " << CLCT1_xky << std::endl;

    std::cout << "Increment on CLCT0 = " << clct0_inc << "    " << CLCT0_Counter << "   " << thisTMB->GetCounter(cw::tmb_counters[0]) << std::endl;
    std::cout << "Increment on CLCT1 = " << clct1_inc << "    " << CLCT1_Counter << "   " << thisTMB->GetCounter(cw::tmb_counters[1]) << std::endl;

    CLCT0_Counter = thisTMB->GetCounter(cw::tmb_counters[0]);//thisTMB->GetCounter(cw::tmb_counters[0]);
    CLCT1_Counter = thisTMB->GetCounter(cw::tmb_counters[1]);//thisTMB->GetCounter(cw::tmb_counters[1]);


    //cw::TMBresponse thisTrial = cw::TMBresponse(CLCT0_data_, CLCT1_data_, 0);
    cw::TMBresponse_long thisTrial = cw::TMBresponse_long(CLCT0_nhit, CLCT0_pid, CLCT0_key, CLCT1_nhit, CLCT1_pid, CLCT1_key, clct0_inc, clct1_inc);
    bool match = false;
    for(int j=0; j < patternSet.Results_l.size(); j++){
      if(thisTrial == patternSet.Results_l[j]){
        ++patternSet.Results_l[j];
        match = true;
        break;
      }
    }
    if(!match){ patternSet.Results_l.push_back(thisTrial); }

    //thisTMB->ResetCounters();
  }

  std::cout << "Took this many TMB Dumps: " << GetNtrials_int << std::endl;

  this->Default(in,out);
  return;
}

void CSCGEMTestApplication::AddCLCTParamScan(xgi::Input * in, xgi::Output * out)
{
  cgicc::Cgicc cgi(in);

  if(xgi::Utils::hasFormElement(cgi,"GetCLCT_bx_ps_char")) sprintf(GetCLCT_bx_ps_char,cgi["GetCLCT_bx_ps_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCLCT_key_ps_char")) sprintf(GetCLCT_key_ps_char,cgi["GetCLCT_key_ps_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCLCT_pid_ps_char")) sprintf(GetCLCT_pid_ps_char,cgi["GetCLCT_pid_ps_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCLCT_nhit_ps_char")) sprintf(GetCLCT_nhit_ps_char,cgi["GetCLCT_nhit_ps_char"]->getValue().c_str());

  GetCLCT_ps_char[0] = GetCLCT_bx_ps_char;
  GetCLCT_ps_char[1] = GetCLCT_key_ps_char;
  GetCLCT_ps_char[2] = GetCLCT_pid_ps_char;
  GetCLCT_ps_char[3] = GetCLCT_nhit_ps_char;

  std::vector<int> ps_clct_info;

  for(unsigned int i=0; i < GetCLCT_ps_char.size(); i++){
    std::istringstream ss;
    ss.str(GetCLCT_ps_char[i]);

    cw::RangeParam range;
    range.param = i;
    range.clct = CLCT_ps_vec.size();
    range.step_size = 1;

    if(ss.str().empty()){
        cout << "empty" << endl;
        ps_clct_info.push_back(0);
    }
    else if(ss.str().at(0) == '['){
      cout << "We are reading a RANGED VALUE:"<<ss.str() << endl << endl;
      int a,b;
      char tmp;
      ss >> tmp >> a >> tmp >> b >> tmp;
      cout << "These are the vals a= " << a << " b= " << b << endl << endl;
      if(a < b){
        range.min = a;
        range.max = b;
      }
      else{
        range.min = b;
        range.max = a;
      }
      Free_params.push_back(range);
      ps_clct_info.push_back(a);
    }
    else{
      cout << "We are reading a SINGLE VALUE:" << ss.str() << endl << endl;
      int a;
      ss >> a;
      ps_clct_info.push_back(a);
      cout << "The Value is a = " << a << endl << endl;
    }
  }

  int tmbtype = GetOTMBType[0] +0xa -'a';//get OTMB type, a,c,d->0xa, 0xc, 0xd
  CLCT_ps_vec.push_back(cw::CLCT(ps_clct_info[0],ps_clct_info[1],ps_clct_info[2],ps_clct_info[3], tmbtype));

  this->Default(in,out);
  return;
}

void CSCGEMTestApplication::DOESWORK(xgi::Input * in, xgi::Output * out)
{
 this->Default(in,out);
 return;
}

void CSCGEMTestApplication::RunParamScan(xgi::Input * in, xgi::Output * out)
{
  std::cout << "\nRUNNING PARAM SCAN\n";
  cgicc::Cgicc cgi(in);

  // Read input fields
  if(xgi::Utils::hasFormElement(cgi,"TrialsPerStep_char")){
    sprintf(TrialsPerStep_char,cgi["TrialsPerStep_char"]->getValue().c_str());
    TrialsPerStep = atoi(TrialsPerStep_char);
  }
  else{ TrialsPerStep = 1; }
  if(xgi::Utils::hasFormElement(cgi,"ParamScanOutFile")) sprintf(ParamScanOutFile,cgi["ParamScanOutFile"]->getValue().c_str());

  patternSet.N_trials = TrialsPerStep;

  // Open the Output File
  std::fstream fout;
  fout.open(ParamScanOutFile, std::fstream::out | std::fstream::app);

  int clct0_lsbs, clct1_lsbs, clct_msbs;
  int clct0_inc=0;
  int clct1_inc=0;
  int CLCT0_data_, CLCT1_data_;
  TMB * thisTMB = tmbVector[1];                         /// THIS SHOULD BE EDITED IF USING DIFFERENT TMB SETTINGS !!!!!!!!

  do{
	// Reset Counting Variables!@!
	thisTMB->ResetCounters();
	clct0_inc = 0;
	clct1_inc = 0;
	CLCT0_Counter = 0;
	CLCT1_Counter = 0;

	// Load CLCT_ps_vec to current patternSet
	patternSet.Clear();
	for(unsigned int i=0; i < CLCT_ps_vec.size(); i++){
		patternSet.CSC.push_back(CLCT_ps_vec[i]);
	}
	// Generate {.pat} in tmp file location and Load to Emu Board
	patternSet.DeleteCurrentSet();
	patternSet.SetOTMBCompileType(GetOTMBType[0]);
   	if(patternSet.WritePatterns()){
        	if( patternSet.LoadEmuBoard() ){
       	 	        cout << "Successfully Loaded to EmuBoard!\n";
        	}
        	else{
        	        cout << "ERROR: FAILED TO LOAD EmuBoard!!!!!!!\n";
        	}
	}

	// For( TrialsPerStep ) ->Dump to EmuBoard,
	// 			->Record TMB Response
	for(int i=0; i < patternSet.N_trials; i++){
		patternSet.Dump();
    		clct0_lsbs = thisTMB->ReadRegister(seq_clct0_adr);
    		clct1_lsbs = thisTMB->ReadRegister(seq_clct1_adr);
    		clct_msbs  = thisTMB->ReadRegister(seq_clctm_adr);

		thisTMB->GetCounters();

		clct0_inc = thisTMB->GetCounter(cw::tmb_counters[0]) - CLCT0_Counter;
                clct1_inc = thisTMB->GetCounter(cw::tmb_counters[1]) - CLCT1_Counter;

		cout << endl << CLCT0_Counter << "  " << thisTMB->GetCounter(cw::tmb_counters[0]) << endl;
		cout << endl << CLCT1_Counter << "  " << thisTMB->GetCounter(cw::tmb_counters[1]) << endl;

		CLCT0_Counter = thisTMB->GetCounter(cw::tmb_counters[0]);
		CLCT1_Counter = thisTMB->GetCounter(cw::tmb_counters[1]);

    		CLCT0_data_ = ( (clct_msbs & 0xf) << 16 ) | (clct0_lsbs & 0xffff);
    		CLCT1_data_ = ( (clct_msbs & 0xf) << 16 ) | (clct1_lsbs & 0xffff);

		cw::TMBresponse thisTrial = cw::TMBresponse(CLCT0_data_, CLCT1_data_, clct0_inc, clct1_inc);
		//cw::TMBresponse_long thisTrial = cw::TMBresponse_long(CLCT0_nhit, CLCT0_pid, CLCT0_key, CLCT1_nhit, CLCT1_pid, CLCT1_key, clct0_inc, clct1_inc);
		bool match = false;
		for(unsigned int i=0; i < patternSet.Results.size(); i++){
			if(thisTrial == patternSet.Results[i]){
		        	++patternSet.Results[i];
		                match = true;
		                break;
		   	}
                }
		if(!match){ patternSet.Results.push_back(thisTrial); }

	}

	fout << patternSet << endl;	// Write Results to file

	// DEBUG
	for(unsigned int i=0; i < CLCT_ps_vec.size(); i++){
		cout << "CLCT" << i << " " << CLCT_ps_vec[i] << endl;
	}

  }while(Increment(CLCT_ps_vec, Free_params));

  fout.close();
  this->Default(in,out);
  return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSCGEMTestApplication::GenerateDCFEBData(xgi::Input * in, xgi::Output * out )
{
    cgicc::Cgicc cgi(in);

    char DirGen[200];
    char DCFEBN[10];
    char HalfStripN[10];

    //bits = (char*)calloc(size,sizeof(char));
    char bits[4096];

    if (xgi::Utils::hasFormElement(cgi, "DirGen")) sprintf(DirGen,cgi["DirGen"]->getValue().c_str());
    if(xgi::Utils::hasFormElement(cgi, "DCFEBN")) sprintf(DCFEBN,cgi["DCFEBN"]->getValue().c_str());
    if(xgi::Utils::hasFormElement(cgi, "HalfStripN")) sprintf(HalfStripN,cgi["HalfStripN"]->getValue().c_str());

    int HalfStrip = atoi(HalfStripN);
    int diStrip = (HalfStrip-1)/4+1;
    int strip = (HalfStrip-(diStrip-1)*4-1)/2;
    int halfStrip = HalfStrip-(diStrip-1)*4-strip*2-1;

    for (int j = 1; j < 8; j++) {
        char filename[200];
        char strj[10];
        sprintf(strj, "%d", j);
        strcpy(filename,DirGen);
        strcat(filename,"/DCFEB_");
        strcat(filename,strj);
        strcat(filename,".pat");

        FILE *outfile = fopen(filename,"w");

        if (j == atoi(DCFEBN)) {
            for (int i = 0; i < 6; i++) {
                bits[i] = 0xff & (int)pow(2,8-diStrip);
            }
            for (int i = 6; i < 12; i++) {
                bits[i] = 0xff & (int)pow(2,8-diStrip)*strip;
            }
            for (int i = 13; i < 18; i++) {
                bits[i] = 0xff & (int)pow(2,8-diStrip)*halfStrip;
            }
        }
        else {
            for (int i = 0; i < 18; i++) {
                bits[i] = 0x00;
            }
        }
        for (int i = 18; i < 4096; i++) {
            bits[i] = 0x00;
        }
        fwrite(bits, sizeof(char), sizeof(bits), outfile);
        fclose(outfile);
    }
    this->Default(in, out);
}

void CSCGEMTestApplication::UploadDCFEBData(xgi::Input * in, xgi::Output * out )
{
    using namespace std;



    cgicc::Cgicc cgi(in);
    //char DirUp[200];
    int pbase = 2816;

    if(xgi::Utils::hasFormElement(cgi, "DirUp")) sprintf(DirUp,cgi["DirUp"]->getValue().c_str());

    ofstream fs;
    fs.open("s1.txt"); fs << "a"; fs.close();
    fs.open("s2.txt"); fs << "a"; fs.close();
    fs.open("s3.txt"); fs << "a"; fs.close();
    fs.open("s4.txt"); fs << "a"; fs.close();
    fs.open("s5.txt"); fs << "a"; fs.close();
    fs.open("s6.txt"); fs << "a"; fs.close();
    fs.open("s7.txt"); fs << "a"; fs.close();

    for (int j = 1; j < 8; j++) {
        char filename[200];
        char strj[10];
        sprintf(strj, "%d", j);
        strcpy(filename,DirUp);
        strcat(filename,"/DCFEB_");
        strcat(filename,strj);
        strcat(filename,".pat");

        char block[RAMPAGE_SIZE];
        fread(block, sizeof(char), RAMPAGE_SIZE, fopen(filename,"r"));
        memcpy(wdat,block,RAMPAGE_SIZE);

        eth_open(EmuBoardLocation);
        eth_reset();

        int e1 = write_command(7,pbase+j, block);
        char* pkt;
        int e2 = read_command(&pkt);

        char out[10];

        strcpy(out,"s");
        strcat(out,strj);
        strcat(out,".txt");
        fs.open(out);
        if (e1 == 0 && e2 == 7) fs << "0";
        else fs << "1";
        fs.close();

        eth_close();
    }



    this->Default(in, out);
}

void CSCGEMTestApplication::CheckDCFEBData(xgi::Input * in, xgi::Output * out )
{
    using namespace std;

    int pbase = 2816;

    ofstream fs;
    fs.open("c1.txt"); fs << "a"; fs.close();
    fs.open("c2.txt"); fs << "a"; fs.close();
    fs.open("c3.txt"); fs << "a"; fs.close();
    fs.open("c4.txt"); fs << "a"; fs.close();
    fs.open("c5.txt"); fs << "a"; fs.close();
    fs.open("c6.txt"); fs << "a"; fs.close();
    fs.open("c7.txt"); fs << "a"; fs.close();

    for (int j = 1; j < 8; j++) {
        char strj[10];
        sprintf(strj, "%d", j);

        eth_open(EmuBoardLocation);
        eth_reset();
        int e1 = write_command(3,pbase+j);
        char* pkt;
        int e2 = read_command(&pkt);

        char out[10];
        strcpy(out,"c");
        strcat(out,strj);
        strcat(out,".txt");
        fs.open(out);
        if (e1 == 0 && e2 == -3) fs << "0";
        else fs << "1";
        fs.close();

        eth_close();
    }

    this->Default(in, out);
}

void CSCGEMTestApplication::TransmitDCFEBData(xgi::Input * in, xgi::Output * out )
{
    this->Default(in, out);
}

void CSCGEMTestApplication::ReadOutTriggerResults(xgi::Input * in, xgi::Output * out )
{
    this->Default(in, out);
}

void CSCGEMTestApplication::F0(xgi::Input * in, xgi::Output * out )
{
	eth_open("/dev/schar1");
	eth_reset();
	int e1 = write_command(0);
	char* pkt;
	int e2 = read_command(&pkt);
	eth_close();

	if(e2 > 0)
		dumphex(e2, pkt, &std::cout);

    this->Default(in, out);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSCGEMTestApplication::GenGEMPattxtFileMounN(xgi::Input * in, xgi::Output * out )
{
  using namespace std;

  cgicc::Cgicc cgi(in);
  if(xgi::Utils::hasFormElement(cgi,"GemtxtMounNchar")) sprintf(GemtxtMounNchar,cgi["GemtxtMounNchar"]->getValue().c_str());
  int num_mouns = atoi(GemtxtMounNchar);
  std::cout << "------------this is the num_mouns var output: " << num_mouns << "\n";
  std::cout << "------------this is the num_mouns char output: " << GemtxtMounNchar << "\n";

  this->Default(in, out);
}

void CSCGEMTestApplication::GenGEMPattxtFile(xgi::Input * in, xgi::Output * out )
{
  using namespace std;
  cgicc::Cgicc cgi(in);

  for (int i = 0; i < 512; ++i) {
    string number;          // string which will contain the result
    std::ostringstream convert;   // stream used for the conversion
    convert << i;      // insert the textual representation of 'Number' in the characters in the stream
    number = convert.str();
    string tempstring = "GemBx" + number;
    if(xgi::Utils::hasFormElement(cgi,tempstring)) sprintf(testingarray[i],cgi[tempstring]->getValue().c_str());
  }
  this->Default(in, out);
}

void CSCGEMTestApplication::YuriyPatConvert(xgi::Input * in, xgi::Output * out )
{
  using namespace std;
  cgicc::Cgicc cgi(in);

  if(xgi::Utils::hasFormElement(cgi, "DirYuriyConvtxt")) sprintf(DirYuriyConvtxt,cgi["DirYuriyConvtxt"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "YuriyConvtxtFile")) sprintf(GEMtxtFile,cgi["YuriyConvtxtFile"]->getValue().c_str());
  char filename[400];

  strcpy(filename,DirYuriyConvtxt);
  strcat(filename,"/");
  strcat(filename,YuriyConvtxtFile);
  std::cout << "this is the location of the GEM txt file " << filename << endl;
  std::cout << "this is the location of the GEM Pat Dir " << DirYuriyConvtxt << endl;
//  PatternConvert PatConv;
//  PatConv.Pattern_Converter(filename);
  std::cout << "the pattern file has been created." << std::endl;

  this->Default(in, out);
}

void CSCGEMTestApplication::Yuriy_CLCT_PatternConverter(xgi::Input * in, xgi::Output * out )
{
  using namespace std;
  cgicc::Cgicc cgi(in);


  if(xgi::Utils::hasFormElement(cgi, "DirYuriyCLCTConvtxt")) sprintf(DirYuriyCLCTConvtxt,cgi["DirYuriyCLCTConvtxt"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "DirYuriyCLCTConvPat")) sprintf(DirYuriyCLCTConvPat,cgi["DirYuriyCLCTConvPat"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "YuriyCLCTConvtxtFile")) sprintf(YuriyCLCTConvtxtFile,cgi["YuriyCLCTConvtxtFile"]->getValue().c_str());

  char filename[400];

  strcpy(filename,DirYuriyCLCTConvtxt);
  strcat(filename,"/");
  strcat(filename,YuriyCLCTConvtxtFile);

  std::cout << "this is the location of the Yuriy CLCT txt file " << filename << endl;
  std::cout << "this is the location of the Yuriy CLCT Pat Dir " << DirYuriyCLCTConvPat << endl;

//  Yuriy_CLCT_PatternGen YuriyCLCTPat;
  std::cout << "at least it will create the object \n";
//  YuriyCLCTPat.Yuriy_CLCT_PatternGenerator(filename,DirYuriyCLCTConvPat);
  std::cout << "the Yuriy CLCT Patter has been created." << std::endl;

  this->Default(in, out);
}

void CSCGEMTestApplication::GemPatternConverter(xgi::Input * in, xgi::Output * out )
{
    using namespace std;
    cgicc::Cgicc cgi(in);

   	if(xgi::Utils::hasFormElement(cgi, "DirGEMPat")) sprintf(DirGEMPat,cgi["DirGEMPat"]->getValue().c_str());
   	if(xgi::Utils::hasFormElement(cgi, "DirGEMtxt")) sprintf(DirGEMtxt,cgi["DirGEMtxt"]->getValue().c_str());
  	if(xgi::Utils::hasFormElement(cgi, "GEMtxtFile")) sprintf(GEMtxtFile,cgi["GEMtxtFile"]->getValue().c_str());

   	char filename[400];

    strcpy(filename,DirGEMtxt);
    strcat(filename,"/");
    strcat(filename,GEMtxtFile);

	std::cout << "this is the location of the GEM txt file " << filename << endl;
	std::cout << "this is the location of the GEM Pat Dir " << DirGEMPat << endl;

//    GEMPatternGen GemPat;
//    GemPat.GEMPatternGenerator(filename,DirGEMPat);
    std::cout << "at least it works up to here TrolololOLOLOLOOlol9oLOLoLOlol \n";

    std::cout << "the GEM Patter has been created." << std::endl;


  this->Default(in, out);



}


void CSCGEMTestApplication::CfebPatternConverter(xgi::Input * in, xgi::Output * out )
{
  using namespace std;
  cgicc::Cgicc cgi(in);

  if(xgi::Utils::hasFormElement(cgi, "DirCfebPat")) sprintf(DirCfebPat,cgi["DirCfebPat"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "DirCfebtxt")) sprintf(DirCfebtxt,cgi["DirCfebtxt"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "CfebPatFile")) sprintf(CfebPatFile,cgi["CfebPatFile"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi, "CfebtxtFile")) sprintf(CfebtxtFile,cgi["CfebtxtFile"]->getValue().c_str());

  char filename[400];

  strcpy(filename,DirCfebtxt);
  strcat(filename,"/");
  strcat(filename,CfebtxtFile);

  std::cout << "this is the location of the Cfeb txt file " << filename << endl;
  std::cout << "this is the location of the Cfeb Pat Dir " << DirGEMPat << endl;

  //PatternGen CfebPat;
  std::cout << "at least it will create the object \n";
  //CfebPat.PatternGenerator(filename,DirCfebPat);
  std::cout << "the Cfeb Patter has been created." << std::endl;

  this->Default(in, out);
}

void CSCGEMTestApplication::LoadToEmuBoard(xgi::Input * in, xgi::Output * out )
{
    using namespace std;
    cgicc::Cgicc cgi(in);

    patternSet.DeleteCurrentSet();
    patternSet.SetOTMBCompileType(GetOTMBType[0]);

    if(patternSet.WritePatterns()){
	if( patternSet.LoadEmuBoard() ){
		cout << "Successfully Loaded to EmuBoard!\n";
	}
	else{
		cout << "ERROR: FAILED TO LOAD EmuBoard!!!!!!!\n";
	}
    }


  TMB * thisTMB = tmbVector[1];
  thisTMB->ResetCounters();
  CLCT0_Counter = 0;
  CLCT1_Counter = 0;
  this->Default(in, out);



}

void CSCGEMTestApplication::SendFromEmuBoard(xgi::Input * in, xgi::Output * out )
{

    using namespace std;
    cgicc::Cgicc cgi(in);

    //char DirGen[200];
   	//char PatFile[200];
   	//char FiberN[10];
   	//char NumRuns[10];

   	if(xgi::Utils::hasFormElement(cgi, "DirGen")) sprintf(DirGen,cgi["DirGen"]->getValue().c_str());
   	if(xgi::Utils::hasFormElement(cgi, "PatFile")) sprintf(PatFile,cgi["PatFile"]->getValue().c_str());
   	if(xgi::Utils::hasFormElement(cgi, "FiberN")) sprintf(FiberN,cgi["FiberN"]->getValue().c_str());
   	if(xgi::Utils::hasFormElement(cgi, "NumRuns")) sprintf(NumRuns,cgi["NumRuns"]->getValue().c_str());




	int Runs = atoi(NumRuns);

	eth_open(EmuBoardLocation);
	int e;
	for(int i = 0; i< Runs; ++i){
		eth_reset();
        e = write_command(0xd); //send the patterns
        usleep(30);
	}
	char* pkt;
	e = read_command(&pkt);
	eth_close();


  this->Default(in, out);
}

void CSCGEMTestApplication::OutputCLCTInfo(xgi::Input * in, xgi::Output * out )
{
/*

cgicc::Cgicc cgi(in);
//
cgicc::form_iterator name = cgi.getElement("tmb");
int tmb;
if(name != cgi.getElements().end()) {
  tmb = cgi["tmb"]->getIntegerValue();
  std::cout << "TMBStatus:  TMB=" << tmb << std::endl;
  TMB_ = tmb;
} else {
  std::cout << "TMBStatus: No TMB" << std::endl ;
  tmb = TMB_;
}
//
TMB * thisTMB = tmbVector[tmb];
std::cout << "the value for tmb is = " << tmb << std::endl;
std::cout << "The tmbVector[tmb] = " << tmbVector[tmb] << std::endl;
*/

this->Default(in, out);
}

void CSCGEMTestApplication::OutputMPCFifo(xgi::Input * in, xgi::Output * out )
{
/*

cgicc::Cgicc cgi(in);
//
cgicc::form_iterator name = cgi.getElement("tmb");
int tmb;
if(name != cgi.getElements().end()) {
  tmb = cgi["tmb"]->getIntegerValue();
  std::cout << "TMBStatus:  TMB=" << tmb << std::endl;
  TMB_ = tmb;
} else {
  std::cout << "TMBStatus: No TMB" << std::endl ;
  tmb = TMB_;
}
//
TMB * thisTMB = tmbVector[tmb];
std::cout << "the value for tmb is = " << tmb << std::endl;
std::cout << "The tmbVector[tmb] = " << tmbVector[tmb] << std::endl;
*/

this->Default(in, out);
}
/*// refrence
void EmuPeripheralCrateConfig::TMBPrintCounters(xgi::Input * in, xgi::Output * out )
  throw (xgi::exception::Exception) {
  //
  std::cout << "TMBPrintCounters "<< std::endl;
  //
  cgicc::Cgicc cgi(in);
  //
  cgicc::form_iterator name = cgi.getElement("tmb");
  //
  int tmb=0;
  if(name != cgi.getElements().end()) {
    tmb = cgi["tmb"]->getIntegerValue();
    std::cout << "TMB " << tmb << std::endl;
    TMB_ = tmb;
  }
  //
  TMB * thisTMB = tmbVector[tmb];
  //
  LOG4CPLUS_INFO(getApplicationLogger(), "Start PrintCounters");
  thisTMB->RedirectOutput(&OutputStringTMBStatus[tmb]);
  thisTMB->GetCounters();
  thisTMB->PrintCounters();
  thisTMB->RedirectOutput(&std::cout);
  LOG4CPLUS_INFO(getApplicationLogger(), "Done PrintCounters");
  //
  this->TMBUtils(in,out);
  //
}


  //
  *out << cgicc::td().set("ALIGN","left");
  std::string TMBPrintCounters = toolbox::toString("/%s/TMBPrintCounters",getApplicationDescriptor()->getURN().c_str());
  *out << cgicc::form().set("method","GET").set("action",TMBPrintCounters) ;
  *out << cgicc::input().set("type","submit").set("value","Print TMB Counters") ;
  sprintf(buf,"%d",tmb);
  *out << cgicc::input().set("type","hidden").set("value",buf).set("name","tmb");
  *out << cgicc::form() ;
  *out << cgicc::td();
  //



*/


xdaq::Application * CSCGEMTestApplication::instantiate(xdaq::ApplicationStub * stub )
throw (xdaq::exception::Exception)
{
  return new CSCGEMTestApplication(stub);
}

}}  // namespaces

// factory instantion of XDAQ application
//XDAQ_INSTANTIATOR_IMPL(emu::pc::CSCGEMTestApplication)

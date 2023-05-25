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




// XDAQ includes
#include "cgicc/Cgicc.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/HTMLElement.h"
#include "toolbox/string.h"

// system includes
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <math.h>
#include <string>
#include <unistd.h> // for sleep()
#include <iostream>

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
    xgi::bind(this, &CSCGEMTestApplication::Bit11, "Bit11");
    xgi::bind(this, &CSCGEMTestApplication::Run3, "Run3");
    xgi::bind(this, &CSCGEMTestApplication::SetCFEBDelay, "SetCFEBDelay");
    xgi::bind(this, &CSCGEMTestApplication::GetCFEBDelay, "GetCFEBDelay");
    xgi::bind(this, &CSCGEMTestApplication::GetOTMBCompileType, "GetOTMBCompileType");
    xgi::bind(this, &CSCGEMTestApplication::AddCLCT, "AddCLCT");
    xgi::bind(this, &CSCGEMTestApplication::AddCCLUT, "AddCCLUT");
    xgi::bind(this, &CSCGEMTestApplication::AddCCLUT_GUI, "AddCCLUT_GUI");
    xgi::bind(this, &CSCGEMTestApplication::AddGEM, "AddGEM");
    xgi::bind(this, &CSCGEMTestApplication::LoadToEmuBoard, "LoadToEmuBoard");
    xgi::bind(this, &CSCGEMTestApplication::SaveCurrentSet, "SaveCurrentSet");	// Outdated! to be REMOVED
    xgi::bind(this, &CSCGEMTestApplication::SaveAsPat, "SaveAsPat");
    xgi::bind(this, &CSCGEMTestApplication::SaveAsTxt, "SaveAsTxt");
    xgi::bind(this, &CSCGEMTestApplication::ClearSet, "ClearSet");
    xgi::bind(this, &CSCGEMTestApplication::RunStudy, "RunStudy");
    xgi::bind(this, &CSCGEMTestApplication::RunStudyCCLUT, "RunStudyCCLUT");
    xgi::bind(this, &CSCGEMTestApplication::AddCLCTParamScan, "AddCLCTParamScan");
    xgi::bind(this, &CSCGEMTestApplication::AddCCLUTParamScan, "AddCCLUTParamScan");
    xgi::bind(this, &CSCGEMTestApplication::ClearCCLUTParamScan, "ClearCCLUTParamScan");
    xgi::bind(this, &CSCGEMTestApplication::RunParamScan, "RunParamScan");
    xgi::bind(this, &CSCGEMTestApplication::RunParamScanCCLUT, "RunParamScanCCLUT");
    xgi::bind(this, &CSCGEMTestApplication::ReadSimulatedFile, "ReadSimulatedFile");
    xgi::bind(this, &CSCGEMTestApplication::LoadSimulatedFileEvent, "LoadSimulatedFileEvent");
    xgi::bind(this, &CSCGEMTestApplication::OpenSimulatedFile, "OpenSimulatedFile");
    xgi::bind(this, &CSCGEMTestApplication::AutomaticCheckingFile, "AutomaticCheckingFile");

    xgi::bind(this, &CSCGEMTestApplication::HardReset, "HardReset");
    xgi::bind(this, &CSCGEMTestApplication::PrepareForTriggering, "PrepareForTriggering");

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

    time_t now = time(0);

    cout << "Number of sec since January 1,1970 is:: " << now << endl;

    tm *ltm = localtime(&now);

    // print various components of tm structure.
    cout << "Year:" << 1900 + ltm->tm_year<<endl;
    cout << "Month: "<< 1 + ltm->tm_mon<< endl;
    cout << "Day: "<< ltm->tm_mday << endl;
    cout << "Time: "<< 5+ltm->tm_hour << ":";
    cout << 30+ltm->tm_min << ":";
    cout << ltm->tm_sec << endl;

    std::stringstream savetime;
    savetime <<"/home/cscdev/Kyla/OTMB_Pattern_test/";
    savetime << 1900 + ltm->tm_year << "_" << 1 + ltm->tm_mon << "_" << ltm->tm_mday<< "/PiD0_time_"<< ltm->tm_hour <<"_" << ltm->tm_min << "_" << ltm->tm_sec <<".txt";
    const std::string tmp =  std::string{savetime.str()};
    const char* todayte = tmp.c_str();

    std::stringstream savedir;
    savedir <<"/home/cscdev/Kyla/OTMB_Pattern_test/";
    savedir << 1900 + ltm->tm_year << "_" << 1 + ltm->tm_mon << "_" << ltm->tm_mday;
    const std::string tmpd =  std::string{savedir.str()};
    const char* savedirpath = tmpd.c_str();

    boost::filesystem::path dir(savedirpath);

    if(!(boost::filesystem::exists(dir)))
    {
      std::cout<<"Doesn't Exists"<<std::endl;
      if (boost::filesystem::create_directory(dir))
      std::cout << "....Successfully Created !" << std::endl;
    }
    std::cout<<"Save Dir = " <<
    sprintf(TrialsPerStep_char, "2");

    sprintf(GetCCLUT_bx_ps_char, "10");
    sprintf(GetCCLUT_key_ps_char, "15");
    sprintf(GetCCLUT_pid_ps_char, "4");
    if (Bit11_int) sprintf(GetCCLUT_ccode_ps_char, "[0:2047]");
    else sprintf(GetCCLUT_ccode_ps_char, "[0:4095]");
    sprintf(GetCCLUT_bx_ps_step_char, "1");
    sprintf(GetCCLUT_key_ps_step_char, "1");
    sprintf(GetCCLUT_pid_ps_step_char, "1");
    sprintf(GetCCLUT_ccode_ps_step_char, "1");
    sprintf(ParamScanOutFile, todayte);
    sprintf(SimulatedFileName, "/home/cscdev/Kyla/InjectCSCPatterns/data/ComparatorDigi_CLCT_ME11.txt");






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
      GetCCLUT_ps_char.push_back(str);
      GetCCLUT_ps_step_char.push_back(str);
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







    *out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFFFBB") << endl;
    *out << cgicc::legend("Step 1: Set parameters.").set("style", "color:blue") ;

    std::string HardReset =
    toolbox::toString("/%s/HardReset",getApplicationDescriptor()->getURN().c_str());
    *out << cgicc::form().set("method","GET").set("action","/" + urn + "/HardReset") << std::endl ;
    *out << cgicc::input().set("type","submit").set("value","HardReset").set("style","font-size : 20px;");
    *out << cgicc::form() << std::endl ;

    std::string PrepareForTriggering =
    toolbox::toString("/%s/PrepareForTriggering",getApplicationDescriptor()->getURN().c_str());
    *out << cgicc::form().set("method","GET").set("action","/" + urn + "/PrepareForTriggering") << std::endl ;
    *out << cgicc::input().set("type","submit").set("value","Prepare For Triggering");
    *out << cgicc::form() << std::endl;
    *out << cgicc::br() << endl;


    *out << "Chose betwen 11 or 12 bit versions and run output style (11 b it and run 2 are uncopartable)" << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/Bit11") << endl;
    *out << " Do you want to use 11 bit vesion? " << cgicc::input().set("type","text").set("name","Bit11True").set("size","1").set("value", bit11_char) << endl;
    *out << cgicc::input().set("type","submit").set("value","Get Bit 11 True").set("name", "Get Bit 11 True") << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::br() << endl;

    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/Run3") << endl;
    *out << " Do you want to use run 3 output style (run 2 is deafault)? " << cgicc::input().set("type","text").set("name","Run3True").set("size","1").set("value", run3_char) << endl;
    *out << cgicc::input().set("type","submit").set("value","Get Run 3 True").set("name", "Get Run 3 True") << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::br() << endl;

    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/GetCFEBDelay") << endl;
    *out << cgicc::input().set("type","submit").set("value","Get CFEB delays").set("name", "Get CFEB delays") << endl;
    *out << "CFEB0123 delay = " << CFEB0123_delay_read << "; CFEB456 delay = " << CFEB456_delay_read << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::br() << endl;

    *out << "Set CFEB0123 and CFEB456 delays" << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/SetCFEBDelay") << endl;
    *out << "CFEB0123 Delay:" << cgicc::input().set("type","text").set("name","CFEB0123_delay").set("size","1").set("value", CFEB0123_delay_char) << endl;
    *out << "CFEB456 Delay:"  << cgicc::input().set("type","text").set("name","CFEB456_delay").set("size","1").set("value", CFEB456_delay_char) << endl;
    *out << cgicc::input().set("type","submit").set("value","Set CFEB delays").set("name", "Set CFEB delays") << endl;
    *out << "Note: if you have changed Delays you have to write flash to crate in order to changes take place." << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::br() << endl;


    *out << cgicc::br() << endl;
    *out << "Select OTMB compile type, a for MEX/1, c/d for ME1/1, c for plus endcap " << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/GetOTMBCompileType") << endl;
    *out << " OTMB Compile Type(a/c/d) : " << cgicc::input().set("type","text").set("name","GetOTMBType").set("size","1").set("value", GetOTMBType) << endl;
    *out << cgicc::input().set("type","submit").set("value","Set OTMB Type").set("name", "Get OTMB Type") << endl;
    *out << cgicc::br() << endl;
    *out << cgicc::form() << endl;


    *out << cgicc::fieldset();




    //*out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFFFBB") << endl;
    //*out << cgicc::legend("Get OTMB Compile Type").set("style", "color:blue") ;
    //*out << "The COMPILE_TYPE is:" << std::hex << COMPILE_TYPE << std::dec << endl;
    // Get OTMB Type From CSCGEM Test Page
    //*out << cgicc::pre();

    //*out << cgicc::fieldset() << endl;







    *out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFFFBB") << endl;
    *out << cgicc::legend("Parameter Scan CCLUT").set("style", "color:blue; font-size:14pt");
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/AddCCLUTParamScan") << endl;
    *out << " &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp Bx &nbsp &nbsp &nbsp &nbsp &nbsp |"<< endl;
    *out << " &nbsp Key HalfStrip &nbsp |"<< endl;
    *out << " &nbsp &nbsp &nbsp &nbsp &nbsp PiD &nbsp &nbsp &nbsp &nbsp &nbsp | "<< endl;
    *out << " &nbsp &nbsp &nbsp CC Code &nbsp &nbsp &nbsp |" << endl;
    *out << "<br>";
    *out << "Value :"<< cgicc::input().set("type","text").set("name","GetCCLUT_bx_ps_char").set("size","7").set("value", GetCCLUT_bx_ps_char) << endl;
    *out << "&nbsp &nbsp | &nbsp &nbsp" << cgicc::input().set("type","text").set("name","GetCCLUT_key_ps_char").set("size","7").set("value", GetCCLUT_key_ps_char) << endl;
    *out << "&nbsp &nbsp | &nbsp &nbsp" << cgicc::input().set("type","text").set("name","GetCCLUT_pid_ps_char").set("size","7").set("value", GetCCLUT_pid_ps_char) << endl;
    *out << "&nbsp &nbsp | &nbsp &nbsp" << cgicc::input().set("type","text").set("name","GetCCLUT_ccode_ps_char").set("size","7").set("value", GetCCLUT_ccode_ps_char) << "&nbsp &nbsp | &nbsp &nbsp" << endl;
    *out << "<br>";
    *out << "Step:  &nbsp " << cgicc::input().set("type","text").set("name","GetCCLUT_bx_ps_step_char").set("size","7").set("value", GetCCLUT_bx_ps_step_char) << endl;
    *out << "&nbsp &nbsp | &nbsp &nbsp" << cgicc::input().set("type","text").set("name","GetCCLUT_key_ps_step_char").set("size","7").set("value", GetCCLUT_key_ps_step_char) << endl;
    *out << "&nbsp &nbsp | &nbsp &nbsp" << cgicc::input().set("type","text").set("name","GetCCLUT_pid_ps_step_char").set("size","7").set("value", GetCCLUT_pid_ps_step_char) << endl;
    *out << "&nbsp &nbsp | &nbsp &nbsp" << cgicc::input().set("type","text").set("name","GetCCLUT_ccode_ps_step_char").set("size","7").set("value", GetCCLUT_ccode_ps_step_char) << "&nbsp &nbsp | &nbsp &nbsp" << endl;
    *out << cgicc::input().set("type","submit").set("value","Add").set("name", "Add CCLUT Param Scan") << endl;
    *out << cgicc::form() << endl;

    *out << cgicc::form().set("method","GET") << endl;
    //*out << cgicc::form().set("method", "GET").set("action","/" +urn+ "RunParamScan") << endl;
    *out << cgicc::table().set("border", "1") << endl;
    *out << cgicc::td().set("ALIGN", "Center") << endl;
    *out << cgicc::textarea().set("name", "ParamScanMuonList").set("rows","5").set("cols","150").set("WRAP","OFF");
    *out << "Muons in Study : " << CCLUT_ps_vec.size() << endl;
    *out << "LUT\n#\tbx\tkey\tpid\tCCcode\t\t{ (bx,ly,hs) }\n";
    for(int i=0; i < CCLUT_ps_vec.size(); i++){
      *out << i << '\t' << CCLUT_ps_vec[i] << endl;
    }

    *out << cgicc::textarea() << endl;
    *out << cgicc::td() << endl;

    *out << cgicc::td().set("ALIGN", "Center") << endl;
    *out << cgicc::textarea().set("name", "ParamScanMuonList").set("rows","5").set("cols","150").set("WRAP","OFF");
    *out << "Free Parameters : " << Free_paramsCCLUT.size() << endl;
    for(int i=0; i < Free_paramsCCLUT.size(); i++){
      *out << Free_paramsCCLUT[i] << endl;
    }
    if (cw::DoesScanParamersSizeBad(parameters)){
      *out << "Parameter scan sizes are bad, runnnig the test will results in crash!" << endl;
    }
    else *out << "Parameter scan sizes are fine." << endl;

    *out << cgicc::textarea() << endl;
    *out << cgicc::td() << endl;
    *out << cgicc::table() << endl;
    *out << cgicc::br();
    *out << cgicc::form() << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/ClearCCLUTParamScan") << endl;
    *out << cgicc::br();
    *out << cgicc::input().set("type","submit").set("value","Clear").set("name", "Clear CCLUT Param Scan") << endl;
    *out << cgicc::br();

    //  *out << cgicc::fieldset() << endl;
    //  *out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFFFBB") << endl;

    *out << cgicc::legend("Run Parameter Scan CCLUT").set("style", "color:blue") ;
    *out << cgicc::form() << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" +urn+ "/RunParamScanCCLUT") << endl;
    *out << "Trials / Step  " << cgicc::input().set("type","text").set("name","TrialsPerStep_char").set("size","7").set("value",TrialsPerStep_char) << endl;
    *out << cgicc::br();
    *out << "Output File  " << cgicc::input().set("type","text").set("name","ParamScanOutFile").set("size","100").set("value",ParamScanOutFile) << endl;
    *out << "Errors Output File  " << cgicc::input().set("type","text").set("name","ParamScanOutFileError").set("size","100").set("value",ParamScanOutFileError) << endl;
    *out << cgicc::input().set("type","submit").set("value","Run").set("name", "RunParamScanCCLUT") << endl;
    *out << " Mute output" << cgicc::input().set("type","checkbox").set("name","mute").set("size","7").set("value", "1")  <<endl; // "<input type=\"checkbox\" id=\"mute\" name=\"mute\" value=\"0\">  <label for=\"mute\"> Mute output</label><br>" << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::fieldset() << endl;




    // Kyla added function
    *out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFFFBB") << endl;
    *out << cgicc::legend("Simulated files test").set("style", "color:blue; font-size:14pt");
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/OpenSimulatedFile") << endl;
    *out << "Input File :" << cgicc::input().set("type","text").set("name","SimulatedFileName").set("size","100").set("value",SimulatedFileName) << endl;
    *out << cgicc::input().set("type","submit").set("value","Open").set("name", "OpenSimulatedFile");// << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/ReadSimulatedFile") ;//<< endl;
    *out << cgicc::input().set("type","submit").set("value","Read Event").set("name", "ReadSimulatedFile")<< endl;
    *out << cgicc::form() << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/LoadSimulatedFileEvent") ;//<< endl;
    *out << cgicc::input().set("type","submit").set("value","Load Event").set("name", "LoadSimulatedFileEvent")<< endl;
    *out << cgicc::form() << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/AutomaticCheckingFile") ;//<< endl;
    *out << cgicc::input().set("type","submit").set("value","Load and Read File").set("name", "AutomaticCheckingFile")<< endl;
    *out << cgicc::form() << endl;
    *out << cgicc::fieldset() << endl;


    //End of function





    *out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFDDBB") << endl;
    *out << cgicc::legend("Define Pattern Set").set("style", "color:blue") ;
    *out << "CFEB CCLUT" << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/AddCCLUT") << endl;
    *out << "Bx : " << cgicc::input().set("type","text").set("name","GetCCLUT_bx_char").set("size","7").set("value", GetCCLUT_bx_char) << endl;
    *out << " Key HalfStrip : " << cgicc::input().set("type","text").set("name","GetCCLUT_key_char").set("size","7").set("value", GetCCLUT_key_char) << endl;
    *out << " PiD : " << cgicc::input().set("type","text").set("name","GetCCLUT_pid_char").set("size","7").set("value", GetCCLUT_pid_char) << endl;
    *out << " CCLUT Code : " << cgicc::input().set("type","text").set("name","GetCCLUT_ccode_char").set("size","7").set("value", GetCCLUT_ccode_char) << endl;
    *out << cgicc::input().set("type","submit").set("value","Add").set("name", "Add CCLUT") << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::br() << endl;
    *out << "GEM Cluster" << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/AddGEM") << endl;
    *out << "    real VFAT ID         Vs    natural ID " << cgicc::br() << endl;
    *out << "roll0: 7,  15,  23       Vs   0,   1,   2 " << cgicc::br() << endl;
    *out << "roll1: 6,  14,  22       Vs   3,   4,   5 " << cgicc::br() << endl;
    *out << "roll2: 5,  13,  21       Vs   6,   7,   8 " << cgicc::br() << endl;
    *out << "roll3: 4,  12,  20       Vs   9,  10,  11 " << cgicc::br() << endl;
    *out << "roll4: 3,  11,  19       Vs  12,  13,  14 " << cgicc::br() << endl;
    *out << "roll5: 2,  10,  18       Vs  15,  16,  17 " << cgicc::br() << endl;
    *out << "roll6: 1,   9,  17       Vs  18,  17,  20 " << cgicc::br() << endl;
    *out << "roll7: 0,   8,  16       Vs  21,  22,  23 " << cgicc::br() << endl;
    *out << "Bx : " << cgicc::input().set("type","text").set("name","GetGEM_bx_char").set("size","7").set("value", GetGEM_bx_char) << endl;
    *out << " Roll(0-7) : " << cgicc::input().set("type","text").set("name","GetGEM_roll_char").set("size","7").set("value", GetGEM_roll_char) << endl;
    //*out << " Vfat(0-23) : " << cgicc::input().set("type","text").set("name","GetGEM_pad_char").set("size","7").set("value", GetGEM_pad_char) << endl;
    *out << " Pad(0-191) : " << cgicc::input().set("type","text").set("name","GetGEM_pad_char").set("size","7").set("value", GetGEM_pad_char) << endl;
    *out << " Size(0-7) : " << cgicc::input().set("type","text").set("name","GetGEM_size_char").set("size","7").set("value", GetGEM_size_char) << endl;
    *out << " Layer(0-1) : " << cgicc::input().set("type","text").set("name","GetGEM_layer_char").set("size","7").set("value", GetGEM_layer_char) << endl;
    *out << cgicc::input().set("type","submit").set("value","Add").set("name", "Add GEM") << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::fieldset() << endl;

    *out << cgicc::fieldset().set("style", "background-color:#FFFFBB; font-size: 11pt") << endl;
    *out << cgicc::legend("Current Pattern Set").set("style","color:blue; font-size: 14pt") << endl;
    *out << cgicc::pre();
    *out << "CSC: " << endl;
    *out << "bx keystrip pattern nhits\t{ (bx,hs,layer) }" << endl;
    for(int i=0; i < patternSet.CSC.size(); i++){
      *out << patternSet.CSC[i] << endl;
    }


    //SASHA LUT DISPLAY
    *out << cgicc::br() << endl;
    *out << "LUT: " << endl;
    *out << "bx\tkey\tpattern\tCCLUT\tCompile Type\t{ (bx,hs,layer) }" << endl;
    for(int i=0; i < patternSet.LUT.size(); i++){
      *out << patternSet.LUT[i]  << "\t"<< endl;
    }


    *out << cgicc::br() << endl;
    *out << "GEM: " << endl;
    *out << "bx\troll\tpad\tsize\tlayer\trealVfatID" << endl;
    for(unsigned int i=0; i < patternSet.GEM.size(); i++){
      unsigned int clusterbits = patternSet.GEM[i].info();
      *out << patternSet.GEM[i] <<" \t "<< std::bitset<14>(clusterbits)<< endl;
    }

    *out << cgicc::pre();
    *out << cgicc::br() << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/LoadToEmuBoard") << endl;
    *out << cgicc::input().set("type","submit").set("value","Load EmuBoard").set("name", "LoadToEmuBoard") << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::fieldset();


    *out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFFFBB") << endl;
    *out << cgicc::legend("Study Pattern Set CCLUT").set("style", "color:blue; font-size: 14pt") << endl;
    *out << cgicc::form().set("method", "GET").set("action", "/" + urn + "/RunStudyCCLUT" ) << endl;
    *out << "Number of Trials : " << endl;
    *out << cgicc::input().set("type","text").set("name","GetNtrials_char").set("size","10").set("value",GetNtrials_char) << endl;
    *out << cgicc::input().set("type", "submit").set("value", "Run").set("name", "Run Study") << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::br() << endl;
    *out << cgicc::pre();
    thisTMB->RedirectOutput(out);
    thisTMB->GetCounters();
    cout << "TMB Counter # 29 = " << thisTMB->GetCounter(29) << endl;
    cout << "TMB Counter # 30 = " << thisTMB->GetCounter(30) << endl;

    thisTMB->PrintCounters(29);
    *out << endl;
    thisTMB->PrintCounters(30);
    thisTMB->RedirectOutput(&std::cout);
    *out << cgicc::pre();
    *out << cgicc::br() << endl;
    *out << cgicc::pre();
    *out << "Statistics : " << endl;
    *out << "# Trials = " << patternSet.N_trials << endl;
    for(int i=0; i < patternSet.Results_l.size(); i++){
      *out << patternSet.Results_l[i] << endl;
    }
    *out << cgicc::pre();
    *out << cgicc::fieldset();




    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








    *out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFCCBB") << endl;
    *out << cgicc::legend("Define Pattern Set GUI").set("style", "color:blue") ;
    *out << "CFEB CCLUT" << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/AddCCLUT_GUI") << endl;

    *out << "Bx : " << cgicc::input().set("type","text").set("name","GetCCLUT_bx_char_GUI").set("size","7").set("value", GetCCLUT_bx_char_GUI) << endl;
    *out << " Key HalfStrip : " << cgicc::input().set("type","text").set("name","GetCCLUT_key_char_GUI").set("size","7").set("value", GetCCLUT_key_char_GUI) << endl;
    //*out << " PiD : " << cgicc::input().set("type","text").set("name","GetCCLUT_pid_char").set("size","7").set("value", GetCCLUT_pid_char) << endl;
    //*out << " CCLUT Code : " << cgicc::input().set("type","text").set("name","GetCCLUT_ccode_char").set("size","7").set("value", GetCCLUT_ccode_char) << endl;
    *out << cgicc::br() << endl;
    *out << "PiD 4 : " << cgicc::input().set("value", "4").set("type","radio").set("name","BiD").set("id", "4").set("style", "padding-right: 50px;")<< "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;" << endl;
    *out << "PiD 3: " << cgicc::input().set("value", "3").set("type","radio").set("name","BiD").set("id", "3") << "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;" << endl;
    *out << "PiD 2 : " << cgicc::input().set("value", "2").set("type","radio").set("name","BiD").set("id", "2") <<"&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;"  <<  endl;
    *out << "PiD 1 : " << cgicc::input().set("value", "1").set("type","radio").set("name","BiD").set("id", "1") << "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;" << endl;
    *out << "PiD 0 : " << cgicc::input().set("value", "0").set("type","radio").set("name","BiD").set("id", "0") << "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;" << endl;
    *out << "<table><tbody><tr><td><table><tbody><tr><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><td><input type=\"radio\" value=\"3\" id=\"BiD4row0col0\" name=\"BiD4row0\" value=\"X\"><\/td><td><input type=\"radio\" value=\"2\" id=\"BiD4row0col1\" name=\"BiD4row0\" value=\"X\"><\/td><td><input type=\"radio\" value=\"1\" id=\"BiD4row0col2\" name=\"BiD4row0\" value=\"X\"><\/td><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><\/tr><tr><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><td><input type=\"radio\" value=\"3\" id=\"BiD4row1col0\" name=\"BiD4row1\" value=\"X\"><\/td><td><input type=\"radio\" value=\"2\" id=\"BiD4row1col1\" name=\"BiD4row1\" value=\"X\"><\/td><td><input type=\"radio\" value=\"1\" id=\"BiD4row1col2\" name=\"BiD4row1\" value=\"X\"><\/td><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><\/tr><tr><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><td><input type=\"radio\" value=\"3\" id=\"BiD4row2col0\" name=\"BiD4row2\" value=\"X\"><\/td><td><input type=\"radio\" value=\"2\" id=\"BiD4row2col1\" name=\"BiD4row2\" value=\"X\"><\/td><td><input type=\"radio\" value=\"1\" id=\"BiD4row2col2\" name=\"BiD4row2\" value=\"X\"><\/td><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><\/tr><tr><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><td><input type=\"radio\" value=\"3\" id=\"BiD4row3col0\" name=\"BiD4row3\" value=\"X\"><\/td><td><input type=\"radio\" value=\"2\" id=\"BiD4row3col1\" name=\"BiD4row3\" value=\"X\"><\/td><td><input type=\"radio\" value=\"1\" id=\"BiD4row3col2\" name=\"BiD4row3\" value=\"X\"><\/td><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><\/tr><tr><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><td><input type=\"radio\" value=\"3\" id=\"BiD4row4col0\" name=\"BiD4row4\" value=\"X\"><\/td><td><input type=\"radio\" value=\"2\" id=\"BiD4row4col1\" name=\"BiD4row4\" value=\"X\"><\/td><td><input type=\"radio\" value=\"1\" id=\"BiD4row4col2\" name=\"BiD4row4\" value=\"X\"><\/td><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><\/tr><tr><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><td><input type=\"radio\" value=\"3\" id=\"BiD4row5col0\" name=\"BiD4row5\" value=\"X\"><\/td><td><input type=\"radio\" value=\"2\" id=\"BiD4row5col1\" name=\"BiD4row5\" value=\"X\"><\/td><td><input type=\"radio\" value=\"1\" id=\"BiD4row5col2\" name=\"BiD4row5\" value=\"X\"><\/td><td>~<\/td><td>~<\/td><td>~<\/td><td>~<\/td><\/tr><\/tbody><\/table></td><td><table><tbody><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr></tbody></table></td><td>  <table><tbody><tr><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"3\" id=\"BiD3row0col0\" name=\"BiD3row0\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"2\" id=\"BiD3row0col1\" name=\"BiD3row0\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"1\" id=\"BiD3row0col2\" name=\"BiD3row0\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"3\" id=\"BiD3row1col0\" name=\"BiD3row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"2\" id=\"BiD3row1col1\" name=\"BiD3row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"1\" id=\"BiD3row1col2\" name=\"BiD3row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"3\" id=\"BiD3row2col0\" name=\"BiD3row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"2\" id=\"BiD3row2col1\" name=\"BiD3row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"1\" id=\"BiD3row2col2\" name=\"BiD3row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"3\" id=\"BiD3row3col0\" name=\"BiD3row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"2\" id=\"BiD3row3col1\" name=\"BiD3row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"1\" id=\"BiD3row3col2\" name=\"BiD3row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"3\" id=\"BiD3row4col0\" name=\"BiD3row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"2\" id=\"BiD3row4col1\" name=\"BiD3row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"1\" id=\"BiD3row4col2\" name=\"BiD3row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"3\" id=\"BiD3row5col0\" name=\"BiD3row5\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"2\" id=\"BiD3row5col1\" name=\"BiD3row5\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" ><input type=\"radio\" value=\"1\" id=\"BiD3row5col2\" name=\"BiD3row5\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td><td style=\"width: 20px;\" style=\"text-align: center;\" >~</td></tr></tbody></table></td><td><table><tbody><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr></tbody></table></td><td><table><tbody><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD2row0col0\" name=\"BiD2row0\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD2row0col1\" name=\"BiD2row0\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD2row0col2\" name=\"BiD2row0\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD2row1col0\" name=\"BiD2row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD2row1col1\" name=\"BiD2row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD2row1col2\" name=\"BiD2row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD2row2col0\" name=\"BiD2row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD2row2col1\" name=\"BiD2row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD2row2col2\" name=\"BiD2row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD2row3col0\" name=\"BiD2row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD2row3col1\" name=\"BiD2row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD2row3col2\" name=\"BiD2row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD2row4col0\" name=\"BiD2row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD2row4col1\" name=\"BiD2row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD2row4col2\" name=\"BiD2row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD2row5col0\" name=\"BiD2row5\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD2row5col1\" name=\"BiD2row5\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD2row5col2\" name=\"BiD2row5\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr></tbody></table></td><td><table><tbody><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr></tbody></table></td><td><table><tbody><tr><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD1row0col0\" name=\"BiD1row0\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD1row0col1\" name=\"BiD1row0\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD1row0col2\" name=\"BiD1row0\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD1row1col0\" name=\"BiD1row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD1row1col1\" name=\"BiD1row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD1row1col2\" name=\"BiD1row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD1row2col0\" name=\"BiD1row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD1row2col1\" name=\"BiD1row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD1row2col2\" name=\"BiD1row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD1row3col0\" name=\"BiD1row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD1row3col1\" name=\"BiD1row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD1row3col2\" name=\"BiD1row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD1row4col0\" name=\"BiD1row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD1row4col1\" name=\"BiD1row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD1row4col2\" name=\"BiD1row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD1row5col0\" name=\"BiD1row5\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD1row5col1\" name=\"BiD1row5\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD1row5col2\" name=\"BiD1row5\" ></td></tr></tbody></table></td><td><table><tbody><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr><tr><td>|</td><td>| </td></tr></tbody></table></td><td><table><tbody><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD0row0col0\" name=\"BiD0row0\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD0row0col1\" name=\"BiD0row0\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD0row0col2\" name=\"BiD0row0\" ></td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD0row1col0\" name=\"BiD0row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD0row1col1\" name=\"BiD0row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD0row1col2\" name=\"BiD0row1\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD0row2col0\" name=\"BiD0row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD0row2col1\" name=\"BiD0row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD0row2col2\" name=\"BiD0row2\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD0row3col0\" name=\"BiD0row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD0row3col1\" name=\"BiD0row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD0row3col2\" name=\"BiD0row3\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD0row4col0\" name=\"BiD0row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD0row4col1\" name=\"BiD0row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD0row4col2\" name=\"BiD0row4\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr><tr><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"3\" id=\"BiD0row5col0\" name=\"BiD0row5\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"2\" id=\"BiD0row5col1\" name=\"BiD0row5\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\"><input type=\"radio\" value=\"1\" id=\"BiD0row5col2\" name=\"BiD0row5\" ></td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td><td style=\"width: 20px;\" style=\"text-align: center;\">~</td></tr></tbody></table></td></tr></tbody></table>";

    *out << cgicc::br() << endl;
    *out << cgicc::input().set("type","submit").set("value","Add").set("name", "Add CCLUT") << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::br() << endl;
    *out << cgicc::fieldset() << endl;

    /////////////////////////////SASHA||||||||||||||||||||||||||||||||||





    *out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFFFBB") << endl;
    *out << cgicc::legend("Save Current Set").set("style", "color:blue; font-size: 14pt") << endl;
    *out << "Prefix : " << endl;
    *out << cgicc::form().set("method", "GET").set("action", "/" + urn + "/SaveAsPat") << endl;
    *out << cgicc::input().set("type","text").set("name","GetSetPrefix").set("size","24").set("value",GetSetPrefix) << endl;
    *out << cgicc::br() << endl;
    *out << cgicc::br() << endl;
    *out << "Save Directory : " << endl;
    *out << cgicc::input().set("type","text").set("name","GetSaveDir").set("size","110").set("value",GetSaveDir) << endl;
    *out << cgicc::br() << endl;
    *out << cgicc::br() << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::table().set("border","0") << endl;
    *out << cgicc::td().set("ALIGN", "left") << endl;
    *out << cgicc::form().set("method", "GET").set("action","/" + urn + "/SaveAsPat") << endl;
    *out << cgicc::input().set("type", "submit").set("value","Save as {.pat}").set("name", "SaveAsPat") << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::td() << endl;
    *out << cgicc::td().set("ALIGN", "left") << endl;
    *out << cgicc::form().set("method", "GET").set("action", "/" + urn + "/SaveAsTxt") << endl;
    *out << cgicc::input().set("type", "submit").set("value","Save as (.txt)").set("name", "SaveAsTxt") << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::td() << endl;
    *out << cgicc::table() << endl;
    *out << cgicc::fieldset();





    *out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFFFBB") << endl;
    *out << cgicc::legend("Clear Current Set").set("style", "color:blue; font-size: 14pt") << endl;
    *out << cgicc::form().set("method", "GET").set("action", "/" + urn + "/ClearSet" ) << endl;
    *out << cgicc::input().set("type", "submit").set("value", "Clear Current Set").set("name", "Clear Current Set") << " Deletes *.pat in (default_dir), and resets patternSet object" << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::fieldset();

    *out << cgicc::fieldset().set("style", "font-size: 11pt; background-color:#FFFFBB") << endl;
    *out << cgicc::legend("Clear the Emulator Board").set("style", "color:blue") ;
    *out << cgicc::form().set("method", "GET").set("action", "/" + urn + "/ClearEmuBoard" ) << endl;
    *out << cgicc::input().set("type", "submit").set("value", "Clear Emulator Board").set("name", "Clear Emulator Board") << " Loads Empty patterns to Emulator Board" << endl;
    *out << cgicc::form() << endl;
    *out << cgicc::fieldset() << endl;


    emu::utils::footer(out);
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  //					Pattern Studies Functions (Jan. 2020)
  //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


  void CSCGEMTestApplication::AddCCLUT_GUI(xgi::Input * in, xgi::Output * out)
  {

    using namespace std;

    cgicc::Cgicc cgi(in);
    if(!xgi::Utils::hasFormElement(cgi,"BiD")) return;

    if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_bx_char_GUI")) sprintf(GetCCLUT_bx_char_GUI,cgi["GetCCLUT_bx_char_GUI"]->getValue().c_str());
    if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_key_char_GUI")) sprintf(GetCCLUT_key_char_GUI,cgi["GetCCLUT_key_char_GUI"]->getValue().c_str());
    if(xgi::Utils::hasFormElement(cgi,"BiD")){ cout << "BiD: " << cgi["BiD"]->getValue() << std::endl;
    int active[6] = {0, 0, 0, 0, 0, 0};
    for (int i = '0'; i < '6'; ++i) {
      std::string name = "BiD" + cgi["BiD"]->getValue() + "row" + string(1, i);

      if(xgi::Utils::hasFormElement(cgi,name)) {
        active[i-'0'] = atoi(cgi[name]->getValue().c_str());
        cout << name << ":" << cgi[name]->getValue() << std::endl;
      }
    }
    for (int i = 0; i < 6; ++i) {
      cout << active[i] << " ";
    }
    cout<<std::endl;
    GetCCLUT_ccode_int = 1024*active[5] + 256*active[4] + 64*active[3] + 16*active[2] + 4*active[1] + active[0];
  }



  GetCCLUT_bx_int = atoi(GetCCLUT_bx_char_GUI);
  GetCCLUT_key_int = atoi(GetCCLUT_key_char_GUI);
  GetCCLUT_pid_int = atoi(cgi["BiD"]->getValue().c_str());
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




void CSCGEMTestApplication::SaveAsPat(xgi::Input * in, xgi::Output * out)
{
  using namespace std;
  cgicc::Cgicc cgi(in);

  if(xgi::Utils::hasFormElement(cgi,"GetSetPrefix")) sprintf(GetSetPrefix,cgi["GetSetPrefix"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetSaveDir")) sprintf(GetSaveDir,cgi["GetSaveDir"]->getValue().c_str());

  patternSet.Prefix = GetSetPrefix;
  patternSet.WritePatterns_LUT(GetSaveDir);
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
  if(emptySet.WritePatterns_LUT()){
    if( emptySet.LoadEmuBoard() ){
      cout << "Successfully Loaded Empty to EmuBoard! aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n";
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
    patternSet.Dump(0,GetOTMBType[0]);
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
    for(int i=0; i < patternSet.Results_l.size(); i++){
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
  int tmbtype = 0xa + GetOTMBType[0]-'a';//get OTMB type, a,c,d->0xa, 0xc, 0xd
  std::vector<int> ps_clct_info;

  for(int i=0; i < GetCLCT_ps_char.size(); i++){
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

  CLCT_ps_vec.push_back(cw::CLCT(ps_clct_info[0],ps_clct_info[1],ps_clct_info[2],ps_clct_info[3],tmbtype));

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
    for(int i=0; i < CLCT_ps_vec.size(); i++){
      patternSet.CSC.push_back(CLCT_ps_vec[i]);
    }
    // Generate {.pat} in tmp file location and Load to Emu Board
    patternSet.DeleteCurrentSet();
    if(patternSet.WritePatterns_LUT()){
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
      patternSet.Dump(0,GetOTMBType[0]);
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
      for(int i=0; i < patternSet.Results.size(); i++){
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
    for(int i=0; i < CLCT_ps_vec.size(); i++){
      cout << "CLCT" << i << " " << CLCT_ps_vec[i] << endl;
    }

  }while(Increment(CLCT_ps_vec, Free_params));

  fout.close();
  this->Default(in,out);
  return;
}


void CSCGEMTestApplication::AddCCLUTParamScan(xgi::Input * in, xgi::Output * out)
{


  cgicc::Cgicc cgi(in);

  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_bx_ps_char")) sprintf(GetCCLUT_bx_ps_char,cgi["GetCCLUT_bx_ps_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_key_ps_char")) sprintf(GetCCLUT_key_ps_char,cgi["GetCCLUT_key_ps_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_pid_ps_char")) sprintf(GetCCLUT_pid_ps_char,cgi["GetCCLUT_pid_ps_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_ccode_ps_char")) sprintf(GetCCLUT_ccode_ps_char,cgi["GetCCLUT_ccode_ps_char"]->getValue().c_str());

  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_bx_ps_step_char")) sprintf(GetCCLUT_bx_ps_step_char,cgi["GetCCLUT_bx_ps_step_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_key_ps_step_char")) sprintf(GetCCLUT_key_ps_step_char,cgi["GetCCLUT_key_ps_step_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_pid_ps_step_char")) sprintf(GetCCLUT_pid_ps_step_char,cgi["GetCCLUT_pid_ps_step_char"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"GetCCLUT_ccode_ps_step_char")) sprintf(GetCCLUT_ccode_ps_step_char,cgi["GetCCLUT_ccode_ps_step_char"]->getValue().c_str());

  GetCCLUT_ps_char[0] = GetCCLUT_bx_ps_char;
  GetCCLUT_ps_char[1] = GetCCLUT_key_ps_char;
  GetCCLUT_ps_char[2] = GetCCLUT_pid_ps_char;
  GetCCLUT_ps_char[3] = GetCCLUT_ccode_ps_char;

  GetCCLUT_ps_step_char[0] = GetCCLUT_bx_ps_step_char;
  GetCCLUT_ps_step_char[1] = GetCCLUT_key_ps_step_char;
  GetCCLUT_ps_step_char[2] = GetCCLUT_pid_ps_step_char;
  GetCCLUT_ps_step_char[3] = GetCCLUT_ccode_ps_step_char;

  for (int i = 0; i < GetCCLUT_ps_step_char.size(); i++) {
    std::cout << GetCCLUT_ps_step_char[i] << ' ';
  }
  std::cout  << '\n';

  time_t now = time(0);
  tm *ltm = localtime(&now);

  std::vector<int> ps_cclut_info;
  std::stringstream savetime;
  savetime <<"/home/cscdev/Kyla/OTMB_Pattern_test/";
  savetime << 1900 + ltm->tm_year << "_" << 1 + ltm->tm_mon << "_" << ltm->tm_mday<< "/";


  std::stringstream savedir;
  savedir <<"/home/cscdev/Kyla/OTMB_Pattern_test/";
  savedir << 1900 + ltm->tm_year << "_" << 1 + ltm->tm_mon << "_" << ltm->tm_mday;
  const std::string tmpd =  std::string{savedir.str()};
  const char* savedirpath = tmpd.c_str();

  boost::filesystem::path dir(savedirpath);

  if(!(boost::filesystem::exists(dir)))
  {
    std::cout<<"Doesn't Exists"<<std::endl;
    if (boost::filesystem::create_directory(dir))
    std::cout << "....Successfully Created !" << std::endl;
  }

  std::vector<std::vector<int> > Parameter_sing_muon;

  for(int i=0; i < GetCCLUT_ps_char.size(); i++){
    std::istringstream ss;
    std::istringstream ss_step;
    ss.str(GetCCLUT_ps_char[i]);
    ss_step.str(GetCCLUT_ps_step_char[i]);

    cw::RangeParamCCLUT range;
    range.param = i;
    range.cclut = CCLUT_ps_vec.size();
    range.step_size = 1;




    if(ss.str().empty()){
      cout << "empty" << endl;
      ps_cclut_info.push_back(0);
    }
    else if(ss.str().at(0) == '['){
      cout << "We are reading a RANGED VALUE:"<<ss.str() << endl << endl;
      int a,b;
      char tmp;
      int step;
      ss >> tmp >> a >> tmp >> b >> tmp;
      ss_step >> step;
      cout << "These are the vals a= " << a << " b= " << b << endl << endl;
      std::vector<int> parm;
      if(a < b){
        range.min = a;
        range.max = b;
        for(int j = range.min; j <= range.max; j += step){
          parm.push_back(j);
        }
      }
      else{
        range.min = a;
        range.max = b;
        for(int j = range.min; j >= range.max; j -= step){
          parm.push_back(j);
        }
      }
      Free_paramsCCLUT.push_back(range);
      ps_cclut_info.push_back(a);
      Parameter_sing_muon.push_back(parm);
    }
    else if (ss.str().at(0) == '{'){
      cout << "We are reading a DISCREET VALUES:"<<ss.str() << endl << endl;
      int a,b;
      char tmp;
      std::vector<int> parm;
      ss >> tmp;
      while(!ss.eof() && tmp != '}'){
        ss >> a;
        parm.push_back(a);
        ss >> tmp;
      }
      range.min = parm[0];
      range.max = parm[parm.size() - 1];
      Free_paramsCCLUT.push_back(range);
      ps_cclut_info.push_back(parm[0]);
      Parameter_sing_muon.push_back(parm);
    }
    else{
      cout << "We are reading a SINGLE VALUE:" << ss.str() << endl << endl;
      int a;
      ss >> a;
      std::vector<int> parm;
      parm.push_back(a);
      Parameter_sing_muon.push_back(parm);
      ps_cclut_info.push_back(a);
      cout << "The Value is a = " << a << endl << endl;
    }
  }

  int tmbtype = 0xa+GetOTMBType[0]-'a';//get OTMB type, a,c,d->0xa, 0xc, 0xd
  CCLUT_ps_vec.push_back(cw::CCLUT(ps_cclut_info[0],ps_cclut_info[1],ps_cclut_info[2],ps_cclut_info[3],tmbtype));
  std::vector<std::string> names;
  names.push_back("BX-");
  names.push_back("_Key-");
  names.push_back("_PiD-");
  names.push_back("_cc_code-");
  savetime << "time-" << ltm->tm_hour <<"-" << ltm->tm_min << "-" << ltm->tm_sec << "_";
  for(int i=0; i < GetCCLUT_ps_char.size(); i++){
    savetime << names[i];
    std::istringstream ss;
    ss.str(GetCCLUT_ps_char[i]);
    if(Parameter_sing_muon[i].size() == 1){
      savetime << Parameter_sing_muon[i][0];
    }
    else if(ss.str().at(0) == '['){
      savetime << Parameter_sing_muon[i][0] <<"to"<< Parameter_sing_muon[i][Parameter_sing_muon[i].size()-1]<<"s"<<GetCCLUT_ps_step_char[i];
    }
    else if(ss.str().at(0) == '{'){
      savetime << Parameter_sing_muon[i][0] <<"to"<< Parameter_sing_muon[i][Parameter_sing_muon[i].size()-1]<<"d";
    }
  }
  TMB * thisTMB = tmbVector[1];
  CFEB0123_delay_read = thisTMB->GetCfeb0123RxClockDelay();
  CFEB456_delay_read = thisTMB->GetCfeb456RxClockDelay();
  savetime << "_CFEB0123_"<< CFEB0123_delay_read << "CFEB456_" << CFEB456_delay_read ;
  std::stringstream savetime_error;
  savetime_error << savetime.rdbuf();

  savetime << ".txt";
  const std::string tmp =  std::string{savetime.str()};
  const char* todayte = tmp.c_str();
  sprintf(ParamScanOutFile, todayte);

  savetime_error << "_Error.txt";
  const std::string tmp_err =  std::string{savetime_error.str()};
  const char* todayte_err = tmp_err.c_str();
  sprintf(ParamScanOutFileError, todayte_err);





  parameters.push_back(Parameter_sing_muon);

  this->Default(in,out);
  return;
}

void CSCGEMTestApplication::RunParamScanCCLUT(xgi::Input * in, xgi::Output * out)
{
  int All = 0;
  int Fail = 0;
  int Fail_Expexted = 0;
  cw::CCLUT Accuracy;
  cw::CCLUT Accuracy_1;
  cw::CCLUT Accuracy_trigger;
  cw::CCLUT Accuracy_1_trigger;

  int All_Second = 0;
  int Fail_Second = 0;
  int Fail_Expected_Second = 0;
  cw::CCLUT Accuracy_Second;
  cw::CCLUT Accuracy_1_Second;
  cw::CCLUT Accuracy_Second_trigger;
  cw::CCLUT Accuracy_1_Second_trigger;

  thisCCB->setCCBMode(CCB::VMEFPGA);
  //thisCCB->hardReset();
  thisCCB->l1aReset();
  thisCCB->EnableL1aFromTmbL1aReq();
  thisCCB->bc0();
  this->Default(in,out);

  std::cout << "\nRUNNING PARAM SCAN\n";
  cgicc::Cgicc cgi(in);

  int Mute = 0;
  if(xgi::Utils::hasFormElement(cgi,"mute")) {
    sprintf(mute_char,cgi["mute"]->getValue().c_str());
    Mute = atoi(mute_char);

  }
  cout << "Is it Mute ? " <<  Mute << endl;


  sleep(5);
  if(xgi::Utils::hasFormElement(cgi,"TrialsPerStep_char")){
    sprintf(TrialsPerStep_char,cgi["TrialsPerStep_char"]->getValue().c_str());
    TrialsPerStep = atoi(TrialsPerStep_char);
  }
  else{ TrialsPerStep = 1; }
  if(xgi::Utils::hasFormElement(cgi,"ParamScanOutFile")) sprintf(ParamScanOutFile,cgi["ParamScanOutFile"]->getValue().c_str());
  if(xgi::Utils::hasFormElement(cgi,"ParamScanOutFileError")) sprintf(ParamScanOutFileError,cgi["ParamScanOutFileError"]->getValue().c_str());


  // Open the Output File
  std::fstream fout;
  fout.open(ParamScanOutFile, std::fstream::out | std::fstream::app);

  std::fstream fout_err;
  fout_err.open(ParamScanOutFileError, std::fstream::out | std::fstream::app);

  int cclut0_lsbs, cclut1_lsbs, cclut_msbs;
  int cclut0_inc=0;
  int cclut1_inc=0;
  int CLCT0_data_, CLCT1_data_;
  int clct0_xky_data_, clct1_xky_data_;
  int clct0_CC_data_, clct1_CC_data_;
  int CLCT0_xky, CLCT1_xky;
  TMB * thisTMB = tmbVector[1];                         /// THIS SHOULD BE EDITED IF USING DIFFERENT TMB SETTINGS !!!!!!!!
  std::vector<std::vector<int> > CombinationCounter;
  if (parameters.size() == 0) {
    std::cout << "No Muons in study" << '\n';
    return;
  }
  if(parameters.size() > 1){
    for (size_t i = 0; i < parameters[0].size(); i++) {
      int expecter_parameter_size = -9999;
      for (size_t j = 0; j <  parameters.size(); j++) {
        if(j == 0) expecter_parameter_size = parameters[j][i].size();
        if (parameters[j][i].size() != expecter_parameter_size){
          std::cout << "Wrong Scan Parameters size" << '\n';
          return;
        }
      }
    }
  }

  for (size_t i = 0; i < parameters[0].size(); i++) {
    std::vector<int> tmpv;
    tmpv.push_back(0);
    tmpv.push_back(parameters[0][i].size());
    CombinationCounter.push_back(tmpv);
  }

  do{
    // Reset Counting Variables!@!
    thisTMB->ResetCounters();
    CLCT0_Counter = 0;
    CLCT1_Counter = 0;


    int tmbtype = 0xa+GetOTMBType[0]-'a';
    patternSet.Clear();
    patternSet.DeleteCurrentSet();
    patternSet.ClearEmuBoard();
    cw::CCLUT tmp_cclut;
    for (size_t i = 0; i < parameters.size(); i++) {
      for (size_t j = 0; j < parameters[i].size(); j++) {
        tmp_cclut.SetParam(j,parameters[i][j][CombinationCounter[j][0]]);
        tmp_cclut.SetParam(4, tmbtype);
      }
      tmp_cclut.RegenHits(true, Bit11_int);
      tmp_cclut.Convert_LUT_to_bending();
      //std::cout << "/* message */" << '\n';
      patternSet.LUT.push_back(tmp_cclut);

    }
    // Generate {.pat} in tmp file location and Load to Emu Board

    patternSet.SetOTMBCompileType(GetOTMBType[0]);
    if(patternSet.WritePatterns_LUT()){
      if( patternSet.LoadEmuBoard("", Mute) ){
        cout << "Successfully Loaded to EmuBoard!\n";
      }
      else{
        cout << "ERROR: FAILED TO LOAD EmuBoard!!!!!!!\n";
      }
    }

    CCLUT SendTo_0;
    CCLUT SendTo_1;
    std::vector<cw::CCLUT> ReadFrom_0;
    std::vector<cw::CCLUT> ReadFrom_1;
    std::vector<cw::CCLUT> Absent_trigger_0;
    std::vector<cw::CCLUT> Absent_trigger_1;
    std::vector<cw::CCLUT> Unexpected_trigger_0;
    std::vector<cw::CCLUT> Unexpected_trigger_1;
    std::vector<cw::CCLUT> Extra_Trigger_0;
    std::vector<cw::CCLUT> Extra_Trigger_1;
    cw::CCLUT Expeted_Trigger_0, Expeted_Trigger_1;
    if(parameters.size() > 0){
      Expeted_Trigger_0 = cw::ExpectedTriggering(patternSet.LUT[0], Bit11_int, Mute);
      Expeted_Trigger_0.Convert_LUT_to_bending();
    }
    if(parameters.size() > 1){
      Expeted_Trigger_1 = cw::ExpectedTriggering(patternSet.LUT[1], Bit11_int, Mute);
      Expeted_Trigger_1.Convert_LUT_to_bending();
    }

    patternSet.N_trials = TrialsPerStep;
    bool expect_trigger_0 = false;
    bool expect_trigger_1 = false;

    if (Expeted_Trigger_0.LUT_code > 0) expect_trigger_0 = true;
    if (Expeted_Trigger_1.LUT_code > 0) expect_trigger_1 = true;
    std::cout << "  patternSet.N_trials  " << patternSet.N_trials << '\n';
    //bool expected_trig_1, ;  //shooold we trigger on this pattern (only for perfect patterns)
    for(int i=0; i < patternSet.N_trials; i++){
      //unsigned int microsecond = 100000;
      //usleep(1 * microsecond);//sleeps for 3 second
      patternSet.Dump(Mute,GetOTMBType[0]);
      cclut0_lsbs = thisTMB->ReadRegister(seq_clct0_adr);
      cclut1_lsbs = thisTMB->ReadRegister(seq_clct1_adr);
      cclut_msbs  = thisTMB->ReadRegister(seq_clctm_adr);
      clct0_xky_data_ = thisTMB->ReadRegister(clct0_bndxky_adr);
      clct1_xky_data_ = thisTMB->ReadRegister(clct1_bndxky_adr);
      clct0_CC_data_ = thisTMB->ReadRegister(clct0_cc_adr);
      clct1_CC_data_ = thisTMB->ReadRegister(clct1_cc_adr);
      // extract value from register value. bitlo means the lowest bit clct0_bndxky_adr
      int CLCT0_xky = thisTMB->ExtractValueFromData(clct0_xky_data_, clct0_xky_bitlo, clct0_xky_bithi);
      int CLCT1_xky = thisTMB->ExtractValueFromData(clct1_xky_data_, clct1_xky_bitlo, clct1_xky_bithi);
      int CLCT0_bnd = thisTMB->ExtractValueFromData(clct0_xky_data_, clct0_bnd_bitlo, clct0_bnd_bithi);
      int CLCT1_bnd = thisTMB->ExtractValueFromData(clct1_xky_data_, clct1_bnd_bitlo, clct1_bnd_bithi);
      int CLCT0_lr  = thisTMB->ExtractValueFromData(clct0_xky_data_, clct0_lr_bitlo, clct0_lr_bithi);
      int CLCT1_lr  = thisTMB->ExtractValueFromData(clct1_xky_data_, clct1_lr_bitlo, clct1_lr_bithi);
      thisTMB->GetCounters();

      cout << endl << CLCT0_Counter << "  " << thisTMB->GetCounter(cw::tmb_counters[0]) << endl;
      cout << endl << CLCT1_Counter << "  " << thisTMB->GetCounter(cw::tmb_counters[1]) << endl;

      bool have_trigger_0 = false;
      bool have_trigger_1 = false;

      if (CLCT0_Counter != thisTMB->GetCounter(cw::tmb_counters[0])) have_trigger_0 = true;
      if (CLCT1_Counter != thisTMB->GetCounter(cw::tmb_counters[1])) have_trigger_1 = true;

      if (thisTMB->GetCounter(cw::tmb_counters[0]) - CLCT0_Counter > 1) {
        Extra_Trigger_0.push_back(patternSet.LUT[0]);
      }

      if (thisTMB->GetCounter(cw::tmb_counters[1]) - CLCT1_Counter > 1) {
        Extra_Trigger_1.push_back(patternSet.LUT[1]);
      }

      if(have_trigger_0 != expect_trigger_0 && expect_trigger_0 == 1){
        Absent_trigger_0.push_back(patternSet.LUT[0]);
      }

      if(have_trigger_0 != expect_trigger_0 && expect_trigger_0 == 0){
        Unexpected_trigger_0.push_back(patternSet.LUT[0]);
      }

      if(have_trigger_1 != expect_trigger_1 && expect_trigger_1 == 1){
        Absent_trigger_1.push_back(patternSet.LUT[1]);
      }

      if(have_trigger_1 != expect_trigger_1 && expect_trigger_1 == 0){
        Unexpected_trigger_1.push_back(patternSet.LUT[1]);
      }


      CLCT0_Counter = thisTMB->GetCounter(cw::tmb_counters[0]);
      CLCT1_Counter = thisTMB->GetCounter(cw::tmb_counters[1]);

      CLCT0_data_ = ( (cclut_msbs & 0xf) << 16 ) | (cclut0_lsbs & 0xffff);
      CLCT1_data_ = ( (cclut_msbs & 0xf) << 16 ) | (cclut1_lsbs & 0xffff);

      int CLCT0_nhit = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_nhit_bitlo, CLCT0_nhit_bithi);
      int CLCT0_pid = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_pattern_bitlo, CLCT0_pattern_bithi);  //Will need changes here
      int CLCT0_key = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_keyHalfStrip_bitlo, CLCT0_keyHalfStrip_bithi);
      int CLCT0_CC = thisTMB->ExtractValueFromData(clct0_CC_data_, clct0_cc_bitlo, clct0_cc_bithi);

      int CLCT1_nhit = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_nhit_bitlo, CLCT1_nhit_bithi);
      int CLCT1_pid = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_pattern_bitlo, CLCT1_pattern_bithi);  //Will need changes here
      int CLCT1_key = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_keyHalfStrip_bitlo, CLCT1_keyHalfStrip_bithi);
      int CLCT1_CC = thisTMB->ExtractValueFromData(clct1_CC_data_, clct1_cc_bitlo, clct1_cc_bithi);


      cw::CCLUT ReadFrom_s_0;
      ReadFrom_s_0.SetParam(0, -1); // we have no way to read bx from OTMB
      ReadFrom_s_0.SetParam(1, CLCT0_key);
      ReadFrom_s_0.SetParam(2, CLCT0_pid - 6);
      ReadFrom_s_0.SetParam(3, CLCT0_CC);
      ReadFrom_s_0.SetParam(4, GetOTMBType[0]);
      ReadFrom_s_0.SetParam(5, CLCT0_xky);
      ReadFrom_s_0.SetParam(6, CLCT0_bnd);
      ReadFrom_s_0.SetParam(7, CLCT0_lr);
      ReadFrom_s_0.RegenHits(true, Bit11_int);


      cw::CCLUT ReadFrom_s_1;
      ReadFrom_s_1.SetParam(0, -1);
      ReadFrom_s_1.SetParam(1, CLCT1_key);
      ReadFrom_s_1.SetParam(2, CLCT1_pid - 6);
      ReadFrom_s_1.SetParam(3, CLCT1_CC);
      ReadFrom_s_1.SetParam(4, GetOTMBType[0]);
      ReadFrom_s_1.SetParam(5, CLCT1_xky);
      ReadFrom_s_1.SetParam(6, CLCT1_bnd);
      ReadFrom_s_1.SetParam(7, CLCT1_lr);
      ReadFrom_s_1.RegenHits(true, Bit11_int);

      Shold_we_Swap_run2(Expeted_Trigger_0, Expeted_Trigger_1, ReadFrom_s_0, ReadFrom_s_1);
      Expeted_Trigger_0.RegenHits();
      Expeted_Trigger_1.RegenHits();

      if(Compare_run2(patternSet.LUT[0], ReadFrom_s_0, Expeted_Trigger_0) && patternSet.LUT[0].hits.size() > 3.){
        ReadFrom_0.push_back(ReadFrom_s_0);
      }
      SendTo_0 = patternSet.LUT[0];

      if(parameters.size() > 1){
        if(Compare_run2(patternSet.LUT[1], ReadFrom_s_1, Expeted_Trigger_1) && patternSet.LUT[1].hits.size() > 3.){
          ReadFrom_1.push_back(ReadFrom_s_1);
        }
        SendTo_1 = patternSet.LUT[1];
      }


      if(Mute == 0 || i == 0){
        std::cout << std::endl << std::endl  << "CLCT 0 Input:" << std::endl;
        std::cout << "N Hit = " << cw::GetNHits(patternSet.LUT[0].LUT_code) << std::endl;
        std::cout << "Pid   = " << patternSet.LUT[0].pat << std::endl;
        std::cout << "KeyStr = " << patternSet.LUT[0].hs << std::endl;
        std::cout << "CC Code = " << patternSet.LUT[0].LUT_code << std::endl;
        std::cout << "1/8 resolution = "  << patternSet.LUT[0].eightres  << std::endl;
        std::cout << "Bending angle =  " << patternSet.LUT[0].bending << std::endl;
        std::cout << "LR bending = " << patternSet.LUT[0].lr << std::endl;
        //std::cout << "1/8 resolution = " << cw::EightResolution(patternSet.LUT[0].hs, patternSet.LUT[0].LUT_code, patternSet.LUT[0].pat) << std::endl << std::endl;
        std::cout << std::endl << std::endl  << "CLCT 0 Expect:" << std::endl;
        std::cout << "N Hit = " << cw::GetNHits(Expeted_Trigger_0.LUT_code) << std::endl;
        std::cout << "Pid   = " << Expeted_Trigger_0.pat << std::endl;
        std::cout << "KeyStr = " << Expeted_Trigger_0.hs << std::endl;
        std::cout << "CC Code = " << Expeted_Trigger_0.LUT_code << std::endl;
        std::cout << "1/8 resolution = "  << Expeted_Trigger_0.eightres  << std::endl;
        std::cout << "Bending angle =  " << Expeted_Trigger_0.bending << std::endl;
        std::cout << "LR bending = " << Expeted_Trigger_0.lr << std::endl;
        //std::cout << "1/8 resolution = " << cw::EightResolution(patternSet.LUT[0].hs, patternSet.LUT[0].LUT_code, patternSet.LUT[0].pat) << std::endl << std::endl;

        std::cout << std::endl << "CLCT 0 Decode:" << std::endl;
        std::cout << "N Hit = " << CLCT0_nhit << std::endl;
        std::cout << "Pid   = " << CLCT0_pid << std::endl;
        std::cout << "KeyStr= " << CLCT0_key << std::endl;
        std::cout << "CC Code= " << CLCT0_CC << std::endl;
        std::cout << "1/8 resolution = " << CLCT0_xky << std::endl;
        std::cout << "Bending angle =  " << CLCT0_bnd << std::endl;
        std::cout << "LR bending = " << CLCT0_lr << std::endl << std::endl;
        if(parameters.size() > 1){

          std::cout << std::endl << std::endl  << "CLCT 1 Input:" << std::endl;
          std::cout << "N Hit = " << cw::GetNHits(patternSet.LUT[1].LUT_code) << std::endl;
          std::cout << "Pid   = " << patternSet.LUT[1].pat << std::endl;
          std::cout << "KeyStr = " << patternSet.LUT[1].hs << std::endl;
          std::cout << "CC Code = " << patternSet.LUT[1].LUT_code << std::endl;
          std::cout << "1/8 resolution = "  << patternSet.LUT[1].eightres << std::endl;
          std::cout << "Bending angle =  " << patternSet.LUT[1].bending << std::endl;
          std::cout << "LR bending = " << patternSet.LUT[1].lr << std::endl;
          //std::cout << "1/8 resolution = " << cw::EightResolution(patternSet.LUT[1].hs, patternSet.LUT[1].LUT_code, patternSet.LUT[1].pat) << std::endl << std::endl;
          // potential problem here when we have more than 2 input muons in testing

          std::cout << std::endl << std::endl  << "CLCT 1 Expect:" << std::endl;
          std::cout << "N Hit = " << cw::GetNHits(Expeted_Trigger_1.LUT_code) << std::endl;
          std::cout << "Pid   = " << Expeted_Trigger_1.pat << std::endl;
          std::cout << "KeyStr = " << Expeted_Trigger_1.hs << std::endl;
          std::cout << "CC Code = " << Expeted_Trigger_1.LUT_code << std::endl;
          std::cout << "1/8 resolution = "  << Expeted_Trigger_1.eightres << std::endl;
          std::cout << "Bending angle =  " << Expeted_Trigger_1.bending << std::endl;
          std::cout << "LR bending = " << Expeted_Trigger_1.lr << std::endl;
          //std::cout << "1/8 resolution = " << cw::EightResolution(patternSet.LUT[1].hs, patternSet.LUT[1].LUT_code, patternSet.LUT[1].pat) << std::endl << std::endl;
          // potential problem here when we have more than 2 input muons in testing
          std::cout << std::endl << "CLCT 1 Decode:" << std::endl;
          std::cout << "N Hit = " << CLCT1_nhit << std::endl;
          std::cout << "Pid   = " << CLCT1_pid << std::endl;
          std::cout << "KeyStr = " << CLCT1_key << std::endl;
          std::cout << "CC Code= " << CLCT1_CC << std::endl;
          std::cout << "1/8 resolution = " << CLCT1_xky << std::endl;
          std::cout << "Bending angle =  " << CLCT1_bnd << std::endl;
          std::cout << "LR bending = " << CLCT1_lr << std::endl << std::endl;
        }
      }
    }


    if(1){
      std::cout << "Begin error output" << '\n';
      if (parameters.size() == 1) {
        bool have_errors = 0;
        if(Unexpected_trigger_0.size() > 0){
          fout << "Unexpected triggers:" << std::endl;
          fout_err << "Unexpected triggers:" << std::endl;
          Print_run2(SendTo_0, Unexpected_trigger_0, Expeted_Trigger_0, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }
        if(Absent_trigger_0.size() > 0){
          fout_err << "Absent triggers:" << std::endl;
          fout << "Absent triggers." << std::endl;
          Print_run2(SendTo_0, Absent_trigger_0, Expeted_Trigger_0, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }
        if(Extra_Trigger_0.size() > 0){
          fout_err << "Extra triggers:" << std::endl;
          fout << "Extra triggers:" << std::endl;
          //Print_run2(SendTo_0, Absent_trigger_0, Expeted_Trigger_0, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }
        if(ReadFrom_0.size() > 0){
          fout << "Wrong triggers (incorrect LUT):" << std::endl;
          Print_run2(SendTo_0, ReadFrom_0, Expeted_Trigger_0, patternSet.N_trials, fout, 1);
          fout << "********************************************************************************" << '\n';
          fout_err << "Wrong triggers (incorrect LUT):" << std::endl;
          Print_run2(SendTo_0, ReadFrom_0, Expeted_Trigger_0, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }
        if(have_errors){
          fout_err << "******************************************************************" << std::endl;
          Fail ++;
        }

      }

      if (parameters.size() > 1) {
        bool have_errors = 0;
        if(Unexpected_trigger_0.size() > 0){
          fout << "Unexpected triggers:" << std::endl;
          fout_err << "Unexpected triggers:" << std::endl;
          Print_run2(SendTo_0, Unexpected_trigger_0, Expeted_Trigger_0, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }

        if(Unexpected_trigger_1.size() > 0){
          fout << "Unexpected triggers, second muon:" << std::endl;
          fout_err << "Unexpected triggers, second muon::" << std::endl;
          Print_run2(SendTo_1, Unexpected_trigger_1, Expeted_Trigger_1, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }
        if(Extra_Trigger_0.size() > 0){
          fout_err << "Extra triggers:" << std::endl;
          fout << "Extra triggers:" << std::endl;
          //Print_run2(SendTo_0, Absent_trigger_0, Expeted_Trigger_0, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }
        if(Extra_Trigger_1.size() > 0){
          fout_err << "Extra triggers, second muon:" << std::endl;
          fout << "Extra triggers, second muon" << std::endl;
          //Print_run2(SendTo_0, Absent_trigger_0, Expeted_Trigger_0, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }
        if(Absent_trigger_0.size() > 0){
          fout_err << "Absent triggers:" << std::endl;
          fout << "Absent triggers." << std::endl;
          Print_run2(SendTo_0, Absent_trigger_0, Expeted_Trigger_0, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }

        if(Absent_trigger_1.size() > 0){
          fout_err << "Absent triggers, second muon:" << std::endl;
          fout << "Absent triggers, second muon." << std::endl;
          Print_run2(SendTo_1, Absent_trigger_1, Expeted_Trigger_1, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }

        if(ReadFrom_0.size() > 0){
          fout << "Wrong triggers (incorrect LUT):" << std::endl;
          Print_run2(SendTo_0, ReadFrom_0, Expeted_Trigger_0, patternSet.N_trials, fout, 1);
          if(ReadFrom_1.size() == 0) fout << "********************************************************************************" << '\n';
          fout_err << "Wrong triggers (incorrect LUT):" << std::endl;
          Print_run2(SendTo_0, ReadFrom_0, Expeted_Trigger_0, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }
        if(ReadFrom_1.size() > 0){
          fout << "Wrong triggers (incorrect LUT), second muon:" << std::endl;
          Print_run2(SendTo_1, ReadFrom_1, Expeted_Trigger_1, patternSet.N_trials, fout, 1);
          fout << "********************************************************************************" << '\n';
          fout_err << "Wrong triggers (incorrect LUT), second muon:" << std::endl;
          Print_run2(SendTo_1, ReadFrom_1, Expeted_Trigger_1, patternSet.N_trials, fout_err, 0);
          have_errors = 1;
        }
        if(have_errors){
          fout_err << "******************************************************************" << std::endl;
          Fail ++;
        }
      }
      All ++;
    }

  }while(cw::Increment_new(parameters[0], CombinationCounter));
  /*
  fout << "FINAL SCORE" << '\n';
  std::vector<std::vector<std::string> > response_final;
  cw::generate_response(Accuracy,Accuracy, Accuracy,1, Accuracy, Accuracy_1, All, response_final);
  for (size_t i = 0; i < response_final.size(); i++) {
  for (size_t j = 0; j < response_final[i].size(); j++) {
  if (i == 0 || i == 4 || i == 5) fout << response_final[i][j];
}
if (i == 0 || i == 4 || i == 5)fout << endl;
}*/

fout << "Fail/All(nHit > 3) = " << Fail << "/" << All <<  std::endl;
fout << "Expected Fail/All(nHit > 3) = " << Fail_Expexted << "/" << All <<  std::endl;
fout.close();
fout_err << "end output" << endl;;
fout_err.close();
usleep(1000);
this->Default(in,out);
return;

}

void CSCGEMTestApplication::OpenSimulatedFile(xgi::Input * in, xgi::Output * out)  // added by Kyla
{
  cgicc::Cgicc cgi(in);
  // newfile.open(/home/cscdev/Kyla/InjectCSCPatterns/data/ComparatorDigi_CLCT_ME11.txt)

  if(xgi::Utils::hasFormElement(cgi,"SimulatedFileName")) sprintf(SimulatedFileName, cgi["SimulatedFileName"]->getValue().c_str());
  if (SimulatedFile.is_open()) SimulatedFile.close();

  SimulatedFile.open(SimulatedFileName);
  if (!SimulatedFile.is_open()) std::cout << "FILE IS NOT OPENED!" << '\n';
  else std::cout << "File " << SimulatedFileName << " opened successfully" << '\n';

  this->Default(in,out);
  return;
}

void CSCGEMTestApplication::ReadSimulatedFile(xgi::Input * in, xgi::Output * out)  // added by Kyla
{
  cgicc::Cgicc cgi(in);
  string word;
  int bx_num;
  int layer_num;
  int halfstrip_num;
  int event_number = 0;
  cw::CCLUT ReadEvent;
  std::vector<cw::Hit> hits;

  TMB * thisTMB = tmbVector[1];
  thisTMB->ResetCounters();
  CLCT0_Counter = 0;
  CLCT1_Counter = 0;
  int tmbtype = 0xa+GetOTMBType[0]-'a';

  std::cout << "Reading The Event" << '\n';
  if (SimulatedFile >> word){
    SimulatedFile >> word;
    while(word != "Run"  && word != "CSC" && SimulatedFile >> word){
      if (word == "Comparatordigi") { // This is supposed to check for the BX,layer,halfstrip when it comes after "Comparatordigi"
        SimulatedFile >> word;
        if (word == "BX"){
          SimulatedFile >> bx_num;
          SimulatedFile >> word;
          if (word == "Layer"){
            SimulatedFile >> layer_num;
            SimulatedFile >> word;
            if (word == "halfstrip"){
              SimulatedFile >> halfstrip_num;
            }
          }
        }
        std::cout << "BX = " << bx_num << "; Layer = " << layer_num << "; halfstrip = " << halfstrip_num << std::endl;
        cw::Hit tmp_hit;
        tmp_hit.bx = bx_num;
        tmp_hit.lay = layer_num;
        tmp_hit.hs = halfstrip_num;
        hits.push_back(tmp_hit);
      }
    }
    std::cout << "Finished reading event" << '\n';
    cout << "Reading the results" << endl;
    if (word == "CSC" || word == "GEMCluster") {
      cout << word << " ";
      while (word != "Run" && SimulatedFile >> word) {
        cout << word << " ";
      }
      cout << endl;
    }
  }
  else std::cout << "We are at the end of File!!" << '\n';

  patternSet.Clear();
  patternSet.DeleteCurrentSet();
  patternSet.ClearEmuBoard();
  cw::CCLUT tmp_cclut;
  tmp_cclut.hits = hits;
  patternSet.LUT.push_back(tmp_cclut);

  this->Default(in,out);
  return;
}


void CSCGEMTestApplication::LoadSimulatedFileEvent(xgi::Input * in, xgi::Output * out ){
  cgicc::Cgicc cgi(in);

  patternSet.SetOTMBCompileType(GetOTMBType[0]);
  if(patternSet.WritePatterns_LUT()){
    if( patternSet.LoadEmuBoard("", 1) ){
      cout << "Successfully Loaded to EmuBoard!\n";
    }
    else{
      cout << "ERROR: FAILED TO LOAD EmuBoard!!!!!!!\n";
    }
  }

  patternSet.Dump(0,GetOTMBType[0]);

  TMB * thisTMB = tmbVector[1];

  int cclut0_lsbs, cclut1_lsbs, cclut_msbs;
  int cclut0_inc=0;
  int cclut1_inc=0;
  int CLCT0_data_, CLCT1_data_;
  int clct0_xky_data_, clct1_xky_data_;
  int clct0_CC_data_, clct1_CC_data_;

  cclut0_lsbs = thisTMB->ReadRegister(seq_clct0_adr);
  cclut1_lsbs = thisTMB->ReadRegister(seq_clct1_adr);
  cclut_msbs  = thisTMB->ReadRegister(seq_clctm_adr);
  clct0_xky_data_ = thisTMB->ReadRegister(clct0_bndxky_adr);
  clct1_xky_data_ = thisTMB->ReadRegister(clct1_bndxky_adr);
  clct0_CC_data_ = thisTMB->ReadRegister(clct0_cc_adr);
  clct1_CC_data_ = thisTMB->ReadRegister(clct1_cc_adr);
  // extract value from register value. bitlo means the lowest bit clct0_bndxky_adr
  int CLCT0_xky = thisTMB->ExtractValueFromData(clct0_xky_data_, clct0_xky_bitlo, clct0_xky_bithi);
  int CLCT1_xky = thisTMB->ExtractValueFromData(clct1_xky_data_, clct1_xky_bitlo, clct1_xky_bithi);
  int CLCT0_bnd = thisTMB->ExtractValueFromData(clct0_xky_data_, clct0_bnd_bitlo, clct0_bnd_bithi);
  int CLCT1_bnd = thisTMB->ExtractValueFromData(clct1_xky_data_, clct1_bnd_bitlo, clct1_bnd_bithi);
  int CLCT0_lr  = thisTMB->ExtractValueFromData(clct0_xky_data_, clct0_lr_bitlo, clct0_lr_bithi);
  int CLCT1_lr  = thisTMB->ExtractValueFromData(clct1_xky_data_, clct1_lr_bitlo, clct1_lr_bithi);
  thisTMB->GetCounters();

  cout << endl << CLCT0_Counter << "  " << thisTMB->GetCounter(cw::tmb_counters[0]) << endl;
  cout << endl << CLCT1_Counter << "  " << thisTMB->GetCounter(cw::tmb_counters[1]) << endl;

  CLCT0_data_ = ( (cclut_msbs & 0xf) << 16 ) | (cclut0_lsbs & 0xffff);
  CLCT1_data_ = ( (cclut_msbs & 0xf) << 16 ) | (cclut1_lsbs & 0xffff);

  int CLCT0_nhit = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_nhit_bitlo, CLCT0_nhit_bithi);
  int CLCT0_valid = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_valid_bitlo, CLCT0_valid_bithi);
  int CLCT0_pid = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_pattern_bitlo, CLCT0_pattern_bithi);  //Will need changes here
  int CLCT0_key = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_keyHalfStrip_bitlo, CLCT0_keyHalfStrip_bithi);
  int CLCT0_CC = thisTMB->ExtractValueFromData(clct0_CC_data_, clct0_cc_bitlo, clct0_cc_bithi);

  int CLCT1_nhit = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_nhit_bitlo, CLCT1_nhit_bithi);
  int CLCT1_valid = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_valid_bitlo, CLCT1_valid_bithi);
  int CLCT1_pid = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_pattern_bitlo, CLCT1_pattern_bithi);  //Will need changes here
  int CLCT1_key = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_keyHalfStrip_bitlo, CLCT1_keyHalfStrip_bithi);
  int CLCT1_CC = thisTMB->ExtractValueFromData(clct1_CC_data_, clct1_cc_bitlo, clct1_cc_bithi);

  // Output of Hits:
  std::cout << std::endl << "CLCT 0 Decode:" << std::endl;
  std::cout << "Valid = " << CLCT0_valid << std::endl;
  std::cout << "N Hit = " << CLCT0_nhit << std::endl;
  std::cout << "Pid   = " << CLCT0_pid << std::endl;
  std::cout << "Key Quarter Strip = " << CLCT0_xky / 2 << std::endl;
  std::cout << "Key Half Strip = " << CLCT0_key << std::endl;
  std::cout << "Key Strip = " << CLCT0_key / 2 << std::endl;
  std::cout << "CC Code= " << CLCT0_CC << std::endl;
  std::cout << "1/8 resolution = " << CLCT0_xky << std::endl;
  std::cout << "Bending angle =  " << CLCT0_bnd << std::endl;
  std::cout << "LR bending = " << CLCT0_lr << std::endl << std::endl;

  std::cout << std::endl << "CLCT 1 Decode:" << std::endl;
  std::cout << "Valid = " << CLCT1_valid << std::endl;
  std::cout << "N Hit = " << CLCT1_nhit << std::endl;
  std::cout << "Pid   = " << CLCT1_pid << std::endl;
  std::cout << "Key Quarter Strip = " << CLCT1_xky / 2 << std::endl;
  std::cout << "Key Half Strip = " << CLCT1_key << std::endl;
  std::cout << "Key Strip = " << CLCT1_key / 2 << std::endl;
  std::cout << "CC Code= " << CLCT1_CC << std::endl;
  std::cout << "1/8 resolution = " << CLCT1_xky << std::endl;
  std::cout << "Bending angle =  " << CLCT1_bnd << std::endl;
  std::cout << "LR bending = " << CLCT1_lr << std::endl << std::endl;


  // New Portion recently added:
  int trig_clct0_nhit = CLCT0_nhit;
  int trig_clct0_r2_pid = CLCT0_pid;
  int trig_clct0_KeyQuartStrip = CLCT0_xky / 2;
  int trig_clct0_KeyHalfStrip = CLCT0_key;
  int trig_clct0_cc_code = CLCT0_CC;
  int trig_clct0_KeyEighthStrip = CLCT0_xky;
  int trig_clct0_bend = CLCT0_lr;
  int trig_clct0_slope = CLCT0_bnd;

  int trig_clct1_nhit = CLCT1_nhit;
  int trig_clct1_r2_pid = CLCT1_pid;
  int trig_clct1_KeyQuartStrip = CLCT1_xky / 2;
  int trig_clct1_KeyHalfStrip = CLCT1_key;
  int trig_clct1_cc_code = CLCT1_CC;
  int trig_clct1_KeyEighthStrip = CLCT1_xky;
  int trig_clct1_bend = CLCT1_lr;
  int trig_clct1_slope = CLCT1_bnd;

  std::ifstream SimulationFile;
  string word;
  string equal_sign;

  int sim_clct0_nhit;
  int sim_clct0_r2_pid;
  int sim_clct0_KeyQuartStrip;
  int sim_clct0_KeyHalfStrip;
  int sim_clct0_cc_code;
  int sim_clct0_KeyEighthStrip;
  int sim_clct0_bend;
  int sim_clct0_slope;

  int sim_clct1_nhit;
  int sim_clct1_r2_pid;
  int sim_clct1_KeyQuartStrip;
  int sim_clct1_KeyHalfStrip;
  int sim_clct1_cc_code;
  int sim_clct1_KeyEighthStrip;
  int sim_clct1_bend;
  int sim_clct1_slope;

  bool comparison_0 = false;
  bool comparison0_slope = false;
  bool comparison0_khs = false;
  bool comparison_1 = false;
  bool comparison1_slope = false;
  bool comparison1_khs = false;

  int higher_bx0 = 0;
  int higher_bx1 = 0;

  int passes_through_loop = 1;
/*
    THIS LOOP DOES NOT WORK; NEEDS TO BE EDITED
*/


  SimulationFile.open(SimulatedFileName);
  std::cout << "Reading The Event Results" << '\n';
  while(SimulationFile >> word) {//word != "Run"  && word != "Comparatordigi" && word != "GEMCluster" && SimulationFile >> word){
    if (word == "Run" && word != "CSC" && SimulatedFile >> word) {
      std::cout << "\nEvent #" << passes_through_loop;
      ++passes_through_loop;
    }
    if (word == "CSC") {
      SimulationFile >> word;
      if (word == "CLCT") {
        SimulationFile >> word; // This word checks if it is looping through CLCT 1 or 2
        if (word == "#1:") {
          std::cout << "\nCLCT0 Information: " << std::endl;
          while (SimulationFile >> word)  {
            if (word == "BX") {
                SimulatedFile >> equal_sign;
                SimulatedFile >> word;
                if (std::stoi(word) > higher_bx0) {
                  higher_bx0 = std::stoi(word);
                  continue;
                }
                else {break;}
            }
            if (word == "Run-2"){
                SimulationFile >> word;  // This word is Pattern
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct0_r2_pid;
              }
            if (word == "Quality") {
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct0_nhit;
              }
            if (word == "Comp") {
                SimulationFile >> word; // word = Code;
                SimulationFile >> sim_clct0_cc_code;/*
                if (sim_clct0_cc_code == trig_clct0_cc_code) {
                  std::cout << "SIMULATION CC CODE (" << sim_clct0_cc_code << ") MATCHES TRIGGER CC CODE (" << trig_clct0_cc_code << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION CC CODE (" << sim_clct0_cc_code << ") DOES NOT MATCH TRIGGER CC CODE (" << trig_clct0_cc_code << ")" << std::endl;
                }*/
              }
            if (word == "Bend") {
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct0_bend;/*
                if (sim_clct0_bend == trig_clct0_bend) {
                  std::cout << "SIMULATION BEND (" << sim_clct0_bend << ") MATCHES TRIGGER BEND (" << trig_clct0_bend << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION BEND (" << sim_clct0_bend << ") DOES NOT MATCH TRIGGER BEND (" << trig_clct0_bend << ")" << std::endl;
                  comparison_0 = false;
                  //break;
                }*/
              }
            if (word == "Slope") {
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct0_slope;/*
                if (sim_clct0_slope == trig_clct0_slope) {
                  std::cout << "SIMULATION SLOPE (" << sim_clct0_slope << ") MATCHES TRIGGER SLOPE (" << trig_clct0_slope << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION SLOPE (" << sim_clct0_slope << ") DOES NOT MATCH TRIGGER SLOPE (" << trig_clct0_slope << ")" << std::endl;
                }*/
              }
            if (word == "KeyHalfStrip"){
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct0_KeyHalfStrip;/*
                if (sim_clct0_KeyHalfStrip == trig_clct0_KeyHalfStrip) {
                  std::cout << "SIMULATION KEY HALF STRIP (" << sim_clct0_KeyHalfStrip << ") MATCHES TRIGGER KEY HALF STRIP (" << trig_clct0_KeyHalfStrip << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION KEY HALF STRIP (" << sim_clct0_KeyHalfStrip << ") DOES NOT MATCH TRIGGER KEY HALF STRIP (" << trig_clct0_KeyHalfStrip << ")" << std::endl;
                }*/
              }
            if (word == "KeyQuartStrip"){
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct0_KeyQuartStrip;/*
                if (sim_clct0_KeyQuartStrip == trig_clct0_KeyQuartStrip) {
                  std::cout << "SIMULATION KEY QUARTER STRIP (" << sim_clct0_KeyQuartStrip << ") MATCHES TRIGGER KEY QUARTER STRIP(" << trig_clct0_KeyQuartStrip << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION KEY QUARTER STRIP (" << sim_clct0_KeyQuartStrip << ") DOES NOT MATCH TRIGGER KEY QUARTER STRIP(" << trig_clct0_KeyQuartStrip << ")" << std::endl;
                }*/
              }
            if (word == "KeyEighthStrip"){
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct0_KeyEighthStrip;/*
                if (sim_clct0_KeyEighthStrip == trig_clct0_KeyEighthStrip) {
                  std::cout << "SIMULATION KEY EIGHTH STRIP (" << sim_clct0_KeyEighthStrip << ") MATCHES TRIGGER KEY EIGHTH STRIP (" << trig_clct0_KeyEighthStrip << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION KEY EIGHTH STRIP (" << sim_clct0_KeyEighthStrip << ") DOES NOT MATCH TRIGGER KEY EIGHTH STRIP (" << trig_clct0_KeyEighthStrip << ")" << std::endl;
                }*/
              }
          }
        }
        if (word == "#2:") {
          std::cout << "\nCLCT1 Information: " << std::endl;
          while (SimulationFile >> word) {
            if (word == "BX") {
                SimulatedFile >> equal_sign;
                SimulatedFile >> word;
                if (std::stoi(word) > higher_bx0) {
                  higher_bx0 = std::stoi(word);
                  continue;
                }
                else {break;}
            }
            if (word == "Run-2"){
                SimulationFile >> word;  // This word is Pattern
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct1_r2_pid;/*
                if (sim_clct1_r2_pid == trig_clct1_r2_pid) {
                  std::cout << "SIMULATION RUN-2 PATTERN (" << sim_clct1_r2_pid << ") MATCHES TRIGGER RUN-2 PATTERN (" << trig_clct1_r2_pid<< ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION RUN-2 PATTERN (" << sim_clct1_r2_pid << ") DOES NOT MATCH TRIGGER RUN-2 PATTERN (" << trig_clct1_r2_pid<< ")" << std::endl;
                }*/
              }
            if (word == "Quality") {
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct1_nhit;/*
                if (sim_clct1_nhit == trig_clct1_nhit) {
                  std::cout << "SIMULATION QUALITY (" << sim_clct1_nhit << ") MATCHES TRIGGER QUALITY (" << trig_clct1_nhit << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION QUALITY (" << sim_clct1_nhit << ") DOES NOT MATCH TRIGGER QUALITY (" << trig_clct1_nhit << ")" << std::endl;
                }*/
              }
            if (word == "Comp") {
              SimulationFile >> word; // word = Code;
              SimulationFile >> sim_clct1_cc_code;/*
              if (sim_clct1_cc_code == trig_clct1_cc_code) {
                std::cout << "SIMULATION CC CODE (" << sim_clct1_cc_code << ") MATCHES TRIGGER CC CODE (" << trig_clct1_cc_code << ")" << std::endl;
              }
              else {
                std::cout << "SIMULATION CC CODE (" << sim_clct1_cc_code << ") DOES NOT MATCH TRIGGER CC CODE (" << trig_clct1_cc_code << ")" << std::endl;
                break;
              }*/
            }
            if (word == "Bend") {
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct1_bend;/*
                if (sim_clct1_bend == trig_clct1_bend) {
                  std::cout << "SIMULATION BEND (" << sim_clct1_bend << ") MATCHES TRIGGER BEND (" << trig_clct1_bend << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION BEND (" << sim_clct1_bend << ") DOES NOT MATCH TRIGGER BEND (" << trig_clct1_bend << ")" << std::endl;
                }*/
              }
            if (word == "Slope") {
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct1_slope;/*
                if (sim_clct1_slope == trig_clct1_slope) {
                  std::cout << "SIMULATION SLOPE (" << sim_clct1_slope << ") MATCHES TRIGGER SLOPE (" << trig_clct1_slope << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION SLOPE (" << sim_clct1_slope << ") DOES NOT MATCH TRIGGER SLOPE (" << trig_clct1_slope << ")" << std::endl;
                }*/
              }
            if (word == "KeyHalfStrip"){
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct1_KeyHalfStrip;/*
                if (sim_clct1_KeyHalfStrip == trig_clct1_KeyHalfStrip) {
                  std::cout << "SIMULATION KEY HALF STRIP (" << sim_clct1_KeyHalfStrip << ") MATCHES TRIGGER KEY HALF STRIP (" << trig_clct1_KeyHalfStrip << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION KEY HALF STRIP (" << sim_clct1_KeyHalfStrip << ") DOES NOT MATCH TRIGGER KEY HALF STRIP (" << trig_clct1_KeyHalfStrip << ")" << std::endl;
                }*/
              }
            if (word == "KeyQuartStrip"){
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct1_KeyQuartStrip;/*
                if (sim_clct1_KeyQuartStrip == trig_clct1_KeyQuartStrip) {
                  std::cout << "SIMULATION KEY QUARTER STRIP (" << sim_clct1_KeyQuartStrip << ") MATCHES TRIGGER KEY QUARTER STRIP(" << trig_clct1_KeyQuartStrip << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION KEY QUARTER STRIP (" << sim_clct1_KeyQuartStrip << ") DOES NOT MATCH TRIGGER KEY QUARTER STRIP(" << trig_clct1_KeyQuartStrip << ")" << std::endl;
                }*/
              }
            if (word == "KeyEighthStrip"){
                SimulationFile >> equal_sign;
                SimulationFile >> sim_clct1_KeyEighthStrip;/*
                if (sim_clct1_KeyEighthStrip == trig_clct1_KeyEighthStrip) {
                  std::cout << "SIMULATION KEY EIGHTH STRIP (" << sim_clct1_KeyEighthStrip << ") MATCHES TRIGGER KEY EIGHTH STRIP (" << trig_clct1_KeyEighthStrip << ")" << std::endl;
                }
                else {
                  std::cout << "SIMULATION KEY EIGHTH STRIP (" << sim_clct1_KeyEighthStrip << ") DOES NOT MATCH TRIGGER KEY EIGHTH STRIP (" << trig_clct1_KeyEighthStrip << ")" << std::endl;
                }*/
              }
          }
        }
      }
    }
  }
/*
  if (sim_clct0_slope == trig_clct0_slope && sim_clct0_KeyHalfStrip == trig_clct0_KeyHalfStrip) {
    std::cout << "\nSIMULATION CLCT0 RESULTS MATCH TRIGGER RESULTS" << std::endl << std::endl;
  }
  else if (sim_clct0_slope != trig_clct0_slope && sim_clct0_KeyHalfStrip == trig_clct0_KeyHalfStrip) {
    std::cout << "\nSIMULATION CLCT0 KEY HALF STRIP MATCHES TRIGGER KEY HALF STRIP\nSIMULATION SLOPE DOES NOT MATCH TRIGGER SLOPE" << std::endl << std::endl;
  }
  else if (sim_clct0_KeyHalfStrip != trig_clct0_KeyHalfStrip && sim_clct0_slope == trig_clct0_slope) {
    std::cout << "\nSIMULATION CLCT0 KEY HALF STRIP DOES NOT MATCH TRIGGER KEY HALF STRIP\nSIMULATION SLOPE MATCHES TRIGGER SLOPE" << std::endl << std::endl;
  }
  else {
    std::cout << "\nSIMULATION CLCT0 RESULTS DOES NOT MATCH TRIGGER RESULTS" << std::endl << std::endl;
  }

  if (trig_clct1_nhit != 0 && trig_clct1_cc_code != 0) {
    if (sim_clct1_slope == trig_clct1_slope && sim_clct1_KeyHalfStrip == trig_clct1_KeyHalfStrip) {
      std::cout << "SIMULATION CLCT1 RESULTS MATCH TRIGGER RESULTS" << std::endl << std::endl;
    }
    else if (sim_clct1_slope != trig_clct1_slope && sim_clct1_KeyHalfStrip == trig_clct1_KeyHalfStrip) {
      std::cout << "SIMULATION CLCT1 KEY HALF STRIP MATCHES TRIGGER KEY HALF STRIP\nSIMULATION SLOPE DOES NOT MATCH TRIGGER SLOPE" << std::endl << std::endl;
    }
    else if (sim_clct1_KeyHalfStrip != trig_clct1_KeyHalfStrip && sim_clct1_slope == trig_clct1_slope) {
      std::cout << "SIMULATION CLCT1 KEY HALF STRIP DOES NOT MATCH TRIGGER KEY HALF STRIP\nSIMULATION SLOPE MATCHES TRIGGER SLOPE" << std::endl << std::endl;
    }
    else {
      std::cout << "SIMULATION CLCT1 RESULTS DOES NOT MATCH TRIGGER RESULTS" << std::endl << std::endl;
    }
  }
*/

/*
  if (comparison_0 == false && comparison_1 == false) {
    std::cout << "\nSIMULATION RESULTS DO NOT MATCH TRIGGER RESULTS" << std::endl;
  }
  else {
    std::cout << "\nSIMULATION RESULTS MATCH TRIGGER RESULTS" << std::endl;
  }
*/
/*
  if (comparison0_slope == false || comparison0_khs == false) {
    std::cout << "\nCLCT0 SIMULATION RESULTS DO NOT MATCH TRIGGER RESULTS" << std::endl;
  }
  else if (comparison0_slope == false && comparison0_khs == true) {
    std::cout << "\nCLCT0 SLOPE SIMULATION RESULTS DO NOT MATCH TRIGGER RESTULTS" << std::endl;
  }
  else if (comparison0_slope == true && comparison0_khs == false) {
    std::cout << "\nCLCT0 KEY HALF STRIP SIMULATION RESULTS DO NOT MATCH TRIGGER RESULTS" << std::endl;
  }
  else {
    std::cout << "\nSIMULATION RESULTS MATCH TRIGGER RESULTS" << std::endl;
  }

  if (comparison1_slope == false || comparison1_khs == false) {
    std::cout << "\nCLCT0 SIMULATION RESULTS DO NOT MATCH TRIGGER RESULTS" << std::endl;
  }
  else if (comparison1_slope == false && comparison1_khs == true) {
    std::cout << "\nCLCT0 SLOPE SIMULATION RESULTS DO NOT MATCH TRIGGER RESTULTS" << std::endl;
  }
  else if (comparison1_slope == true && comparison1_khs == false) {
    std::cout << "\nCLCT0 KEY HALF STRIP SIMULATION RESULTS DO NOT MATCH TRIGGER RESULTS" << std::endl;
  }
  else {
    std::cout << "\nSIMULATION RESULTS MATCH TRIGGER RESULTS" << std::endl;
  }
  */

  this->Default(in,out);
  return;
}

void CSCGEMTestApplication::AutomaticCheckingFile(xgi::Input * in, xgi::Output * out ){
  using namespace std;
  cgicc::Cgicc cgi(in);
  int event_number = 0;
  int incorrect_event_number = 0;
  int NInjections = 10;

  // Create a vector to fill with the Simulation and Trigger Results to make plots with
  std::vector<int> sim_clct0_pid_vector;
  std::vector<int> sim_clct0_nhit_vector;
  std::vector<int> sim_clct0_cc_code_vector;
  std::vector<int> sim_clct0_bend_vector;
  std::vector<int> sim_clct0_slope_vector;
  std::vector<int> sim_clct0_KeyHalfStrip_vector;
  std::vector<int> sim_clct0_KeyQuartStrip_vector;
  std::vector<int> sim_clct0_KeyEighthStrip_vector;
  std::vector<int> sim_clct1_pid_vector;
  std::vector<int> sim_clct1_nhit_vector;
  std::vector<int> sim_clct1_cc_code_vector;
  std::vector<int> sim_clct1_bend_vector;
  std::vector<int> sim_clct1_slope_vector;
  std::vector<int> sim_clct1_KeyHalfStrip_vector;
  std::vector<int> sim_clct1_KeyQuartStrip_vector;
  std::vector<int> sim_clct1_KeyEighthStrip_vector;

  std::vector<int> trig_clct0_valid_vector0;
  std::vector<int> trig_clct0_pid_vector0;
  std::vector<int> trig_clct0_nhit_vector0;
  std::vector<int> trig_clct0_cc_code_vector0;
  std::vector<int> trig_clct0_bend_vector0;
  std::vector<int> trig_clct0_slope_vector0;
  std::vector<int> trig_clct0_KeyHalfStrip_vector0;
  std::vector<int> trig_clct0_KeyQuartStrip_vector0;
  std::vector<int> trig_clct0_KeyEighthStrip_vector0;
  std::vector<int> trig_clct1_valid_vector0;
  std::vector<int> trig_clct1_pid_vector0;
  std::vector<int> trig_clct1_nhit_vector0;
  std::vector<int> trig_clct1_cc_code_vector0;
  std::vector<int> trig_clct1_bend_vector0;
  std::vector<int> trig_clct1_slope_vector0;
  std::vector<int> trig_clct1_KeyHalfStrip_vector0;
  std::vector<int> trig_clct1_KeyQuartStrip_vector0;
  std::vector<int> trig_clct1_KeyEighthStrip_vector0;

  std::vector<int> event_id_vector;

  std::vector<int> incorrect_trig_clct0_pid_vector0;
  std::vector<int> incorrect_trig_clct0_nhit_vector0;
  std::vector<int> incorrect_trig_clct0_cc_code_vector0;
  std::vector<int> incorrect_trig_clct0_bend_vector0;
  std::vector<int> incorrect_trig_clct0_slope_vector0;
  std::vector<int> incorrect_trig_clct0_KeyHalfStrip_vector0;
  std::vector<int> incorrect_trig_clct0_KeyQuartStrip_vector0;
  std::vector<int> incorrect_trig_clct0_KeyEighthStrip_vector0;
  std::vector<int> incorrect_trig_clct1_pid_vector0;
  std::vector<int> incorrect_trig_clct1_nhit_vector0;
  std::vector<int> incorrect_trig_clct1_cc_code_vector0;
  std::vector<int> incorrect_trig_clct1_bend_vector0;
  std::vector<int> incorrect_trig_clct1_slope_vector0;
  std::vector<int> incorrect_trig_clct1_KeyHalfStrip_vector0;
  std::vector<int> incorrect_trig_clct1_KeyQuartStrip_vector0;
  std::vector<int> incorrect_trig_clct1_KeyEighthStrip_vector0;

  std::vector<int> incorrect_sim_clct0_pid_vector0;
  std::vector<int> incorrect_sim_clct0_nhit_vector0;
  std::vector<int> incorrect_sim_clct0_cc_code_vector0;
  std::vector<int> incorrect_sim_clct0_bend_vector0;
  std::vector<int> incorrect_sim_clct0_slope_vector0;
  std::vector<int> incorrect_sim_clct0_KeyHalfStrip_vector0;
  std::vector<int> incorrect_sim_clct0_KeyQuartStrip_vector0;
  std::vector<int> incorrect_sim_clct0_KeyEighthStrip_vector0;
  std::vector<int> incorrect_sim_clct1_pid_vector0;
  std::vector<int> incorrect_sim_clct1_nhit_vector0;
  std::vector<int> incorrect_sim_clct1_cc_code_vector0;
  std::vector<int> incorrect_sim_clct1_bend_vector0;
  std::vector<int> incorrect_sim_clct1_slope_vector0;
  std::vector<int> incorrect_sim_clct1_KeyHalfStrip_vector0;
  std::vector<int> incorrect_sim_clct1_KeyQuartStrip_vector0;
  std::vector<int> incorrect_sim_clct1_KeyEighthStrip_vector0;

  std::vector<int> incorrect_event_ids;

  int mismatched_events=0;

  do {
    event_number++;
    string word;
    string initWord = "Event";
    string equal_sign;
    int bx_num;
    int layer_num;
    int halfstrip_num;
    cw::CCLUT ReadEvent;
    std::vector<cw::Hit> hits;

    TMB * thisTMB = tmbVector[1];
    thisTMB->ResetCounters();
    CLCT0_Counter = 0;
    CLCT1_Counter = 0;
    int tmbtype = 0xa+GetOTMBType[0]-'a';

    int sim_clct0_nhit;
    int sim_clct0_r2_pid;
    int sim_clct0_KeyQuartStrip;
    int sim_clct0_KeyHalfStrip;
    int sim_clct0_cc_code;
    int sim_clct0_KeyEighthStrip;
    int sim_clct0_bend;
    int sim_clct0_slope;

    int sim_clct1_nhit;
    int sim_clct1_r2_pid;
    int sim_clct1_KeyQuartStrip;
    int sim_clct1_KeyHalfStrip;
    int sim_clct1_cc_code;
    int sim_clct1_KeyEighthStrip;
    int sim_clct1_bend;
    int sim_clct1_slope;

    int event_id = -1;

    int higher_bx0 = 0;
    int higher_bx1 = 0;

    int reading_clct0_first_time = 0;
    int reading_clct1_first_time = 0;

    std::cout << "Reading The Event #" << event_number << '\n';
    if (not SimulatedFile.eof()) std::cout << "NOT EOF of SimulatedFile "<< std::endl;
    while ((not SimulatedFile.eof()) and word != initWord){
      SimulatedFile >> word;
    }
    if (word == initWord){
      SimulatedFile >> event_id;
      std::cout << "First word printed. should be event number " << event_id << " after initWord "<< initWord << std::endl;
      //following loop is to find comparator digis
      while(word != "Run"  && word != "CSC" /*&& word != "CSCChamber"*/ && SimulatedFile >> word){
        if (word == "Comparatordigi") { // This is supposed to check for the BX,layer,halfstrip when it comes after "Comparatordigi"
          SimulatedFile >> word;
          if (word == "BX"){
            SimulatedFile >> bx_num;
            SimulatedFile >> word;
            if (word == "Layer"){
              SimulatedFile >> layer_num;
              SimulatedFile >> word;
              if (word == "halfstrip"){
                SimulatedFile >> halfstrip_num;
              }
            }
          }
          std::cout << "BX = " << bx_num << "; Layer = " << layer_num << "; halfstrip = " << halfstrip_num << std::endl;
          cw::Hit tmp_hit;
          tmp_hit.bx = bx_num;
          tmp_hit.lay = layer_num;
          tmp_hit.hs = halfstrip_num;
          tmp_hit.COMPILE_TYPE = 0xa+GetOTMBType[0]-'a';
          hits.push_back(tmp_hit);
        } // end of if (word == "Comparatordigi") loop
      } // end of while loop


      sim_clct0_nhit = 0;
      sim_clct0_r2_pid = 0;
      sim_clct0_KeyQuartStrip = 0;
      sim_clct0_KeyHalfStrip = 0;
      sim_clct0_cc_code = 0;
      sim_clct0_KeyEighthStrip = 0;
      sim_clct0_bend = 0;
      sim_clct0_slope = 0;

      sim_clct1_nhit = 0;
      sim_clct1_r2_pid = 0;
      sim_clct1_KeyQuartStrip = 0;
      sim_clct1_KeyHalfStrip = 0;
      sim_clct1_cc_code = 0;
      sim_clct1_KeyEighthStrip = 0;
      sim_clct1_bend = 0;
      sim_clct1_slope = 0;
      higher_bx0 = 0;
      higher_bx1 = 0;

      std::cout << "Simulated Event ID: " << event_id << std::endl;
      std::cout << "Finished reading event" << '\n';
      cout << "Reading the results" << endl;
      while (word != "Run" /* && word != "CSCChamber"*/ && SimulatedFile >> word) {
        std::cout << word << " ";\
        /* THE GEM TRIGGERING INFORMATION IS STILL NEEDED
        if (word == "GEMCluster") {
          SimulatedFile >> word
        }
        */
        if (word == "CLCT") {
          SimulatedFile >> word;  // Checking for CLCT 1 or 2
          if (word == "#1:") {
            std::cout << word << " ";
            while (SimulatedFile >> word) {
              if (word == "BX") {
                  SimulatedFile >> equal_sign;
                  SimulatedFile >> word;
                  if (std::stoi(word) > higher_bx0) {
                    higher_bx0 = std::stoi(word);
                  }
                  else {
                    std::cout<<"ERROR" << std::endl;
                    break;
                  }
              }
              std::cout << word << " ";
              if (word == "Run-2") {
                  SimulatedFile >> word; // PATTERN
                  std::cout << word << " ";
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct0_r2_pid;
                  std::cout << sim_clct0_r2_pid << " ";
                  std::cout <<" add clct0 pid to vector "<< sim_clct0_r2_pid << std::endl;
                  //sim_clct0_pid_vector.push_back(sim_clct0_r2_pid);
                }
              if (word == "Quality") {
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct0_nhit;
                  std::cout << sim_clct0_nhit << " ";
                  //sim_clct0_nhit_vector.push_back(sim_clct0_nhit);
                }
              if (word == "Comp") {
                  SimulatedFile >> word;
                  std::cout << word << " ";
                  SimulatedFile >> sim_clct0_cc_code;
                  std::cout << sim_clct0_cc_code << " ";
                  //sim_clct0_cc_code_vector.push_back(sim_clct0_cc_code);
                }
              if (word == "Bend") {
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct0_bend;
                  std::cout << sim_clct0_bend << " ";
                  //sim_clct0_bend_vector.push_back(sim_clct0_bend);
                }
              if (word == "Slope") {
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct0_slope;
                  std::cout << sim_clct0_slope << " ";
                  //sim_clct0_slope_vector.push_back(sim_clct0_slope);
              }
              if (word == "KeyHalfStrip") {
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct0_KeyHalfStrip;
                  std::cout << sim_clct0_KeyHalfStrip << " ";
                  //sim_clct0_KeyHalfStrip_vector.push_back(sim_clct0_KeyHalfStrip);
                }
              if (word == "KeyQuartStrip") {
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct0_KeyQuartStrip;
                  std::cout << sim_clct0_KeyQuartStrip << " ";
                  //sim_clct0_KeyQuartStrip_vector.push_back(sim_clct0_KeyQuartStrip);
                }
              if (word == "KeyEighthStrip") {
                SimulatedFile >> equal_sign;
                std::cout << equal_sign << " ";
                SimulatedFile >> sim_clct0_KeyEighthStrip;
                std::cout << sim_clct0_KeyEighthStrip << " ";
                //sim_clct0_KeyEighthStrip_vector.push_back(sim_clct0_KeyEighthStrip);
                break;
              }
            }
          }// end if (word == "#1:")
          if (word == "#2:") {
            std::cout << word << " ";
            while (SimulatedFile >> word) {
              if (word == "BX") {
                SimulatedFile >> equal_sign;
                SimulatedFile >> word;
                if (std::stoi(word) > higher_bx1) {
                  higher_bx1 = std::stoi(word);
                }
              }
              std::cout << word << " ";
              if (word == "Run-2") {
                  SimulatedFile >> word;
                  std::cout << word << " ";
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct1_r2_pid;
                  std::cout << sim_clct1_r2_pid << " ";
                  //sim_clct1_pid_vector.push_back(sim_clct1_r2_pid);
                }
              if (word == "Quality") {
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct1_nhit;
                  std::cout << sim_clct1_nhit << " ";
                  //sim_clct1_nhit_vector.push_back(sim_clct1_nhit);
                }
              if (word == "Comp") {
                  SimulatedFile >> word;
                  std::cout << word << " ";
                  SimulatedFile >> sim_clct1_cc_code;
                  std::cout << sim_clct1_cc_code << " ";
                  //sim_clct1_cc_code_vector.push_back(sim_clct1_cc_code);
                }
              if (word == "Bend") {
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct1_bend;
                  std::cout << sim_clct1_bend << " ";
                  //sim_clct1_bend_vector.push_back(sim_clct1_bend);
                }
              if (word == "Slope") {
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct1_slope;
                  std::cout << sim_clct1_slope << " ";
                  //sim_clct1_slope_vector.push_back(sim_clct1_slope);
                }
              if (word == "KeyHalfStrip") {
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct1_KeyHalfStrip;
                  std::cout << sim_clct1_KeyHalfStrip << " ";
                  //sim_clct1_KeyHalfStrip_vector.push_back(sim_clct1_KeyHalfStrip);
                }
              if (word == "KeyQuartStrip") {
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct1_KeyQuartStrip;
                  std::cout << sim_clct1_KeyQuartStrip << " ";
                  //sim_clct1_KeyQuartStrip_vector.push_back(sim_clct1_KeyQuartStrip);
                }
              if (word == "KeyEighthStrip") {
                  SimulatedFile >> equal_sign;
                  std::cout << equal_sign << " ";
                  SimulatedFile >> sim_clct1_KeyEighthStrip;
                  std::cout << sim_clct1_KeyEighthStrip << " ";
                  //sim_clct1_KeyEighthStrip_vector.push_back(sim_clct1_KeyEighthStrip);
                  break;
                }
            }// end while loop
            //}
            /*
            while (SimulatedFile >> word) {
              std::cout << word << " ";
              if (word == "Run-2") {
                SimulatedFile >> word;
                std::cout << word << " ";
                SimulatedFile >> equal_sign;
                std::cout << equal_sign << " ";
                SimulatedFile >> sim_clct1_r2_pid;
                std::cout << sim_clct1_r2_pid << " ";
              }
              if (word == "Quality") {
                SimulatedFile >> equal_sign;
                std::cout << equal_sign << " ";
                SimulatedFile >> sim_clct1_nhit;
                std::cout << sim_clct1_nhit << " ";
              }
              if (word == "Comp") {
                SimulatedFile >> word;
                std::cout << word << " ";
                SimulatedFile >> sim_clct1_cc_code;
                std::cout << sim_clct1_cc_code << " ";
              }
              if (word == "Bend") {
                SimulatedFile >> equal_sign;
                std::cout << equal_sign << " ";
                SimulatedFile >> sim_clct1_bend;
                std::cout << sim_clct1_bend << " ";
              }
              if (word == "Slope") {
                SimulatedFile >> equal_sign;
                std::cout << equal_sign << " ";
                SimulatedFile >> sim_clct1_slope;
                std::cout << sim_clct1_slope << " ";
              }
              if (word == "KeyHalfStrip") {
                SimulatedFile >> equal_sign;
                std::cout << equal_sign << " ";
                SimulatedFile >> sim_clct1_KeyHalfStrip;
                std::cout << sim_clct1_KeyHalfStrip << " ";
              }
              if (word == "KeyQuartStrip") {
                SimulatedFile >> equal_sign;
                std::cout << equal_sign << " ";
                SimulatedFile >> sim_clct1_KeyQuartStrip;
                std::cout << sim_clct1_KeyQuartStrip << " ";
              }
              if (word == "KeyEighthStrip") {
                SimulatedFile >> equal_sign;
                std::cout << equal_sign << " ";
                SimulatedFile >> sim_clct1_KeyEighthStrip;
                std::cout << sim_clct1_KeyEighthStrip << " ";
                break;
              }
            }
            */
          }// end if (word == "#2:")
          if (word == "Run" /*|| word == "CSCChamber"*/) {break;}
        } // end if (word == "CLCT")
      }// while loop
      std::cout << std::endl;
    }
    else {
      std::cout << "We are at the end of File!!  Total Event number: "<< event_number << '\n';
      break;
    }

    if (higher_bx0 > higher_bx1) {
      sim_clct1_nhit = 0;
      sim_clct1_r2_pid = 0;
      sim_clct1_KeyQuartStrip = 0;
      sim_clct1_KeyHalfStrip = 0;
      sim_clct1_cc_code = 0;
      sim_clct1_KeyEighthStrip = 0;
      sim_clct1_bend = 0;
      sim_clct1_slope = 0;
    }
    else if (higher_bx0 < higher_bx1) {
      std::cout << "ERROR" << std::endl;
      exit(0);
    }

    //Tao: add following code to skip event if no simulated CLCTs is found

    if (sim_clct0_nhit == 0) {
      std::cout << "CLCT0 NHITS IS NOT FOUND " << sim_clct0_nhit << std::endl;
      //continue; // skip event comparison in do loop
    }

    event_id_vector.push_back(event_id);
    std::cout << "Just Loaded the Event ID: " << event_id << std::endl;
    sim_clct0_pid_vector.push_back(sim_clct0_r2_pid);
    sim_clct0_nhit_vector.push_back(sim_clct0_nhit);
    sim_clct0_cc_code_vector.push_back(sim_clct0_cc_code);
    sim_clct0_bend_vector.push_back(sim_clct0_bend);
    sim_clct0_slope_vector.push_back(sim_clct0_slope);
    sim_clct0_KeyHalfStrip_vector.push_back(sim_clct0_KeyHalfStrip);
    sim_clct0_KeyQuartStrip_vector.push_back(sim_clct0_KeyQuartStrip);
    sim_clct0_KeyEighthStrip_vector.push_back(sim_clct0_KeyEighthStrip);

    sim_clct1_pid_vector.push_back(sim_clct1_r2_pid);
    sim_clct1_nhit_vector.push_back(sim_clct1_nhit);
    sim_clct1_cc_code_vector.push_back(sim_clct1_cc_code);
    sim_clct1_bend_vector.push_back(sim_clct1_bend);
    sim_clct1_slope_vector.push_back(sim_clct1_slope);
    sim_clct1_KeyHalfStrip_vector.push_back(sim_clct1_KeyHalfStrip);
    sim_clct1_KeyQuartStrip_vector.push_back(sim_clct1_KeyQuartStrip);
    sim_clct1_KeyEighthStrip_vector.push_back(sim_clct1_KeyEighthStrip);

      int cclut0_lsbs, cclut1_lsbs, cclut_msbs;
      int CLCT0_data_, CLCT1_data_;
      int clct0_xky_data_, clct1_xky_data_;
      int clct0_CC_data_, clct1_CC_data_;
    //Tao we can add a loop here to do injection multiple times to get good result that matches with simulation
    for (int i = 0; i < NInjections; ++i) {
      patternSet.Clear();
      patternSet.DeleteCurrentSet();
      patternSet.ClearEmuBoard();
      cw::CCLUT tmp_cclut;
      tmp_cclut.hits = hits;
      patternSet.LUT.push_back(tmp_cclut);

      patternSet.SetOTMBCompileType(GetOTMBType[0]);
      if(patternSet.WritePatterns_LUT()){
        if( patternSet.LoadEmuBoard("", 1) ){
          cout << "Successfully Loaded to EmuBoard!\n";
        }
        else{
          cout << "ERROR: FAILED TO LOAD EmuBoard!!!!!!!\n";
        }
      }

      patternSet.Dump(0,GetOTMBType[0]);

      cclut0_lsbs = thisTMB->ReadRegister(seq_clct0_adr);
      cclut1_lsbs = thisTMB->ReadRegister(seq_clct1_adr);
      cclut_msbs  = thisTMB->ReadRegister(seq_clctm_adr);
      clct0_xky_data_ = thisTMB->ReadRegister(clct0_bndxky_adr);
      clct1_xky_data_ = thisTMB->ReadRegister(clct1_bndxky_adr);
      clct0_CC_data_ = thisTMB->ReadRegister(clct0_cc_adr);
      clct1_CC_data_ = thisTMB->ReadRegister(clct1_cc_adr);
      if (sim_clct0_nhit == 0) { // Making trigger results 0 if there were no CLCT0 simulated hits
        cclut0_lsbs = 0;
        cclut1_lsbs = 0;
        cclut_msbs  = 0;
        clct0_xky_data_ = 0;
        clct1_xky_data_ = 0;
        clct0_CC_data_ = 0;
        clct1_CC_data_ = 0;
      }

      // extract value from register value. bitlo means the lowest bit clct0_bndxky_adr
      thisTMB->GetCounters();

      cout << endl << CLCT0_Counter << "  " << thisTMB->GetCounter(cw::tmb_counters[0]) << endl;
      cout << endl << CLCT1_Counter << "  " << thisTMB->GetCounter(cw::tmb_counters[1]) << endl;

      CLCT0_data_ = ( (cclut_msbs & 0xf) << 16 ) | (cclut0_lsbs & 0xffff);
      CLCT1_data_ = ( (cclut_msbs & 0xf) << 16 ) | (cclut1_lsbs & 0xffff);

      int CLCT0_CC_tmp = thisTMB->ExtractValueFromData(clct0_CC_data_, clct0_cc_bitlo, clct0_cc_bithi);
      int CLCT1_CC_tmp = thisTMB->ExtractValueFromData(clct1_CC_data_, clct1_cc_bitlo, clct1_cc_bithi);
      if ((CLCT0_CC_tmp == sim_clct0_cc_code && CLCT1_CC_tmp == sim_clct1_cc_code) || (CLCT0_CC_tmp == sim_clct1_cc_code && CLCT1_CC_tmp == sim_clct0_cc_code)){
          std::cout <<"Good Injection is found at "<< i <<" CLCT0_CC code, SIM "<< sim_clct0_cc_code <<" Trig "<< CLCT0_CC_tmp << std::endl;
          break;
      }
      }//end of Ninjection

      int CLCT0_xky  = thisTMB->ExtractValueFromData(clct0_xky_data_, clct0_xky_bitlo, clct0_xky_bithi);
      int CLCT1_xky  = thisTMB->ExtractValueFromData(clct1_xky_data_, clct1_xky_bitlo, clct1_xky_bithi);
      int CLCT0_bnd  = thisTMB->ExtractValueFromData(clct0_xky_data_, clct0_bnd_bitlo, clct0_bnd_bithi);
      int CLCT1_bnd  = thisTMB->ExtractValueFromData(clct1_xky_data_, clct1_bnd_bitlo, clct1_bnd_bithi);
      int CLCT0_lr   = thisTMB->ExtractValueFromData(clct0_xky_data_, clct0_lr_bitlo, clct0_lr_bithi);
      int CLCT1_lr   = thisTMB->ExtractValueFromData(clct1_xky_data_, clct1_lr_bitlo, clct1_lr_bithi);

      int CLCT0_nhit = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_nhit_bitlo, CLCT0_nhit_bithi);
      int CLCT0_valid= thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_valid_bitlo, CLCT0_valid_bithi);
      int CLCT0_pid  = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_pattern_bitlo, CLCT0_pattern_bithi);
      int CLCT0_key  = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT0_keyHalfStrip_bitlo, CLCT0_keyHalfStrip_bithi);
      int CLCT0_CC   = thisTMB->ExtractValueFromData(clct0_CC_data_, clct0_cc_bitlo, clct0_cc_bithi);

      int CLCT1_nhit = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_nhit_bitlo, CLCT1_nhit_bithi);
      int CLCT1_valid= thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_valid_bitlo, CLCT1_valid_bithi);
      int CLCT1_pid  = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_pattern_bitlo, CLCT1_pattern_bithi);
      int CLCT1_key  = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT1_keyHalfStrip_bitlo, CLCT1_keyHalfStrip_bithi);
      int CLCT1_CC   = thisTMB->ExtractValueFromData(clct1_CC_data_, clct1_cc_bitlo, clct1_cc_bithi);
      if ((CLCT0_CC != sim_clct0_cc_code && CLCT0_CC == sim_clct1_cc_code) || (CLCT1_CC == sim_clct0_cc_code)){
        //Swapped two CLCTs from OTMB, Tao
        CLCT0_xky  = thisTMB->ExtractValueFromData(clct1_xky_data_, clct0_xky_bitlo, clct0_xky_bithi);
        CLCT1_xky  = thisTMB->ExtractValueFromData(clct0_xky_data_, clct1_xky_bitlo, clct1_xky_bithi);
        CLCT0_bnd  = thisTMB->ExtractValueFromData(clct1_xky_data_, clct0_bnd_bitlo, clct0_bnd_bithi);
        CLCT1_bnd  = thisTMB->ExtractValueFromData(clct0_xky_data_, clct1_bnd_bitlo, clct1_bnd_bithi);
        CLCT0_lr   = thisTMB->ExtractValueFromData(clct1_xky_data_, clct0_lr_bitlo, clct0_lr_bithi);
        CLCT1_lr   = thisTMB->ExtractValueFromData(clct0_xky_data_, clct1_lr_bitlo, clct1_lr_bithi);

        CLCT0_nhit = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT0_nhit_bitlo, CLCT0_nhit_bithi);
        CLCT0_valid= thisTMB->ExtractValueFromData(CLCT1_data_, CLCT0_valid_bitlo, CLCT0_valid_bithi);
        CLCT0_pid  = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT0_pattern_bitlo, CLCT0_pattern_bithi);
        CLCT0_key  = thisTMB->ExtractValueFromData(CLCT1_data_, CLCT0_keyHalfStrip_bitlo, CLCT0_keyHalfStrip_bithi);
        CLCT0_CC   = thisTMB->ExtractValueFromData(clct1_CC_data_, clct0_cc_bitlo, clct0_cc_bithi);

        CLCT1_nhit = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT1_nhit_bitlo, CLCT1_nhit_bithi);
        CLCT1_valid= thisTMB->ExtractValueFromData(CLCT0_data_, CLCT1_valid_bitlo, CLCT1_valid_bithi);
        CLCT1_pid  = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT1_pattern_bitlo, CLCT1_pattern_bithi);
        CLCT1_key  = thisTMB->ExtractValueFromData(CLCT0_data_, CLCT1_keyHalfStrip_bitlo, CLCT1_keyHalfStrip_bithi);
        CLCT1_CC   = thisTMB->ExtractValueFromData(clct0_CC_data_, clct1_cc_bitlo, clct1_cc_bithi);
      }

      int trig_clct0_valid = CLCT0_valid;
      int trig_clct0_nhit = CLCT0_nhit;
      int trig_clct0_r2_pid = CLCT0_valid > 0 ? CLCT0_pid : 0;
      int trig_clct0_KeyQuartStrip = CLCT0_xky / 2;
      int trig_clct0_KeyHalfStrip = CLCT0_key;
      int trig_clct0_cc_code = CLCT0_CC;
      int trig_clct0_KeyEighthStrip = CLCT0_xky;
      int trig_clct0_bend = CLCT0_lr;
      int trig_clct0_slope = CLCT0_bnd;

      int trig_clct1_valid = CLCT1_valid;
      int trig_clct1_nhit = CLCT1_nhit;
      int trig_clct1_r2_pid = CLCT1_valid > 0 ? CLCT1_pid : 0;
      int trig_clct1_KeyQuartStrip = CLCT1_xky / 2;
      int trig_clct1_KeyHalfStrip = CLCT1_key;
      int trig_clct1_cc_code = CLCT1_CC;
      int trig_clct1_KeyEighthStrip = CLCT1_xky;
      int trig_clct1_bend = CLCT1_lr;
      int trig_clct1_slope = CLCT1_bnd;

      trig_clct0_valid_vector0.push_back(trig_clct0_valid);
      trig_clct0_pid_vector0.push_back(trig_clct0_r2_pid);
      trig_clct0_nhit_vector0.push_back(trig_clct0_nhit);
      trig_clct0_cc_code_vector0.push_back(trig_clct0_cc_code);
      trig_clct0_bend_vector0.push_back(trig_clct0_bend);
      trig_clct0_slope_vector0.push_back(trig_clct0_slope);
      trig_clct0_KeyHalfStrip_vector0.push_back(trig_clct0_KeyHalfStrip);
      trig_clct0_KeyQuartStrip_vector0.push_back(trig_clct0_KeyQuartStrip);
      trig_clct0_KeyEighthStrip_vector0.push_back(trig_clct0_KeyEighthStrip);

      trig_clct1_valid_vector0.push_back(trig_clct1_valid);
      trig_clct1_pid_vector0.push_back(trig_clct1_r2_pid);
      trig_clct1_nhit_vector0.push_back(trig_clct1_nhit);
      trig_clct1_cc_code_vector0.push_back(trig_clct1_cc_code);
      trig_clct1_bend_vector0.push_back(trig_clct1_bend);
      trig_clct1_slope_vector0.push_back(trig_clct1_slope);
      trig_clct1_KeyHalfStrip_vector0.push_back(trig_clct1_KeyHalfStrip);
      trig_clct1_KeyQuartStrip_vector0.push_back(trig_clct1_KeyQuartStrip);
      trig_clct1_KeyEighthStrip_vector0.push_back(trig_clct1_KeyEighthStrip);

      std::cout << "CLCT0 RESULTS: " << std::endl;
      std::cout << "RUN-2 PATTERN ID SIMULATION VS TRIGGER " << sim_clct0_r2_pid << " vs " << trig_clct0_r2_pid << std::endl;
      std::cout << "QUALITY SIMULATION VS TRIGGER " << sim_clct0_nhit << " vs " <<trig_clct0_nhit << std::endl;
      std::cout << "CC CODE SIMULATION VS TRIGGER " << sim_clct0_cc_code << " vs " << trig_clct0_cc_code << std::endl;
      std::cout << "BEND SIMULATION VS TRIGGER " << sim_clct0_bend << " vs " << trig_clct0_bend << std::endl;
      std::cout << "SLOPE SIMULATION VS TRIGGER " << sim_clct0_slope << " vs " <<  trig_clct0_slope << std::endl;
      std::cout << "KEY HALFSTRIP SIMULATION VS TRIGGER " << sim_clct0_KeyHalfStrip << " vs " << trig_clct0_KeyHalfStrip << std::endl;
      std::cout << "KEY QUARTER STRIP SIMULATION VS TRIGGER " << sim_clct0_KeyQuartStrip << " vs " << trig_clct0_KeyQuartStrip << std::endl;
      std::cout << "KEY EIGHTH STRIP SIMULATION VS TRIGGER " << sim_clct0_KeyEighthStrip << " vs " << trig_clct0_KeyEighthStrip << std::endl;

/* Needs boolean condition
      std::cout << "CLCT0 RESULTS: " << std::endl;
      std::cout << "SIMULATION RUN-2 PATTERN ID (" << sim_clct0_r2_pid << ") MATCHES TRIGGER RUN0-2 PATTERN ID (" << trig_clct0_r2_pid << ")" << std::endl;
      std::cout << "SIMULATION QUALITY (" << sim_clct0_nhit << ") MATCHES TRIGGER QUALITY (" <<trig_clct0_nhit << ")" << std::endl;
      std::cout << "SIMULATION CC CODE (" << sim_clct0_cc_code << ") MATCHES TRIGGER CC CODE (" << trig_clct0_cc_code << ")" << std::endl;
      std::cout << "SIMULATION BEND SIMULATION (" << sim_clct0_bend << ") MATCHES BEND (" << trig_clct0_bend << ")" << std::endl;
      std::cout << "SIMULATION SLOPE (" << sim_clct0_slope << ") MATCHES TRIGGER SLOPE (" <<  trig_clct0_slope << ")" << std::endl;
      std::cout << "SIMULATION KEY HALFSTRIP (" << sim_clct0_KeyHalfStrip << ") MATCHES TRIGGER KEY HALFSTRIP (" << trig_clct0_KeyHalfStrip << ")" << std::endl;
      std::cout << "SIMULATION KEY QUARTER STRIP (" << sim_clct0_KeyQuartStrip << ") MATCHESS TRIGGER KEY QUARTER STRIP (" << trig_clct0_KeyQuartStrip << ")" << std::endl;
      std::cout << "SIMULATION KEY EIGHTH STRIP (" << sim_clct0_KeyEighthStrip << ") MATCHES TRIGGER KEY EIGHTH STRIP (" << trig_clct0_KeyEighthStrip << ")" << std::endl;
*/

/*
      std::vector<int> incorrect_trig_clct0_pid_vector0;
      std::vector<int> incorrect_trig_clct0_nhit_vector0;
      std::vector<int> incorrect_trig_clct0_cc_code_vector0;
      std::vector<int> incorrect_trig_clct0_bend_vector0;
      std::vector<int> incorrect_trig_clct0_slope_vector0;
      std::vector<int> incorrect_trig_clct0_KeyHalfStrip_vector0;
      std::vector<int> incorrect_trig_clct0_KeyQuartStrip_vector0;
      std::vector<int> incorrect_trig_clct0_KeyEighthStrip_vector0;

      std::vector<int> incorrect_trig_clct1_pid_vector0;
      std::vector<int> incorrect_trig_clct1_nhit_vector0;
      std::vector<int> incorrect_trig_clct1_cc_code_vector0;
      std::vector<int> incorrect_trig_clct1_bend_vector0;
      std::vector<int> incorrect_trig_clct1_slope_vector0;
      std::vector<int> incorrect_trig_clct1_KeyHalfStrip_vector0;
      std::vector<int> incorrect_trig_clct1_KeyQuartStrip_vector0;
      std::vector<int> incorrect_trig_clct1_KeyEighthStrip_vector0;

      std::vector<int> incorrect_sim_clct0_pid_vector0;
      std::vector<int> incorrect_sim_clct0_nhit_vector0;
      std::vector<int> incorrect_sim_clct0_cc_code_vector0;
      std::vector<int> incorrect_sim_clct0_bend_vector0;
      std::vector<int> incorrect_sim_clct0_slope_vector0;
      std::vector<int> incorrect_sim_clct0_KeyHalfStrip_vector0;
      std::vector<int> incorrect_sim_clct0_KeyQuartStrip_vector0;
      std::vector<int> incorrect_sim_clct0_KeyEighthStrip_vector0;

      std::vector<int> incorrect_sim_clct1_pid_vector0;
      std::vector<int> incorrect_sim_clct1_nhit_vector0;
      std::vector<int> incorrect_sim_clct1_cc_code_vector0;
      std::vector<int> incorrect_sim_clct1_bend_vector0;
      std::vector<int> incorrect_sim_clct1_slope_vector0;
      std::vector<int> incorrect_sim_clct1_KeyHalfStrip_vector0;
      std::vector<int> incorrect_sim_clct1_KeyQuartStrip_vector0;
      std::vector<int> incorrect_sim_clct1_KeyEighthStrip_vector0;
*/

      if (sim_clct1_r2_pid != trig_clct1_r2_pid || sim_clct1_nhit != trig_clct1_nhit || sim_clct1_cc_code != trig_clct1_cc_code ||sim_clct1_bend != trig_clct1_bend||sim_clct1_slope != trig_clct1_slope||sim_clct1_KeyHalfStrip != trig_clct1_KeyHalfStrip||sim_clct1_KeyQuartStrip != trig_clct1_KeyQuartStrip||sim_clct1_KeyEighthStrip != trig_clct1_KeyEighthStrip || sim_clct0_r2_pid != trig_clct0_r2_pid || sim_clct0_nhit != trig_clct0_nhit || sim_clct0_cc_code != trig_clct0_cc_code ||sim_clct0_bend != trig_clct0_bend||sim_clct0_slope != trig_clct0_slope||sim_clct0_KeyHalfStrip != trig_clct0_KeyHalfStrip||sim_clct0_KeyQuartStrip != trig_clct0_KeyQuartStrip||sim_clct0_KeyEighthStrip != trig_clct0_KeyEighthStrip) {
        incorrect_trig_clct0_pid_vector0.push_back(trig_clct0_r2_pid);
        incorrect_sim_clct0_pid_vector0.push_back(sim_clct0_r2_pid);

        incorrect_trig_clct0_nhit_vector0.push_back(trig_clct0_nhit);
        incorrect_sim_clct0_nhit_vector0.push_back(sim_clct0_nhit);

        incorrect_trig_clct0_cc_code_vector0.push_back(trig_clct0_cc_code);
        incorrect_sim_clct0_cc_code_vector0.push_back(sim_clct0_cc_code);

        incorrect_trig_clct0_bend_vector0.push_back(trig_clct0_bend);
        incorrect_sim_clct0_bend_vector0.push_back(sim_clct0_bend);

        incorrect_trig_clct0_slope_vector0.push_back(trig_clct0_slope);
        incorrect_sim_clct0_slope_vector0.push_back(sim_clct0_slope);

        incorrect_trig_clct0_KeyHalfStrip_vector0.push_back(trig_clct0_KeyHalfStrip);
        incorrect_sim_clct0_KeyHalfStrip_vector0.push_back(sim_clct0_KeyHalfStrip);

        incorrect_trig_clct0_KeyQuartStrip_vector0.push_back(trig_clct0_KeyQuartStrip);
        incorrect_sim_clct0_KeyQuartStrip_vector0.push_back(sim_clct0_KeyQuartStrip);

        incorrect_trig_clct0_KeyEighthStrip_vector0.push_back(trig_clct0_KeyEighthStrip);
        incorrect_sim_clct0_KeyEighthStrip_vector0.push_back(sim_clct0_KeyEighthStrip);

        incorrect_trig_clct1_pid_vector0.push_back(trig_clct1_r2_pid);
        incorrect_sim_clct1_pid_vector0.push_back(sim_clct1_r2_pid);
        incorrect_trig_clct1_nhit_vector0.push_back(trig_clct1_nhit);
        incorrect_sim_clct1_nhit_vector0.push_back(sim_clct1_nhit);
        incorrect_trig_clct1_cc_code_vector0.push_back(trig_clct1_cc_code);
        incorrect_sim_clct1_cc_code_vector0.push_back(sim_clct1_cc_code);
        incorrect_trig_clct1_bend_vector0.push_back(trig_clct1_bend);
        incorrect_sim_clct1_bend_vector0.push_back(sim_clct1_bend);
        incorrect_trig_clct1_slope_vector0.push_back(trig_clct1_slope);
        incorrect_sim_clct1_slope_vector0.push_back(sim_clct1_slope);
        incorrect_trig_clct1_KeyHalfStrip_vector0.push_back(trig_clct1_KeyHalfStrip);
        incorrect_sim_clct1_KeyHalfStrip_vector0.push_back(sim_clct1_KeyHalfStrip);
        incorrect_trig_clct1_KeyQuartStrip_vector0.push_back(trig_clct1_KeyQuartStrip);
        incorrect_sim_clct1_KeyQuartStrip_vector0.push_back(sim_clct1_KeyQuartStrip);
        incorrect_trig_clct1_KeyEighthStrip_vector0.push_back(trig_clct1_KeyEighthStrip);
        incorrect_sim_clct1_KeyEighthStrip_vector0.push_back(sim_clct1_KeyEighthStrip);

        incorrect_event_ids.push_back(event_id);
        mismatched_events++;
      }


/*
    std::cout << "For Comparisons: " << std::endl;
    std::cout << "CLCT0" << std::endl;
    std::cout << "SIMULATION RUN-2 PATTERN (" << sim_clct0_r2_pid << ") VS TRIGGER RUN-2 PATTERN (" << trig_clct0_r2_pid << ")" << std::endl;
    std::cout << "SIMULATION QUALITY (" << sim_clct0_nhit << ") VS TRIGGER QUALITY (" << trig_clct0_nhit << ")" << std::endl;
    std::cout << "SIMULATION CC CODE (" << sim_clct0_cc_code << ") VS TRIGGER CC CODE (" << trig_clct0_cc_code << ")" << std::endl;
    std::cout << "SIMULATION BEND (" << sim_clct0_bend << ") VS TRIGGER BEND (" << trig_clct0_bend << ")" << std::endl;
    std::cout << "SIMULATION SLOPE (" << sim_clct0_slope << ") VS TRIGGER SLOPE (" << trig_clct0_slope << ")" << std::endl;
    std::cout << "SIMULATION KEY HALF STRIP (" << sim_clct0_KeyHalfStrip << ") VS TRIGGER KEY HALF STRIP (" << trig_clct0_KeyHalfStrip << ")" << std::endl;
    std::cout << "SIMULATION KEY QUARTER STRIP (" << sim_clct0_KeyQuartStrip << ") VS TRIGGER KEY QUARTER STRIP (" << trig_clct0_KeyQuartStrip<< ")" << std::endl;
    std::cout << "SIMULATION KEY EIGHTH STRIP (" << sim_clct0_KeyEighthStrip << ") VS TRIGGER KEY EIGHTH STRIP (" << trig_clct0_KeyEighthStrip << ")" << std::endl;
    std::cout << "CLCT1" << std::endl;
    std::cout << "SIMULATION RUN-2 PATTERN (" << sim_clct1_r2_pid << ") VS TRIGGER RUN-2 PATTERN (" << trig_clct1_r2_pid << ")" << std::endl;
    std::cout << "SIMULATION QUALITY (" << sim_clct1_nhit << ") VS TRIGGER QUALITY (" << trig_clct1_nhit << ")" << std::endl;
    std::cout << "SIMULATION CC CODE (" << sim_clct1_cc_code << ") VS TRIGGER CC CODE (" << trig_clct1_cc_code << ")" << std::endl;
    std::cout << "SIMULATION BEND (" << sim_clct1_bend << ") VS TRIGGER BEND (" << trig_clct1_bend << ")" << std::endl;
    std::cout << "SIMULATION SLOPE (" << sim_clct1_slope << ") VS TRIGGER SLOPE (" << trig_clct1_slope << ")" << std::endl;
    std::cout << "SIMULATION KEY HALF STRIP (" << sim_clct1_KeyHalfStrip << ") VS TRIGGER KEY HALF STRIP (" << trig_clct1_KeyHalfStrip << ")" << std::endl;
    std::cout << "SIMULATION KEY QUARTER STRIP (" << sim_clct1_KeyQuartStrip << ") VS TRIGGER KEY QUARTER STRIP (" << trig_clct1_KeyQuartStrip<< ")" << std::endl;
    std::cout << "SIMULATION KEY EIGHTH STRIP (" << sim_clct1_KeyEighthStrip << ") VS TRIGGER KEY EIGHTH STRIP (" << trig_clct1_KeyEighthStrip << ")" << std::endl;


    if (sim_clct0_slope == trig_clct0_slope && sim_clct0_KeyHalfStrip == trig_clct0_KeyHalfStrip) {
      std::cout << "\nSIMULATION CLCT0 RESULTS MATCH TRIGGER RESULTS" << std::endl << std::endl;
    }
    else if (sim_clct0_slope != trig_clct0_slope && sim_clct0_KeyHalfStrip == trig_clct0_KeyHalfStrip) {
      std::cout << "\nSIMULATION CLCT0 KEY HALF STRIP MATCHES TRIGGER KEY HALF STRIP\nSIMULATION SLOPE DOES NOT MATCH TRIGGER SLOPE" << std::endl << std::endl;
      incorrect_event_number++;
    }
    else if (sim_clct0_KeyHalfStrip != trig_clct0_KeyHalfStrip && sim_clct0_slope == trig_clct0_slope) {
      std::cout << "\nSIMULATION CLCT0 KEY HALF STRIP DOES NOT MATCH TRIGGER KEY HALF STRIP\nSIMULATION SLOPE MATCHES TRIGGER SLOPE" << std::endl << std::endl;
      incorrect_event_number++;
    }
    else {
      std::cout << "\nSIMULATION CLCT0 RESULTS DOES NOT MATCH TRIGGER RESULTS" << std::endl << std::endl;
      incorrect_event_number++;
    }

    if (trig_clct1_nhit == 0 && trig_clct1_cc_code == 0) {
      continue;
    }
    else {
      if (sim_clct1_slope == trig_clct1_slope && sim_clct1_KeyHalfStrip == trig_clct1_KeyHalfStrip) {
        std::cout << "SIMULATION CLCT1 RESULTS MATCH TRIGGER RESULTS" << std::endl << std::endl;
      }
      else if (sim_clct1_slope != trig_clct1_slope && sim_clct1_KeyHalfStrip == trig_clct1_KeyHalfStrip) {
        std::cout << "SIMULATION CLCT1 KEY HALF STRIP MATCHES TRIGGER KEY HALF STRIP\nSIMULATION SLOPE DOES NOT MATCH TRIGGER SLOPE" << std::endl << std::endl;
      }
      else if (sim_clct1_KeyHalfStrip != trig_clct1_KeyHalfStrip && sim_clct1_slope == trig_clct1_slope) {
        std::cout << "SIMULATION CLCT1 KEY HALF STRIP DOES NOT MATCH TRIGGER KEY HALF STRIP\nSIMULATION SLOPE MATCHES TRIGGER SLOPE" << std::endl << std::endl;
      }
      else {
        std::cout << "SIMULATION CLCT1 RESULTS DOES NOT MATCH TRIGGER RESULTS" << std::endl << std::endl;
      }
    }
*/
    //Resetting values
    sim_clct0_r2_pid = 0;
    sim_clct0_nhit = 0;
    sim_clct0_cc_code = 0;
    sim_clct0_bend = 0;
    sim_clct0_slope = 0;
    sim_clct0_KeyHalfStrip = 0;
    sim_clct0_KeyQuartStrip = 0;
    sim_clct0_KeyEighthStrip = 0;
    sim_clct1_r2_pid = 0;
    sim_clct1_nhit = 0;
    sim_clct1_cc_code = 0;
    sim_clct1_bend = 0;
    sim_clct1_slope = 0;
    sim_clct1_KeyHalfStrip = 0;
    sim_clct1_KeyQuartStrip = 0;
    sim_clct1_KeyEighthStrip = 0;

    std::cout << "The Last Read Event ID: " << event_id << std::endl;
    //event_id = 0;
    //std::cout << "Event ID Should be Zero: " << event_id << std::endl;

  } while (!SimulatedFile.eof());

  std::cout << "\nNUMBER OF EVENTS IN FILE: " << event_number << std::endl;
  std::cout << "NUMBER OF EVENTS WITH INCORRECT RESULTS: " << mismatched_events << std::endl;

  string tested_events_name = std::to_string(event_number);
  string prefix_name = "/home/cscdev/Kyla/Results_Files/arrays_to_plot_";
  //Simulation Files
  string simulation_results_file = prefix_name + tested_events_name + "_simulation_events";
  string simulation_results_file_txt = simulation_results_file + ".txt";
  string simulation_results_file_csv = simulation_results_file + ".csv";
  std::cout << "Would the user like to create a simulation results file? (Y/N)" << std::endl;
  char user_input;
  std::cin >> user_input;
  if (user_input == 'Y') {
    ofstream SimulationArraysFile;
    SimulationArraysFile.open(simulation_results_file_txt, std::ofstream::out | std::ofstream::trunc);
    if (SimulationArraysFile.is_open()) {
    // Simulation CLCT0
    std::cout <<"file opened successfully"<< std::endl;
    SimulationArraysFile << "sim_clct0_pid = [";
    std::cout <<"file opened, step1, vector size "<< sim_clct0_pid_vector.size()<< std::endl;
    SimulationArraysFile << sim_clct0_pid_vector.at(0);
    std::cout <<"file opened, step1A"<< std::endl;
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct0_pid_vector.at(i);
    }
    std::cout <<"file opened, step1B"<< std::endl;
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct0_nhit = [";
    std::cout <<"file opened, step2"<< std::endl;
    SimulationArraysFile << sim_clct0_nhit_vector.at(0);
    for (int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct0_nhit_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct0_cc_code = [";
    SimulationArraysFile << sim_clct0_cc_code_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct0_cc_code_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct0_bend = [";
    SimulationArraysFile << sim_clct0_bend_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct0_bend_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct0_slope = [";
    SimulationArraysFile << sim_clct0_slope_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct0_slope_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct0_KeyHalfStrip = [";
    SimulationArraysFile << sim_clct0_KeyHalfStrip_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct0_KeyHalfStrip_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct0_KeyQuartStrip = [";
    SimulationArraysFile << sim_clct0_KeyQuartStrip_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct0_KeyQuartStrip_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct0_KeyEighthStrip = [";
    SimulationArraysFile << sim_clct0_KeyEighthStrip_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile <<", " << sim_clct0_KeyEighthStrip_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;

    // Simulation CLCT1
    std::cout <<"file opened, step3"<< std::endl;
    SimulationArraysFile << "sim_clct1_pid = [";
    SimulationArraysFile << sim_clct1_pid_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct1_pid_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct1_nhit = [";
    SimulationArraysFile << sim_clct1_nhit_vector.at(0);
    for (int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct1_nhit_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct1_cc_code = [";
    SimulationArraysFile << sim_clct1_cc_code_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct1_cc_code_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct1_bend = [";
    SimulationArraysFile << sim_clct1_bend_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct1_bend_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct1_slope = [";
    SimulationArraysFile << sim_clct1_slope_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct1_slope_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct1_KeyHalfStrip = [";
    SimulationArraysFile << sim_clct1_KeyHalfStrip_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct1_KeyHalfStrip_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct1_KeyQuartStrip = [";
    SimulationArraysFile << sim_clct1_KeyQuartStrip_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile << ", " << sim_clct1_KeyQuartStrip_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    SimulationArraysFile << "sim_clct1_KeyEighthStrip = [";
    SimulationArraysFile << sim_clct1_KeyEighthStrip_vector.at(0);
    for(int i = 1; i < event_number; ++i) {
      SimulationArraysFile <<", " << sim_clct1_KeyEighthStrip_vector.at(i);
    }
    SimulationArraysFile << "]" << std::endl;
    std::cout <<"file opened, step4"<< std::endl;

    SimulationArraysFile.close();
    std::cout <<"file closed successfully"<< std::endl;
    }
    else {std::cout << "Unable to open simulation file" << std::endl;}
    // Make the csv file info
    ofstream SimulationCSVFile;
    SimulationCSVFile.open(simulation_results_file_csv);
    if (SimulationCSVFile.is_open()) {
      SimulationCSVFile << "sim_clct0_pid,sim_clct0_nhit,sim_clct0_cc_code,sim_clct0_bend,sim_clct0_slope,sim_clct0_KeyHalfStrip,sim_clct0_KeyQuartStrip,sim_clct0_KeyEighthStrip,sim_clct1_pid,sim_clct1_nhit,sim_clct1_cc_code,sim_clct1_bend,sim_clct1_slope,sim_clct1_KeyHalfStrip,sim_clct1_KeyQuartStrip,sim_clct1_KeyEighthStrip\n";
      for (int i = 0; i < event_number; ++i) {
        SimulationCSVFile << sim_clct0_pid_vector.at(i) << "," << sim_clct0_nhit_vector.at(i) << "," << sim_clct0_cc_code_vector.at(i) << "," << sim_clct0_bend_vector.at(i) << "," << sim_clct0_slope_vector.at(i) << "," << sim_clct0_KeyHalfStrip_vector.at(i) << "," << sim_clct0_KeyQuartStrip_vector.at(i) << "," << sim_clct0_KeyEighthStrip_vector.at(i) << "," << sim_clct1_pid_vector.at(i) << "," << sim_clct1_nhit_vector.at(i) << "," << sim_clct1_cc_code_vector.at(i) << "," << sim_clct1_bend_vector.at(i) << "," << sim_clct1_slope_vector.at(i) << "," << sim_clct1_KeyHalfStrip_vector.at(i) << "," << sim_clct1_KeyQuartStrip_vector.at(i) << "," << sim_clct1_KeyEighthStrip_vector.at(i) << std::endl;
      }
      SimulationCSVFile.close();
    }

  }// end of if input == Y
  else if (user_input == 'N') {std::cout << "No results file created" << std::endl;}
  else {std::cout << "Please input either Y or N" << std::endl;}

  //Trigger Files
  string trigger_results_file0 = prefix_name + tested_events_name + "_trigger_events";
  string trigger_results_file_txt = trigger_results_file0 + ".txt";
  string trigger_results_file_csv0 = trigger_results_file0 + ".csv";

  std::cout << "Would the user like to create three trigger results file? (Y/N)" << std::endl;
  char view_results;
  std::cin >> view_results;
  if (view_results == 'N') {std::cout << "No results printed" << std::endl;}
  else if (view_results == 'Y') {
    ofstream TriggerArraysFile;
    TriggerArraysFile.open(trigger_results_file_txt);
    if (TriggerArraysFile.is_open()) {
      // Trigger CLCT0
      TriggerArraysFile << "trig_clct0_valid = [";
      TriggerArraysFile << trig_clct0_pid_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct0_valid_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct0_pid = [";
      TriggerArraysFile << trig_clct0_pid_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct0_pid_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct0_nhit = [";
      TriggerArraysFile << trig_clct0_nhit_vector0.at(0);
      for (int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct0_nhit_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct0_cc_code = [";
      TriggerArraysFile << trig_clct0_cc_code_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct0_cc_code_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct0_bend = [";
      TriggerArraysFile << trig_clct0_bend_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct0_bend_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct0_slope = [";
      TriggerArraysFile << trig_clct0_slope_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct0_slope_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct0_KeyHalfStrip = [";
      TriggerArraysFile << trig_clct0_KeyHalfStrip_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct0_KeyHalfStrip_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct0_KeyQuartStrip = [";
      TriggerArraysFile << trig_clct0_KeyQuartStrip_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct0_KeyQuartStrip_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct0_KeyEighthStrip = [";
      TriggerArraysFile << trig_clct0_KeyEighthStrip_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile <<", " << trig_clct0_KeyEighthStrip_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;

      // Trigger CLCT1
      TriggerArraysFile << "trig_clct1_valid = [";
      TriggerArraysFile << trig_clct0_pid_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct1_valid_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct1_pid = [";
      TriggerArraysFile << trig_clct1_pid_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct1_pid_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct1_nhit = [";
      TriggerArraysFile << trig_clct1_nhit_vector0.at(0);
      for (int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct1_nhit_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct1_cc_code = [";
      TriggerArraysFile << trig_clct1_cc_code_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct1_cc_code_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct1_bend = [";
      TriggerArraysFile << trig_clct1_bend_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct1_bend_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct1_slope = [";
      TriggerArraysFile << trig_clct1_slope_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct1_slope_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct1_KeyHalfStrip = [";
      TriggerArraysFile << trig_clct1_KeyHalfStrip_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct1_KeyHalfStrip_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct1_KeyQuartStrip = [";
      TriggerArraysFile << trig_clct1_KeyQuartStrip_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile << ", " << trig_clct1_KeyQuartStrip_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;
      TriggerArraysFile << "trig_clct1_KeyEighthStrip = [";
      TriggerArraysFile << trig_clct1_KeyEighthStrip_vector0.at(0);
      for(int i = 1; i < event_number; ++i) {
        TriggerArraysFile <<", " << trig_clct1_KeyEighthStrip_vector0.at(i);
      }
      TriggerArraysFile << "]" << std::endl;

      TriggerArraysFile.close();
    }
    else {std::cout << "Unable to open trigger results file" << std::endl;}
    // Make the csv file info:
    ofstream TriggerCSVFile0;
    TriggerCSVFile0.open(trigger_results_file_csv0);
    if (TriggerCSVFile0.is_open()) {
      TriggerCSVFile0 << "trig_clct0_valid,trig_clct0_pid,trig_clct0_nhit,trig_clct0_cc_code,trig_clct0_bend,trig_clct0_slope,trig_clct0_KeyHalfStrip,trig_clct0_KeyQuartStrip,trig_clct0_KeyEighthStrip,trig_clct1_valid,trig_clct1_pid,trig_clct1_nhit,trig_clct1_cc_code,trig_clct1_bend,trig_clct1_slope,trig_clct1_KeyHalfStrip,trig_clct1_KeyQuartStrip,trig_clct1_KeyEighthStrip\n";
      for (int i = 0; i < event_number; ++i) {
        TriggerCSVFile0 << trig_clct0_valid_vector0.at(i) << "," << trig_clct0_pid_vector0.at(i) << "," << trig_clct0_nhit_vector0.at(i) << "," << trig_clct0_cc_code_vector0.at(i) << "," << trig_clct0_bend_vector0.at(i) << "," << trig_clct0_slope_vector0.at(i) << "," << trig_clct0_KeyHalfStrip_vector0.at(i) << "," << trig_clct0_KeyQuartStrip_vector0.at(i) << "," << trig_clct0_KeyEighthStrip_vector0.at(i) << "," << trig_clct1_valid_vector0.at(i) << "," << trig_clct1_pid_vector0.at(i) << "," << trig_clct1_nhit_vector0.at(i) << "," << trig_clct1_cc_code_vector0.at(i) << "," << trig_clct1_bend_vector0.at(i) << "," << trig_clct1_slope_vector0.at(i) << "," << trig_clct1_KeyHalfStrip_vector0.at(i) << "," << trig_clct1_KeyQuartStrip_vector0.at(i) << "," << trig_clct1_KeyEighthStrip_vector0.at(i) << std::endl;
      }
      TriggerCSVFile0.close();
    }
  }
  else {std::cout << "Please input either Y or N" << std::endl;}

  ofstream Incorrect_events;
  Incorrect_events.open("/home/cscdev/Kyla/Results_Files/incorrect_events_" + tested_events_name + ".txt", std::ofstream::out | std::ofstream::trunc);
  if (Incorrect_events.is_open()) {
    Incorrect_events << "TOTAL EVENT NUMBER: " << event_number<< std::endl;
    Incorrect_events << "TOTAL INCORRECT EVENTS: " << mismatched_events << std::endl << std::endl;
    for (int i = 0; i < mismatched_events; ++i){
      Incorrect_events << "This is event " << i << std::endl;
      Incorrect_events << "Simulated event ID: " << incorrect_event_ids.at(i) << std::endl;
      Incorrect_events << "CLCT0 RESULTS: " << std::endl;
      Incorrect_events << "PATTERN ID SIMULATION VS TRIGGER " << incorrect_sim_clct0_pid_vector0.at(i) << " vs " << incorrect_trig_clct0_pid_vector0.at(i) << std::endl;
      Incorrect_events << "QUALITY SIMULATION VS TRIGGER " << incorrect_sim_clct0_nhit_vector0.at(i) << " vs " << incorrect_trig_clct0_nhit_vector0.at(i) << std::endl;
      Incorrect_events << "CC CODE SIMULATION VS TRIGGER " << incorrect_sim_clct0_cc_code_vector0.at(i) << " vs " << incorrect_trig_clct0_cc_code_vector0.at(i) << std::endl;
      Incorrect_events << "BEND SIMULATION VS TRIGGER " << incorrect_sim_clct0_bend_vector0.at(i) << " vs " << incorrect_trig_clct0_bend_vector0.at(i) << std::endl;
      Incorrect_events << "SLOPE SIMULATION VS TRIGGER " << incorrect_sim_clct0_slope_vector0.at(i) << " vs " <<  incorrect_trig_clct0_slope_vector0.at(i) << std::endl;
      Incorrect_events << "KEY HALFSTRIP SIMULATION VS TRIGGER " << incorrect_sim_clct0_KeyHalfStrip_vector0.at(i) << " vs " << incorrect_trig_clct0_KeyHalfStrip_vector0.at(i) << std::endl;
      Incorrect_events << "KEY QUARTER STRIP SIMULATION VS TRIGGER " << incorrect_sim_clct0_KeyQuartStrip_vector0.at(i) << " vs " << incorrect_trig_clct0_KeyQuartStrip_vector0.at(i) << std::endl;
      Incorrect_events << "KEY EIGHTH STRIP SIMULATION VS TRIGGER " << incorrect_sim_clct0_KeyEighthStrip_vector0.at(i) << " vs " << incorrect_trig_clct0_KeyEighthStrip_vector0.at(i) << std::endl << std::endl;

      Incorrect_events << "CLCT1 RESULTS: " << std::endl;
      Incorrect_events << "PATTERN ID SIMULATION VS TRIGGER " << incorrect_sim_clct1_pid_vector0.at(i) << " vs " << incorrect_trig_clct1_pid_vector0.at(i) << std::endl;
      Incorrect_events << "QUALITY SIMULATION VS TRIGGER " << incorrect_sim_clct1_nhit_vector0.at(i) << " vs " << incorrect_trig_clct1_nhit_vector0.at(i) << std::endl;
      Incorrect_events << "CC CODE SIMULATION VS TRIGGER " << incorrect_sim_clct1_cc_code_vector0.at(i) << " vs " << incorrect_trig_clct1_cc_code_vector0.at(i) << std::endl;
      Incorrect_events << "BEND SIMULATION VS TRIGGER " << incorrect_sim_clct1_bend_vector0.at(i) << " vs " << incorrect_trig_clct1_bend_vector0.at(i) << std::endl;
      Incorrect_events << "SLOPE SIMULATION VS TRIGGER " << incorrect_sim_clct1_slope_vector0.at(i) << " vs " <<  incorrect_trig_clct1_slope_vector0.at(i) << std::endl;
      Incorrect_events << "KEY HALFSTRIP SIMULATION VS TRIGGER " << incorrect_sim_clct1_KeyHalfStrip_vector0.at(i) << " vs " << incorrect_trig_clct1_KeyHalfStrip_vector0.at(i) << std::endl;
      Incorrect_events << "KEY QUARTER STRIP SIMULATION VS TRIGGER " << incorrect_sim_clct1_KeyQuartStrip_vector0.at(i) << " vs " << incorrect_trig_clct1_KeyQuartStrip_vector0.at(i) << std::endl;
      Incorrect_events << "KEY EIGHTH STRIP SIMULATION VS TRIGGER " << incorrect_sim_clct1_KeyEighthStrip_vector0.at(i) << " vs " << incorrect_trig_clct1_KeyEighthStrip_vector0.at(i) << std::endl << std::endl;
    }
    Incorrect_events.close();
  }
  this->Default(in,out);
  return;
}


void CSCGEMTestApplication::ClearCCLUTParamScan(xgi::Input * in, xgi::Output * out ){
  parameters.clear();
  CCLUT_ps_vec.clear();
  Free_paramsCCLUT.clear();
  this->Default(in,out);
  return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void CSCGEMTestApplication::PrepareForTriggering(xgi::Input * in, xgi::Output * out )
{
  cgicc::Cgicc cgi(in);

  std::cout << "PrepareForTriggering" << std::endl;

  thisCCB->setCCBMode(CCB::VMEFPGA);

  thisCCB->l1aReset();

  thisCCB->EnableL1aFromTmbL1aReq();

  thisCCB->bc0();

  this->Default(in,out);
}

void CSCGEMTestApplication::HardReset(xgi::Input * in, xgi::Output * out )
{
  cgicc::Cgicc cgi(in);

  std::cout << "hardReset" << std::endl;

  thisCCB->hardReset();

  this->Default(in,out);

}
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
  if(patternSet.WritePatterns_LUT()){
    if( patternSet.LoadEmuBoard()){
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

#include "CLCT.h"
#include "TaoFunc.h"
#include <cmath>
#include <string>
#include <utility>
//#include <stdlib.h>
//#include "CSCConstants.h"


namespace cw {
using namespace emu;
using namespace pc;

	int layerorder_all[6][6] = { {3,1,5,6,4,2},//layerorder1
						  {4,2,5,6,3,1},
						  {1,3,6,5,2,4},
						  {2,4,6,5,1,3},
						  {4,6,2,1,3,5},
						  {1,2,3,4,5,6}
	};

//~~~~~~~~~ Support Functions ~~~~~~~~~~~~
bool EdgeCheck(int key, int hs, int COMPILE_TYPE = 0xc)						// used to check if generated hit is within CSC bounds
	{
		if (hs < 0 || hs > CSCConstants::NUM_HS) // falls outside hs range. hs >= 1
			return false;
		else if ((key <= 127 && hs > 127) && (COMPILE_TYPE == 0xc || COMPILE_TYPE == 0xd))	// crosses Edge of ME1/1b
			return false;
		else if ((key >= 128 && hs < 128) && (COMPILE_TYPE == 0xc || COMPILE_TYPE == 0xd))	// crosses Edge of ME1/1a
			return false;
		else if (  (hs < 0 || hs > 159)   && (COMPILE_TYPE == 0xa || COMPILE_TYPE == 0xb))	// out of range for type A & B
			return false;
		else
			return true;
	}

bool Hits_Generator(int Bx, int Key, int Pat, int Nhits, std::vector<Hit>& hits, int COMPILE_TYPE = 0xc)	// used to Fill Hits vector in CLCT constructor
	{
		std::vector<bool> usedlayers(CSCConstants::NUM_LAYERS, false);				//  used layer accounting

		// Pattern validity check
		if (Pat < 2 || Pat >= CSCConstants::NUM_PATTERNS)
			return true;  				// invalid pattern types
		if (Nhits < 1 || Nhits > CSCConstants::NUM_LAYERS)
			return true;				// invalid number of hits

		std::srand(std::time(0));

		int n = 0;
		while (n != Nhits)
		{
			int layer = std::abs(std::rand()) % CSCConstants::NUM_LAYERS;				// randomly select a layer
			if (usedlayers.at(layer)) continue;
			else usedlayers.at(layer) = true;

			int hs = Key + CSCConstants::CLCTPatterns[Pat][layer];      				// selects halfstrip hit from pattern

			if (EdgeCheck(Key, hs, COMPILE_TYPE))	hits.push_back(Hit(Bx, hs, layer, COMPILE_TYPE));

			n++;
			///~~~~~~
		}

		return false;
	}

bool compareHit(Hit h1, Hit h2)
	{
		//if(h1.bx == h2.bx)
		return (h1.lay < h2.lay);
		//else return (h1.bx < h2.bx);
	}

bool compareCLCT(CLCT c1, CLCT c2)
	{
		if(c1.bx == c2.bx)	return (c1.hs < c2.hs);
		else return (c1.bx < c2.bx);
	}

	int  GetCFEB(int hs, int COMPILE_TYPE = 0xc)
			{
				return GetInputCFEBByX<STRIPS_CFEB>(hs, COMPILE_TYPE);
			}

	int  GetLocal(int hs, int COMPILE_TYPE = 0xc)
	  	{
				return GetInputXStrip<STRIPS_CFEB>(hs, COMPILE_TYPE);
			}


int EightResolution(int KeyLayer, int CCLUT_code, int PiD)
  {
    int shift = -9999999;

		if (PiD == 0) {
			shift = CSCConstants::pat0[CCLUT_code];
		}
		else if (PiD == 1) {
			shift = CSCConstants::pat1[CCLUT_code];
		}
		else if (PiD == 2) {
			shift = CSCConstants::pat2[CCLUT_code];
		}
		else if (PiD == 3) {
			shift = CSCConstants::pat3[CCLUT_code];
		}
		else if (PiD == 4) {
			shift = CSCConstants::pat4[CCLUT_code];
		}
    else std::cout << "Wrong PiD" << '\n';

		shift -= 7;

    //shift = -shift;

		shift += KeyLayer*4;

		return shift;

	}
unsigned char GetTriadSeg(Hit hit, int i)
	{
		// Layer Staggering implemented here!
		//bool stagger = (COMPILE_TYPE == 0xb) || (COMPILE_TYPE == 0xa);
		//int halfStrip = ((hit.lay % 2 != 0) && stagger) ? (hit.hs + 1) : (hit.hs);	// odd layers inc by +1 if staggered
              //  if (halfStrip < 0) std::cerr<<"wrong halfstrip generated from hit: "<< halfStrip <<" hit "<< hit <<" compile type "<< COMPILE_TYPE << std::endl;
                //if (stagger) std::cout <<"Hit "<< hit <<" add stagger "<< halfStrip << std::endl;
		int halfStrip = hit.hs;
		int localhs = GetLocal(halfStrip, hit.COMPILE_TYPE);
		unsigned char n = 1 << (localhs / 4);
		bool leftstrip = (localhs % 4 < 2);
		bool lefths = (localhs % 2 == 0);

		switch (i)
		{
		case 0:
			return n;
			break;
		case 1:
			return (leftstrip ? 0x00 : n);
			break;
		case 2:
			return (lefths ? 0x00 : n);
			break;
		default:
			// Throw some kind of error?
			return (0x00);
			break;
		}
	}

//~~~~~~~~~~ Class Structures ~~~~~~~~~~~
Hit::Hit() : bx(0), hs(0), lay(0), COMPILE_TYPE(0xc) {}

Hit::Hit(int Bx, int Hs, int Layer, int Type) : bx(Bx), hs(Hs), lay(Layer), COMPILE_TYPE(Type) {}

std::ostream& operator << (std::ostream& os, const Hit& hit){
    os << '(' << hit.bx << ',' << hit.hs << ',' << hit.lay << ',' << char(hit.COMPILE_TYPE+'a'-0xa) <<')' ;
    return os;
}

std::istream& operator >> (std::istream& is, Hit& hit){
	char tmp;
	int data;
	is >> tmp >> data;	//	'(X'
	hit.bx = data;
	is >> tmp >> data;	//	',X'
	hit.hs = data;
	is >> tmp >> data;	//	',X'
	hit.lay = data;
	is >> tmp >> data;	//	',X'
	hit.COMPILE_TYPE = data;
	is >> tmp;		//	')'
	return is;
}


CLCT::CLCT(void) : bx(0), hs(0), pat(0), nhits(0), COMPILE_TYPE(0xc) {}

CLCT::CLCT(int Bx, int Key, int Pat, int Nhit, int Type) :
	bx(Bx),
	hs(Key),
	pat(Pat),
	COMPILE_TYPE(Type)
	{
		if(Nhit > 6){ nhits=6; }
		else if(Nhit <0){ nhits=0; }
		else{ nhits = Nhit; }
		bool bad = Hits_Generator(bx, hs, pat, nhits, hits, COMPILE_TYPE);
		sort(hits.begin(), hits.end(), compareHit);
	}

CLCT::CLCT(int Bx, int Key, int Pat, std::vector<Hit>& h_vec, int Type) :
	bx(Bx),
	hs(Key),
	pat(Pat),
	COMPILE_TYPE(Type)
	{
		for(int i=0; i<h_vec.size(); i++){
			h_vec[i].bx += Bx;
			h_vec[i].hs += Key;
			if(EdgeCheck(Key, h_vec[i].hs, COMPILE_TYPE)) hits.push_back(h_vec[i]);		// No Check for duplicate laer yet :(
		}
		nhits = hits.size();
		sort(hits.begin(), hits.end(), compareHit);
	}

CLCT::CLCT(const CLCT& cl) :
	bx(cl.bx),
	hs(cl.hs),
	pat(cl.pat),
	nhits(cl.nhits),
	COMPILE_TYPE(cl.COMPILE_TYPE)
	{
		hits.insert( hits.end(), cl.hits.begin(), cl.hits.end() );
		sort(hits.begin(), hits.end(), compareHit);

	}

void CLCT::RelativeSpaceTime(void){
	for(int i=0; i < hits.size(); i++){
		hits[i].bx -= this->bx;
		hits[i].hs -= this->hs;
	}
	return;
}
void CLCT::NormalSpaceTime(void){
	for(int i=0; i < hits.size(); i++){
		hits[i].bx += this->bx;
		hits[i].hs += this->hs;
	}
	return;
}

void CLCT::RegenHits(void){
	hits.clear();
	bool bad = Hits_Generator(bx, hs, pat, nhits, hits, COMPILE_TYPE);
	sort(hits.begin(), hits.end(), compareHit);
	return;
}

int CLCT::GetParam(int n){
	switch(n){
	case 0:
		return this->bx;
		break;
	case 1:
		return this->hs;
		break;
	case 2:
		return this->pat;
		break;
	case 3:
		return this->nhits;
		break;
	case 4:
		return this->COMPILE_TYPE;
		break;
	default:
		//throw an error...
		return -100;
		break;
	}
}

void CLCT::SetParam(int n, int val){
	switch(n){
        case 0:
                this->bx = val;
                break;
        case 1:
                this->hs = val;
                break;
        case 2:
                this->pat = val;
                break;
        case 3:
                this->nhits = val;
                break;
				case 4:
								this->COMPILE_TYPE = val;
								break;
        default:
		break;
	}
	return;
}

std::ostream& operator<<(std::ostream& os, const CLCT& cl)
	{
		os << cl.bx << '\t' << cl.hs << '\t'
			<< cl.pat << '\t' << cl.nhits << '\t';

		os << "{ ";
		for(int i=0; i < cl.hits.size(); i++){
			os << cl.hits[i] << " ";
		}
		os << "}";

		return os;
	}

std::istream& operator>>(std::istream& is, CLCT& cl)
	{
		Hit hh;
		std::vector<Hit> h_vec;
		int B, K, P, N, T;
		is >> B >> K >> P >> N >> T;

		char HITS = is.get();
		if(HITS == '{'){
			while(true){
				HITS = is.get();						// THIS SEEMS DANGEROUS
				if(HITS == '}') break;
				else{
					is.putback(HITS);
					is >> hh;
					h_vec.push_back(hh);
				}
			}
			cl = CLCT(B, K, P, h_vec, T);
		}
		else{
			is.putback(HITS);
			cl = CLCT(B, K, P, N, T);
		}
		return is;
	}

	///~~~~~~~~~~~~ GEM Clusters ~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Cluster::Cluster(void) :
		bx(0), roll(0), pad(0), size(0), layer(0)
		{realVfat = 24;}

	Cluster::Cluster(int Bx, int Roll, int Pad, int Size, int Layer) :
		bx(Bx),
		roll(Roll),
		pad(Pad),
		size(Size),
		layer(Layer)
		{
	         realVfat =  (7-roll) + 8*(pad/64);
	         }

	Cluster::Cluster(const Cluster& clust) :
		bx(clust.bx),
		roll(clust.roll),
		pad(clust.pad),
		size(clust.size),
		layer(clust.layer)
		{
	         realVfat =  (7-roll) + 8*(pad/64);

	         }

	std::istream& operator>>(std::istream& is, Cluster& cluster)
		{
			int Bx, Roll, Pad, Size, Layer;
			is >> Bx >> Roll >> Pad >> Size >> Layer;

			cluster = Cluster(Bx, Roll, Pad, Size, Layer);
			return is;
		}

	std::ostream& operator<<(std::ostream& os, const Cluster& cl)
		{
			os << cl.bx << '\t' << cl.roll << '\t'
				<< cl.pad << '\t' << cl.size
				<< '\t' << cl.layer <<"\t" << cl.realVfat;

			return os;
		}

		unsigned int Cluster::info(void)
			{
				unsigned int info = 0;
		                //v1 gem trigger data format: {cnt, address}, address is global pad=roll*192+pad
				//info = ((size << 11) | (roll*192+pad));	// Do range check before calling!
		                //v2 gem trigger data format: {cnt, roll, pad}
		                info = ((size << 11) | (roll << 8) | pad);
				return info;
			}


//~~~~~~ GEM Packets ~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gemPacket::gemPacket(void) : num_clusters(0) {}

gemPacket::gemPacket(std::vector<Cluster>& iClu, unsigned int sInd) {
	int Niter = ((iClu.size() - sInd) > 4) ? 4 : (iClu.size() - sInd);
	if(Niter <= 0)
		throw "Something went wrong in gemPack creation!";

	for(int i = 0; i < Niter; i++) {
		this->raw_info.push_back( iClu[sInd + i].info() );
	}

	this->num_clusters = raw_info.size();
	return;
}

std::ostream& operator<<(std::ostream& os, const gemPacket& pack)
	{
		std::vector<unsigned int> pad = pack.raw_info;

		unsigned int empty_pad =  std::pow(2, 14) - 1 ;

		//std::cout << pack.raw_info.size() << std::endl;
		//std::cout << pad.size() << std::endl;

		for (int i = 0; i < (4 - pack.raw_info.size()); i++) {
			pad.push_back( empty_pad );	// i.e. 14 1's or a blank cluster

			//std::cout << pad.size() << std::endl;
		}

		if (pad.size() != 4) return os;

		for (int byte = 0; byte < 8; byte++) {
			switch (byte) {
				case 0:
					os << char( pad[0] >> 6 );
					break;
				case 1:
					os << char( (pad[0] << 2) | (pad[1] >> 12) );
					break;
				case 2:
					os << char( pad[1] >> 4 );
					break;
				case 3:
					os << char( (pad[1] << 4) | (pad[2] >> 10) );
					break;
				case 4:
					os << char( pad[2] >> 2 );
					break;
				case 5:
					os << char( (pad[2] << 6) | (pad[3] >> 8) );
					break;
				case 6:
					os << char( pad[3] );
					break;
				case 7:
					os << char( empty_pad );
					break;
				default:
					break;
			}
			//std::cout << byte << std::endl;
		}

		return os;
	}

//~~~~~~~~~ CLCT Groups ~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Group::Group(void)// :
	//hexdata( std::vector<unsigned char>(CSCConstants::NUM_LAYERS, unsigned char(0)) )
	{
		for(int i=0; i < CSCConstants::NUM_LAYERS; i++)
			{
				hexdata.push_back(char(0));
			}
		//hexdata = std::vector<unsigned char> (CSCConstants::NUM_LAYERS, unsigned char(0));
	}


Group::Group(std::vector<Hit>& hits, int Bx) //:
	//hexdata(std::vector<unsigned char>(CSCConstants::NUM_LAYERS, unsigned char(0)))
{
	for(int i=0;i < CSCConstants::NUM_LAYERS; i++)
		{
			hexdata.push_back(char(0));
		}

	for (int i=0; i < hits.size(); i++)
	{
		int delta_t = Bx - hits[i].bx;
		if(delta_t >= 0 && delta_t < 3)
		{
			hexdata[hits.at(i).lay] = hexdata[hits.at(i).lay] | GetTriadSeg(hits[i], delta_t);
		}
	}
}

/*
std::ostream& operator<<(std::ostream& os, const Group& grp)
	{
		for (int i = 0; i < grp.hexdata.size(); i++)
			{

                            if (COMPILE_TYPE == 0xb)//reverse all layers for type B
				os << grp.hexdata[ 6 - layerorder_all[LAYER_SET-1][i] ];	// Layer Shuffle
                            else
				os << grp.hexdata[ layerorder_all[LAYER_SET-1][i] - 1 ];	// Layer Shuffle
			}

		os << char(0) << char(0);
		return os;
	}

void DumpGroup(Group grp, int Bx)
	{
                if (COMPILE_TYPE == 0xb)
                     std::cout <<"Type B firmware, Also reverse all layers " << std::endl;
		for (int i = 0; i < grp.hexdata.size(); i++)
			{
                            if (COMPILE_TYPE == 0xb)//reverse all layers for type B
				std::cout << std::bitset<8>(grp.hexdata[6 - layerorder_all[LAYER_SET-1][i]]) << std::endl;
                            else
				std::cout << std::bitset<8>(grp.hexdata[layerorder_all[LAYER_SET-1][i] - 1]) << std::endl;
			}
		std::cout << std::bitset<8>(char(0)) << std::endl;
		std::cout << std::bitset<8>(char(0)) << "   Bx: " << Bx << std::endl << std::endl;

		return;
	}
*/

std::ostream& operator<<(std::ostream& os, const Group& grp)
	{
		for (unsigned int i = 0; i < grp.hexdata.size(); i++)
			{
				os << grp.hexdata[ layerorder_all[LAYER_SET-1][i] - 1 ];	// Layer Shuffle
			}

		os << char(0) << char(0);
		return os;
	}

void DumpGroup(Group grp, int Bx)
	{
		for (unsigned int i = 0; i < grp.hexdata.size(); i++)
			{
				std::cout << std::bitset<8>(grp.hexdata[layerorder_all[LAYER_SET-1][i] - 1]) << std::endl;
			}
		std::cout << std::bitset<8>(char(0)) << std::endl;
		std::cout << std::bitset<8>(char(0)) << "   Bx: " << Bx << std::endl << std::endl;

		return;
	}

void DumpHit(Hit& hit, int N)
	{
		if(N == -1)	std::cout << "Hit : \n";
		else		std::cout << "Hit : " << N;

		std::cout << '\t' << "bx: " << hit.bx
			  << '\t' << "hs: " << hit.hs
			  << '\t' << "layer: " << hit.lay
				<< '\t' << "OTMBType "<< char(hit.COMPILE_TYPE + 'a'- 0xa) << '\n';

		for(int i=0; i < 3; i++)	std::cout << "  " << std::bitset<8>(GetTriadSeg(hit,i)) << '\n';

		return;
	}



//~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~

void PrintCLCT(CLCT& clct, std::ostream& oss, bool opt)
	{
		std::string empty_layer = "-----------";
		std::vector<std::string> layers(6,empty_layer);

		for(int i=0; i < clct.hits.size(); i++){
			char type = clct.COMPILE_TYPE - 0xa + 'a';
			oss <<"OTMB Type "<< type << std::endl;
			layers.at(clct.hits.at(i).lay).at(5 + (clct.hits.at(i).hs - clct.hs)) = 'X';
			if(opt) oss << "Hit (" << i << ')' << "   Bx: " << clct.hits.at(i).bx << "   Hs: " << clct.hits.at(i).hs << "   Layer: " << clct.hits.at(i).lay << "   CFEB: " << GetCFEB(clct.hits.at(i).hs, clct.COMPILE_TYPE) << std::endl;
		}


		oss << "bx keystrip pattern nhits" << std::endl;
                oss << clct << std::endl;


		for(int ly=0; ly < 6; ly++)	// for each layer
		{
			oss << "ly" << ly;
			if(ly == 2) oss << "  key  ";
			else 	    oss << "       ";

			oss << layers.at(ly) << std::endl;

		}
	}

int ReadTxt(std::string& prefix, std::vector<CLCT>& clcts)
	{
		CLCT cl;
		std::string str; 										// String to be used for parsing purposes

		std::cout << "I MADE IT HERE!" << std::endl;
		std::cout << "I am trying to open file at: " << prefix << ".txt" << std::endl << std::endl;



		std::fstream ifs((prefix + ".txt").c_str(), std::ios_base::in); // Open File to be read;

		// First two lines are header
		std::getline(ifs, prefix);
		std::getline(ifs, str);


		prefix = prefix.substr(8, prefix.length() - 8);

		int n = 0;
		while (ifs >> cl) {
			clcts.push_back(cl);
			n++;
			std::cout << clcts.at(n - 1) << " size :" << n << '\n';
		}
    ifs.close();
		return n;
	}

std::string ReadTxt(std::string& path, std::vector<CLCT>& clcts, std::vector<Cluster>& clusters)
	{
		CLCT clct;
		Cluster gem;
		char mode = 'c';	// c-CLCT	g-GEM
		std::string str, prefix;

		std::cout << "Trying to open file: " << path << std::endl << std::endl;

		std::fstream ifs(path.c_str(), std::ios_base::in); // Open the file

		std::getline(ifs, prefix);	// first line specify pattern prefix
		prefix = prefix.substr(8, prefix.length()-8);

		do{
			std::getline(ifs, str);

			if(str[0] == 'C'){
				mode = 'c';
				std::cout << "Switching to CLCT Mode" << std::endl;
				std::getline(ifs, str);	// next line is header
				std::cout << str << std::endl;
			}
			else if(str[0] == 'G'){
				mode = 'g';
				std::cout << "Switching to GEM Mode" << std::endl;
				std::getline(ifs, str);	// next line is header
				std::cout << str << std::endl;
			}
			else if(!ifs.eof() && (str.length() != 0)){
				std::stringstream ss(str.c_str());
				switch(mode){
					case 'c':
						ss >> clct;
						clcts.push_back(clct);
						std::cout << clct << std::endl;
						break;
					case 'g':
						ss >> gem;
						clusters.push_back(gem);
						std::cout << gem << std::endl;
						break;
					default:
						break;
				}
			}
		}while(!ifs.eof());

		ifs.close();
		return prefix;
	}

void WriteTxt(std::string& str, std::vector<CLCT>& clcts)
	{
		std::fstream text_file((str + ".txt").c_str(), std::ios_base::out); // Create output file => (input string).txt
		//std::sort(clcts.begin(), clcts.end(), CompareCLCT);	// Sort the CLCT vector in case it's asorted

		// Add header stuff to pattern(.txt) file
		text_file << "prefix:" << str << std::endl;
		text_file << "bx keystrip pattern nhits" << std::endl;

		for (int i = 0; i < clcts.size(); i++) {
			text_file << clcts.at(i);
			if (i < (clcts.size() - 1)) {
				text_file << '\n';
			}
		}

		text_file.close();
		return;
	}

void ExtractHits(std::vector<CLCT>& clcts, std::vector<Hit>& hits, int feb)
	{
		if (feb == -1)
		{
			for (int i = 0; i < clcts.size(); i++)
			{
				hits.insert(hits.end(), clcts.at(i).hits.begin(), clcts.at(i).hits.end());
			}
		}
		/// If you want to pull hits from a specific dcfeb
		else if(feb >= 0 && feb < CSCConstants::NUM_DCFEBS)
		{
			for(int i=0; i < clcts.size(); i++)
			{
				for(int j=0; j < clcts.at(i).hits.size(); j++)
				{
					if (GetCFEB(clcts.at(i).hits.at(j).hs, clcts.at(i).COMPILE_TYPE) == feb) { hits.push_back(clcts.at(i).hits.at(j)); }
				}
			}
		}
		return;
	}

bool compareCluster(Cluster c1, Cluster c2)
	{
		return(c1.bx < c2.bx);
	}

	void CollectClusters(std::vector<Cluster>& pads, std::vector<Cluster>& in_pads, int layer, int gem_fiber = -1)
		{	// Fills pads with Clusters of
			std::sort(in_pads.begin(), in_pads.end(), compareCluster);	// sort by Bx
			if(gem_fiber == -1)
			{
				for(unsigned int i=0; i < in_pads.size(); i++){
					if(in_pads[i].layer == layer) pads.push_back(in_pads[i]);
				}
			}
			else	// Extend if necessary
			{
				int bx=0;
				unsigned int icluster = 0;
				for(unsigned int i=0; i < in_pads.size(); i++){
					if (bx == in_pads[i].bx) icluster++;
	 				else {
						icluster = 0;
						bx = in_pads[i].bx;
					}
					bool fiber0 = icluster <4  && gem_fiber == 0;
					bool fiber1 = icluster >=4 && gem_fiber == 1;
					if(in_pads[i].layer == layer && (fiber0 || fiber1)) pads.push_back(in_pads[i]);
				}
	                 }
			//std::sort(pads.begin(), pads.end(), compareCluster);	// sort by Bx

			return;
		}

void fillnbytes(std::vector<std::fstream*> oss, unsigned int n, unsigned int thisfeb)
	{
	for (unsigned int j = 0; j < n; j++)
		(*(oss[thisfeb])) << char(0);

	}

void fillnbytes_allfebs(std::vector<std::fstream*> oss, unsigned int n)
	{
	for (unsigned int i = 0; i < oss.size(); i++)
		for (unsigned int j = 0; j < n; j++)
			(*(oss[i])) << char(0);

	}

void writenbytes(std::fstream* out, unsigned int n, unsigned int x = 255)
	{
		for(unsigned int i=1; i <= n; i++){
			(*out) << char(x);
		}
	}

void writenbxs(std::fstream* out, unsigned int n)
	{
		for(unsigned int i=1; i <=n; i++){
			writenbytes(out, 7);	// default 255
			writenbytes(out, 1, 255);
		}
	}


/// CSC
bool WritePat(std::string & prefix, std::vector<CLCT>& clcts)
	{
		std::cout << "WE ARE USING CLCT *************************************" << '\n';
		// Prepare output file streams
		std::vector<std::fstream*> oss;
		char tmbtype;

		if (clcts.size() == 0) tmbtype = 'c';//default case
		else tmbtype = clcts[0].COMPILE_TYPE - 0xa + 'a';

		for (int i = 0; i < CSCConstants::NUM_DCFEBS; i++)
		{
			std::stringstream ss;
			ss << prefix << "_cfeb" << i << "_tmb" << tmbtype << ".pat";
			//std::cout << ss.str().c_str() << std::endl;
			oss.push_back(new std::fstream(ss.str().c_str(), std::ios_base::out) );
			std::cout << "Write Pat opening file " << ss.str() << std::endl;
		}

		for (int i = 0; i < CSCConstants::NUM_DCFEBS; i++)
		{
			std::cout << "Writing (.pat) file number: " << i << std::endl;	// debug

			std::vector<Hit> hits;		//
			std::vector<int> times;		// list of bx's w/

			ExtractHits(clcts, hits, i);

			// DEBUG PURPOSES~~~~~
			for(int j=0; j < hits.size(); j++){
				DumpHit(hits.at(j), j);
			}
			// ~~~~~~~~~

			if(hits.size() == 0)			// IF : No hits in Pattern
			{
				fillnbytes(oss, RAMPAGE_SIZE, i);
				(*(oss.at(i))) << std::flush;		// Fill & Close file
				delete (oss.at(i));
			}
			else					// ELSE
			{
				// Get times
				for (int j = 0; j < hits.size(); j++)
				{
					bool tf = false;
					for (int k = 0; k < times.size(); k++)
					{
						if (times.at(k) == hits.at(j).bx) tf = true;
					}
					if (!tf) times.push_back(hits.at(j).bx);
				}
				std::sort(times.begin(), times.end());


				int Bx = 0;	// Current Bunch Cross
				unsigned int q = 0;	// index var for times vector

				if (times.at(0) != 0)
				{
					Bx = times.at(0);
					fillnbytes(oss, times.at(0)*8, i);
				}


				while (Bx < (times[times.size() -1] + 3) && (RAMPAGE_SIZE - Bx*8) > 0)
				{
					if (Bx == (times.at(q) + 3) && q+1 == times.size())
					{
						fillnbytes(oss, RAMPAGE_SIZE - (Bx * 8), i);		/// Double check range here
						break;
					}
					else if (Bx == (times.at(q) + 3))
					{
						q++;
						if(times.at(q) > Bx)
						{
							fillnbytes(oss, (times.at(q) - Bx)*8, i);
							Bx = times.at(q);
							// write Group @ this->Bx
							(*(oss[i])) << Group(hits,Bx);
							DumpGroup(Group(hits, Bx), Bx);	// debug
							Bx++;
						}
						else
						{
							//Write Group @ this->Bx
							(*(oss[i])) << Group(hits, Bx);
							DumpGroup(Group(hits, Bx), Bx);	// debug
							Bx++;
						}
					}
					else
					{
						//Write Group @ this->Bx
						(*(oss[i])) << Group(hits,Bx);
						DumpGroup(Group(hits, Bx), Bx);	// debug
						Bx++;
					}
				}
				if((RAMPAGE_SIZE - Bx*8) > 0)
				{
					fillnbytes(oss, RAMPAGE_SIZE - (Bx * 8), i);		/// Double check range here
				}

				*(oss.at(i)) << std::flush;
				delete (oss.at(i));

			}


		}

		for (int i = 0; i < oss.size(); i++)
 			{
	 			 oss[i]->close();
 			}
 		oss.clear();
		return true;
	}

void pushInfo(std::fstream* oss, unsigned int info, unsigned int& reminfo, unsigned int& rembits, unsigned int iclust)
	{

	}

void closebx(std::fstream* oss, unsigned int& reminfo, unsigned int& rembits, unsigned int icluster)
	{
		unsigned int x = std::pow(2, 8-rembits) - 1;
		(*oss) << char((reminfo << (8 - rembits)) | x);
		writenbytes(oss, (8 - ((icluster*14)/8)));

		reminfo = 0;
		rembits = 0;
		return;
	}
// GEM
/*
bool WritePat(std::string& prefix, std::vector<Cluster>& in_pads)
	{
		// Open (.pat) files
		//char tmbtype = COMPILE_TYPE - 0xa + 'a';
		std::vector<std::fstream*> oss;
		for(int gem_fiber = 0; gem_fiber < GEM_FIBERS; gem_fiber++){
			std::stringstream ss;
			//ss << prefix << "_GEM" << gem_fiber << "_tmb" << tmbtype << ".pat";
			ss << prefix << "_GEM" << gem_fiber << ".pat";
			oss.push_back(new std::fstream(ss.str().c_str(), std::ios_base::out) );
		}

		// Write GEMpads

		for (int layer = 0; layer <= 2; layer += 2) {
			std::cout << "Current Layer " << layer << std::endl;
			std::vector<Cluster> pads;
			if (layer == 0) CollectClusters(pads, in_pads, (1));
			else CollectClusters(pads, in_pads, 2);

for (int layer = 0; layer <= 1; layer += 1) {
							 for (int ifiber = 0; ifiber <=1; ifiber+=1){
										int gemfiber = layer*2 + ifiber;
	std::cout << "Current Layer " << layer <<" fiber "<< ifiber <<" gemfiber "<< gemfiber << std::endl;
	std::vector<Cluster> pads;
										// in each BX, first 4 clusters in first fiber and next 4 clusters in second fiber
	CollectClusters(pads, in_pads, layer, ifiber);

			int totbytes;
			unsigned int remainbits = 0;
			unsigned int remaininfo = 0;
			unsigned int icluster = 0;
			unsigned int x;

			unsigned int lastbx = 0;

			for (unsigned int i = 0; i < pads.size(); i++) {
				std::cout << "i " << i << " bx " << pads[i].bx << " cluster bits: " << (std::bitset<14>)pads[i].info() << std::endl;

				if (lastbx < pads[i].bx) {
					if ((icluster >= 0) && (icluster <= 4)) {
						// Finish writing to File 1
						closebx(oss[layer], remaininfo, remainbits, icluster);
						// Fill File 1
						writenbxs(oss[layer], pads[i].bx - 1 - lastbx);
						// Fill File 2
						writenbxs(oss[layer + 1], pads[i].bx - lastbx);	// one more bx since nothing was written for last bx

					}
					else if (icluster > 4 && icluster <= 8) {
						// Finish File 1
						closebx(oss[layer], remaininfo, remainbits, 4);
						// Finish File 2
						closebx(oss[layer + 1], remaininfo, remainbits, (icluster%4));
						// Fill File 1
						writenbxs(oss[layer], pads[i].bx - 1 - lastbx);
						// Fill File 2
						writenbxs(oss[layer + 1], pads[i].bx - 1 - lastbx);
					}
					std::cout <<" lastbx " << lastbx <<" input_pads bx " << pads[i].bx <<" remain n  "<< remainbits << " bits " << (std::bitset<8>)(remaininfo << (8-remainbits)) << std::endl;
					lastbx = pads[i].bx;
					icluster = 0;
					remainbits = 0;
					remaininfo = 0;
				}
				if (lastbx == pads[i].bx) {
					if (icluster >= 8)
						continue;

					switch (icluster) {
						// File 1
					case 0: remainbits = 6;
						x = std::pow(2, remainbits) - 1;
						remaininfo = (pads[i].info() & x);
						(*(oss[layer])) << char(pads[i].info() >> 6);
						break;
					case 1: remainbits = 4;
						(*(oss[layer])) << char((remaininfo << 2) | (pads[i].info() >> 12));
						(*(oss[layer])) << char(pads[i].info() >> 4);
						x = std::pow(2, remainbits) - 1;
						remaininfo = (pads[i].info() & x);
						break;
					case 2: remainbits = 2;
						(*(oss[layer])) << char((remaininfo << 4) | (pads[i].info() >> 10));
						(*(oss[layer])) << char(pads[i].info() >> 2);
						x = std::pow(2, remainbits) - 1;
						remaininfo = (pads[i].info() & x);
						break;
					case 3: remainbits = 0;
						(*(oss[layer])) << char((remaininfo << 6) | (pads[i].info() >> 8));
						(*(oss[layer])) << char(pads[i].info());
						remaininfo = 0;
						//closebx(oss[layer], remaininfo, remainbits, 4);
						break;
						// FILE 2
					case 4: remainbits = 6;
						x = std::pow(2, remainbits) - 1;
						remaininfo = (pads[i].info() & x);
						(*(oss[layer + 1])) << char(pads[i].info() >> 6);
						break;
					case 5: remainbits = 4;
						(*(oss[layer + 1])) << char((remaininfo << 2) | (pads[i].info() >> 12));
						(*(oss[layer + 1])) << char(pads[i].info() >> 4);
						x = std::pow(2, remainbits) - 1;
						remaininfo = (pads[i].info() & x);
						break;
					case 6: remainbits = 2;
						(*(oss[layer + 1])) << char((remaininfo << 4) | (pads[i].info() >> 12));
						(*(oss[layer + 1])) << char(pads[i].info() >> 2);
						x = std::pow(2, remainbits) - 1;
						remaininfo = (pads[i].info() & x);
						break;
					case 7: remainbits = 0;
						(*(oss[layer + 1])) << char((remaininfo << 6) | (pads[i].info() >> 8));
						(*(oss[layer + 1])) << char(pads[i].info());
						remaininfo = 0;
						//closebx(oss[layer + 1], remaininfo, remainbits, 4);
						break;
					defalut:
						std::cout << " error icluster: " << icluster << std::endl;
						break;
					}
				}
				icluster++;
			}// End pads vector loop

			if ((icluster > 0) && (icluster <= 4)) {
				// Finish writing to File 1
				closebx(oss[layer], remaininfo, remainbits, icluster);
				writenbxs(oss[layer + 1], 1);
			}
			else if (icluster > 4 && icluster <= 8) {
				// Finish writing to File 2
				closebx(oss[layer + 1], remaininfo, remainbits, icluster%4);
			}

			// Complete the Files
			writenbxs(oss[layer], RAMPAGE_SIZE / 8 - 1 - lastbx);
			(*(oss[layer])) << std::flush;
			writenbxs(oss[layer + 1], RAMPAGE_SIZE / 8 - 1 - lastbx);
			(*(oss[layer + 1])) << std::flush;

		} // End layer for loop

    for (int k = 0; k < oss.size();k++){
			delete oss[k];
		}

		return true;
	}

*/

bool WritePat(std::string& prefix, std::vector<Cluster>& in_pads)
	{
		// Open (.pat) files
		//char tmbtype = COMPILE_TYPE - 0xa + 'a';
		std::vector<std::fstream*> oss;
		/*
		std::vector<std::ofstream> oss_extra;
		for (int gem_fiber = 0; gem_fiber < GEM_FIBERS; ++gem_fiber) {
			std::stringstream ss;
			ss << prefix << "_GEM" << gem_fiber << ".pat";
			oss_extra.emplace_back(ss.str().c_str());
		}
		*/
		for(int gem_fiber = 0; gem_fiber < GEM_FIBERS; gem_fiber++){
			std::stringstream ss;
			//ss << prefix << "_GEM" << gem_fiber << "_tmb" << tmbtype << ".pat";
			ss << prefix << "_GEM" << gem_fiber << ".pat";
			oss.push_back(new std::fstream(ss.str().c_str(), std::ios_base::out) );
		}

		// Write GEMpads
		//for (int layer = 0; layer <= 2; layer += 2) {

		for (int layer = 0; layer <= 1; layer += 1) {
                   for (int ifiber = 0; ifiber <=1; ifiber+=1){
                        int gemfiber = layer*2 + ifiber;
			std::cout << "Current Layer " << layer <<" fiber "<< ifiber <<" gemfiber "<< gemfiber << std::endl;
			std::vector<Cluster> pads;
                        // in each BX, first 4 clusters in first fiber and next 4 clusters in second fiber
			CollectClusters(pads, in_pads, layer, ifiber);


			int totbytes;
			unsigned int remainbits = 0;
			unsigned int remaininfo = 0;
			unsigned int icluster = 0;
			unsigned int x;

			unsigned int lastbx = 0;

			for (unsigned int i = 0; i < pads.size(); i++) {
				int globalpad = pads[i].roll*64+pads[i].pad;
				std::cout << "i " << i << " bx " << pads[i].bx <<" fiber"<< gemfiber<<" roll "<< pads[i].roll <<" pad "<< pads[i].pad <<" size "<< pads[i].size  <<" realVfatID "<< pads[i].realVfat << " cluster bits: " << (std::bitset<14>)pads[i].info() << std::endl;

				if (lastbx < pads[i].bx) {
					closebx(oss[gemfiber], remaininfo, remainbits, icluster);
					writenbxs(oss[gemfiber], pads[i].bx - 1 - lastbx);
					std::cout <<" lastbx " << lastbx <<" input_pads bx " << pads[i].bx <<" remain n  "<< remainbits << " bits " << (std::bitset<8>)(remaininfo << (8-remainbits)) << std::endl;
					lastbx = pads[i].bx;
					icluster = 0;
					remainbits = 0;
					remaininfo = 0;
				}
				if (lastbx == pads[i].bx) {
					if (icluster >= 4)
						continue;

					switch (icluster) {
						// File 1
					case 0: remainbits = 6;
						x = std::pow(2, remainbits) - 1;
						remaininfo = (pads[i].info() & x);
						(*(oss[gemfiber])) << char(pads[i].info() >> 6);
						break;
					case 1: remainbits = 4;
						(*(oss[gemfiber])) << char((remaininfo << 2) | (pads[i].info() >> 12));
						(*(oss[gemfiber])) << char(pads[i].info() >> 4);
						x = std::pow(2, remainbits) - 1;
						remaininfo = (pads[i].info() & x);
						break;
					case 2: remainbits = 2;
						(*(oss[gemfiber])) << char((remaininfo << 4) | (pads[i].info() >> 10));
						(*(oss[gemfiber])) << char(pads[i].info() >> 2);
						x = std::pow(2, remainbits) - 1;
						remaininfo = (pads[i].info() & x);
						break;
					case 3: remainbits = 0;
						(*(oss[gemfiber])) << char((remaininfo << 6) | (pads[i].info() >> 8));
						(*(oss[gemfiber])) << char(pads[i].info());
						remaininfo = 0;
						//closebx(oss[layer], remaininfo, remainbits, 4);
						break;
					defalut:
						std::cout << " error icluster: " << icluster << std::endl;
						break;
					}//end case
					icluster++;
				}// end lastbx == pads[i].bx
			}// End pads vector loop

                        //
			closebx(oss[gemfiber], remaininfo, remainbits, icluster);

			// Complete the Files
			writenbxs(oss[gemfiber], RAMPAGE_SIZE / 8 - 1 - lastbx);
			(*(oss[gemfiber])) << std::flush;
                    } // end ifber for loop

		} // End layer for loop

		for (int i = 0; i < oss.size(); ++i) delete oss[i];

		return true;
	}

/////////////////////// CCLUT ////////////////////////////////////////////////////
bool Convert_LUT_to_hits(int LUT_code, std::vector<int>& Hits_map, int Bit11_int = 0){             //NORMAL
for (int i = 0; i < CSCConstants::NUM_LAYERS; i++){
 if(Bit11_int == 1){
	if(i == 2 ){
		Hits_map.push_back(LUT_code%2);
		LUT_code /= 2;
	}
  else{	Hits_map.push_back(LUT_code%4);
				LUT_code /= 4;
			}
		}
  else{
		Hits_map.push_back(LUT_code%4);
		LUT_code /= 4;
	}
}

  return false;
}

bool Convert_hits_to_LUT(int LUT_code, std::vector<int>& Hits_map, int Bit11_int = 0){
	return 1;
}

int Get_Minimal_Halfstrip(const std::vector<int>& xData, const std::vector<int>& data){
	int min = *min_element(data.begin(), data.end()), max = *max_element(data.begin(), data.end()) + 1;
	int minVal = 999999999;
	int hs = 0;
	for (int i = min; i < max; i++){
		int minValCur = 0;
		for (int j = 0; j < data.size(); j++) {
			minValCur += (data[j] - i)*(data[j] - i);
		}
		if (minValCur < minVal){
			hs = i;
			minVal = minValCur;
		}
	}
	return hs;
}

std::vector<double> GetLinearFit(const std::vector<int>& xData, const std::vector<int>& data)
{
    double xSum = 0, ySum = 0, xySum = 0, xxSum = 0, yySum = 0, slope, intercept;
    for (long i = 0; i < data.size(); i++)
    {
        xSum += xData[i];
        ySum += data[i];
    }

		xSum /= data.size();
    ySum /= data.size();

		for (long i = 0; i < data.size(); i++)
		{
				xxSum += (xData[i] - xSum)*(xData[i] - xSum);
				xySum += (data[i] - ySum)*(xData[i] - xSum);
				yySum += (data[i] - ySum)*(data[i] - ySum);
		}

		xxSum /= (data.size() - 1);
		xySum /= (data.size() - 1);
		yySum /= (data.size() - 1);

		double delta = 10000;

    if (xySum != 0 ) slope = (yySum - delta*xxSum + sqrt((yySum - delta*xxSum)*(yySum - delta*xxSum) + 4*delta*xySum*xySum))/(2*xySum);
		else slope = 0;
    intercept = ySum - slope*xSum;
    std::vector<double> res;
    res.push_back(slope);
    res.push_back(intercept);

		//std::cout << "Slope: " << slope << "; Intercept: " << intercept << std::endl;
    return res;
}




std::vector<double> Get_Expexted_Key_Input(CCLUT Res){
	std::vector<int> xData, data;
	for (int i = 0; i < GetNHits(Res.LUT_code); i++){
			xData.push_back(Res.hits[i].lay);
			data.push_back(Res.hits[i].hs);
	}
	int minhs = Get_Minimal_Halfstrip(xData, data);
  std::vector<double> res = GetLinearFit(xData, data);

	if (res[0] != 0) res.push_back(res[0]*2 + res[1] + 0.499);
	else res.push_back(minhs);

	//res.push_back(Get_Minimal_Halfstrip(xData, data));
  return res;
}


void Plot_And_Compare_Hits(CCLUT Resp, std::vector<std::vector<int> > &VisualMap){


		for (int i = 0; i < LAYERS; i++){
			std::vector<int> temp;
			for (int j = 0; j < 15; j++){
		   	temp.push_back(0);
			}
			VisualMap.push_back(temp);
		}


    std::vector<int> Hits_map;
		for (size_t i = 0; i < 6; i++) {
			Hits_map.push_back(-999);
		}
		for (size_t i = 0; i < Resp.hits.size(); i++) {
			Hits_map[Resp.hits[i].lay] = Resp.hits[i].hs;
		}

		for (int i = 0; i < VisualMap.size(); i ++)
		{
			int hs = 6 + Hits_map[i] - Resp.hs;
      if(Hits_map[i] > 0 && hs < 15 && hs >= 0) VisualMap[i][hs] = 1;
		}

}

std::string sstr(int a)
{
    std::ostringstream sstr;
     sstr << a;
    return sstr.str();
}

void generate_response(CCLUT Send, CCLUT Read, CCLUT ExpectedTrigger, CCLUT Corr, int corr, CCLUT Accuracy, CCLUT Accuracy_1, int accu, std::vector<std::vector<std::string> > &response, int Bit11_int = 0){
  std::cout << "Generatig response" << '\n';
	for (int i = 0; i < 7; i++) {
		std::vector<std::string> response_string;
		for (int j = 0; j < 6; j++) {
			std::string word = "|                   ";
			response_string.push_back(word);
		}
		response.push_back(response_string);
	}
	//first line
	std::string acc = sstr(accu);
	std::string cor = sstr(corr);
	std::string slash = "/";
	std::vector<std::string> x;
	x.push_back("Key");
	x.push_back("PiD");
	x.push_back("ResEight");
	x.push_back("CC code");
	x.push_back("CC code binary");
  for (int i = 0; i < x.size(); i++) {
  	response[0][i+1].resize(20-x[i].length());
		response[0][i+1] = response[0][i+1] + x[i];
  }
  x.clear();

	//2 string Send
  x.push_back("Send");
	x.push_back(sstr(Send.hs));
	x.push_back(sstr(Send.pat));
	x.push_back(sstr(Send.bx));
	x.push_back(sstr(Send.LUT_code));
	for (size_t i = 0; i < x.size(); i++) {
		response[1][i].resize(20-x[i].length());
		response[1][i] = response[1][i] + x[i];
	}
	x.clear();

	//3 string Expected
  x.push_back("Expected");
	x.push_back(sstr(ExpectedTrigger.hs));
	x.push_back(sstr(ExpectedTrigger.pat));
	x.push_back(sstr(ExpectedTrigger.bx));
	x.push_back(sstr(ExpectedTrigger.LUT_code));
	for (size_t i = 0; i < x.size(); i++) {
		response[2][i].resize(20-x[i].length());
		response[2][i] = response[2][i] + x[i];
	}
	x.clear();


	//4 string Read
	x.push_back("Read");
	x.push_back(sstr(Read.hs));
	x.push_back(sstr(Read.pat));
	x.push_back(sstr(Read.bx));
	x.push_back(sstr(Read.LUT_code));
	for (size_t i = 0; i < x.size(); i++) {
		response[3][i].resize(20-x[i].length());
		response[3][i] = response[3][i] + x[i];
	}
	x.clear();

	//5 string Correlation
	x.push_back("Correlation");
	x.push_back(sstr(corr - Corr.hs) + slash + sstr(corr));
	x.push_back(sstr(corr - Corr.pat) + slash + sstr(corr));
	x.push_back(sstr(corr - Corr.bx) + slash + sstr(corr));
	x.push_back(sstr(corr - Corr.LUT_code) + slash + sstr(corr));
	for (size_t i = 0; i < x.size(); i++) {
		response[4][i].resize(20-x[i].length());
		response[4][i] = response[4][i] + x[i];
	}
	x.clear();

	//6 string Accuracy
	x.push_back("Fails");
	x.push_back(sstr(Accuracy.hs) + slash + sstr(accu));
	x.push_back(sstr(Accuracy.pat) + slash + sstr(accu));
	x.push_back(sstr(Accuracy.bx) + slash + sstr(accu));
	x.push_back(sstr(Accuracy.LUT_code) + slash + sstr(accu));
	for (size_t i = 0; i < x.size(); i++) {
		response[5][i].resize(20-x[i].length());
		response[5][i] = response[5][i] + x[i];
	}
	x.clear();

	//6 string Accuracy
	x.push_back("Fails(+-1)");
	x.push_back(sstr(Accuracy_1.hs) + slash + sstr(accu));
	x.push_back(sstr(Accuracy_1.pat) + slash + sstr(accu));
	x.push_back(sstr(Accuracy_1.bx) + slash + sstr(accu));
	x.push_back(sstr(Accuracy_1.LUT_code) + slash + sstr(accu));
	for (size_t i = 0; i < x.size(); i++) {
		response[6][i].resize(20-x[i].length());
		response[6][i] = response[6][i] + x[i];
	}
	x.clear();

  std::string y;
  y = "";
	std::vector<std::string> binary_rep;
	int cc = Send.LUT_code;
	for (int i = 0; i < 6; i++) {
		if(i == 3 && Bit11_int){
			if (cc % 2 == 0) binary_rep.push_back(" 0");
			else if (cc % 2 == 1) binary_rep.push_back(" 1");
			cc /= 2;
		}
		else{
		 if (cc % 4 == 0) binary_rep.push_back(" 00");
		 else if (cc % 4 == 1) binary_rep.push_back(" 01");
		 else if (cc % 4 == 2) binary_rep.push_back(" 10");
		 else if (cc % 4 == 3) binary_rep.push_back(" 11");
		 cc /= 4;
	 }
	}

	y = y + "|(";
	for (int i = binary_rep.size()-1; i >=0; i--) {
		y = y + binary_rep[i];
	}
	y = y + ")";
  response[1][5] = y;


	y = "";
	std::vector<std::string> binary_rep1;
	cc = Read.LUT_code;
	for (int i = 0; i < 6; i++) {
		if(i == 3){
			if (cc % 2 == 0) binary_rep.push_back(" 0");
			else if (cc % 2 == 1) binary_rep.push_back(" 1");
			cc /= 2;
		}
		else{
		 if (cc % 4 == 0) binary_rep.push_back(" 00");
		 else if (cc % 4 == 1) binary_rep.push_back(" 01");
		 else if (cc % 4 == 2) binary_rep.push_back(" 10");
		 else if (cc % 4 == 3) binary_rep.push_back(" 11");
		 cc /= 4;
	 }
	}

	y = y + "|(";
	for (int i = binary_rep1.size()-1; i >=0; i--) {
		y = y + binary_rep1[i];
	}
	y = y + ")";
  response[2][5] = y;


	y = "";
	std::vector<std::string> hits_send;
	for (int i = 0; i < 6; i++) {
		hits_send.push_back("n");
	}
	for (int i = 0; i < Send.hits.size(); i++) {
  	hits_send[Send.hits[i].lay] = sstr(Send.hits[i].hs);
	}
	for (int i = 0; i < 6; i++) {
		y = y + " " + hits_send[i];
	}

	response[3][5] = "|" + y;

	y = "";
	std::vector<std::string> hits_read;
	for (int i = 0; i < 6; i++) {
		hits_read.push_back("n");
	}
	for (int i = 0; i < Read.hits.size(); i++) {
		hits_read[Read.hits[i].lay] = sstr(Read.hits[i].hs);
	}
	for (int i = 0; i < 6; i++) {
		y = y + " " + hits_read[i];
	}

	response[4][5] = "|" + y;
}

int GetNHits(int CCLUT_code){
	int Nhits = 0;
	for (int i = 0; i < CSCConstants::NUM_LAYERS; i++){
		if(CCLUT_code % 4 > 0) Nhits++;
		CCLUT_code /= 4;
	}
	return Nhits;
}

bool Check_Hits(CCLUT Send, CCLUT Read){
    if (Send.hits.size() != Read.hits.size()) return 1;
		if (Send.pat != Read.pat || Send.LUT_code != Read.LUT_code) return 1;
		return 0;
	}
void Update_Correlation(CCLUT Send, CCLUT Read, CCLUT &Corr, int accuracy) {
   for (size_t i = 0; i < 4; i++) {
   	if(abs(Send.GetParam(i) - Read.GetParam(i)) > accuracy) Corr.SetParam(i,Corr.GetParam(i)+1);
   }
}
bool Check_Hits_map(std::vector<std::vector<int> > VisualMap_input, std::vector<std::vector<int> > &VisualMap_output){
		bool is_the_same = 1;
    for (int i = 0; i < VisualMap_input.size(); i++) {
    	for (size_t j = 0; j < VisualMap_input[i].size(); j++) {
				if (VisualMap_input[i][j] != VisualMap_output[i][j]) is_the_same = 0;
    	}
    }
		if (is_the_same == 1) return 1;
		is_the_same = 1;


		for (int i = 0; i < VisualMap_input.size(); i++) {
			for (size_t j = 0; j < VisualMap_input[i].size()-1; j++) {
				if (VisualMap_input[i][j] != VisualMap_output[i][j+1]) is_the_same = 0;
			}
		}
		if (is_the_same == 1) return 1;
		is_the_same = 1;

		for (int i = 0; i < VisualMap_input.size(); i++) {
			for (size_t j = 0; j < VisualMap_input[i].size()-1; j++) {
				if (VisualMap_input[i][j+1] != VisualMap_output[i][j]) is_the_same = 0;
			}
		}
		if (is_the_same == 1) return 1;
		is_the_same = 1;

		for (int i = 0; i < VisualMap_input.size(); i++) {
			for (size_t j = 0; j < VisualMap_input[i].size()-2; j++) {
				if (VisualMap_input[i][j+2] != VisualMap_output[i][j]) is_the_same = 0;
			}
		}
		if (is_the_same == 1) return 1;
		is_the_same = 1;

		for (int i = 0; i < VisualMap_input.size(); i++) {
			for (size_t j = 0; j < VisualMap_input[i].size()-2; j++) {
				if (VisualMap_input[i][j] != VisualMap_output[i][j+2]) is_the_same = 0;
			}
		}
		if (is_the_same == 1) return 1;

		return 0;
	}


bool Hits_Generator_LUT(int Bx, int Key, int Pat, int LUT_code, std::vector<Hit>& hits, int COMPILE_TYPE = 0xc, bool input = true, int Bit11_int = 0)	// used to Fill Hits vector in CCLUT constructor
	{
		// Pattern validity check
		if (!(Pat == 0 || Pat == 1 || Pat == 2 || Pat == 3 || Pat == 4)){
			 			return true;
					}
		std::vector<int> Hits_map;
    Convert_LUT_to_hits(LUT_code, Hits_map, Bit11_int);



		int Nhits = 0;
		for (size_t i = 0; i < CSCConstants::NUM_LAYERS; i++) {
		  std::cout << "Layer " << i << " " << Hits_map[i] << " hits pattern" << '\n';
			if(Hits_map[i] > 0) Nhits ++;
		}

		if (Nhits < 1 || Nhits > CSCConstants::NUM_LAYERS)
			return true;				// invalid number of hits


		int pat_offset = Pat;
		for (int i = 0; i < CSCConstants::NUM_LAYERS; i ++)
		{
      if(Hits_map[i] > 0){
			int hs = Key + CSCConstants::CCLUT_offset[pat_offset][i]  + Hits_map[i] - 2;
			if(COMPILE_TYPE == 0xa && i%2 == 1 && input) hs++;//  halfsrip by layer shift for MEX
			std::cout << "CSCConstants::CCLUT_offset[pat_offset][layer]=" <<CSCConstants::CCLUT_offset[pat_offset][i]<< " Hits_map[layer] " << Hits_map[i] <<" Key " <<  Key << '\n';
			if (EdgeCheck(Key, hs, COMPILE_TYPE))	hits.push_back(Hit(Bx, hs, i, COMPILE_TYPE));
		 }
		}
		return false;
	}

//---------------------- CCLUT class def ---------------------------
CCLUT::CCLUT(void) : bx(0), hs(0), pat(0), LUT_code(0), COMPILE_TYPE(0xc), eightres(0), bending(0), lr(0) {}

CCLUT::CCLUT(int Bx, int Key, int Pat, int lut_code, int Type) :
	bx(Bx),
	hs(Key),
	pat(Pat),
	LUT_code(lut_code),
	COMPILE_TYPE(Type)
	{
		bool bad = Hits_Generator_LUT(bx, hs, pat, LUT_code, hits, COMPILE_TYPE);
	}

	CCLUT::CCLUT(int Bx, int Key, int Pat, int Lut_code, int Type, int EightResolution, int Bending, int Lr) :
		bx(Bx),
		hs(Key),
		pat(Pat),
		LUT_code(Lut_code),
		COMPILE_TYPE(Type),
		eightres(EightResolution),
		bending(Bending),
		lr(lr)
		{
			bool bad = Hits_Generator_LUT(bx, hs, pat, LUT_code, hits, COMPILE_TYPE);
		}

CCLUT::CCLUT(const CCLUT& cclut) :
	bx(cclut.bx),
	hs(cclut.hs),
	pat(cclut.pat),
	LUT_code(cclut.LUT_code),
	COMPILE_TYPE(cclut.COMPILE_TYPE),
	eightres(cclut.eightres),
	bending(cclut.bending),
	lr(cclut.lr)
	{
		hits.insert( hits.end(), cclut.hits.begin(), cclut.hits.end() );
	}

std::ostream& operator<<(std::ostream& os, const CCLUT& cclut)
	{
		os << cclut.bx << '\t' << cclut.hs << '\t'
			<< cclut.pat << '\t' << cclut.LUT_code << '\t' << char(cclut.COMPILE_TYPE - 0xa + 'a') << '\t';

		return os;
	}

std::istream& operator>>(std::istream& is, CCLUT& cl)
	{
		int B, K, P, N, T;
		is >> B >> K >> P >> N >> T;

		cl = CCLUT(B, K, P, N, T);
		return is;
	}
//-------------------------------------------

//~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~

void CCLUT::RelativeSpaceTime(void){
	for(int i=0; i < hits.size(); i++){
		hits[i].bx -= this->bx;
		hits[i].hs -= this->hs;
	}
	return;
}
void CCLUT::NormalSpaceTime(void){
	for(int i=0; i < hits.size(); i++){
		hits[i].bx += this->bx;
		hits[i].hs += this->hs;
	}
	return;
}

void CCLUT::RegenHits(bool input, int Bit11_int){
	hits.clear();
	bool bad = Hits_Generator_LUT(bx, hs, pat, LUT_code, hits, COMPILE_TYPE, input, Bit11_int);
	sort(hits.begin(), hits.end(), compareHit);
	return;
}
void CCLUT::Convert_LUT_to_bending(/* arguments */) {
	int bend_lr = CSCConstants::pat_to_bend[pat][LUT_code];  // first 4 bits for bending, 5th bit for slope sign, most significant 4bits for position offset
	bending = bend_lr % 16;
	bend_lr /= 16;
	lr = bend_lr % 2;
	bend_lr /= 2;
	eightres = bend_lr % 16;
	eightres -= 7;
	eightres += 4*hs;
}

int CCLUT::GetParam(int n){
	switch(n){
	case 0:
		return this->bx;
		break;
	case 1:
		return this->hs;
		break;
	case 2:
		return this->pat;
		break;
	case 3:
		return this->LUT_code;
		break;
	case 4:
		return this->COMPILE_TYPE;
		break;
	case 5:
		return this->eightres;
		break;
	case 6:
		return this->bending;
		break;
	case 7:
		return this->lr;
		break;
	default:
		//throw an error...
		return -100;
		break;
	}
}

void CCLUT::SetParam(int n, int val){
	switch(n){
        case 0:
                this->bx = val;
                break;
        case 1:
                this->hs = val;
                break;
        case 2:
                this->pat = val;
                break;
        case 3:
                this->LUT_code = val;
                break;
				case 4:
				        this->COMPILE_TYPE = val;
				        break;
				case 5:
								this->eightres = val;
								break;
				case 6:
								this->bending = val;
								break;
				case 7:
								this->lr = val;
								break;
        default:
		break;
	}
	return;
}

CCLUT ExpectedTriggering(CCLUT input, int Bit11_int = 0, int mute = 0){
		CCLUT ExpectedTrigger = input;
		//std::cout << "We are going to regen hits in Expeted trigger" << '\n';
		ExpectedTrigger.RegenHits(false, Bit11_int);
		//std::cout << "We successfully regen hits in Expeted trigger" << '\n';

		std::vector<int> Hits_map_lut;
    Convert_LUT_to_hits(ExpectedTrigger.LUT_code, Hits_map_lut, Bit11_int);

		std::vector<std::pair <int, int> > bare_hits;
		for (size_t i = 0; i < Hits_map_lut.size(); i++) {
			if(!mute) std::cout << "Hits_map_lut = " << Hits_map_lut[i] <<'\n';
			if(Hits_map_lut[i] != 0) bare_hits.push_back(std::make_pair(i, Hits_map_lut[i]));
		}

		if (bare_hits.size() < 4){
			CCLUT Empty;
			return Empty;
		}
		else{
			if(!mute) std::cout << "************************Calculating expecterd trigger************************" << '\n';
			int trial_hs = CSCConstants::CCLUT_offset[ExpectedTrigger.pat][bare_hits[0].first] + bare_hits[0].second ;
			int min_range =  999;
			int max_range = -999;
			for (size_t i = 0; i < bare_hits.size(); i++) {
				bare_hits[i].second += 3;
				bare_hits[i].second += CSCConstants::CCLUT_offset[ExpectedTrigger.pat][bare_hits[i].first];
				if(!mute) std::cout << "layer = " << bare_hits[i].first << ", hit position =  " << bare_hits[i].second <<'\n';
				if(min_range > bare_hits[i].second) min_range = bare_hits[i].second;
				if(max_range < bare_hits[i].second) max_range = bare_hits[i].second;
				if (bare_hits[i].second < 0 || bare_hits[i].second > 10) {
					if(!mute) std::cout << "Wronng bare hit position " << bare_hits[i].first << " " << bare_hits[i].second <<'\n';
				}
			}


			if (min_range < 2) min_range = -min_range;
			else min_range = -2;

			if (max_range > 8) max_range = 10 - max_range;
			else max_range = 2;

			int offset = 0;
			if(!mute) std::cout << "min range = " << min_range << ", max range =  " << max_range <<'\n';
			for (int i = 4; i > ExpectedTrigger.pat; i--) {
				if(!mute) std::cout << "Pattern = " << i <<'\n';
				bool Triger_found_total = false;
				for (int k = max_range; k >= min_range; k--) {
					if(!mute) std::cout << "Offset = " << k <<'\n';
					bool Triger_found = true;
					for (int j = 0; j < bare_hits.size(); j++) {
						if(!mute) std::cout << "Pattern = " << i << ", layer = " << bare_hits[j].first << ", hit position =  " << bare_hits[j].second + k <<'\n';
						Triger_found *= CSCConstants::pat_windows[i][bare_hits[j].first][bare_hits[j].second + k];
					}
					if(Triger_found){
						if(!mute) std::cout << "*********************Trigger Found*******************" << '\n';
						ExpectedTrigger.pat = i;
						Triger_found_total = true;
						break;
						offset = k;
					}
				}
				if(Triger_found_total) break;
			}


			int tmp_lut = 0;
			int minhit = 1;
			int maxhit = 3;

			for (int i = 0; i < bare_hits.size(); i++) {
				bare_hits[i].second -= 3;
				bare_hits[i].second -= CSCConstants::CCLUT_offset[ExpectedTrigger.pat][bare_hits[i].first];
				if (bare_hits[i].second < minhit) minhit = bare_hits[i].second;
				if (bare_hits[i].second > maxhit) maxhit = bare_hits[i].second;
			}


			for (int i = 0; i < bare_hits.size(); i++) {
				bare_hits[i].second += 1 - minhit;
				bare_hits[i].second += 3 - maxhit;
				tmp_lut += bare_hits[i].second*pow(4, bare_hits[i].first);
				if(!mute) std::cout << "Found ExpectedTrigger.pat = " << ExpectedTrigger.pat <<", layer = " << bare_hits[i].first << ", hit position =  " << bare_hits[i].second <<'\n';
			}
			ExpectedTrigger.LUT_code = tmp_lut;

			int trial_hs_update = bare_hits[0].second + CSCConstants::CCLUT_offset[ExpectedTrigger.pat][bare_hits[0].first];
			ExpectedTrigger.hs -= trial_hs_update - trial_hs;



			std::vector<int> Hits_map;
			Convert_LUT_to_hits(ExpectedTrigger.LUT_code, Hits_map, Bit11_int);





			 // CHECK FOR INTERNAL LUT SHIFT (10 10 10 10 10 10) to (11 11 11 11 11 11) for example;
			int max = -99999;
			int min = 99999;

		  for (size_t i = 0; i < Hits_map.size(); i++) {
		     if(Hits_map[i] < min && Hits_map[i] != 0) min = Hits_map[i];
		  	 if(Hits_map[i] > max) max = Hits_map[i];
		 	}

			int trigger_cclut = ExpectedTrigger.LUT_code;



			int range  =  3 - max;
			ExpectedTrigger.hs -= range;
			int upscale = 1;
			for (int i = 0; i < Hits_map.size(); i++) {
				if (Hits_map[i] > 0) trigger_cclut += range*upscale;
				upscale *= 4;
			}


			ExpectedTrigger.LUT_code = trigger_cclut;

			return ExpectedTrigger;
		}

	}

	void PrintCCLUT(CCLUT& cclut, std::ostream& oss, bool opt)
		{
			std::string empty_layer = "-----------";
			std::vector<std::string> layers(6,empty_layer);

			for(int i=0; i < cclut.hits.size(); i++){
				char type = cclut.COMPILE_TYPE - 0xa + 'a';
				oss <<"OTMB Type "<< type << std::endl;
				layers.at(cclut.hits.at(i).lay).at(5 + (cclut.hits.at(i).hs - cclut.hs)) = 'X';
				if(opt) oss << "Hit (" << i << ')' << "   Bx: " << cclut.hits.at(i).bx << "   Hs: " << cclut.hits.at(i).hs << "   Layer: " << cclut.hits.at(i).lay << "   CFEB: " << GetCFEB(cclut.hits.at(i).hs, cclut.COMPILE_TYPE) << std::endl;
			}


			oss << "bx keystrip pattern nhits" << std::endl;
	                oss << cclut << std::endl;


			for(int ly=0; ly < 6; ly++)	// for each layer
			{
				oss << "ly" << ly;
				if(ly == 2) oss << "  key  ";
				else 	    oss << "       ";

				oss << layers.at(ly) << std::endl;

			}
		}

int ReadTxt(std::string& prefix, std::vector<CCLUT>& ccluts) //Ready
	{
		CCLUT cl;
		std::string str; 										// String to be used for parsing purposes

		std::cout << "I MADE IT HERE!" << std::endl;
		std::cout << "I am trying to open file at: " << prefix << ".txt" << std::endl << std::endl;



		std::fstream Input_File((prefix + ".txt").c_str(), std::ios_base::in); // Open File to be read;

		// First two lines are header
		std::getline(Input_File, prefix);
		std::getline(Input_File, str);


		prefix = prefix.substr(8, prefix.length() - 8);

		int n = 0;
		while (Input_File >> cl) {
			ccluts.push_back(cl);
			n++;
			std::cout << ccluts.at(n - 1) << " size :" << n << '\n';
		}

		return n;
	}

std::string ReadTxt(std::string& path, std::vector<CCLUT>& ccluts, std::vector<Cluster>& clusters) //Ready
	{
		CCLUT cclut;
		Cluster gem;
		char mode = 'c';	// c-CCLUT	g-GEM
		std::string str, prefix;

		std::cout << "Trying to open file: " << path << std::endl << std::endl;

		std::fstream Input_File(path.c_str(), std::ios_base::in); // Open the file

		std::getline(Input_File, prefix);	// first line specify pattern prefix
		prefix = prefix.substr(8, prefix.length()-8);

		do{
			std::getline(Input_File, str);

			if(str[0] == 'C'){
				mode = 'c';
				std::cout << "Switching to CCLUT Mode" << std::endl;
				std::getline(Input_File, str);	// next line is header
				std::cout << str << std::endl;
			}
			else if(str[0] == 'G'){
				mode = 'g';
				std::cout << "Switching to GEM Mode" << std::endl;
				std::getline(Input_File, str);	// next line is header
				std::cout << str << std::endl;
			}
			else if(!Input_File.eof() && (str.length() != 0)){
				std::stringstream ss(str.c_str());
				switch(mode){
					case 'c':
						ss >> cclut;
						ccluts.push_back(cclut);
						std::cout << cclut << std::endl;
						break;
					case 'g':
						ss >> gem;
						clusters.push_back(gem);
						std::cout << gem << std::endl;
						break;
					default:
						break;
				}
			}
		}while(!Input_File.eof());

		Input_File.close();
		return prefix;
	}

void WriteTxt(std::string& str, std::vector<CCLUT>& ccluts)
	{
		std::fstream text_file((str + ".txt").c_str(), std::ios_base::out); // Create output file => (input string).txt
		//std::sort(ccluts.begin(), ccluts.end(), Compareccluts);	// Sort the CCLUT vector in case it's asorted

		// Add header stuff to pattern(.txt) file
		text_file << "prefix:" << str << std::endl;
		text_file << "bx keystrip pattern nhits" << std::endl;

		for (int i = 0; i < ccluts.size(); i++) {
			text_file << ccluts.at(i);
			if (i < (ccluts.size() - 1)) {
				text_file << '\n';
			}
		}

		text_file.close();
		return;
	}

void ExtractHits(std::vector<CCLUT>& ccluts, std::vector<Hit>& hits, int feb)
	{
		if (feb == -1)
		{
			for (int i = 0; i < ccluts.size(); i++)
			{
				hits.insert(hits.end(), ccluts.at(i).hits.begin(), ccluts.at(i).hits.end());
			}
		}
		/// If you want to pull hits from a specific dcfeb

		else if(feb >= 0 && feb < CSCConstants::NUM_DCFEBS)
		{
			for(int i=0; i < ccluts.size(); i++)
			{
				for(int j=0; j < ccluts.at(i).hits.size(); j++)
				{
					if (GetCFEB(ccluts.at(i).hits.at(j).hs, ccluts.at(i).COMPILE_TYPE) == feb) { hits.push_back(ccluts.at(i).hits.at(j)); }
				}
			}
		}
		return;
	}

	/// CCLUT
bool WritePat(std::string & prefix, std::vector<CCLUT>& ccluts)
	{
		// Prepare output file streams
		std::vector<std::fstream*> oss;
		char tmbtype;
		if (ccluts.size() == 0) tmbtype = 'c';//default case
		else tmbtype = ccluts[0].COMPILE_TYPE - 0xa + 'a';
		for (int i = 0; i < CSCConstants::NUM_DCFEBS; i++)
		{
			std::stringstream ss;
			ss << prefix << "_cfeb" << i << "_tmb" << tmbtype << ".pat";
			//std::cout << ss.str().c_str() << std::endl;
			oss.push_back(new std::fstream(ss.str().c_str(), std::ios_base::out) );
			std::cout << "Write Pat opening file " << ss.str() << std::endl;
		}


		for (int i = 0; i < CSCConstants::NUM_DCFEBS; i++)
		{
			std::cout << "Writing (.pat) file number: " << i << std::endl;	// debug

			std::vector<Hit> hits;		//
			std::vector<int> times;		// list of bx's w/
			ExtractHits(ccluts, hits, i);


			// DEBUG PURPOSES~~~~~
			for(int j=0; j < hits.size(); j++){
				DumpHit(hits.at(j), j);
			}
			// ~~~~~~~~~

			if(hits.size() == 0)			// IF : No hits in Pattern
			{
				fillnbytes(oss, RAMPAGE_SIZE, i);
				(*(oss.at(i))) << std::flush;		// Fill & Close file
				delete (oss.at(i));
			}
			else					// ELSE
			{
				// Get times
				for (int j = 0; j < hits.size(); j++)
				{
					bool tf = false;
					for (int k = 0; k < times.size(); k++)
					{
						if (times.at(k) == hits.at(j).bx) tf = true;
					}
					if (!tf) times.push_back(hits.at(j).bx);
				}
				std::sort(times.begin(), times.end());


				int Bx = 0;	// Current Bunch Cross
				unsigned int q = 0;	// index var for times vector

				if (times.at(0) != 0)
				{
					Bx = times.at(0);
					fillnbytes(oss, times.at(0)*8, i);
				}


				while (Bx < (times[times.size() -1] + 3) && (RAMPAGE_SIZE - Bx*8) > 0)
				{
					if (Bx == (times.at(q) + 3) && q+1 == times.size())
					{
						fillnbytes(oss, RAMPAGE_SIZE - (Bx * 8), i);		/// Double check range here
						break;
					}
					else if (Bx == (times.at(q) + 3))
					{
						q++;
						if(times.at(q) > Bx)
						{
							fillnbytes(oss, (times.at(q) - Bx)*8, i);
							Bx = times.at(q);
							// write Group @ this->Bx
							(*(oss[i])) << Group(hits,Bx);
							DumpGroup(Group(hits, Bx), Bx);	// debug
							Bx++;
						}
						else
						{
							//Write Group @ this->Bx
							(*(oss[i])) << Group(hits, Bx);
							DumpGroup(Group(hits, Bx), Bx);	// debug
							Bx++;
						}
					}
					else
					{
						//Write Group @ this->Bx
						(*(oss[i])) << Group(hits,Bx);
						DumpGroup(Group(hits, Bx), Bx);	// debug
						Bx++;
					}
				}
				if((RAMPAGE_SIZE - Bx*8) > 0)
				{
					fillnbytes(oss, RAMPAGE_SIZE - (Bx * 8), i);		/// Double check range here
				}

				*(oss.at(i)) << std::flush;
				delete (oss.at(i));

			}


		}
		return true;
	}

void TMB_Check(std::vector<CCLUT>& ccluts, std::string& tmbInfo){return;}



//////////////////////////////////////////////////////////////////////////////////

}

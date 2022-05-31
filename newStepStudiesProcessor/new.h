#ifndef SimStepStudiesProcessor_h
#define SimStepStudiesProcessor_h

#include "marlin/Processor.h"
#include "lcio.h"
#include <string>
#include <vector>
#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"

using namespace lcio ;
using namespace marlin ;



class SimStepStudiesProcessor : public Processor //classe SimStepStudiesProcessor dérivée de Processor
{
	public:
		virtual Processor*  newProcessor() { return new SimStepStudiesProcessor ; }
		SimStepStudiesProcessor();
		virtual void init(); // initialize the processor, e.g. book histograms.
		virtual std::vector<float> processEvent( LCEvent * evt ); // Called for every event
		virtual void end();
	protected: //or private ?
	    StringVec m_colNames{}; //Input collection names.
	//std::string m_outputColName{}; //Output collection name
	private:
		std::map<float, float> mymap; //distance in key and time in value
	    float calc_distance(const float *pos1, const float *pos2);
	    float calc_temps(float t1, float t2)
	    std::string a_rootFileName;
	    std::vector<float>vectormindistance;
	    float a_super_minimum;
	    float a_distance_minimale;
	    TFile *a_rootFile;
	    TTree *a_tree;
	    TH1F *a_hdist;
	    TH1F *a_hdistzoom;
};

#endif

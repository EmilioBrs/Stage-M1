#include "SimStepStudiesProcessor.h"
#include <IMPL/LCCollectionVec.h>
#include <cmath>
#include "EVENT/CalorimeterHit.h"
#include "EVENT/MCParticle.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <deque>
#include "TFile.h"


// ----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"

SimStepStudiesProcessor aSimStepStudiesProcessor;

SimStepStudiesProcessor::SimStepStudiesProcessor() : Processor("SimStepStudiesProcessor")
{
  _description = "SimStepStudiesProcessor : Perform some studies (TO UPDATE) based on output of SimHitStepsToCaloHitProcessor";

  StringVec colDefault = {"G4StepAsHit"};
  registerInputCollections( LCIO::CALORIMETERHIT,
			    "InputG4StepAsCaloHitsCollections" , 
			    "Name of the input collections containing G4Step stored as Calorimeter Hits"  ,
			    m_colNames ,
			    colDefault ) ;

  registerProcessorParameter( "ROOTfilename", "Nom du fichier root de sortie" , a_rootFileName, std::string("test.root"));
}

void SimStepStudiesProcessor::init()
{
  printParameters();

  a_rootFile = new TFile(a_rootFileName.c_str(),"recreate");

  a_hdist = new TH1F("mindist","Distribution des distances minimales",100,0,50.);
  a_hdistzoom = new TH1F("mindistzoom","Zoom autour de 0",100,0,1e-2);

  a_tree= new TTree("distanceTree","Mon TTree avec les distances");
  a_tree->Branch("mindist",&a_distance_minimale,"mindist/F");
  a_super_minimum=1e20;
}

float SimStepStudiesProcessor::calc_distance(const float *pos1, const float *pos2)
{
	float diffx=pos1[0]-pos2[0];
	float diffy=pos1[1]-pos2[1];
	float diffz=pos1[2]-pos2[2];
	float distancecalculee = sqrt(diffx*diffx+diffy*diffy+diffz*diffz);
	return distancecalculee;
}

float SimStepStudiesProcessor::calc_temps(float t1, float t2)
{
	float tempscalcule = abs(t1-t2);
	return tempscalcule;
}

std::vector<float> SimStepStudiesProcessor::processEvent( LCEvent * evt )
{
	//if ( not isFirstEvent() ) return;
	a_distance_minimale=1E20;
	float distance=1E20;
	for(std::string& colName : m_colNames)
	{
		LCCollection* col = NULL;
		try
		{
			col = evt->getCollection( colName );
		}
		catch( lcio::DataNotAvailableException &e )
		{
			streamlog_out(WARNING) << colName << " collection not available" << std::endl;
			col = NULL;
		}
		if (not col) continue;
		
		for(int i=0; i<col->getNumberOfElements(); ++i)
		{
			CalorimeterHit* hit=dynamic_cast<CalorimeterHit*> ( col->getElementAt(i) );
			if (not hit) continue;

			MCParticle* mcp=dynamic_cast<MCParticle*>( hit->getRawHit() );
			if (not mcp)
			std::cout << " :-( ";
			
			for(int j=i+1; j<col->getNumberOfElements(); ++j)
			{
				CalorimeterHit* hitb=dynamic_cast<CalorimeterHit*> ( col->getElementAt(j) );
				if (not hitb) continue;

				if (hit->getCellID0() != hitb->getCellID0()) continue;
        		MCParticle* mcpb=dynamic_cast<MCParticle*>( hitb->getRawHit() );

			    if (mcp->id() != mcpb->id() )
			    {
			    	distance = calc_distance(hit->getPosition(), hitb->getPosition() );
			    	time = calc_time(hit->getTime(), hitb->getTime())

			    	mymap.insert(std::pair<float, float>(distance, time));

			    	if(distance<a_distance_minimale)
			    	{
			          	a_distance_minimale = distance;
			    	}
			    }
			}
		}
	}
	//std::cout<<"Distance minimale : "<<a_distance_minimale<<" mm"<<std::endl;
	a_hdist->Fill(a_distance_minimale);
	a_hdistzoom->Fill(a_distance_minimale);
	a_tree->Fill();
}


void SimStepStudiesProcessor::end()
{
	a_rootFile->cd();
	a_hdist->Write();
	a_hdistzoom->Write();
	a_tree->Write();
	a_rootFile->Close();
	delete a_rootFile;
	//std::cout << "Le super minimum est " << a_super_minimum << " mm." << std::endl;
	
	std::ofstream file;
	file.open("/home/ilc/ebourasseau/StageProcessor/results/datafiles/superminimums.txt", std::ios::app);
	file << a_super_minimum << std::endl;
	file.close();

	std::deque<std::float> d;
	for (it = mymap.begin(); it != mymap.end(); ++it)
	{
		d.push_back(it->first);
	}
	std::sort(d.begin(), d.end());
}

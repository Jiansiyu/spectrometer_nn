/*
 * Generate the data used for neural network training
 *
 */
#include <TString.h>
#include <TChain.h>
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>

Bool_t SieveHoleCut(Int_t colID, Int_t rowID){
//    std::cout<<"Col:"<<colID <<"  rowID:"<<rowID<<std::endl;
    return true;
}

std::map<TString,double> getCombination(double  x, double theta,double y, double phi){
    // name pattern x0th0y0ph0
    Int_t maxExpoIndex = 2;
    std::queue<TString> combinations;

    for (Int_t index = 0 ; index<= maxExpoIndex; index ++){
        combinations.push(Form("x%dth%dy%dph%d",index,0,0,0));
    }

    Int_t queueSize = combinations.size();
    // index the theta
    for (auto i = 0; i< queueSize; i++){
        TString str = combinations.front();
        const char* title = str.Data();
        combinations.pop();

        Int_t xIndex  =  title[1]-'0';

        for (auto index = 0; index<=maxExpoIndex;index++){
            TString str = Form("x%dth%dy%dph%d",xIndex,index,0,0);
            combinations.push(str);
        }
    }

    queueSize = combinations.size();
    // index the y
    for (auto i = 0; i< queueSize; i++){
        TString str = combinations.front();
        const char* title = str.Data();
        combinations.pop();

        Int_t xIndex  =  title[1]-'0';
        Int_t thIndex =  title[4]-'0';

        for (auto index = 0; index<=maxExpoIndex;index++){
            TString str =Form("x%dth%dy%dph%d",xIndex,thIndex,index,0);
            combinations.push(str);
        }
    }

    queueSize = combinations.size();
    for (auto i = 0; i< queueSize; i++){
        TString str = combinations.front();
        const char* title = str.Data();
        combinations.pop();

        Int_t xIndex  =  title[1]-'0';
        Int_t thIndex =  title[4]-'0';
        Int_t yIndex  =  title[6]-'0';

        for (auto index = 0; index<=maxExpoIndex;index++){
            TString str = Form("x%dth%dy%dph%d",xIndex,thIndex,yIndex,index);
            combinations.push(str);
        }
    }


    // apply cut on the total order 
    queueSize = combinations.size();
    for (auto i = 0; i< queueSize; i++){
        TString str = combinations.front();
        const char* title = str.Data();
        combinations.pop();

        Int_t xIndex  =  title[1]-'0';
        Int_t thIndex =  title[4]-'0';
        Int_t yIndex  =  title[6]-'0';
        Int_t phIndex =  title[9]-'0';
        if ((xIndex + thIndex + yIndex + phIndex <= 2)){
            TString str = Form("x%dth%dy%dph%d",xIndex,thIndex,yIndex,phIndex);
            combinations.push(str);
        }
    }



//    queueSize = combinations.size();
//    for (auto i = 0; i< queueSize; i++){
//        auto title = combinations.front();
//        combinations.pop();
//        std::cout<<title.Data()<<std::endl;
//    }

    std::map<TString,double> res;
//pop the data and calculate the combinations
    queueSize = combinations.size();
    for (auto i = 0; i< queueSize; i++){
        TString str = combinations.front();
        const char* title = str.Data();
        combinations.pop();
        Int_t xIndex  =  title[1]-'0';
        Int_t thIndex =  title[4]-'0';
        Int_t yIndex  =  title[6]-'0';
        Int_t phIndex =  title[9]-'0';
        res[str] = pow(x,xIndex)*pow(theta,thIndex)*pow(y,yIndex)*pow(phi,phIndex);

//        std::cout<<str.Data()<<" :: "<<res[str] <<"  "<<xIndex<<","<<thIndex<<","<<yIndex<<","<<phIndex<<std::endl;
    }
    return  res;
}

void GetMinSieveEvent(TString fnameTemplate="./data/checkSieve_%d.root",Int_t sieveMinCT = 0){

    Int_t runList[]={2239,2240,2241,2244,2245,2256,2257};

    std::vector<Int_t> SieveEvtCT;
    std::map<Int_t,std::set<Int_t>> cutIDBuff;

    for (Int_t runIndex = 0; runIndex < sizeof(runList)/sizeof(Int_t); runIndex++){
        auto runID = runList[runIndex];
        // load the file and connect the data, try to get the number of the sieveholes
        std::set<Int_t> cutIDSet;
        TString filename = Form(fnameTemplate.Data(),runID);

        TChain *chain = new TChain("OptRes");
        chain->Add(filename.Data());

        // get the number of the cutID
        // map the data

        int evtID;
        int CutID;
        int SieveRowID;
        int SieveColID;
        int KineID;

        double bpmX;     // the bpm coordination sys
        double bpmY;
        double bpmZ;

        double focalTh;   //the focal plane variable
        double focalPh;
        double focalX;
        double focalY;

        double TargCalTh;  // the calculated theoretical Value on the target coordination sys
        double TargCalPh;

        chain->SetBranchAddress("evtID",&evtID);
        chain->SetBranchAddress("CutID",&CutID);
        chain->SetBranchAddress("SieveRowID",&SieveRowID);
        chain->SetBranchAddress("SieveColID",&SieveColID);
        chain->SetBranchAddress("KineID",&KineID);

        chain->SetBranchAddress("focalTh",&focalTh);
        chain->SetBranchAddress("focalPh",&focalPh);
        chain->SetBranchAddress("focalX",&focalX);
        chain->SetBranchAddress("focalY",&focalY);
        // get the beam position
        chain->SetBranchAddress("bpmX",&bpmX);
        chain->SetBranchAddress("bpmY",&bpmY);

        chain->SetBranchAddress("targCalcTh",&TargCalTh);
        chain->SetBranchAddress("targCalcPh",&TargCalPh);

        for(Long64_t i = 0 ; i < chain->GetEntries(); i ++){
            chain->GetEntry(i);
            //TODO apply cut on the sieve holes
            if (SieveHoleCut(SieveColID,SieveRowID))
            cutIDSet.insert(CutID);
        }

        //Get the event number
        for (std::set<Int_t>::iterator iter = cutIDSet.begin(); iter != cutIDSet.end(); iter++){
            auto cutid = *iter;
            // get the entries
            auto evtCT = chain->GetEntries(Form("CutID == %d",cutid));
            SieveEvtCT.push_back(evtCT);
        }

        cutIDBuff[runID] = cutIDSet;

    }

    std::cout<<*std::min_element(SieveEvtCT.begin(),SieveEvtCT.end()) <<std::endl;

    auto SieveEvtMin = *std::min_element(SieveEvtCT.begin(),SieveEvtCT.end());

    // loop on the files
    if(sieveMinCT <= 0) sieveMinCT  = SieveEvtMin;


    for (Int_t runIndex = 0; runIndex < sizeof(runList)/sizeof(Int_t); runIndex++) {
        auto runID = runList[runIndex];
        // load the file and connect the data, try to get the number of the sieveholes
        TString filename = Form(fnameTemplate.Data(), runID);

        TChain *chain = new TChain("OptRes");
        chain->Add(filename.Data());

        // get the number of the cutID
        // map the data
        int evtID;
        int CutID;
        int SieveRowID;
        int SieveColID;
        int KineID;

        double bpmX;     // the bpm coordination sys
        double bpmY;
        double bpmZ;

        double focalTh;   //the focal plane variable
        double focalPh;
        double focalX;
        double focalY;

        double TargCalTh;  // the calculated theoretical Value on the target coordination sys
        double TargCalPh;

        chain->SetBranchAddress("evtID", &evtID);
        chain->SetBranchAddress("CutID", &CutID);
        chain->SetBranchAddress("SieveRowID", &SieveRowID);
        chain->SetBranchAddress("SieveColID", &SieveColID);
        chain->SetBranchAddress("KineID", &KineID);

        chain->SetBranchAddress("focalTh", &focalTh);
        chain->SetBranchAddress("focalPh", &focalPh);
        chain->SetBranchAddress("focalX", &focalX);
        chain->SetBranchAddress("focalY", &focalY);
        // get the beam position
        chain->SetBranchAddress("bpmX", &bpmX);
        chain->SetBranchAddress("bpmY", &bpmY);

        chain->SetBranchAddress("targCalcTh", &TargCalTh);
        chain->SetBranchAddress("targCalcPh", &TargCalPh);

        std::cout <<"Working on "<<runID<<std::endl;
        {
            //loop on the event and write the data to csv file
            std::map<Int_t, Int_t> SieveEvtTracker;   //# sieve event tracker
            Int_t SieveTotalConter = 0;
            for (std::set<Int_t>::iterator iter = cutIDBuff[runID].begin(); iter != cutIDBuff[runID].end(); iter++) {
                auto cutid = *iter;
                SieveEvtTracker[cutid] = 0;
            }

            std::ofstream csvfileIO;
            csvfileIO.open(Form("./result/PRex_DataSet_%d.csv", runID));
            csvfileIO << "evtID,runID,CutID,SieveRowID,SieveColID,bpmX,bpmY,focal_x,focal_y,focal_th,focal_ph,targCalTh,targCalPh\n";
            Long64_t entries = chain->GetEntries();
            for (Long64_t entry = 0; entry < entries; entry++) {
                // TODO need to change to random access the entry
                chain->GetEntry(entry);
                if (SieveEvtTracker[CutID] < sieveMinCT) {
                    SieveEvtTracker[CutID] += 1;
                    SieveTotalConter += 1;
                    // write the data to the csv files
                    csvfileIO << Form("%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f \n", evtID,runID,CutID,SieveRowID,SieveColID,bpmX, bpmY, focalX, focalY,
                                      focalTh, focalPh, TargCalTh, TargCalPh);

                }

                if (SieveTotalConter >= SieveEvtTracker.size() * sieveMinCT) break;
            }
            csvfileIO.close();
        }

        {
            //loop on the event and write the data to csv file
            std::map<Int_t,Int_t> SieveEvtTracker;   //# sieve event tracker
            Int_t SieveTotalConter=0;
            for (std::set<Int_t>::iterator iter = cutIDBuff[runID].begin(); iter != cutIDBuff[runID].end(); iter++){
                auto cutid = *iter;
                SieveEvtTracker[cutid] = 0;
            }
            std::ofstream csvFullfileIO; // include all the combinations
            csvFullfileIO.open(Form("./result/PRex_DataSet_Full_%d.csv",runID));
            csvFullfileIO<<"evtID,runID,CutID,SieveRowID,SieveColID,bpmX,bpmY";
            std::map<TString,double> titleComb = getCombination(0,0,0,0);
            for (auto iter = titleComb.begin(); iter!=titleComb.end();iter++){
                TString str = iter->first;
//                std::cout<<str.Data()<<std::endl;
                csvFullfileIO<<","<<str.Data();
            }
            csvFullfileIO<<",targCalTh,targCalPh\n";
            Long64_t entries = chain->GetEntries();
            for (Long64_t entry = 0; entry < entries; entry++) {
                // TODO need to change to random access the entry
                chain->GetEntry(entry);
                if (SieveEvtTracker[CutID] < sieveMinCT) {
                    SieveEvtTracker[CutID] += 1;
                    SieveTotalConter += 1;
                    csvFullfileIO << Form("%d,%d,%d,%d,%d,%f,%f", evtID,runID, CutID,SieveRowID,SieveColID,bpmX, bpmY);

                    auto newComb = getCombination(focalX,focalTh,focalY,focalPh);
                    for (auto iter = newComb.begin();iter!=newComb.end();iter ++){
                        csvFullfileIO <<","<<iter->second;
//                        std::cout<<(iter->first).Data()<<"  : "<<iter->second <<"      (x,th,y,ph):: "<<focalX<<", "<<focalTh<<", "<<focalY<<", "<<focalPh
//                        <<std::endl;
                    }
                    csvFullfileIO<<Form(",%f,%f\n",TargCalTh,TargCalPh);
                }

                if (SieveTotalConter >= SieveEvtTracker.size() * sieveMinCT) break;
            }
            csvFullfileIO.close();
        }
        chain->Delete();
    }

}


int main(){
    std::cout<<"This is test functions"<<std::endl;
    GetMinSieveEvent();
    return 1;
}
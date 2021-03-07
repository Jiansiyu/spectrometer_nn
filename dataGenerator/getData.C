/*
 * Generate the data used for neural network training
 *
 */
#include <TString.h>
#include <TChain.h>
#include <iostream>
#include <TRandom.h>
#include <TRandom3.h>


Bool_t SieveHoleCut(Int_t colID, Int_t rowID){
//    std::cout<<"Col:"<<colID <<"  rowID:"<<rowID<<std::endl;
    return true;
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


        //loop on the event and write the data to csv file
        std::map<Int_t,Int_t> SieveEvtTracker;   //# sieve event tracker
        for (std::set<Int_t>::iterator iter = cutIDBuff[runID].begin(); iter != cutIDBuff[runID].end(); iter++){
            auto cutid = *iter;
            SieveEvtTracker[cutid] = 0;
        }

        Long64_t entries = chain->GetEntries();
        for (Long64_t entry = 0 ; entry  < entries; entry ++ ){
            // TODO need to change to random access the entry



        }

        chain->Delete();
    }



}


int main(){
    std::cout<<"This is test functions"<<std::endl;
    GetMinSieveEvent();
    return 1;
}
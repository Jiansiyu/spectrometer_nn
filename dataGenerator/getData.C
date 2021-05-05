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
#include "map"

Bool_t SieveHoleCut(Int_t colID, Int_t rowID){
//    std::cout<<"Col:"<<colID <<"  rowID:"<<rowID<<std::endl;
    if (colID ==3 & rowID == 3){
        return false;
    } else {
        return true;
    }

}


// get the prefix combinations
//TODO warning, currently only support the X parameter
std::map<TString,double> getCombination(double  x, double theta,double y, double phi, TString absTerm){
    if (absTerm != "X"){
        std::cout <<"Currently do not support"<<std::endl;
    }
    TString absPrefix = "ZXabs_";

    // name pattern x0th0y0ph0
    Int_t maxExpoIndex = 5;
    std::queue<TString> combinations;

    // start project the x dimesion
    //TODO need to find a smarter way to process the parameter
    for (Int_t index = 1 ; index<= maxExpoIndex; index ++){
        combinations.push(Form("x%dth%dy%dph%d",index,0,0,0));
    }



    // project the theta parameter
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

    // project the y parameter
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

    // project the phi parameter
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
        if ((xIndex + thIndex + yIndex + phIndex <= 7)){
            TString str = Form("x%dth%dy%dph%d",xIndex,thIndex,yIndex,phIndex);
            combinations.push(str);
        }
    }

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

        if (xIndex %2 == 0 ) continue;
        x = std::abs(x);

        str = absPrefix + str;  // update the name
        res[str] = pow(x,xIndex)*pow(theta,thIndex)*pow(y,yIndex)*pow(phi,phIndex);
    }

    return  res;
}

std::map<TString,double> getCombination(double  x, double theta,double y, double phi){
    // name pattern x0th0y0ph0
    Int_t maxExpoIndex = 5;
    std::queue<TString> combinations;

    // index the x dimension
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

    // index the phi dimension
    queueSize = combinations.size();
    for (auto i = 0; i< queueSize; i++){
        TString str = combinations.front();
        const char* title = str.Data();
        combinations.pop();

        Int_t xIndex  =  title[1]-'0';
        Int_t thIndex =  title[4]-'0';
        Int_t yIndex  =  title[6]-'0';

        for (auto index = 0; index<=maxExpoIndex;index++){
            if (index %2 == 1) {
                if(!(((thIndex == 1)||(thIndex == 2))&&((yIndex == 0)||(yIndex == 1)))){
                    continue;
                }
            }
            //add some more terms

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
        if ((xIndex + thIndex + yIndex + phIndex <= 7)){
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
    }

    //_____________________________________________
    // merge the X combination test result
    // should be disabled in donot what to add this term
//    auto absXres = getCombination(x,theta,y,phi,"X");
//    res.insert(absXres.begin(),absXres.end());

    return  res;
}




//_______________________________________________________________________________________________________
void GetMinSieveY(TString fnameTemplate="./data/data_y_all/CheckVertex_Report_%d.root",Int_t sieveMinCT = 100){
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


        double tfiletargCalcY;
        double tfiletargProjY;



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


        chain->SetBranchAddress("tfiletargCalcY",&tfiletargCalcY);
        chain->SetBranchAddress("tfiletargProjY",&tfiletargProjY);



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

        //double TargCalTh;  // the calculated theoretical Value on the target coordination sys
        //double TargCalPh;

        double tfiletargCalcY;
        double tfiletargProjY;

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

        chain->SetBranchAddress("tfiletargCalcY",&tfiletargCalcY);
        chain->SetBranchAddress("tfiletargProjY",&tfiletargProjY);

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
            csvfileIO.open(Form("./result/regY/PRex_DataSet_Vertex_%d.csv", runID));
            csvfileIO << "evtID,runID,CutID,SieveRowID,SieveColID,bpmX,bpmY,focal_x,focal_y,focal_th,focal_ph,tfiletargCalcY,tfiletargProjY\n";
            Long64_t entries = chain->GetEntries();
            for (Long64_t entry = 0; entry < entries; entry++) {
                // TODO need to change to random access the entry
                chain->GetEntry(entry);
                if (SieveEvtTracker[CutID] < sieveMinCT) {
                    SieveEvtTracker[CutID] += 1;
                    SieveTotalConter += 1;
                    // write the data to the csv files
                    csvfileIO << Form("%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f \n", evtID,runID,CutID,SieveRowID,SieveColID,bpmX, bpmY, focalX, focalY,
                                      focalTh, focalPh, tfiletargCalcY, tfiletargProjY);

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
            csvFullfileIO.open(Form("./result/regY/PRex_DataSet_Vertex_Full_%d.csv",runID));
            csvFullfileIO<<"evtID,runID,CutID,SieveRowID,SieveColID,bpmX,bpmY";
            std::map<TString,double> titleComb = getCombination(0,0,0,0);
            for (auto iter = titleComb.begin(); iter!=titleComb.end();iter++){
                TString str = iter->first;
                csvFullfileIO<<","<<str.Data();
            }
            csvFullfileIO<<",tfiletargCalcY,tfiletargProjY\n";
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
                    csvFullfileIO<<Form(",%f,%f\n",tfiletargCalcY, tfiletargProjY);
                }

                if (SieveTotalConter >= SieveEvtTracker.size() * sieveMinCT) break;
            }
            csvFullfileIO.close();
        }
        chain->Delete();
    }

}

//_______________________________________________________________________________________________________
//Data set instruction
//
//PRex LHRS focal:
//PRex RHRS focal: ./data/PRex_RHRS_focal/checkSieve_%d.root
//
void GetMinSieveEvent(TString fnameTemplate="./data/data_focal/checkSieve_%d.root",Int_t sieveMinCT = 0){

    Int_t runList[]={2239,2240,2241,2244,2245,2256,2257};
//    Int_t runList[]={21363,21364,21365,21366,21368,21369,21370,21380,21381};

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

        // write the focal.x focal.y focal.th focal.ph approach
        {
            //loop on the event and write the data to csv file
            std::map<Int_t, Int_t> SieveEvtTracker;   //# sieve event tracker
            Int_t SieveTotalConter = 0;
            for (std::set<Int_t>::iterator iter = cutIDBuff[runID].begin(); iter != cutIDBuff[runID].end(); iter++) {
                auto cutid = *iter;
                SieveEvtTracker[cutid] = 0;
            }

            std::ofstream csvfileIO;
            csvfileIO.open(Form("./result/equal/PRex_DataSet_%d.csv", runID));
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

        // a more comprehensive combination approach
        {
            //loop on the event and write the data to csv file
            std::map<Int_t,Int_t> SieveEvtTracker;   //# sieve event tracker
            Int_t SieveTotalConter=0;
            for (std::set<Int_t>::iterator iter = cutIDBuff[runID].begin(); iter != cutIDBuff[runID].end(); iter++){
                auto cutid = *iter;
                SieveEvtTracker[cutid] = 0;
            }
            std::ofstream csvFullfileIO; // include all the combinations
            csvFullfileIO.open(Form("./result/equal/PRex_DataSet_Full_%d.csv",runID));
            csvFullfileIO<<"evtID,runID,CutID,SieveRowID,SieveColID,bpmX,bpmY";
            std::map<TString,double> titleComb = getCombination(0,0,0,0);
            for (auto iter = titleComb.begin(); iter!=titleComb.end();iter++){
                TString str = iter->first;
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
                    }
                    csvFullfileIO<<Form(",%f,%f\n",TargCalTh,TargCalPh);
                }

                if (SieveTotalConter >= SieveEvtTracker.size() * sieveMinCT) break;
            }
            csvFullfileIO.close();
        }

        // get the unequal event number for each sieve hole
        // unequal event: require all the sieve hole event number larger than the cerntain number
        // and keep all the event before the event

        {
            //loop on the event and write the data to csv file
            std::map<Int_t, Int_t> SieveEvtTracker;   //# sieve event tracker
            Int_t SieveTotalConter = 0;
            for (std::set<Int_t>::iterator iter = cutIDBuff[runID].begin(); iter != cutIDBuff[runID].end(); iter++) {
                auto cutid = *iter;
                SieveEvtTracker[cutid] = 0;
            }

            std::ofstream csvfileIO;
            csvfileIO.open(Form("./result/unequal/PRex_DataSet_%d.csv", runID));
            csvfileIO << "evtID,runID,CutID,SieveRowID,SieveColID,bpmX,bpmY,focal_x,focal_y,focal_th,focal_ph,targCalTh,targCalPh\n";
            Long64_t entries = chain->GetEntries();
            for (Long64_t entry = 0; entry < entries; entry++) {
                // TODO need to change to random access the entry
                chain->GetEntry(entry);
                SieveEvtTracker[CutID] += 1;
                // write the data to the csv files
                csvfileIO << Form("%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f \n", evtID,runID,CutID,SieveRowID,SieveColID,bpmX, bpmY, focalX, focalY,
                                  focalTh, focalPh, TargCalTh, TargCalPh);

                // check the minimum number of the sieve holes
                auto it = min_element(SieveEvtTracker.begin(), SieveEvtTracker.end(),
                                      [](decltype(SieveEvtTracker)::value_type& l, decltype(SieveEvtTracker)::value_type& r) -> bool { return l.second < r.second; });
                if (it->second > sieveMinCT) break;

            }
            csvfileIO.close();
        }


        // write the high order data array
        {
            //loop on the event and write the data to csv file
            std::map<Int_t,Int_t> SieveEvtTracker;   //# sieve event tracker
            Int_t SieveTotalConter=0;
            for (std::set<Int_t>::iterator iter = cutIDBuff[runID].begin(); iter != cutIDBuff[runID].end(); iter++){
                auto cutid = *iter;
                SieveEvtTracker[cutid] = 0;
            }
            std::ofstream csvFullfileIO; // include all the combinations
            csvFullfileIO.open(Form("./result/unequal/PRex_DataSet_Full_%d.csv",runID));
            csvFullfileIO<<"evtID,runID,CutID,SieveRowID,SieveColID,bpmX,bpmY";
            std::map<TString,double> titleComb = getCombination(0,0,0,0);
            for (auto iter = titleComb.begin(); iter!=titleComb.end();iter++){
                TString str = iter->first;
                csvFullfileIO<<","<<str.Data();
            }
            csvFullfileIO<<",targCalTh,targCalPh\n";
            Long64_t entries = chain->GetEntries();
            for (Long64_t entry = 0; entry < entries; entry++) {
                // TODO need to change to random access the entry
                chain->GetEntry(entry);

                SieveEvtTracker[CutID] += 1;
                csvFullfileIO << Form("%d,%d,%d,%d,%d,%f,%f", evtID,runID, CutID,SieveRowID,SieveColID,bpmX, bpmY);

                auto newComb = getCombination(focalX,focalTh,focalY,focalPh);
                for (auto iter = newComb.begin();iter!=newComb.end();iter ++){
                    csvFullfileIO <<","<<iter->second;
                }
                csvFullfileIO<<Form(",%f,%f\n",TargCalTh,TargCalPh);

                // check the minimum number of the sieve holes
                auto it = min_element(SieveEvtTracker.begin(), SieveEvtTracker.end(),
                                      [](decltype(SieveEvtTracker)::value_type& l, decltype(SieveEvtTracker)::value_type& r) -> bool { return l.second < r.second; });
                if (it->second > sieveMinCT) break;
            }
            csvFullfileIO.close();
        }
        chain->Delete();
    }

}


int main(){
    std::cout<<"This is test functions"<<std::endl;
//    GetMinSieveY();
    GetMinSieveEvent();
    return 1;
}
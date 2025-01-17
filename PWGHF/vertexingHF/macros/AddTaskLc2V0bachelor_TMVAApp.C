class AliAnalysisTaskSELc2V0bachelorTMVAAppMine;

AliAnalysisTaskSELc2V0bachelorTMVAApp* AddTaskLc2V0bachelor_TMVAApp(Int_t nvars, TString library = "_6_12", TString finname="Lc2V0bachelorCuts.root",
									Float_t ptMin=0, Float_t ptMax=24,
									Bool_t theMCon=kTRUE,
									Bool_t fillTree=kFALSE,
									Bool_t onTheFly=kFALSE,
									Bool_t keepingOnlyHIJINGbkd=kFALSE,
									TString suffixName="",
									Bool_t debugFlag = kFALSE,
									Bool_t useXmlWeightsFile = kTRUE,
									Bool_t useWeightsLibrary = kFALSE,
									TString xmlWeightsFile = "$ALICE_PHYSICS/PWGHF/vertexingHF/TMVA/LHC19c2a_TMVAClassification_BDT_2_4_noP.weights.xml"){
  
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskLc2V0bachelor", "No analysis manager to connect to.");
    return NULL;
  }  

  /*
  TString inputVariablesBDT = "massK0S,tImpParBach,tImpParV0,bachelorPt,combinedProtonProb,DecayLengthK0S*0.497/v0P,cosPAK0S,CosThetaStar,signd0";
  TObjArray *tokens = inputVariablesBDT.Tokenize(",");
  tokens->Print();
  std::vector<std::string> inputNamesVec;
  for(Int_t i=0; i<tokens->GetEntries(); i++){
    TString variable = ((TObjString*)(tokens->At(i)))->String();
    string tmpvar = variable.Data();
    inputNamesVec.push_back(tmpvar);
  }
  
  IClassifierReader* fBDTReader = new ReadBDT_Default(inputNamesVec);
  */
  
  // cuts are stored in a TFile generated by makeTFile4CutsLc2V0bachelor.C in ./macros/
  // set there the cuts!!!!!
  Bool_t stdcuts=kFALSE;
  TFile* filecuts;
  if( finname.EqualTo("") ) {
    stdcuts=kTRUE; 
  } else {
    filecuts=TFile::Open(finname.Data());
    if(!filecuts ||(filecuts&& !filecuts->IsOpen())){
      Printf("Input file not found : check your cut object"); return NULL;
    }
  }

  AliRDHFCutsLctoV0* RDHFCutsLctoV0anal = new AliRDHFCutsLctoV0();
  if (stdcuts) RDHFCutsLctoV0anal->SetStandardCutsPP2010();
  else RDHFCutsLctoV0anal = (AliRDHFCutsLctoV0*)filecuts->Get("LctoV0AnalysisCuts");
  RDHFCutsLctoV0anal->SetName("LctoV0AnalysisCuts");
  RDHFCutsLctoV0anal->SetMinPtCandidate(ptMin);
  RDHFCutsLctoV0anal->SetMaxPtCandidate(ptMax);
  RDHFCutsLctoV0anal->SetUseCentrality(1);
  
  // mm let's see if everything is ok
  if (!RDHFCutsLctoV0anal) {
    cout << "Specific AliRDHFCutsLctoV0 not found\n";
    return NULL;
  }
  
  Int_t nvarsSpectators = 0;
  TString namesTMVAvars, namesTMVAvarsSpectators;
  if (nvars == 14) namesTMVAvars = "massK0S,tImpParBach,tImpParV0,bachelorPt,DecayLengthK0S*0.497/v0P,cosPAK0S,CosThetaStar,signd0,bachelorP,nSigmaTOFpr,nSigmaTPCpr,nSigmaTPCpi,nSigmaTPCka,bachTPCmom";
  else if (nvars == 11) {
    namesTMVAvars = "massK0S,tImpParBach,tImpParV0,DecayLengthK0S*0.497/v0P,cosPAK0S,CosThetaStar,signd0,nSigmaTOFpr,nSigmaTPCpr,nSigmaTPCpi,nSigmaTPCka";
    nvarsSpectators = 15;
    namesTMVAvarsSpectators = "massLc2K0Sp,LcPt,massLc2Lambdapi,massLambda,massLambdaBar,cosPAK0S,V0positivePt,V0negativePt,dcaV0pos,dcaV0neg,v0Pt,dcaV0,V0positiveEta,bachelorEta,centrality";
  }
  else if (nvars == 10) namesTMVAvars = "massK0S,tImpParBach,tImpParV0,DecayLengthK0S*0.497/v0P,cosPAK0S,signd0,nSigmaTOFpr,nSigmaTPCpr,nSigmaTPCpi,nSigmaTPCka";
  else if (nvars == 7) {
    namesTMVAvars = "massK0S,tImpParBach,tImpParV0,DecayLengthK0S*0.497/v0P,cosPAK0S,CosThetaStar,signd0";
    nvarsSpectators = 15;
    namesTMVAvarsSpectators = "massLc2K0Sp,LcPt,massLc2Lambdapi,massLambda,massLambdaBar,cosPAK0S,V0positivePt,V0negativePt,dcaV0pos,dcaV0neg,v0Pt,dcaV0,V0positiveEta,bachelorEta,centrality";
  }

  //CREATE THE TASK
  printf("CREATE TASK\n");
  AliAnalysisTaskSELc2V0bachelorTMVAApp *task = new AliAnalysisTaskSELc2V0bachelorTMVAApp("AliAnalysisTaskSELc2V0bachelorTMVAApp", RDHFCutsLctoV0anal, onTheFly);
  //  task->SetMVReader(fBDTReader);
  task->SetNVars(nvars);
  task->SetNamesTMVAVariables(namesTMVAvars);
  task->SetTMVAlibName("libvertexingHFTMVA.so");
  task->SetTMVAlibPtBin(library);
  task->SetFillTree(fillTree);

  Printf("************* fillTree = %d", (Int_t)fillTree);
  task->SetMC(theMCon);
  task->SetKeepingOnlyHIJINGBkg(keepingOnlyHIJINGbkd);
  task->SetK0sAnalysis(kTRUE);
  task->SetDebugLevel(0);
  task->SetDebugHistograms(debugFlag);
  // TMVA reader
  task->SetNVarsSpectators(nvarsSpectators);
  task->SetNamesTMVAVariablesSpectators(namesTMVAvarsSpectators);
  task->SetUseXmlWeightsFile(useXmlWeightsFile);
  task->SetUseWeightsLibrary(useWeightsLibrary);
  task->SetXmlWeightsFile(TString(gSystem->ExpandPathName(Form("%s", xmlWeightsFile.Data()))));
  
  mgr->AddTask(task);
  
  // Create and connect containers for input/output  
  //TString outputfile = AliAnalysisManager::GetCommonFileName();
  //TString outputfile = Form("Lc2K0Sp_tree_pA%s.root", suffixName.Data());
  //TString outputfile = Form("Lc2K0Sp_tree_pA%s%s.root", ptBin.Data(), suffixName.Data());
  TString outputfile = "AnalysisResults.root";
  TString output1name="", output2name="", output3name="", output4name="", output5name="", output6name="", output7name="";

  output1name = Form("treeList%s%s", suffixName.Data(), library.Data());
  output2name = Form("Lc2pK0Scounter%s%s", suffixName.Data(), library.Data());
  output3name = Form("Lc2pK0SCuts%s%s", suffixName.Data(), library.Data());
  output4name = Form("treeSgn%s%s", suffixName.Data(), library.Data());
  output5name = Form("treeBkg%s%s", suffixName.Data(), library.Data());
  output6name = Form("listHistoKF%s%s", suffixName.Data(), library.Data());
  output7name = Form("listWeights%s%s", suffixName.Data(), library.Data());

  mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
  AliAnalysisDataContainer *coutput1   = mgr->CreateContainer(output1name, TList::Class(), AliAnalysisManager::kOutputContainer, outputfile.Data()); // trees
  mgr->ConnectOutput(task, 1, coutput1);
  
  AliAnalysisDataContainer *coutputLc2 = mgr->CreateContainer(output2name, TList::Class(), AliAnalysisManager::kOutputContainer, outputfile.Data()); //counter
  mgr->ConnectOutput(task, 2, coutputLc2);
  
  AliAnalysisDataContainer *coutputLc3 = mgr->CreateContainer(output3name, TList::Class(), AliAnalysisManager::kOutputContainer, outputfile.Data()); // cuts
  mgr->ConnectOutput(task, 3, coutputLc3);
  
  AliAnalysisDataContainer *coutput4   = mgr->CreateContainer(output4name, TTree::Class(), AliAnalysisManager::kOutputContainer, outputfile.Data()); // trees
  mgr->ConnectOutput(task, 4, coutput4);
  
  AliAnalysisDataContainer *coutput5   = mgr->CreateContainer(output5name, TTree::Class(), AliAnalysisManager::kOutputContainer, outputfile.Data()); // trees
  mgr->ConnectOutput(task, 5, coutput5);
  
  AliAnalysisDataContainer *coutput6   = mgr->CreateContainer(output6name, TList::Class(), AliAnalysisManager::kOutputContainer, outputfile.Data()); // trees
  mgr->ConnectOutput(task, 6, coutput6);  

  AliAnalysisDataContainer *coutput7   = mgr->CreateContainer(output7name, TList::Class(), AliAnalysisManager::kOutputContainer, outputfile.Data()); // trees
  mgr->ConnectOutput(task, 7, coutput7);  

  return task;
  
}


#include "CommonAnaFunctions.h"
#include "CATS.h"
#include "CATStools.h"
#include "DLM_Source.h"
#include "DLM_Potentials.h"
#include "DLM_WfModel.h"

#include "TString.h"
#include "TH2F.h"
#include "TFile.h"
#include "TROOT.h"

DLM_CleverLevy** CleverLevy = NULL;
unsigned NumCleverLevyObjects = 0;
//POT:
//  "AV18"
void SetUpCats_pp(CATS& Kitty, const TString& POT, const TString& SOURCE){

    CATSparameters* cPars = NULL;
    //DLM_CleverLevy* CleverLevy = NULL;
    if(!CleverLevy){
        CleverLevy = new DLM_CleverLevy* [1000];
    }
    CleverLevy[NumCleverLevyObjects] = NULL;
    CATSparameters* cPotPars1S0 = NULL;
    CATSparameters* cPotPars3P0 = NULL;
    CATSparameters* cPotPars3P1 = NULL;
    CATSparameters* cPotPars3P2 = NULL;

    if(SOURCE=="Gauss"){
        cPars = new CATSparameters(CATSparameters::tSource,1,true);
        cPars->SetParameter(0,1.2);
        Kitty.SetAnaSource(GaussSource, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Cauchy"){
        cPars = new CATSparameters(CATSparameters::tSource,1,true);
        cPars->SetParameter(0,1.2);
        Kitty.SetAnaSource(CauchySource, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Levy_Nolan"){
        cPars = new CATSparameters(CATSparameters::tSource,2,true);
        cPars->SetParameter(0,1.2);
        cPars->SetParameter(1,1.6);
        Kitty.SetAnaSource(LevySource3D, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Levy_Single"){
        cPars = new CATSparameters(CATSparameters::tSource,2,true);
        cPars->SetParameter(0,sqrt(1.6)*1.2);
        cPars->SetParameter(1,1.6);
        Kitty.SetAnaSource(LevySource3D_single, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Levy_Diff"){
        cPars->SetParameter(0,0.5*1.6*1.2);
        cPars->SetParameter(1,1.6);
        Kitty.SetAnaSource(LevySource3D_2particle, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="CleverLevy_Nolan"){
        CleverLevy[NumCleverLevyObjects] = new DLM_CleverLevy();
        CleverLevy[NumCleverLevyObjects]->InitStability(20,1,2);
        CleverLevy[NumCleverLevyObjects]->InitScale(35,0.25,2.0);
        CleverLevy[NumCleverLevyObjects]->InitRad(256,0,64);
        CleverLevy[NumCleverLevyObjects]->InitType(2);
        Kitty.SetAnaSource(CatsSourceForwarder, CleverLevy[NumCleverLevyObjects], 2);
        Kitty.SetAnaSource(0,1.2);
        Kitty.SetAnaSource(1,1.6);
        Kitty.SetUseAnalyticSource(true);
        NumCleverLevyObjects++;
    }
    else if(SOURCE=="CleverLevy_Single"){
        CleverLevy[NumCleverLevyObjects] = new DLM_CleverLevy();
        CleverLevy[NumCleverLevyObjects]->InitStability(20,1,2);
        CleverLevy[NumCleverLevyObjects]->InitScale(35,0.25,2.0);
        CleverLevy[NumCleverLevyObjects]->InitRad(256,0,64);
        CleverLevy[NumCleverLevyObjects]->InitType(0);
        Kitty.SetAnaSource(CatsSourceForwarder, CleverLevy[NumCleverLevyObjects], 2);
        Kitty.SetAnaSource(0,sqrt(1.6)*1.2);
        Kitty.SetAnaSource(1,1.6);
        Kitty.SetUseAnalyticSource(true);
        NumCleverLevyObjects++;
    }
    else if(SOURCE=="CleverLevy_Diff"){
        CleverLevy[NumCleverLevyObjects] = new DLM_CleverLevy();
        CleverLevy[NumCleverLevyObjects]->InitStability(20,1,2);
        CleverLevy[NumCleverLevyObjects]->InitScale(35,0.25,2.0);
        CleverLevy[NumCleverLevyObjects]->InitRad(256,0,64);
        CleverLevy[NumCleverLevyObjects]->InitType(1);
        Kitty.SetAnaSource(CatsSourceForwarder, CleverLevy[NumCleverLevyObjects], 2);
        Kitty.SetUseAnalyticSource(true);
        Kitty.SetAnaSource(0,0.5*1.6*1.2);
        Kitty.SetAnaSource(1,1.6);
        NumCleverLevyObjects++;
    }
    else if(SOURCE=="Gauss_mT_Reso"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (Gauss_mT_Reso)\n");
        goto CLEAN_SetUpCats_pp;
    }
    else if(SOURCE=="EPOS"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (EPOS)\n");
        goto CLEAN_SetUpCats_pp;
    }
    else if(SOURCE=="EPOSrescaled"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (EPOSrescaled)\n");
        goto CLEAN_SetUpCats_pp;
    }
    else if(SOURCE=="Levy_mT_Reso"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (Levy_mT_Reso)\n");
        goto CLEAN_SetUpCats_pp;
    }
    else{
        printf("\033[1;31mERROR:\033[0m Non-existing source '%s'\n",SOURCE.Data());
        goto CLEAN_SetUpCats_pp;
    }

    if(POT=="AV18"){
        //#,#,POT_ID,POT_FLAG,t_tot,t1,t2,s,l,j
        double PotPars1S0[8]={NN_AV18,v18_Coupled3P2,1,1,1,0,0,0};
        double PotPars3P0[8]={NN_AV18,v18_Coupled3P2,1,1,1,1,1,0};
        double PotPars3P1[8]={NN_AV18,v18_Coupled3P2,1,1,1,1,1,1};
        double PotPars3P2[8]={NN_AV18,v18_Coupled3P2,1,1,1,1,1,2};
        cPotPars1S0 = new CATSparameters(CATSparameters::tPotential,8,true); cPotPars1S0->SetParameters(PotPars1S0);
        cPotPars3P0 = new CATSparameters(CATSparameters::tPotential,8,true); cPotPars3P0->SetParameters(PotPars3P0);
        cPotPars3P1 = new CATSparameters(CATSparameters::tPotential,8,true); cPotPars3P1->SetParameters(PotPars3P1);
        cPotPars3P2 = new CATSparameters(CATSparameters::tPotential,8,true); cPotPars3P2->SetParameters(PotPars3P2);
    }
    else{
        printf("\033[1;31mERROR:\033[0m Non-existing pp potential '%s'\n",POT.Data());
        goto CLEAN_SetUpCats_pp;
    }
    Kitty.SetMomentumDependentSource(false);
    Kitty.SetThetaDependentSource(false);
    Kitty.SetExcludeFailedBins(false);

    Kitty.SetQ1Q2(1);
    Kitty.SetPdgId(2212, 2212);
    Kitty.SetRedMass( 0.5*Mass_p );

    Kitty.SetNumChannels(4);
    Kitty.SetNumPW(0,2);
    Kitty.SetNumPW(1,2);
    Kitty.SetNumPW(2,2);
    Kitty.SetNumPW(3,2);
    Kitty.SetSpin(0,0);
    Kitty.SetSpin(1,1);
    Kitty.SetSpin(2,1);
    Kitty.SetSpin(3,1);
    Kitty.SetChannelWeight(0, 3./12.);
    Kitty.SetChannelWeight(1, 1./12.);
    Kitty.SetChannelWeight(2, 3./12.);
    Kitty.SetChannelWeight(3, 5./12.);

    if(cPotPars1S0) Kitty.SetShortRangePotential(0,0,fDlmPot,*cPotPars1S0);
    if(cPotPars3P0) Kitty.SetShortRangePotential(1,1,fDlmPot,*cPotPars3P0);
    if(cPotPars3P1) Kitty.SetShortRangePotential(2,1,fDlmPot,*cPotPars3P1);
    if(cPotPars3P2) Kitty.SetShortRangePotential(3,1,fDlmPot,*cPotPars3P2);

    CLEAN_SetUpCats_pp: ;
    if(cPars){delete cPars; cPars=NULL;}
    //if(CleverLevy){delete CleverLevy; CleverLevy=NULL;}
    if(cPotPars1S0){delete cPotPars1S0; cPotPars1S0=NULL;}
    if(cPotPars3P0){delete cPotPars3P0; cPotPars3P0=NULL;}
    if(cPotPars3P1){delete cPotPars3P1; cPotPars3P1=NULL;}
    if(cPotPars3P2){delete cPotPars3P2; cPotPars3P2=NULL;}

}
//POT:
//  "LO"
//  "NLO"
//  "NLO_Coupled_S"
//  "Usmani"
void SetUpCats_pL(CATS& Kitty, const TString& POT, const TString& SOURCE){
    CATSparameters* cPars = NULL;
    CATSparameters* pPars = NULL;
    //DLM_CleverLevy* CleverLevy = NULL;
    if(!CleverLevy){
        CleverLevy = new DLM_CleverLevy* [100];
    }
    CleverLevy[NumCleverLevyObjects] = NULL;
    CATSparameters* cPotPars1S0 = NULL;
    CATSparameters* cPotPars3S1 = NULL;

    DLM_Histo<complex<double>>*** ExternalWF=NULL;
    unsigned NumChannels=0;

    if(SOURCE=="Gauss"){
        cPars = new CATSparameters(CATSparameters::tSource,1,true);
        cPars->SetParameter(0,1.2);
        Kitty.SetAnaSource(GaussSource, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Cauchy"){
        cPars = new CATSparameters(CATSparameters::tSource,1,true);
        cPars->SetParameter(0,1.2);
        Kitty.SetAnaSource(CauchySource, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Levy_Nolan"){
        cPars = new CATSparameters(CATSparameters::tSource,2,true);
        cPars->SetParameter(0,1.2);
        cPars->SetParameter(1,1.2);
        Kitty.SetAnaSource(LevySource3D, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Levy_Single"){
        cPars = new CATSparameters(CATSparameters::tSource,2,true);
        cPars->SetParameter(0,sqrt(1.2)*1.2);
        cPars->SetParameter(1,1.2);
        Kitty.SetAnaSource(LevySource3D_single, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Levy_Diff"){
        cPars->SetParameter(0,0.5*1.2*1.2);
        cPars->SetParameter(1,1.2);
        Kitty.SetAnaSource(LevySource3D_2particle, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="CleverLevy_Nolan"){
        CleverLevy[NumCleverLevyObjects] = new DLM_CleverLevy();
        CleverLevy[NumCleverLevyObjects]->InitStability(20,1,2);
        CleverLevy[NumCleverLevyObjects]->InitScale(35,0.25,2.0);
        CleverLevy[NumCleverLevyObjects]->InitRad(256,0,64);
        CleverLevy[NumCleverLevyObjects]->InitType(2);
        Kitty.SetAnaSource(CatsSourceForwarder, CleverLevy[NumCleverLevyObjects], 2);
        Kitty.SetAnaSource(0,1.2);
        Kitty.SetAnaSource(1,1.2);
        Kitty.SetUseAnalyticSource(true);
        NumCleverLevyObjects++;
    }
    else if(SOURCE=="CleverLevy_Single"){
        CleverLevy[NumCleverLevyObjects] = new DLM_CleverLevy();
        CleverLevy[NumCleverLevyObjects]->InitStability(20,1,2);
        CleverLevy[NumCleverLevyObjects]->InitScale(35,0.25,2.0);
        CleverLevy[NumCleverLevyObjects]->InitRad(256,0,64);
        CleverLevy[NumCleverLevyObjects]->InitType(0);
        Kitty.SetAnaSource(CatsSourceForwarder, CleverLevy[NumCleverLevyObjects], 2);
        Kitty.SetAnaSource(0,sqrt(1.2)*1.2);
        Kitty.SetAnaSource(1,1.2);
        Kitty.SetUseAnalyticSource(true);
        NumCleverLevyObjects++;
    }
    else if(SOURCE=="CleverLevy_Diff"){
        CleverLevy[NumCleverLevyObjects] = new DLM_CleverLevy();
        CleverLevy[NumCleverLevyObjects]->InitStability(20,1,2);
        CleverLevy[NumCleverLevyObjects]->InitScale(35,0.25,2.0);
        CleverLevy[NumCleverLevyObjects]->InitRad(256,0,64);
        CleverLevy[NumCleverLevyObjects]->InitType(1);
        Kitty.SetAnaSource(CatsSourceForwarder, CleverLevy[NumCleverLevyObjects], 2);
        Kitty.SetAnaSource(0,0.5*1.2*1.2);
        Kitty.SetAnaSource(1,1.2);
        Kitty.SetUseAnalyticSource(true);
        NumCleverLevyObjects++;
    }
    else if(SOURCE=="Gauss_mT_Reso"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (Gauss_mT_Reso)\n");
        goto CLEAN_SetUpCats_pL;
    }
    else if(SOURCE=="EPOS"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (EPOS)\n");
        goto CLEAN_SetUpCats_pL;
    }
    else if(SOURCE=="EPOSrescaled"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (EPOSrescaled)\n");
        goto CLEAN_SetUpCats_pL;
    }
    else if(SOURCE=="Levy_mT_Reso"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (Levy_mT_Reso)\n");
        goto CLEAN_SetUpCats_pL;
    }
    else{
        printf("\033[1;31mERROR:\033[0m Non-existing source '%s'\n",SOURCE.Data());
        goto CLEAN_SetUpCats_pL;
    }

    if(POT=="LO"){
        ExternalWF = Init_pL_Haidenbauer(   "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/Haidenbauer/pLambdaLO_600/",
                                Kitty, 0, 600);
        NumChannels=2;
    }
    else if(POT=="NLO"){
        ExternalWF = Init_pL_Haidenbauer(   "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/Haidenbauer/pLambdaNLO/",
                                Kitty, 1, 600);
        NumChannels=2;
    }
    else if(POT=="NLO_Coupled_S"){
        ExternalWF = Init_pL_Haidenbauer(   "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/Haidenbauer/pLambdaNLO_Coupling/",
                                Kitty, 2, 600);
        NumChannels=4;
    }
    else if(POT=="Usmani"){
        //#,#,POT_ID,POT_FLAG,t_tot,t1,t2,s,l,j
        double PotPars1S0[8]={pL_UsmaniOli,0,0,0,0,0,0,0};
        double PotPars3S1[8]={pL_UsmaniOli,0,0,0,0,1,0,1};
        cPotPars1S0 = new CATSparameters(CATSparameters::tPotential,8,true); cPotPars1S0->SetParameters(PotPars1S0);
        cPotPars3S1 = new CATSparameters(CATSparameters::tPotential,8,true); cPotPars3S1->SetParameters(PotPars3S1);
        NumChannels=2;
    }
    else{
        printf("\033[1;31mERROR:\033[0m Non-existing pp potential '%s'\n",POT.Data());
        goto CLEAN_SetUpCats_pL;
    }

    Kitty.SetMomentumDependentSource(false);
    Kitty.SetThetaDependentSource(false);
    Kitty.SetExcludeFailedBins(false);

    Kitty.SetQ1Q2(0);
    Kitty.SetPdgId(2212, 3122);
    Kitty.SetRedMass( (Mass_p*Mass_L)/(Mass_p+Mass_L) );

    Kitty.SetNumChannels(NumChannels);
    for(unsigned uCh=0; uCh<NumChannels; uCh++){
        Kitty.SetNumPW(uCh,1);
        Kitty.SetSpin(uCh, uCh%2==0?0:1);
        Kitty.SetChannelWeight(uCh, uCh%2==0?0.25:0.75);

        if(cPotPars1S0&&uCh==0)Kitty.SetShortRangePotential(uCh,0,fDlmPot,*cPotPars1S0);
        else if(cPotPars3S1&&uCh==1) Kitty.SetShortRangePotential(uCh,0,fDlmPot,*cPotPars3S1);
        else if(ExternalWF){
            //for(unsigned uMomBin=0; uMomBin<Kitty.GetNumMomBins(); uMomBin++){
                //Kitty.UseExternalWaveFunction(uMomBin,uCh,0,WaveFunctionU[uMomBin][uCh][0], NumRadBins, RadBins, PhaseShifts[uMomBin][uCh][0]);
//printf("Look at that view (%u)!\n",uCh);
                Kitty.SetExternalWaveFunction(uCh,0,ExternalWF[0][uCh][0],ExternalWF[1][uCh][0]);
//printf(" --Look at that view (%u)!\n",uCh);
            //}
        }
        else{
            printf("\033[1;31mERROR:\033[0m SetUpCats_pL says that you should NEVER see this message! BIG BUG!\n");
            goto CLEAN_SetUpCats_pL;
        }

    }
//Kitty.KillTheCat();
//printf("------------------------");
    CLEAN_SetUpCats_pL: ;

    if(cPars){delete cPars; cPars=NULL;}
    if(pPars){delete pPars; pPars=NULL;}
    //if(CleverLevy){delete CleverLevy; CleverLevy=NULL;}
    if(cPotPars1S0){delete cPotPars1S0; cPotPars1S0=NULL;}
    if(cPotPars3S1){delete cPotPars3S1; cPotPars3S1=NULL;}
    CleanUpWfHisto(Kitty,ExternalWF);

}
//POT:
//  "pXim_Lattice" (the first version)
//  "pXim_HALQCD1" (the second version)
void SetUpCats_pXim(CATS& Kitty, const TString& POT, const TString& SOURCE){
    CATSparameters* cPars = NULL;
    CATSparameters* pPars = NULL;
    //DLM_CleverLevy* CleverLevy = NULL;
    if(!CleverLevy){
        CleverLevy = new DLM_CleverLevy* [100];
    }
    CleverLevy[NumCleverLevyObjects] = NULL;
    CATSparameters* cPotParsI0S0 = NULL;
    CATSparameters* cPotParsI0S1 = NULL;
    CATSparameters* cPotParsI1S0 = NULL;
    CATSparameters* cPotParsI1S1 = NULL;

    if(SOURCE=="Gauss"){
        cPars = new CATSparameters(CATSparameters::tSource,1,true);
        cPars->SetParameter(0,1.2);
        Kitty.SetAnaSource(GaussSource, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Cauchy"){
        cPars = new CATSparameters(CATSparameters::tSource,1,true);
        cPars->SetParameter(0,1.2);
        Kitty.SetAnaSource(CauchySource, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Levy_Nolan"){
        cPars = new CATSparameters(CATSparameters::tSource,2,true);
        cPars->SetParameter(0,1.2);
        cPars->SetParameter(1,1.8);
        Kitty.SetAnaSource(LevySource3D, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Levy_Single"){
        cPars = new CATSparameters(CATSparameters::tSource,2,true);
        cPars->SetParameter(0,sqrt(1.8)*1.2);
        cPars->SetParameter(1,1.8);
        Kitty.SetAnaSource(LevySource3D_single, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="Levy_Diff"){
        cPars->SetParameter(0,0.5*1.8*1.2);
        cPars->SetParameter(1,1.8);
        Kitty.SetAnaSource(LevySource3D_2particle, *cPars);
        Kitty.SetUseAnalyticSource(true);
    }
    else if(SOURCE=="CleverLevy_Nolan"){
        CleverLevy[NumCleverLevyObjects] = new DLM_CleverLevy();
        CleverLevy[NumCleverLevyObjects]->InitStability(20,1,2);
        CleverLevy[NumCleverLevyObjects]->InitScale(35,0.25,2.0);
        CleverLevy[NumCleverLevyObjects]->InitRad(256,0,64);
        CleverLevy[NumCleverLevyObjects]->InitType(2);
        Kitty.SetAnaSource(CatsSourceForwarder, CleverLevy[NumCleverLevyObjects], 2);
        Kitty.SetUseAnalyticSource(true);
        Kitty.SetAnaSource(0,1.2);
        Kitty.SetAnaSource(1,1.8);
        NumCleverLevyObjects++;
    }
    else if(SOURCE=="CleverLevy_Single"){
        CleverLevy[NumCleverLevyObjects] = new DLM_CleverLevy();
        CleverLevy[NumCleverLevyObjects]->InitStability(20,1,2);
        CleverLevy[NumCleverLevyObjects]->InitScale(35,0.25,2.0);
        CleverLevy[NumCleverLevyObjects]->InitRad(256,0,64);
        CleverLevy[NumCleverLevyObjects]->InitType(0);
        Kitty.SetAnaSource(CatsSourceForwarder, CleverLevy[NumCleverLevyObjects], 2);
        Kitty.SetAnaSource(0,sqrt(1.8)*1.2);
        Kitty.SetAnaSource(1,1.8);
        Kitty.SetUseAnalyticSource(true);
        NumCleverLevyObjects++;
    }
    else if(SOURCE=="CleverLevy_Diff"){
        CleverLevy[NumCleverLevyObjects] = new DLM_CleverLevy();
        CleverLevy[NumCleverLevyObjects]->InitStability(20,1,2);
        CleverLevy[NumCleverLevyObjects]->InitScale(35,0.25,2.0);
        CleverLevy[NumCleverLevyObjects]->InitRad(256,0,64);
        CleverLevy[NumCleverLevyObjects]->InitType(1);
        Kitty.SetAnaSource(CatsSourceForwarder, CleverLevy[NumCleverLevyObjects], 2);
        Kitty.SetUseAnalyticSource(true);
        Kitty.SetAnaSource(0,0.5*1.8*1.2);
        Kitty.SetAnaSource(1,1.8);
        NumCleverLevyObjects++;
    }
    else if(SOURCE=="Gauss_mT_Reso"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (Gauss_mT_Reso)\n");
        goto CLEAN_SetUpCats_pp;
    }
    else if(SOURCE=="EPOS"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (EPOS)\n");
        goto CLEAN_SetUpCats_pp;
    }
    else if(SOURCE=="EPOSrescaled"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (EPOSrescaled)\n");
        goto CLEAN_SetUpCats_pp;
    }
    else if(SOURCE=="Levy_mT_Reso"){
        printf("\033[1;33mWARNING:\033[0m The CommonAnaFunction is still under construction (Levy_mT_Reso)\n");
        goto CLEAN_SetUpCats_pp;
    }
    else{
        printf("\033[1;31mERROR:\033[0m Non-existing source '%s'\n",SOURCE.Data());
        goto CLEAN_SetUpCats_pp;
    }

    if(POT=="pXim_Lattice"){
        //#,#,POT_ID,POT_FLAG,t_tot,t1,t2,s,l,j
        double PotParsI0S0[9]={pXim_Lattice,12,0,-1,1,0,0,0,0};
        double PotParsI0S1[9]={pXim_Lattice,12,0,-1,1,1,0,1,0};
        double PotParsI1S0[9]={pXim_Lattice,6,1,1,1,0,0,0,0};
        double PotParsI1S1[9]={pXim_Lattice,6,1,1,1,1,0,1,0};
        cPotParsI0S0 = new CATSparameters(CATSparameters::tPotential,9,true); cPotParsI0S0->SetParameters(PotParsI0S0);
        cPotParsI0S1 = new CATSparameters(CATSparameters::tPotential,9,true); cPotParsI0S1->SetParameters(PotParsI0S1);
        cPotParsI1S0 = new CATSparameters(CATSparameters::tPotential,9,true); cPotParsI1S0->SetParameters(PotParsI1S0);
        cPotParsI1S1 = new CATSparameters(CATSparameters::tPotential,9,true); cPotParsI1S1->SetParameters(PotParsI1S1);
    }
    else if(POT=="pXim_HALQCD1"){
        //#,#,POT_ID,POT_FLAG,t_tot,t1,t2,s,l,j
        double PotParsI0S0[9]={pXim_HALQCD1,12,0,-1,1,0,0,0,0};
        double PotParsI0S1[9]={pXim_HALQCD1,12,0,-1,1,1,0,1,0};
        double PotParsI1S0[9]={pXim_HALQCD1,12,1,1,1,0,0,0,0};
        double PotParsI1S1[9]={pXim_HALQCD1,12,1,1,1,1,0,1,0};
        cPotParsI0S0 = new CATSparameters(CATSparameters::tPotential,9,true); cPotParsI0S0->SetParameters(PotParsI0S0);
        cPotParsI0S1 = new CATSparameters(CATSparameters::tPotential,9,true); cPotParsI0S1->SetParameters(PotParsI0S1);
        cPotParsI1S0 = new CATSparameters(CATSparameters::tPotential,9,true); cPotParsI1S0->SetParameters(PotParsI1S0);
        cPotParsI1S1 = new CATSparameters(CATSparameters::tPotential,9,true); cPotParsI1S1->SetParameters(PotParsI1S1);
    }
    else{
        printf("\033[1;31mERROR:\033[0m Non-existing pp potential '%s'\n",POT.Data());
        goto CLEAN_SetUpCats_pp;
    }
    Kitty.SetMomentumDependentSource(false);
    Kitty.SetThetaDependentSource(false);
    Kitty.SetExcludeFailedBins(false);

    Kitty.SetQ1Q2(-1);
    Kitty.SetPdgId(2212, 3122);
    Kitty.SetRedMass( (Mass_p*Mass_Xim)/(Mass_p+Mass_Xim) );

    Kitty.SetNumChannels(4);
    Kitty.SetNumPW(0,1);
    Kitty.SetNumPW(1,1);
    Kitty.SetNumPW(2,1);
    Kitty.SetNumPW(3,1);
    Kitty.SetSpin(0,0);
    Kitty.SetSpin(1,1);
    Kitty.SetSpin(2,0);
    Kitty.SetSpin(3,1);
    Kitty.SetChannelWeight(0, 1./8.);
    Kitty.SetChannelWeight(1, 3./8.);
    Kitty.SetChannelWeight(2, 1./8.);
    Kitty.SetChannelWeight(3, 3./8.);

    if(cPotParsI0S0) Kitty.SetShortRangePotential(0,0,fDlmPot,*cPotParsI0S0);
    if(cPotParsI0S1) Kitty.SetShortRangePotential(1,0,fDlmPot,*cPotParsI0S1);
    if(cPotParsI1S0) Kitty.SetShortRangePotential(2,0,fDlmPot,*cPotParsI1S0);
    if(cPotParsI1S1) Kitty.SetShortRangePotential(3,0,fDlmPot,*cPotParsI1S1);

    CLEAN_SetUpCats_pp: ;
    if(cPars){delete cPars; cPars=NULL;}
    if(pPars){delete pPars; pPars=NULL;}
    //if(CleverLevy){delete CleverLevy; CleverLevy=NULL;}
    if(cPotParsI0S0){delete cPotParsI0S0; cPotParsI0S0=NULL;}
    if(cPotParsI0S1){delete cPotParsI0S1; cPotParsI0S1=NULL;}
    if(cPotParsI1S0){delete cPotParsI1S0; cPotParsI1S0=NULL;}
    if(cPotParsI1S1){delete cPotParsI1S1; cPotParsI1S1=NULL;}
}

void SetUpBinning_pp(const TString& DataSample, unsigned& NumMomBins, double*& MomBins, double*& FitRegion){
    if(DataSample=="pp13TeV_MB_Run2paper"){
        const double kMin=0;
        const double kStep=4;
        NumMomBins=94;//(i.e. max=376 MeV)
        if(MomBins) delete [] MomBins;
        MomBins = new double [NumMomBins];
        MomBins[0] = kMin;
        for(unsigned uBin=1; uBin<=NumMomBins; uBin++){
            MomBins[uBin] = MomBins[uBin-1]+kStep;
        }
        if(FitRegion) delete [] FitRegion;
        FitRegion = new double [4];
        FitRegion[0] = MomBins[0];
        FitRegion[1] = MomBins[NumMomBins];
        FitRegion[2] = MomBins[NumMomBins]+kStep;
        FitRegion[3] = MomBins[NumMomBins]+kStep*31.;//till 500
    }
    else if(DataSample=="pp13TeV_HM_March19"){
        const double kMin=4;
        const double kStep=4;
        NumMomBins=94;//(i.e. max=376 MeV)
        if(MomBins) delete [] MomBins;
        MomBins = new double [NumMomBins];
        MomBins[0] = kMin;
        for(unsigned uBin=1; uBin<=NumMomBins; uBin++){
            MomBins[uBin] = MomBins[uBin-1]+kStep;
        }
        if(FitRegion) delete [] FitRegion;
        FitRegion = new double [4];
        FitRegion[0] = MomBins[0];
        FitRegion[1] = MomBins[NumMomBins];
        FitRegion[2] = MomBins[NumMomBins]+kStep;
        FitRegion[3] = MomBins[NumMomBins]+kStep*31.;//till 500
    }
    else if(DataSample=="pPb5TeV_Run2paper"){
        const double kMin=0;
        const double kStep=4;
        NumMomBins=94;//(i.e. max=376 MeV)
        if(MomBins) delete [] MomBins;
        MomBins = new double [NumMomBins+1];
        MomBins[0] = kMin;
        for(unsigned uBin=1; uBin<=NumMomBins; uBin++){
            MomBins[uBin] = MomBins[uBin-1]+kStep;
        }
    }
    else{
        printf("\033[1;31mERROR:\033[0m The data sample '%s' does not exist\n",DataSample.Data());
        NumMomBins=0;
        return;
    }
}
void SetUpBinning_pL(const TString& DataSample, unsigned& NumMomBins, double*& MomBins, double*& FitRegion){

    if(DataSample=="pp13TeV_MB_Run2paper"){
        const double kMin=0;
        const double kFineMin=272;
        const double kFineMax=304;
        const double kMax=336;
        const double kCoarseStep=16;
        const double kFineStep=8;

        //the number of coarse bins below kFineMin
        //floor/ceil combination makes sure that we include the WHOLE region we want in the fine binning,
        //and if there is rounding needed it is done so that we make our region larger, not smaller!
        unsigned NumCoarseBinsBelow = floor((kFineMin-kMin)/kCoarseStep);
        unsigned NumFineBins = ceil((kFineMax-double(NumCoarseBinsBelow)*kCoarseStep)/kFineStep);
        //we floor the highest point, to make sure we do not run out of the range provided by experimental data
        unsigned NumCoarseBinsAbove = floor((kMax-double(NumCoarseBinsBelow)*kCoarseStep-double(NumFineBins)*kFineStep)/kCoarseStep);

        NumMomBins=NumCoarseBinsBelow+NumFineBins+NumCoarseBinsAbove;

        if(MomBins) delete [] MomBins;
        MomBins = new double [NumMomBins+1];
        MomBins[0] = kMin;
        for(unsigned uBin=1; uBin<=NumMomBins; uBin++){
            if(uBin<=NumCoarseBinsBelow||uBin>NumCoarseBinsBelow+NumFineBins){
                MomBins[uBin] = MomBins[uBin-1]+kCoarseStep;
            }
            else{
                MomBins[uBin] = MomBins[uBin-1]+kFineStep;
            }
        }
        if(FitRegion) delete [] FitRegion;
        FitRegion = new double [4];
        FitRegion[0] = MomBins[0];
        FitRegion[1] = MomBins[NumMomBins];
        FitRegion[2] = MomBins[NumMomBins]+kCoarseStep;
        FitRegion[3] = MomBins[NumMomBins]+kCoarseStep*10.;//till 496
    }
    else if(DataSample=="pp13TeV_HM_March19"){
        const double kMin=0;
        const double kFineMin=264;
        const double kFineMax=312;
        const double kMax=336;
        const double kCoarseStep=12;
        const double kFineStep=8;

        //the number of coarse bins below kFineMin
        //floor/ceil combination makes sure that we include the WHOLE region we want in the fine binning,
        //and if there is rounding needed it is done so that we make our region larger, not smaller!
        unsigned NumCoarseBinsBelow = floor((kFineMin-kMin)/kCoarseStep);
        unsigned NumFineBins = ceil((kFineMax-double(NumCoarseBinsBelow)*kCoarseStep)/kFineStep);
        //we floor the highest point, to make sure we do not run out of the range provided by experimental data
        unsigned NumCoarseBinsAbove = floor((kMax-double(NumCoarseBinsBelow)*kCoarseStep-double(NumFineBins)*kFineStep)/kCoarseStep);

        NumMomBins=NumCoarseBinsBelow+NumFineBins+NumCoarseBinsAbove;

        if(MomBins) delete [] MomBins;
        MomBins = new double [NumMomBins+1];
        MomBins[0] = kMin;
        for(unsigned uBin=1; uBin<=NumMomBins; uBin++){
            if(uBin<=NumCoarseBinsBelow||uBin>NumCoarseBinsBelow+NumFineBins){
                MomBins[uBin] = MomBins[uBin-1]+kCoarseStep;
            }
            else{
                MomBins[uBin] = MomBins[uBin-1]+kFineStep;
            }
        }
        if(FitRegion) delete [] FitRegion;
        FitRegion = new double [4];
        FitRegion[0] = MomBins[0];
        FitRegion[1] = MomBins[NumMomBins];
        FitRegion[2] = MomBins[NumMomBins]+kCoarseStep;
        FitRegion[3] = MomBins[NumMomBins]+kCoarseStep*10.;//till 496
    }
    else if(DataSample=="pPb5TeV_Run2paper"){
        printf("\033[1;33mWARNING:\033[0m pPb5TeV_Run2paper is not available yet (in SetUpBinning_pL)!\n");
        NumMomBins=0;
        return;
    }
    else{
        printf("\033[1;31mERROR:\033[0m The data sample '%s' does not exist\n",DataSample.Data());
        NumMomBins=0;
        return;
    }

}


void GetPurities_p(const TString& DataSample, const int& Variation, double* Purities){
    double PurityProton;
    if(DataSample=="pp13TeV_MB_Run2paper"){
        PurityProton = 0.989859;
    }
    else if(DataSample=="pp13TeV_HM_March19"){
        printf("\033[1;33mWARNING:\033[0m pp13TeV_HM_March19 is not available yet!\n");
        PurityProton = 0.989859;
    }
    else if(DataSample=="pPb5TeV_Run2paper"){
        PurityProton = 0.984265;
    }
    else{
        printf("\033[1;31mERROR:\033[0m The data sample '%s' does not exist\n",DataSample.Data());
        PurityProton = 1.0;
    }

    //following my lambda pars with the 3 possible modifications
    //for the proton:
    //0 = primary
    //1 = from Lambda
    //2 = other feeddown (flat)
    //3 = missidentified
    //const unsigned NumChannels_p = 4;
    //if(Purities){delete [] Purities; Purities = new double [NumChannels_p];}
    Purities[0] = PurityProton;
    Purities[1] = PurityProton;
    Purities[2] = PurityProton;
    Purities[3] = 1.-PurityProton;
}
void GetPurities_L(const TString& DataSample, const int& Variation, double* Purities){
    double PurityLambda;
    if(DataSample=="pp13TeV_MB_Run2paper"){
        PurityLambda = 0.96768;
    }
    else if(DataSample=="pp13TeV_HM_March19"){
        printf("\033[1;33mWARNING:\033[0m pp13TeV_HM_March19 is not available yet!\n");
        PurityLambda = 0.96768;
    }
    else if(DataSample=="pPb5TeV_Run2paper"){
        PurityLambda = 0.937761;
    }
    else{
        printf("\033[1;31mERROR:\033[0m The data sample '%s' does not exist\n",DataSample.Data());
        PurityLambda = 1.0;
    }

    //for the Lambda:
    //0 = primary
    //1 = from Sigma0
    //2 = from Xim
    //3 = from Xi0
    //4 = missidentified
    Purities[0] = PurityLambda;
    Purities[1] = PurityLambda;
    Purities[2] = PurityLambda;
    Purities[3] = PurityLambda;
    Purities[4] = 1.-PurityLambda;
}
void GetPurities_Xim(const TString& DataSample, const int& Variation, double* Purities){
    double PurityXim;
    if(DataSample=="pp13TeV_MB_Run2paper"){
        PurityXim = 0.956;
    }
    else if(DataSample=="pp13TeV_HM_March19"){
        printf("\033[1;33mWARNING:\033[0m pp13TeV_HM_March19 is not available yet!\n");
        PurityXim = 0.956;
    }
    else if(DataSample=="pPb5TeV_Run2paper"){
        PurityXim = 0.88;
    }
    else{
        printf("\033[1;31mERROR:\033[0m The data sample '%s' does not exist\n",DataSample.Data());
        PurityXim = 1.0;
    }

    //0 = primary
    //1 = from Xi-(1530)
    //2 = from Xi0(1530)
    //3 = from Omega
    //4 = missidentified
    Purities[0] = PurityXim;
    Purities[1] = PurityXim;
    Purities[2] = PurityXim;
    Purities[3] = PurityXim;
    Purities[4] = 1.-PurityXim;
}
//no variations are possible
void GetFractions_p(const TString& DataSample, const int& Variation, double* Fractions){
    //following my lambda pars with the 3 possible modifications
    //for the proton:
    //0 = primary
    //1 = from Lambda
    //2 = other feeddown (flat)
    //3 = missidentified
    double Modify_pp=1;
    switch(Variation){
        default : Modify_pp=1; break;
    }
    double pp_f0;//primary protons
    double pp_f1;//fraction of Lambda
    if(DataSample=="pp13TeV_MB_Run2paper"){
        pp_f0 = 0.87397;
        pp_f1 = 0.0882211;
    }
    else if(DataSample=="pp13TeV_HM_March19"){
        printf("\033[1;33mWARNING:\033[0m pp13TeV_HM_March19 is not available yet!\n");
        pp_f0 = 0.87397;
        pp_f1 = 0.0882211;
    }
    else if(DataSample=="pPb5TeV_Run2paper"){
        pp_f0 = 0.862814;
        pp_f1 = 0.09603;
    }
    else{
        printf("\033[1;31mERROR:\033[0m The data sample '%s' does not exist\n",DataSample.Data());
        pp_f0 = 1.0;
        pp_f1 = 0.0;
    }
    double arrayPercLamProton = pp_f1/(1.-pp_f0)*Modify_pp;
    Fractions[0] = pp_f0;
    Fractions[1] = (1.-pp_f0)*(arrayPercLamProton);
    Fractions[2] = (1.-pp_f0)*(1.-arrayPercLamProton);
    Fractions[3] = 1.;
}
//Variation -> use the two digits
//first digit->Modify_SigL
//second digit->Modify_XiL
//0 -> default; 1 = -20%; 2 = +20%
void GetFractions_L(const TString& DataSample, const int& Variation, double* Fractions){
    double Modify_SigL=1;
    double Modify_XiL=1;
    switch(Variation%10){
        case 0 : Modify_SigL=1; break;
        case 1 : Modify_SigL=0.8;break;
        case 2 : Modify_SigL=1.2; break;
        default : Modify_SigL=1; break;
    }
    switch(Variation/10){
        case 0 : Modify_XiL=1; break;
        case 1 : Modify_XiL=0.8;break;
        case 2 : Modify_XiL=1.2; break;
        default : Modify_XiL=1; break;
    }
    double pL_f0;//fraction of primary Lambdas
    double pL_f1;//fraction of Sigma0
    double pL_f2;//fractions of Xi0/m
    if(DataSample=="pp13TeV_MB_Run2paper"){
        pL_f0 = 0.601008;
        pL_f1 = 0.200336;
        pL_f2 = 0.0993283;
    }
    else if(DataSample=="pp13TeV_HM_March19"){
        printf("\033[1;33mWARNING:\033[0m pp13TeV_HM_March19 is not available yet!\n");
        pL_f0 = 0.601008;
        pL_f1 = 0.200336;
        pL_f2 = 0.0993283;
    }
    else if(DataSample=="pPb5TeV_Run2paper"){
        pL_f0 = 0.521433;
        pL_f1 = 0.173811;
        pL_f2 = 0.152378;
    }
    else{
        printf("\033[1;31mERROR:\033[0m The data sample '%s' does not exist\n",DataSample.Data());
        pL_f0 = 1.0;
        pL_f1 = 0.0;
        pL_f2 = 0.0;
    }
    double SigLambdaPrimDir = pL_f0+pL_f1;
    double arrayPercSigLambda=pL_f1/pL_f0*Modify_SigL;
    double arrayPercXiLambda=pL_f2/(1.-pL_f0-pL_f1)*Modify_XiL;
    double FracOfLambda = 1./(1.+arrayPercSigLambda);
    //0 is primary
    //1 is from Sigma0
    //2 is is from Xim
    //3 is is the flat feeddown
    //4 is for the missid
    Fractions[0] = SigLambdaPrimDir*FracOfLambda;
    Fractions[1] = SigLambdaPrimDir*(1.-FracOfLambda);
    Fractions[2] = (1.-SigLambdaPrimDir)*(arrayPercXiLambda);
    Fractions[3] = 1.-Fractions[0]-Fractions[1]-Fractions[2];
    Fractions[4] = 1.;
}
void GetFractions_Xim(const TString& DataSample, const int& Variation, double* Fractions){
    //0 = primary
    //1 = from Xi-(1530)
    //2 = from Omega
    //3 = flat
    //4 = missidentified

    //ratio Xi-(1530) to Xi-
    const double Xim1530_to_Xim = 0.32*(1./3.);
    //ratio Xi0(1530) to Xi0 (n=neutral)
    //const double Xin1530_to_Xim = 0.32*(2./3.);
    const double Omegam_to_Xim = 0.1;
    const double OmegamXim_BR = 0.086;
    //the ratios that we have for Xis are referred to the total number of Xi particles (which already include all contributions)
    //hence Xi1530_to_Xi indeed is simply the number of Xis that stem from a Xi1530
    Fractions[0] = 1.-3.*Xim1530_to_Xim-Omegam_to_Xim*OmegamXim_BR;
    Fractions[1] = Xim1530_to_Xim;
    Fractions[2] = Omegam_to_Xim*OmegamXim_BR;
    Fractions[3] = 1.-Fractions[2]-Fractions[1];
    Fractions[4] = 1.;
}
//0 is primary
//1 is pL->pp
//2 is flat feed
//3 is missid
void SetUpLambdaPars_pp(const TString& DataSample, const int& Variation_p, double* lambda_pars){
    double Purities_p[4];
    double Fraction_p[4];
    GetPurities_p(DataSample,Variation_p,Purities_p);
    GetFractions_p(DataSample,Variation_p,Fraction_p);
    lambda_pars[0] = Purities_p[0]*Fraction_p[0]*Purities_p[0]*Fraction_p[0];
    lambda_pars[1] = Purities_p[0]*Fraction_p[0]*Purities_p[1]*Fraction_p[1]*2.;
    lambda_pars[3] = (Purities_p[0]+Purities_p[0]+Purities_p[3])*Purities_p[3];
    lambda_pars[2] = 1.-lambda_pars[3]-lambda_pars[1]-lambda_pars[0];

    //double SUM=0;
    //for(unsigned uLam=0; uLam<4; uLam++){
    //    printf("λ(pp)_%u = %.1f\n",uLam,lambda_pars[uLam]*100.);
    //    SUM+=lambda_pars[uLam]*100.;
    //}
    //printf("SUM: %.1f\n------------\n",SUM);
}
//0 is primary
//1 is pSigma0->pL
//2 is pXim->pL
//3 is the flat feeddown
//4 is missid
void SetUpLambdaPars_pL(const TString& DataSample, const int& Variation_p, const int& Variation_L, double* lambda_pars){
    double Purities_p[4];
    double Fraction_p[4];
    double Purities_L[5];
    double Fraction_L[5];
    GetPurities_p(DataSample,Variation_p,Purities_p);
    GetFractions_p(DataSample,Variation_p,Fraction_p);
    GetPurities_L(DataSample,Variation_L,Purities_L);
    GetFractions_L(DataSample,Variation_L,Fraction_L);
    lambda_pars[0] =    Purities_p[0]*Fraction_p[0]*Purities_L[0]*Fraction_L[0];
    lambda_pars[1] =    Purities_p[0]*Fraction_p[0]*Purities_L[1]*Fraction_L[1];
    lambda_pars[2] =    Purities_p[0]*Fraction_p[0]*Purities_L[2]*Fraction_L[2];
    lambda_pars[4] =    Purities_p[0]*Purities_L[4]+Purities_p[3]*Purities_L[0]+Purities_p[3]*Purities_L[4];
    lambda_pars[3] =    1.-lambda_pars[0]-lambda_pars[1]-lambda_pars[2]-lambda_pars[4];

    //double SUM=0;
    //for(unsigned uLam=0; uLam<5; uLam++){
    //    printf("λ(pΛ)_%u = %.1f\n",uLam,lambda_pars[uLam]*100.);
    //    SUM+=lambda_pars[uLam]*100.;
    //}
    //printf("SUM: %.1f\n------------\n",SUM);
}
//0 is primary
//1 is from Xim1530
//2 is from Xin1530
//3
//4 is missid
void SetUpLambdaPars_pXim(const TString& DataSample, const int& Variation_p, const int& Variation_Xim, double* lambda_pars){
    double Purities_p[4];
    double Fraction_p[4];
    double Purities_Xim[5];
    double Fraction_Xim[5];
    GetPurities_p(DataSample,Variation_p,Purities_p);
    GetFractions_p(DataSample,Variation_p,Fraction_p);
    GetPurities_Xim(DataSample,Variation_Xim,Purities_Xim);
    GetFractions_Xim(DataSample,Variation_Xim,Fraction_Xim);
    lambda_pars[0] =    Purities_p[0]*Fraction_p[0]*Purities_Xim[0]*Fraction_Xim[0];
    lambda_pars[1] =    Purities_p[0]*Fraction_p[0]*Purities_Xim[1]*Fraction_Xim[1];
    lambda_pars[2] =    Purities_p[0]*Fraction_p[0]*Purities_Xim[2]*Fraction_Xim[2];
    lambda_pars[4] =    Purities_p[0]*Purities_Xim[4]+Purities_p[3]*Purities_Xim[0]+Purities_p[3]*Purities_Xim[4];
    lambda_pars[3] =    1.-lambda_pars[0]-lambda_pars[1]-lambda_pars[2]-lambda_pars[4];

    //double SUM=0;
    //for(unsigned uLam=0; uLam<5; uLam++){
    //    printf("λ(pΞ)_%u = %.1f\n",uLam,lambda_pars[uLam]*100.);
    //    SUM+=lambda_pars[uLam]*100.;
    //}
    //printf("SUM: %.1f\n------------\n",SUM);
}

//        DataPeriod=="pp13TeV"?  :
//                                ;

TH2F* GetResolutionMatrix(const TString& DataSample,const TString&& System){
    TString FileName;
    TString HistoName;

    if(DataSample=="pp13TeV_MB_Run2paper"){
        FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pp_13TeV/ResolutionMatrices/Sample6_MeV_compact.root";
    }
    else if(DataSample=="pp13TeV_HM_March19"){
        printf("\033[1;33mWARNING:\033[0m pp13TeV_HM_March19 is not available yet!\n");
        FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pp_13TeV/ResolutionMatrices/Sample6_MeV_compact.root";
    }
    else if(DataSample=="pPb5TeV_Run2paper"){
        FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pPb_5TeV/ResolutionMatrices/Sample3_MeV_compact.root";
    }
    else{
        printf("\033[1;31mERROR:\033[0m The data sample '%s' does not exist\n",DataSample.Data());
        FileName = "";
    }

    if(System=="pp"){
        HistoName = "hSigmaMeV_Proton_Proton";
    }
    else if(System=="pLambda"){
        HistoName = "hSigmaMeV_Proton_Lambda";
    }
    else if(System=="LambdaLambda"){
        HistoName = "hSigmaMeV_Lambda_Lambda";
    }
    else if(System=="pXim"){
        HistoName = "hSigmaMeV_Proton_Xim";
    }
    else{
        printf("\033[1;31mERROR:\033[0m The system '%s' does not exist\n",System.Data());
    }
    ///FUCKING ROOT SUCKS!!!!!! SUCK MY COCK!!!! SUCK IT YOU BITCH!!!!!!!!!!
    //so we need to copy our histogram, as else we lose it when we delete the file
    //and we need to change to the "central" root directory, as else histoCopy will also be lost
    //and we need to play with the name a little bit, else we are fucked!
    TFile* FileROOT = new TFile(FileName, "read");
    TH2F* histo = (TH2F*)FileROOT->Get(HistoName);
    TString Name = histo->GetName();
    gROOT->cd();
    TH2F *histoCopy = (TH2F*)histo->Clone("histoCopy");
    delete FileROOT;
    histoCopy->SetName(Name);
    return histoCopy;
}
TH2F* GetResidualMatrix(const TString&& FinalSystem, const TString& InitialSystem){
    TString FileName;
    TString HistoName;

    FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/DecayMatrices/run2_decay_matrices_old.root";

    if(FinalSystem=="pp"&&InitialSystem=="pLambda"){
        HistoName = "hRes_pp_pL";
    }
    else if(FinalSystem=="pLambda"&&InitialSystem=="pSigma0"){
        HistoName = "hRes_pL_pSigma0";
    }
    else if(FinalSystem=="pLambda"&&InitialSystem=="pXim"){
        HistoName = "hRes_pL_pXim";
    }
    else if(FinalSystem=="pXim"&&InitialSystem=="pXim1530"){
        HistoName = "hRes_pXim_pXim1530";
    }
    else{
        printf("\033[1;31mERROR:\033[0m The decay '%s->%s' does not exist\n",InitialSystem.Data(),FinalSystem.Data());
    }
    TFile* FileROOT = new TFile(FileName, "read");
    TH2F* histo = (TH2F*)FileROOT->Get(HistoName);
    TString Name = histo->GetName();
    gROOT->cd();
    TH2F *histoCopy = (TH2F*)histo->Clone("histoCopy");
    delete FileROOT;
    histoCopy->SetName(Name);
    return histoCopy;
}

//iReb = 0 is 4 MeV, 1 is 8, 2 is 12, 3 is 16, 4 is 20
TH1F* GetAliceExpCorrFun(const TString& DataSample,const TString& System,const int& iReb){
    TString FileName;
    TString HistoName;

    if(DataSample=="pp13TeV_MB_Run2paper"){
        if(System=="pp"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pp_13TeV/Sample8/CFOutput_pp.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else if(System=="pLambda"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pp_13TeV/Sample8/CFOutput_pL.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else if(System=="LambdaLambda"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pp_13TeV/Sample8/CFOutput_LL.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else if(System=="pXim"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pp_13TeV/Sample8/CFOutput_pXi.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else{
            printf("\033[1;31mERROR:\033[0m The system '%s' does not exist\n",System.Data());
        }
    }
    else if(DataSample=="pp13TeV_HM_March19"){
        if(System=="pp"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pp_13TeV/Sample8HM/CFOutput_pp.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else if(System=="pLambda"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pp_13TeV/Sample8HM/CFOutput_pL.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else if(System=="LambdaLambda"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pp_13TeV/Sample8HM/CFOutput_LL.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else if(System=="pXim"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pp_13TeV/Sample8HM/CFOutput_pXi.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else{
            printf("\033[1;31mERROR:\033[0m The system '%s' does not exist\n",System.Data());
        }
    }
    else if(DataSample=="pPb5TeV_Run2paper"){
        if(System=="pp"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pPb_5TeV/Sample8/ClosePairRej/CFOutput_pp.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else if(System=="pLambda"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pPb_5TeV/Sample8/ClosePairRej/CFOutput_pL.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else if(System=="LambdaLambda"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pPb_5TeV/Sample8/ClosePairRej/CFOutput_LL.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else if(System=="pXim"){
            FileName = "/home/dmihaylov/Dudek_Ubuntu/Work/Kclus/GeneralFemtoStuff/CorrelationFiles_2018/ALICE_pPb_5TeV/Sample8/ClosePairRej/CFOutput_pXi.root";
            HistoName = TString::Format("hCk_ReweightedMeV_%i",iReb);
        }
        else{
            printf("\033[1;31mERROR:\033[0m The system '%s' does not exist\n",System.Data());
        }
    }
    else{
        printf("\033[1;31mERROR:\033[0m The data sample '%s' does not exist\n",DataSample.Data());
        FileName = "";
    }

    TFile* FileROOT = new TFile(FileName, "read");
    TH1F* histo = (TH1F*)FileROOT->Get(HistoName);
    TString Name = histo->GetName();
    gROOT->cd();
    TH1F *histoCopy = (TH1F*)histo->Clone("histoCopy");
    delete FileROOT;
    histoCopy->SetName(Name);
    return histoCopy;
}

void Clean_CommonAnaFunctions(){
    for(unsigned uLevy=0; uLevy<NumCleverLevyObjects; uLevy++){
        delete CleverLevy[uLevy];
        CleverLevy[uLevy] = NULL;
    }
    delete [] CleverLevy;
    CleverLevy=NULL;
}

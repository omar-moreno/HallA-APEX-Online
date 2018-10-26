/*
  ParamMan.cc
*/


#include "ParamMan.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <map>
using namespace std;

ParamMan::ParamMan( const char* filename )
  : ParamFileName(filename)
{
  F1S2.cid=CID_F1S2;
  for(int i=0; i<nS2*4; i++){
    F1S2.tdcOffset[i]=0.00;   F1S2.tdcGain[i]=0.05623;
  }

  F1S0.cid=CID_F1S0;
  for(int i=0; i<nS0*4; i++){
    F1S0.tdcOffset[i]=0.00;   F1S0.tdcGain[i]=0.05623;
  }

  RF.cid=CID_RF;
  for(int i=0; i<nRF*2; i++){
    RF.tdcOffset[i]=0.00;   RF.tdcGain[i]=0.05623;
  }

  FbS2.cid=CID_FbS2;
  for(int i=0; i<nS2*4; i++){
    FbS2.tdcOffset[i]=0.00;   FbS2.tdcGain[i]=0.5;
  }

  FbS0.cid=CID_FbS0;
  for(int i=0; i<nS0*4; i++){
    FbS0.tdcOffset[i]=0.00;   FbS0.tdcGain[i]=0.5;
  }

}
////////////////////////////////
//ParamMan::~ParamMan()
//{
//}
///////////////////////////////////
bool ParamMan::SetVal( void )
{
  static const std::string funcname = "ParamMan::SetVal";
  FILE *fp;
  char str[MaxChar];
  
  if((fp=fopen(ParamFileName,"r"))==0){
    std::cerr << "[" << funcname << "]: file open fail" << std::endl;
    return false;
    //exit(-1);
  }
  
  int cid,seg,lr,tb;
  double p0, p1;
  
  while(fgets(str,MaxChar,fp)!=0){
    if(str[0]=='#') continue;
    else if(sscanf(str,"%d %d %d %d %lf %lf",&cid,&seg,&lr,&tb,
		   &p0,&p1)==6){

      if(cid==F1S2.cid){
        F1S2.tdcOffset[seg+nS2*(lr+2*tb)]=p0;
        F1S2.tdcGain[seg+nS2*tb]=p1;}
      else if(cid==F1S0.cid){
        F1S0.tdcOffset[seg+nS0*(lr+2*tb)]=p0;
        F1S0.tdcGain[seg+nS0*tb]=p1;}
      else if(cid==RF.cid){
        RF.tdcOffset[seg+nRF*lr]=p0;
        RF.tdcGain[seg+nRF*lr]=p1;}
      else if(cid==FbS2.cid){
        FbS2.tdcOffset[seg+nS2*(lr+2*tb)]=p0;
        FbS2.tdcGain[seg+nS2*tb]=p1;}
      else if(cid==FbS0.cid){
        FbS0.tdcOffset[seg+nS0*(lr+2*tb)]=p0;
        FbS0.tdcGain[seg+nS0*tb]=p1;}

     else{
 	    std::cerr << "[" << funcname << "]: new fail (A) "
 		      << " Cid=" << std::setw(2) << cid
 		      << " lr="  << std::setw(2) << lr
		      << " tb=" << std::setw(2) << tb << std::endl;
     }
	
    }   /* if(sscanf...) */
  }       /* while(fgets...) */
  fclose(fp);
  
//   std::cout << "[" << funcname << "]: Initialization finished" << std::endl;
  return true;
}

///////////////////////////////////
double ParamMan::time( int cid, int seg, int lr, int tb, double tdc )
{
  static const std::string funcname = "ParamMan::time";

  if(cid==F1S2.cid )
    return F1S2.tdcGain[seg+nS2*(lr+2*tb)]*(tdc-F1S2.tdcOffset[seg+nS2*(lr+2*tb)]);
  else if(cid==F1S0.cid )
    return F1S0.tdcGain[seg+nS0*(lr+2*tb)]*(tdc-F1S0.tdcOffset[seg+nS0*(lr+2*tb)]);
  else if(cid==RF.cid )
    return RF.tdcGain[seg+nRF*tb]       *(tdc-RF.tdcOffset[seg+nRF*lr]);
  else if(cid==FbS2.cid )
    return FbS2.tdcGain[seg+nS2*(lr+2*tb)]*(tdc-FbS2.tdcOffset[seg+nS2*(lr+2*tb)]);
  else if(cid==FbS0.cid )
    return FbS0.tdcGain[seg+nS0*(lr+2*tb)]*(tdc-FbS0.tdcOffset[seg+nS0*(lr+2*tb)]);
  else   cerr << "[" << funcname << "]: unknown id" << endl;

  return -1.;
}

///////////////////////////////////
void ParamMan::SetTdcOffset( int cid, int seg, int lr, int tb,
				 double tdcOffset )
{
  static const std::string funcname = "ParamMan::SettdcOffset";
  
  if(cid==F1S2.cid)         F1S2.tdcOffset[seg+nS2*(lr+2*tb)]=tdcOffset;
  else if(cid==F1S0.cid)    F1S0.tdcOffset[seg+nS0*(lr+2*tb)]=tdcOffset;
  else if(cid==RF.cid)      RF.tdcOffset[seg+nRF*lr]       =tdcOffset;
  else if(cid==FbS2.cid)    FbS2.tdcOffset[seg+nS2*(lr+2*tb)]=tdcOffset;
  else if(cid==FbS0.cid)    FbS0.tdcOffset[seg+nS0*(lr+2*tb)]=tdcOffset;
  else   cerr << "[" << funcname << "]: unknown id" << endl;
}
///////////////////////////////////
void ParamMan::SetTdcGain( int cid, int seg, int lr, int tb,
			       double tdcGain )
{
  static const std::string funcname = "ParamMan::SettdcGain";
  
  if(cid==F1S2.cid)          F1S2.tdcGain[seg+nS2*(lr+2*tb)]=tdcGain;
  else if(cid==F1S0.cid)     F1S0.tdcGain[seg+nS0*(lr+2*tb)]=tdcGain;
  else if(cid==RF.cid)       RF.tdcGain[seg+nRF*lr]       =tdcGain;
  else if(cid==FbS2.cid)     FbS2.tdcGain[seg+nS2*(lr+2*tb)]=tdcGain;
  else if(cid==FbS0.cid)     FbS0.tdcGain[seg+nS0*(lr+2*tb)]=tdcGain;
  else   cerr << "[" << funcname << "]: unknown id" << endl;
}
///////////////////////////////////
void ParamMan::SetTimeTune( int cid, int seg, int lr, int tb,
				double time )
{
  static const std::string funcname = "ParamMan::SetNpeTune";
  
  if(cid==F1S2.cid )
    F1S2.tdcOffset[seg+nS2*(lr+2*tb)]+=time/F1S2.tdcGain[seg+nS2*(lr+2*tb)];
  else if(cid==F1S0.cid )
    F1S0.tdcOffset[seg+nS0*(lr+2*tb)]+=time/F1S0.tdcGain[seg+nS0*(lr+2*tb)];
  else if(cid==RF.cid )
    RF.tdcOffset[seg+nRF*lr]+=time/RF.tdcGain[seg+nRF*lr];
  else if(cid==FbS2.cid )
    FbS2.tdcOffset[seg+nS2*(lr+2*tb)]+=time/FbS2.tdcGain[seg+nS2*(lr+2*tb)];
  else if(cid==FbS0.cid )
    FbS0.tdcOffset[seg+nS0*(lr+2*tb)]+=time/FbS0.tdcGain[seg+nS0*(lr+2*tb)];
  else   cerr << "[" << funcname << "]: unknown id" << endl;
}

///////////////////////////////////
double ParamMan::GetTdcOffset( int cid, int seg, int lr, int tb )
{
  static const std::string funcname = "ParamMan::GetTdcOffset";

  if(cid==F1S2.cid )          return F1S2.tdcOffset[seg+nS2*(lr+2*tb)];
  else if(cid==F1S0.cid )     return F1S0.tdcOffset[seg+nS0*(lr+2*tb)];
  else if(cid==RF.cid )       return RF.tdcOffset[seg+nRF*lr];
  else if(cid==FbS2.cid )     return FbS2.tdcOffset[seg+nS2*(lr+2*tb)];
  else if(cid==FbS0.cid )     return FbS0.tdcOffset[seg+nS0*(lr+2*tb)];
  else   cerr << "[" << funcname << "]: unknown id" << endl;

  return -1.;
}

///////////////////////////////////
double ParamMan::GetTdcGain( int cid, int seg, int lr, int tb )
{
  static const std::string funcname = "ParamMan::GetTdcGain";

  if(cid==F1S2.cid )          return F1S2.tdcGain[seg+nS2*(lr+2*tb)];
  else if(cid==F1S0.cid )     return F1S0.tdcGain[seg+nS0*(lr+2*tb)];
  else if(cid==RF.cid )       return RF.tdcGain[seg+nRF*lr];
  else if(cid==FbS2.cid )     return FbS2.tdcGain[seg+nS2*(lr+2*tb)];
  else if(cid==FbS0.cid )     return FbS0.tdcGain[seg+nS0*(lr+2*tb)];
  else   cerr << "[" << funcname << "]: unknown id" << endl;

  return -1.;
}


///////////////////////////////////
void ParamMan::WriteToFile(const char* OutputFileName)   //wrinting param file
{
  ofstream fout;
  if( fout.is_open() ) fout.close();
  fout.open(OutputFileName, ios::out|ios::trunc);
  fout.setf(ios_base::fixed);
  //fout.open(name.str().c_str(), std::ios::out|std::ios::trunc);
  //fout.setf(std::ios_base::fixed);
  fout << "#" << endl
       << "#  "  << OutputFileName << endl
       << "#" << endl;
  fout << "#F1 TDC#" << endl;
  for(int lr=0; lr<2; lr++){//lr
    if(lr==0)fout << "# Left HRS"<< endl;
    if(lr==1)fout << "# Right HRS"<< endl;
    fout << "# CID SEG LR  TB      Offs        Conv. factor[ns/ch]" << endl;
    fout << "# F1S2"<< endl;
    for(int tb=0; tb<2; tb++){//tb
      for(int i=0; i<nS2;i++)
        fout << std::setw(4) << CID_F1S2
             << std::setw(4) << i
             << std::setw(4) << lr
             << std::setw(4) << tb
             << std::setw(13) << std::setprecision(6)
             << F1S2.tdcOffset[i+nS2*(lr+2*tb)]
             << std::setw(11) << std::setprecision(6)
             << F1S2.tdcGain[i+nS2*(lr+2*tb)] << endl;
    }

    fout << "# F1S0"<< endl;
    for(int tb=0; tb<2; tb++){//tb
      for(int i=0; i<nS0;i++)
        fout << std::setw(4) << CID_F1S0
             << std::setw(4) << i
             << std::setw(4) << lr
             << std::setw(4) << tb
             << std::setw(13) << std::setprecision(6)
             << F1S0.tdcOffset[i+nS0*(lr+2*tb)]
             << std::setw(11) << std::setprecision(6)
             << F1S0.tdcGain[i+nS0*(lr+2*tb)] << endl;
    }

    fout << "#Fb TDC#" << endl;
    fout << "# CID SEG LR  TB      Offs        Conv. factor[ns/ch]" << endl;
    fout << "# FbS2"<< endl;
    for(int tb=0; tb<2; tb++){//tb
      for(int i=0; i<nS2;i++)
        fout << std::setw(4) << CID_FbS2
             << std::setw(4) << i
             << std::setw(4) << lr
             << std::setw(4) << tb
             << std::setw(13) << std::setprecision(6)
             << FbS2.tdcOffset[i+nS2*(lr+2*tb)]
             << std::setw(11) << std::setprecision(6)
             << FbS2.tdcGain[i+nS2*(lr+2*tb)] << endl;
    }

    fout << "# FbS0"<< endl;
    for(int tb=0; tb<2; tb++){//tb
      for(int i=0; i<nS0;i++)
        fout << std::setw(4) << CID_FbS0
             << std::setw(4) << i
             << std::setw(4) << lr
             << std::setw(4) << tb
             << std::setw(13) << std::setprecision(6)
             << FbS0.tdcOffset[i+nS0*(lr+2*tb)]
             << std::setw(11) << std::setprecision(6)
             << FbS0.tdcGain[i+nS0*(lr+2*tb)] << endl;
    }
  fout << "# RF"<< endl;
    for(int i=0; i<nRF;i++)
        fout << std::setw(4) << CID_RF
             << std::setw(4) << i
             << std::setw(4) << lr
             << std::setw(4) << 0
             << std::setw(13) << std::setprecision(6)
             << RF.tdcOffset[i+nRF*lr]
             << std::setw(11) << std::setprecision(6)
             << RF.tdcGain[i+nRF*lr] << endl;
  }
  if(fout.is_open()) fout.close();
  cout << OutputFileName << " was written"<<endl;

  cout<<"R.s2.R.off = "<<flush;
  for(int i=0; i<nS2;i++){
    cout<<FbS2.tdcOffset[i+nS2]<<" "<<flush;
  }
  cout<<endl;

  cout<<"R.s2.L.off = "<<flush;
  for(int i=0; i<nS2;i++){
    cout<<FbS2.tdcOffset[i+3*nS2]<<" "<<flush;
  }
  cout<<endl;
}

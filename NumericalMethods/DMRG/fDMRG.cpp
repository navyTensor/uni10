#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>
#include <map>
using namespace std;
#include "uni10.hpp"
using namespace uni10;
#include "DMRG_tools.cpp"
#include "hamiltonian.cpp"

int main(){
	/*** Initialization ***/
  const int chi = 30;
  const int N = 50;
	UniTensor H0 = theModel(1, 0, 0, 1, 0.1, 0);

	vector<Bond> bond2;
	bond2.push_back(H0.bond(0));
	bond2.push_back(H0.bond(2));

	UniTensor Id(bond2, "Id");
	Id.identity();

  vector<UniTensor> HLs;
  vector<UniTensor> HRs;
	HLs.push_back(H0);
	HRs.push_back(H0);
  vector<UniTensor> As;
  vector<UniTensor> Bs;
  As.push_back(Id);
  Bs.push_back(Id);

	Network HLn("HL.net");
	Network HRn("HR.net");

  Matrix psi;
	/*** END initilization ***/

  double Ep = 0, E0;
	for(int l = 1; l < N; l++){
		UniTensor SB = combineH(H0, HLs[l - 1], HRs[l - 1]);

    int iter;
    UniTensor GS = findGS(SB, E0, psi, iter);

    UniTensor A, B;
    int D = updateMPS(GS, chi, A, B);
    As.push_back(A);
    Bs.push_back(B);

    cout<<"N = "<< 2 * (l+1) <<", D = " << D << setprecision(10) << ", E = " << E0  << ", e = " << E0 / (2 * (l + 1)) <<", iter = "<<iter<<", dE = "<<(E0 - Ep)/2<<endl;
    UniTensor newHL, newHR;
    updateH(HLs[l - 1], HRs[l - 1], A, B, H0, H0, HLn, HRn, newHL, newHR);
    HLs.push_back(newHL);
    HRs.push_back(newHR);
    Ep = E0;
	}
  sweep(N, chi, N-1, 1, H0, HLs, HRs, HLn, HRn);
}
/*$TET$pipeline$!cpp-copyright!*/
/*--------------------------------------------------------------------------*/
/*  Copyright 2010-2015 Sergey Vostokin                                     */
/*                                                                          */
/*  Licensed under the Apache License, Version 2.0 (the "License");         */
/*  you may not use this file except in compliance with the License.        */
/*  You may obtain a copy of the License at                                 */
/*                                                                          */
/*  http://www.apache.org/licenses/LICENSE-2.0                              */
/*                                                                          */
/*  Unless required by applicable law or agreed to in writing, software     */
/*  distributed under the License is distributed on an "AS IS" BASIS,       */
/*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*/
/*  See the License for the specific language governing permissions and     */
/*  limitations under the License.                                          */
/*--------------------------------------------------------------------------*/
/*$TET$*/

/*$TET$pipeline$!templet!*/
/*
~I= 
	+B ? call->O;
	 O ! ret ->B.

*L = 
	r:I ? call -> op;
	op(r?call,r!ret).

*R =
	l:I ! ret -> op;
	+init(l!call);
	op(l?ret,l!call).

*C = 
	l:I ! ret  -> op;
	r:I ? call -> op;
	
	+init(l!call);
	op(l?ret,r?call,l!call,r!ret).
*/
/*$TET$*/

#include "pipeline.h"

/*$TET$pipeline$!cpp-prologue!*/
extern void oper(double**m, int t, int s);
extern int ts[];
extern const int T;
extern double** mN;

#undef max
/*$TET$*/
//////////////////////class I////////////////////
I::I(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Channel(a)
{
/*$TET$I$!constructor!*/
// place your code here
/*$TET$*/
}

I::~I()
{
/*$TET$I$!destructor!*/
// place your code here
/*$TET$*/
}

//////////////////////class L////////////////////
L::L(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Process(a)
{
/*$TET$L$!constructor!*/
// place your code here
/*$TET$*/
}

L::~L()
{
/*$TET$L$!destructor!*/
// place your code here
/*$TET$*/
}

/*$TET$L$!usercode!*/
// place your code here
/*$TET$*/

bool L::op(/*in*/I::call*p1,/*out*/I::ret*p2)
{
/*$TET$L$op*/
	if (ts[pid] < T){
		oper(mN, ts[pid], pid); ts[pid]++;
		return true;
	}
	return false;
/*$TET$*/
}

void L::_run(int _selector,TEMPLET_DBG::Channel*_channel)
{
	bool res;
/*$TET$L$!run!*/
// place run method interception code here
/*$TET$*/

}

//////////////////////class R////////////////////
R::R(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Process(a)
{
/*$TET$R$!constructor!*/
// place your code here
/*$TET$*/
}

R::~R()
{
/*$TET$R$!destructor!*/
// place your code here
/*$TET$*/
}

/*$TET$R$!usercode!*/
// place your code here
/*$TET$*/

bool R::init(/*out*/I::call*p1)
{
/*$TET$R$init*/
// place your code here
	return true;
/*$TET$*/
}

bool R::op(/*in*/I::ret*p1,/*out*/I::call*p2)
{
/*$TET$R$op*/
	if (ts[pid] < T){
		oper(mN, ts[pid], pid); ts[pid]++;
		return true;
	}
	return false;
/*$TET$*/
}

void R::_run(int _selector,TEMPLET_DBG::Channel*_channel)
{
	bool res;
/*$TET$R$!run!*/
// place run method interception code here
/*$TET$*/

}

//////////////////////class C////////////////////
C::C(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Process(a)
{
/*$TET$C$!constructor!*/
// place your code here
/*$TET$*/
}

C::~C()
{
/*$TET$C$!destructor!*/
// place your code here
/*$TET$*/
}

/*$TET$C$!usercode!*/
// place your code here
/*$TET$*/

bool C::init(/*out*/I::call*p1)
{
/*$TET$C$init*/
// place your code here
	return true;
/*$TET$*/
}

bool C::op(/*in*/I::ret*p1,/*in*/I::call*p2,/*out*/I::call*p3,/*out*/I::ret*p4)
{
/*$TET$C$op*/
	if (ts[pid] < T){
		oper(mN, ts[pid], pid); ts[pid]++;
		return true;
	}
	return false;
/*$TET$*/
}

void C::_run(int _selector,TEMPLET_DBG::Channel*_channel)
{
	bool res;
/*$TET$C$!run!*/
// place run method interception code here
/*$TET$*/

}

//////////////////////class pipeline////////////////////
pipeline::pipeline(int NT): TEMPLET_DBG::Assemble(NT)
{
/*$TET$pipeline$!constructor!*/
// place your code here
/*$TET$*/
}

pipeline::~pipeline()
{
/*$TET$pipeline$!destructor!*/
// place your code here
/*$TET$*/
}

/*$TET$pipeline$!cpp-epilogue!*/
const int N = 3;
const int M = 3;
const int T = 3;
const int P = 3;

double** m1;
double** mN;

double** create()
{
	double delta = 100.0 / (M + 1);
	double** m;

	m = new double*[N + 2];

	m[0] = new double[M + 2];
	m[0][0] = 100.0;
	for (int j = 1; j < M + 1; j++) m[0][j] = 100.0 - delta*j;
	m[0][M + 1] = 0.0;

	for (int i = 1; i < N + 1; i++){
		m[i] = new double[M + 2];
		m[i][0] = 100.0;
		for (int j = 1; j < M + 1; j++) m[i][j] = 0.0;
		m[i][M + 1] = 0.0;
	}

	m[N + 1] = new double[M + 2];
	m[N + 1][0] = 100.0;
	for (int j = 1; j < M + 1; j++) m[N + 1][j] = 100.0 - delta*j;
	m[N + 1][M + 1] = 0.0;

	return m;
}

void print(double**m)
{
	for (int i = 0; i < N + 2; i++){
		for (int j = 0; j < M + 2; j++)
			std::cout << m[i][j] << " ";
		std::cout << '\n';
	}
	std::cout << '\n';
}

double eps(double**m1, double**m2)
{
	double eps = 0.0;
	for (int i = 0; i < N + 2; i++)
		for (int j = 0; j < M + 2; j++)
			eps = std::max(eps, abs(m1[i][j] - m2[i][j]));
	return eps;
}

void oper(double**m, int t, int s)
{
	for (int j = 1; j < M + 1; j++)
		m[s + 1][j] = 0.25*(m[s + 1][j - 1] + m[s + 1][j + 1] + m[s][j] + m[s + 2][j]);
}

void seq_alg()
{
	for (int t = 0; t < T; t++)
		for (int s = 0; s < N; s++)
			oper(m1, t, s);
}

pipeline e(P);
L* l;
C* c[N-2];
R* r;
int  ts[N];

void paral_alg()
{
	for (int i = 0; i < N; i++) ts[i] = 0;

	l = e.new_L(); l->pid = 0;
	for (int i = 0; i < N - 2; i++){ c[i] = e.new_C(); c[i]->pid = i + 1; }
	r = e.new_R(); r->pid = N - 1;
	
	l->p_r(c[0]->p_l());
	for (int i = 0; i < N - 3; i++) c[i]->p_r(c[i+1]->p_l());
	c[N - 3]->p_r(r->p_l());

	e.run();
}

void main()
{
	m1 = create();
	mN = create();

	seq_alg();
	print(m1);

	paral_alg();
	print(mN);

	std::cout << "eps=" << eps(m1, mN) << "\n\n";
}
/*$TET$*/


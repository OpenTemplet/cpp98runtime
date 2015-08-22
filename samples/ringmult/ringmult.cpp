/*$TET$ringmult$!cpp-copyright!*/
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

/*$TET$ringmult$!templet!*/
/*
~Link=
	+BEG ? col -> PROC;
	PROC ! ready ->BEG.
  
*Proc=
	l : Link ? col -> proc;
	r : Link ! ready -> send;
	+beg(r!col)->proc;
	send(r?ready,r!col)->proc;
	proc(l?col,l!ready)->send.
  
*/
/*$TET$*/

#include "ringmult.h"

/*$TET$ringmult$!cpp-prologue!*/
/*$TET$*/
//////////////////////class Link////////////////////
Link::Link(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Channel(a)
{
/*$TET$Link$!constructor!*/
/*$TET$*/
}

Link::~Link()
{
/*$TET$Link$!destructor!*/
/*$TET$*/
}

//////////////////////class Proc////////////////////
Proc::Proc(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Process(a)
{
/*$TET$Proc$!constructor!*/
/*$TET$*/
}

Proc::~Proc()
{
/*$TET$Proc$!destructor!*/
/*$TET$*/
}

/*$TET$Proc$!usercode!*/
/*$TET$*/

bool Proc::beg(/*out*/Link::col*p1)
{
/*$TET$Proc$beg*/
	for(int i=0;i<N;i++) p1->b[i]=b[i];
	p1->c=col;
	empty=true;
	return true;
/*$TET$*/
}

bool Proc::send(/*in*/Link::ready*p1,/*out*/Link::col*p2)
{
/*$TET$Proc$send*/
	if(empty)return false;
	for(int i=0;i<N;i++) p2->b[i]=b[i];
	p2->c=col;
	empty=true;
	return true;
/*$TET$*/
}

bool Proc::proc(/*in*/Link::col*p1,/*out*/Link::ready*p2)
{
/*$TET$Proc$proc*/
	if(n==N || !empty)return false;

	for(int i=0;i<N;i++)b[i]=p1->b[i];
	col=p1->c;
	empty=false;

	c[col]=0.0;
	for(int i=0;i<N;i++)c[col]+=a[i]*b[i];

	cout<<"c("<<row<<","<<col<<")="<<c[col]<<" ";
	n++;
	
	return true;
/*$TET$*/
}

void Proc::_run(int _selector,TEMPLET_DBG::Channel*_channel)
{
	bool res;
/*$TET$Proc$!run!*/
/*$TET$*/

}

//////////////////////class ringmult////////////////////
ringmult::ringmult(int NT): TEMPLET_DBG::Assemble(NT)
{
/*$TET$ringmult$!constructor!*/
/*$TET$*/
}

ringmult::~ringmult()
{
/*$TET$ringmult$!destructor!*/
/*$TET$*/
}

/*$TET$ringmult$!cpp-epilogue!*/
int main(int,char**)
{
	// последовательное умножение матриц

	double a[N][N],b[N][N],c[N][N];

	cout<<"A=\n";
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			a[i][j]=N*i+j;
			cout<<a[i][j]<<" ";
		}
		cout<<"\n";
	}

	cout<<"\nB=\n";
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			b[i][j]=N*i+j;
			cout<<b[i][j]<<" ";
		}
		cout<<"\n";
	}
	
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			c[i][j]=0.0;
			for(int k=0;k<N;k++)c[i][j]+=a[i][k]*b[k][j];
		}
	}

	cout<<"\nC=\n";
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			cout<<c[i][j]<<" ";
		}
		cout<<'\n';
	}

	cout<<"--------------------------------\n";

	// параллельное умножение матриц на кольце процессов
	ringmult t(N); // управл€ющий объект
	Proc* proc[N]; // процессы

	for(int i=0;i<N;i++){
		// строим и инициализируем процессы
		proc[i]=t.new_Proc();

		proc[i]->row=i;
		proc[i]->col=i;
		proc[i]->n=0;
		proc[i]->empty=false;
	}

	// соедин€ем процессы в кольцо
	for(int i=0;i<N-1;i++) proc[i+1]->p_l(proc[i]->p_r());
	proc[0]->p_l(proc[N-1]->p_r());

	// инициализируем матрицы
	cout<<"A=\n";
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			proc[i]->a[j]=N*i+j;
			cout<<proc[i]->a[j]<<" ";
		}
		cout<<"\n";
	}
	
	cout<<"\nB=\n";
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			proc[j]->b[i]=N*i+j;
			cout<<proc[j]->b[i]<<" ";
		}
		cout<<"\n";
	}

	// запускаем вычислени€
	t.run();

	// выводим результат
	cout<<"\nC=\n";
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			cout<<proc[i]->c[j]<<" ";
		}
		cout<<'\n';
	}

	return 0;
}
/*$TET$*/


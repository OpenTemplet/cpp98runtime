/*$TET$ringmult$!h-copyright!*/
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

#ifndef _TEMPLET_MODULE_ringmult
#define _TEMPLET_MODULE_ringmult

#include <string.h>
#include <assert.h>

#include "dbg/tet.h"

/*$TET$ringmult$!h-prologue!*/
#include <iostream>
using namespace std;
const int N=3;
/*$TET$*/

class Link:public TEMPLET_DBG::Channel{
public:
	Link(TEMPLET_DBG::Assemble*a);
	~Link();
public:
	struct col{//<BEG> 
/*$TET$Link$col*/
		double b[N];
		int c;
/*$TET$*/
	};
	struct ready{//<PROC> 
/*$TET$Link$ready*/
/*$TET$*/
	};
};

class Proc:public TEMPLET_DBG::Process{
public:
	Proc(TEMPLET_DBG::Assemble*a);
	~Proc();
private:
	//methods
	bool beg(/*out*/Link::col*p1);
	bool send(/*in*/Link::ready*p1,/*out*/Link::col*p2);
	bool proc(/*in*/Link::col*p1,/*out*/Link::ready*p2);

/*$TET$Proc$!userdata!*/
public:
	double a[N],b[N],c[N];
	int row,col;
	int n;
	bool empty;
/*$TET$*/

public:
	void p_l(Link*p){}
	Link* p_r(){return 0; }
protected:
	virtual void _run(int _selector,TEMPLET_DBG::Channel*_channel);
};

class ringmult:public TEMPLET_DBG::Assemble{
public:
	ringmult(int NT);
	~ringmult();

/*$TET$ringmult$!userdata!*/
/*$TET$*/
public:
	Proc*new_Proc(){return 0;}
};

/*$TET$ringmult$!h-epilogue!*/
/*$TET$*/
#endif

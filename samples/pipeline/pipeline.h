/*$TET$pipeline$!h-copyright!*/
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

#ifndef _TEMPLET_MODULE_pipeline
#define _TEMPLET_MODULE_pipeline

#include <string.h>
#include <assert.h>

#include "dbg/tet.h"

/*$TET$pipeline$!h-prologue!*/
#include <iostream>
#include <algorithm>
/*$TET$*/

class I:public TEMPLET_DBG::Channel{
public:
	I(TEMPLET_DBG::Assemble*a);
	~I();
public:
	struct call{//<B> 
/*$TET$I$call*/
// place your code here
/*$TET$*/
	};
	struct ret{//<O> 
/*$TET$I$ret*/
// place your code here
/*$TET$*/
	};
};

class L:public TEMPLET_DBG::Process{
public:
	L(TEMPLET_DBG::Assemble*a);
	~L();
private:
	//methods
	bool op(/*in*/I::call*p1,/*out*/I::ret*p2);

/*$TET$L$!userdata!*/
public:
	int pid;
/*$TET$*/

public:
	void p_r(I*p){}
protected:
	virtual void _run(int _selector,TEMPLET_DBG::Channel*_channel);
};

class R:public TEMPLET_DBG::Process{
public:
	R(TEMPLET_DBG::Assemble*a);
	~R();
private:
	//methods
	bool init(/*out*/I::call*p1);
	bool op(/*in*/I::ret*p1,/*out*/I::call*p2);

/*$TET$R$!userdata!*/
public:
	int pid;
/*$TET$*/

public:
	I* p_l(){return 0; }
protected:
	virtual void _run(int _selector,TEMPLET_DBG::Channel*_channel);
};

class C:public TEMPLET_DBG::Process{
public:
	C(TEMPLET_DBG::Assemble*a);
	~C();
private:
	//methods
	bool init(/*out*/I::call*p1);
	bool op(/*in*/I::ret*p1,/*in*/I::call*p2,/*out*/I::call*p3,/*out*/I::ret*p4);

/*$TET$C$!userdata!*/
public:
	int pid;
/*$TET$*/

public:
	I* p_l(){return 0; }
	void p_r(I*p){}
protected:
	virtual void _run(int _selector,TEMPLET_DBG::Channel*_channel);
};

class pipeline:public TEMPLET_DBG::Assemble{
public:
	pipeline(int NT);
	~pipeline();

/*$TET$pipeline$!userdata!*/
// place your code here
/*$TET$*/
public:
	L*new_L(){return 0;}
	R*new_R(){return 0;}
	C*new_C(){return 0;}
};

/*$TET$pipeline$!h-epilogue!*/
/*$TET$*/
#endif

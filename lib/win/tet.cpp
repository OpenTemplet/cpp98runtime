/*--------------------------------------------------------------------------*/
/*  Copyright 2010-2013 Sergey Vostokin                                     */
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

#include "tet.h"
#include <assert.h>

namespace TEMPLET_WIN {

bool Channel::_run()
{
	Process*p;
	int _selector;

	assert(_active==RTL_CLI||_active==RTL_SRV);

	if(_active==RTL_CLI){p=_cliPort;_selector=_cli_selector;}
	if(_active==RTL_SRV){p=_srvPort;_selector=_srv_selector;}
	
	EnterCriticalSection(&(p->_csp));

	if(_active==RTL_CLI)_active=CLI;
	if(_active==RTL_SRV)_active=SRV;
	
	p->_run(_selector,this);
	
	LeaveCriticalSection(&(p->_csp));

	return true;
}

void Channel::resend()
{
	assert(_active==CLI||_active==SRV);
	_active=(_active==CLI)?RTL_CLI:RTL_SRV;
	_assemble->_send(this);
}
Assemble::~Assemble()
{
	assert(!_running);
	for(int i=0;i<_num_threads;i++)CloseHandle(_threads[i]);
	free(_threads);
	DeleteCriticalSection(&_cs);
	CloseHandle(_que_not_empty);

	std::vector<Channel*>::iterator itc;
	for(itc=channels.begin();itc!=channels.end();itc++)delete(*itc);
	std::vector<Process*>::iterator itp;
	for(itp=processes.begin();itp!=processes.end();itp++)delete(*itp);
}

void Assemble::_send(Channel*c)
{
	EnterCriticalSection(&_cs);
	ready.push(c);if(ready.size()==1)SetEvent(_que_not_empty);
	LeaveCriticalSection(&_cs);
}

void Assemble::_run()
{
	Channel*c;

	while(_goon){
		
		EnterCriticalSection(&_cs);
		while(ready.size()==0){
			_waitcount++;
	
			if(_waitcount==_num_threads){
				_retstatus=true;_goon=false;
				SetEvent(_que_not_empty);
				LeaveCriticalSection(&_cs);
				return;
			}
			
			LeaveCriticalSection(&_cs);
			WaitForSingleObject(_que_not_empty,INFINITE); 
			EnterCriticalSection(&_cs);
			
			_waitcount--;
		}
		c=ready.front();
		ready.pop();
		LeaveCriticalSection(&_cs);

		if(!_goon)return;
		if(!c->_run()){_goon=false;_retstatus=false;SetEvent(_que_not_empty);}
	}
}

Assemble::Assemble(int nth)
{
	_running=false;
	_num_threads=nth;
	_threads=(HANDLE*)calloc(_num_threads,sizeof(HANDLE));
	InitializeCriticalSection(&_cs);
	_que_not_empty=CreateEvent(NULL,FALSE,FALSE,NULL);
}

DWORD WINAPI _adapter(LPVOID p)
{
	Assemble*a=(Assemble*)p;
	a->_run();
	return 0;
}

bool Assemble::run()
{
	int i;DWORD id;

	assert(!_running);
	
	_running=true;
	_goon=true;
	_waitcount=0;

	for(i=0;i<_num_threads;i++){
		_threads[i]=CreateThread(NULL,0,_adapter,this,0,&id);
		assert(_threads[i]!=NULL);
	}

	DWORD r=WaitForMultipleObjects(_num_threads,_threads,TRUE,INFINITE);
	assert(WAIT_OBJECT_0<=r && r<=(WAIT_OBJECT_0+_num_threads-1));

	_running=false;
	return _retstatus;
}
}

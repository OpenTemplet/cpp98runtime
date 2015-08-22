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
#include <cstdlib>

namespace TEMPLET_NIX {

bool Channel::_run()
{
	Process*p;
	int _selector;

	assert(_active==RTL_CLI||_active==RTL_SRV);
	
	if(_active==RTL_CLI){p=_cliPort;_selector=_cli_selector;}
	if(_active==RTL_SRV){p=_srvPort;_selector=_srv_selector;}
	
	pthread_mutex_lock(&(p->_csp));

	if(_active==RTL_CLI)_active=CLI;
	if(_active==RTL_SRV)_active=SRV;
	
	p->_run(_selector,this);
	
	pthread_mutex_unlock(&(p->_csp));
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
	free(_threads);
	pthread_mutex_destroy(&_cs);
	pthread_cond_destroy(&_que_not_empty);

	std::vector<Channel*>::iterator itc;
	for(itc=channels.begin();itc!=channels.end();itc++)delete(*itc);
	std::vector<Process*>::iterator itp;
	for(itp=processes.begin();itp!=processes.end();itp++)delete(*itp);
}

void Assemble::_send(Channel*c)
{
	pthread_mutex_lock(&_cs);
	ready.push(c);if(ready.size()==1)pthread_cond_signal(&_que_not_empty);
	pthread_mutex_unlock(&_cs);
}

void Assemble::_run(){
	Channel*c;

	while(_goon){
		
		pthread_mutex_lock(&_cs);
		while(ready.size()==0){
			_waitcount++;
	
			if(_waitcount==_num_threads){
				_retstatus=true;_goon=false;
				pthread_cond_signal(&_que_not_empty);
				pthread_mutex_unlock(&_cs);
				return;
			}

			pthread_cond_wait(&_que_not_empty, &_cs);

			_waitcount--;
		}
		c=ready.front();
		ready.pop();
		pthread_mutex_unlock(&_cs);

		if(!_goon)return;
		if(!c->_run()){_goon=false;_retstatus=false;pthread_cond_signal(&_que_not_empty);}
	}
}

Assemble::Assemble(int nth)
{
	_running=false;
	_num_threads=nth;
	_threads=(pthread_t*)calloc(_num_threads,sizeof(pthread_t));
	pthread_mutex_init(&_cs,NULL);
	pthread_cond_init (&_que_not_empty,NULL);
}

void* _adapter(void* p)
{
	Assemble*a=(Assemble*)p;
	a->_run();
	return 0;
}

bool Assemble::run()
{
	int i;
	assert(!_running);
	_running=true;
	_goon=true;
	_waitcount=0;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(i=0;i<_num_threads;i++){
		int res=pthread_create(&_threads[i],&attr,_adapter,this);
		assert(res==0);
	}

	for(i=0;i<_num_threads;i++)
		pthread_join(_threads[i], NULL);

	pthread_attr_destroy(&attr);

	_running=false;
	return _retstatus;
}
}

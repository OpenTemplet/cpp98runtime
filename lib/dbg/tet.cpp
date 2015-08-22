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
#include <cstddef>
#include <cstdlib>

namespace TEMPLET_DBG {

bool Channel::_run()
{
	Process*p;
	int _selector;
	assert(_active==RTL_CLI||_active==RTL_SRV);
	if(_active==RTL_CLI){p=_cliPort;_active=CLI;_selector=_cli_selector;}
	if(_active==RTL_SRV){p=_srvPort;_active=SRV;_selector=_srv_selector;}
	p->_run(_selector,this);
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
	std::vector<Channel*>::iterator itc;
	for(itc=channels.begin();itc!=channels.end();itc++)delete(*itc);
	std::vector<Process*>::iterator itp;
	for(itp=processes.begin();itp!=processes.end();itp++)delete(*itp);
}

bool Assemble::run(){
	size_t rsize;
	std::vector<Channel*>::iterator it;

	while(rsize=ready.size()){
		std::vector<Channel*>::difference_type n=rand()%rsize;
		it=ready.begin()+n;
		Channel*c=*it;
		ready.erase(it);
		if(!c->_run())return false;
	}
	return true;
}

}
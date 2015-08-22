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

#ifndef _TEMPLET_DBG
#define _TEMPLET_DBG

#include <queue>
#include <vector>
#include <windows.h>

namespace TEMPLET_WIN {

class Assemble;
class Channel;
class Process;
class Activator;

class Assemble{
	friend class Channel; 
	friend class Activator;
	friend DWORD WINAPI _adapter(LPVOID p);
protected:
	virtual ~Assemble();
	Assemble(int NT);
public:	
	bool run();
private:	
	void _run();
public:
	void _regChan(Channel*chan){channels.push_back(chan);}
	void _regProc(Process*prc){processes.push_back(prc);}
private:
	void _send(Channel*c);
private:	
	std::vector<Channel*> channels;
	std::vector<Process*> processes;
	std::queue<Channel*> ready;
private:
	CRITICAL_SECTION _cs;
	HANDLE _que_not_empty;
	volatile bool _goon;
	bool _running;
	int _num_threads;
	HANDLE* _threads;
	volatile bool _retstatus;
	volatile unsigned _waitcount;
};

class Channel{
	friend class Assemble;
	friend class Process;
	friend class Activator;
protected:
	Channel(Assemble*a):_assemble(a),_active(CLI),
		_cliPort(0),_srvPort(0),_cli_selector(0),_srv_selector(0){}
	virtual ~Channel(){}
public:
	enum ChanActivity{CLI,SRV,RTL_CLI,RTL_SRV};
protected:
	void _send(){_assemble->_send(this);}
	ChanActivity _active;
public:
	void resend();
private:
	bool _run();
public:
	Process* _cliPort;
	Process* _srvPort;
	int _cli_selector;
	int _srv_selector;
private:
	Assemble* _assemble;
};

class Activator:public Channel{
public:
	Activator(Assemble*a,Process*p):Channel(a){_cliPort=p;}
	void _send(int port){_cli_selector=port;_active=RTL_CLI;_assemble->_send(this);}
};

class Process{
	friend class Channel;
	friend class Assemble;
protected:
	void lock(){EnterCriticalSection(&_csp);}
	void unlock(){LeaveCriticalSection(&_csp);}
protected:
	Process(Assemble*a):_assemble(a){InitializeCriticalSection(&_csp);}
	virtual ~Process(){DeleteCriticalSection(&_csp);}
protected:
	Activator* _createActivator(){return new Activator(_assemble,this);}
private:
	virtual void _run(int _selector,Channel*)=0;
public:
	Assemble* _assemble;
private:
	CRITICAL_SECTION _csp;
};

}
#endif

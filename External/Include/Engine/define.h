#pragma once

#define SINGLETON(type)\
				private:\
					type();\
					type(const type& _origin) = delete;\
					~type();\
				public:\
					static type* GetInst()\
					{\
						static type Inst;\
						return &Inst;\
					}\
					type* operator = (const type& _other) = delete;
#pragma once
#include "LayerStructure.h"

//로컬 MAC주소를 받아오기 위해 추가함
#include <IPHlpApi.h>
#pragma comment(lib, "iphlpapi.lib")

class DatalinkLayer : public LayerStructure
{
public:
	DatalinkLayer(char* pName);
	~DatalinkLayer();

	BOOL			Receive(unsigned char* ppayload);
	BOOL			Send(unsigned char* ppayload, int packetLength);//이더넷 레이어로 내려와서, 이더넷 레이어가 자신의 프레임의 Data에 넣어야 하는건 packet이다. 그래서 packetSize다
																	//엄밀히, 네트워크층의 자료구조를 부르는 이름은 '패킷'이 아니라 '데이터그램'이다. 패킷은 IP프로토콜이 사용하는 자료구조를 말한다. 그러나 오늘날 네트워크층의 프로토콜은 전부 IP이므로, 그냥 네트워크층의 자료구조를 패킷이라 부른다.
	void			Refresh();

	void			SetDstAddress(unsigned char* pAddress);
	void			SetSrcAddress(unsigned char* pAddress);
	unsigned char*	GetDstAddress();
	unsigned char*	GetSrcAddress();

	typedef struct ETHERNET_FRAME {//데이터링크층의 프로토콜은 '이더넷'을 사용하기로 하고, 이것은 이더넷에서 사용되는 프레임의 자료구조. EF는 "Ethernet Frame"의 약자
		unsigned char	EF_dstaddr[6];						//size : 6 Byte
		unsigned char	EF_srcaddr[6];						//size : 6 Byte
		unsigned short	EF_type;							//size : 2 Byte
		unsigned char	EF_data[ETHERNET_FRAME_DATA_SIZE];	//size : 1500 Byte		==> total : 1514 Byte
	};
	/*
	이보다 복잡한 형태의 이더넷 프레임은, 특정 상황에서 사용되는 프레임인듯 하다. 일단 이 프레임 구조로도 컴퓨터 5대를 연결한 토폴로지에서
	스태틱 라우팅/포워딩이 잘 수행되었기 때문에, 기본적으로 이더넷 프레임의 구조는 이것이라고 단정할 수 있다.
	*/

private:
	void			ResetFrame();
	ETHERNET_FRAME	_frame;//만약 '이더넷'이외의 다른 프로토콜을 사용하기로 한다면, 해당 프로토콜의 자료구조를 구조체로 선언한 뒤, ETHERNET_FRAME을 그것으로 바꿔주면 끝.
	CString			getLocalMACAddress();
	void			setSrcMACAddress();
};


#include "stdafx.h"
#include "PhysicsLayer.h"

/*
	물리 층의 Send는 당연히 프레임을 '시그널'로(데이터 to 시그널) 바꿔서 전깃줄에 걸어주는 역할을 해야 하지만,
	우리는 소프트웨어로만 그것을 구현하는 것이고 심지어 지금은 일단 단일 디바이스 내의 IPC에 대해서 다루기 때문에,
	그냥 파일에 쓰는것을 '송신'이 완료된 것으로 가정한다.

	따라서, '수신'또한 해당 파일을 '읽는'것으로 대신한다. Receive함수는 이를 바탕으로 구현되어 있다.
*/

PhysicsLayer::PhysicsLayer() {}
PhysicsLayer::~PhysicsLayer(){
	TRY{
		CFile::Remove(_T("ipcBuffer.txt"));
	}CATCH(CFileException, e){
		
	}END_CATCH
}

BOOL PhysicsLayer::Receive(){
	TRY{
		CFile bufFile(_T("ipcBuffer.txt"), CFile::modeRead);
		int frameLength = ETHERNET_FRAME_HEADER_SIZE + ETHERNET_FRAME_DATA_SIZE;
		unsigned char* ppayload = new unsigned char[frameLength + 1];

		bufFile.Read(ppayload, frameLength);
		ppayload[frameLength] = '\0';

		if (!this->_upperLayer->Receive(ppayload)) {
			bufFile.Close();
			return FALSE;
		}
	}CATCH(CFileException, e) {
		return FALSE;
	}END_CATCH

	return TRUE;
}
BOOL PhysicsLayer::Send(unsigned char* ppayload, int frameLength){
	TRY{
		CFile bufFile(_T("ipcBuffer.txt"), CFile::modeCreate | CFile::modeWrite);
		bufFile.Write(ppayload, frameLength);
		bufFile.Close();
	}CATCH(CFileException, e) {
		return FALSE;
	}END_CATCH

	return TRUE;
}
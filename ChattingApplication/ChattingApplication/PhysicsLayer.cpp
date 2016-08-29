#include "stdafx.h"
#include "PhysicsLayer.h"



/*
¹°¸®ÃþÀº ÇÊ¿äÇÏÁö ¾Ê´Ù.
*/

PhysicsLayer::PhysicsLayer(char* pName) : LayerStructure(pName) {

}
PhysicsLayer::~PhysicsLayer() {
	TRY{
		CFile::Remove("ipcBuffer.txt");
	}CATCH(CFileException, e) {

	}END_CATCH
}
void PhysicsLayer::Refresh() {}

BOOL PhysicsLayer::Receive() {
	//AfxMessageBox("¹°¸®Ãþ Receive È£ÃâµÊ");
	TRY{
		CFile bufFile("ipcBuffer.txt", CFile::modeRead);
	int frameLength = ETHERNET_FRAME_HEADER_SIZE + ETHERNET_FRAME_DATA_SIZE;
	unsigned char* ppayload = new unsigned char[frameLength + 1];

	bufFile.Read(ppayload, frameLength);
	ppayload[frameLength] = '\0';
	if (!this->_pUpperLayer->Receive(ppayload)) {
		bufFile.Close();
		return FALSE;
	}
	}CATCH(CFileException, e) {
		return FALSE;
	}END_CATCH

		return TRUE;
}
BOOL PhysicsLayer::Send(unsigned char* ppayload, int frameLength) {
	//AfxMessageBox("¹°¸®Ãþ Send È£ÃâµÊ");
	TRY{
		CFile bufFile("ipcBuffer.txt", CFile::modeCreate | CFile::modeWrite);
	bufFile.Write(ppayload, frameLength);
	bufFile.Close();
	}CATCH(CFileException, e) {
		return FALSE;
	}END_CATCH

		return TRUE;
}
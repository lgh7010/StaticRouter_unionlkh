#include "stdafx.h"
#include "PhysicsLayer.h"

/*
	���� ���� Send�� �翬�� �������� '�ñ׳�'��(������ to �ñ׳�) �ٲ㼭 �����ٿ� �ɾ��ִ� ������ �ؾ� ������,
	�츮�� ����Ʈ����θ� �װ��� �����ϴ� ���̰� ������ ������ �ϴ� ���� ����̽� ���� IPC�� ���ؼ� �ٷ�� ������,
	�׳� ���Ͽ� ���°��� '�۽�'�� �Ϸ�� ������ �����Ѵ�.

	����, '����'���� �ش� ������ '�д�'������ ����Ѵ�. Receive�Լ��� �̸� �������� �����Ǿ� �ִ�.
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
#include "stdafx.h"
#include "LayerStructure.h"

//�����ڿ� �Ҹ���
LayerStructure::LayerStructure(char* pName)
: _upperLayerCount(0), _pUnderLayer(NULL) {
	this->_pLayerName = pName;
}
LayerStructure::~LayerStructure(){}

//�ֿ� �����Լ���
void LayerStructure::SetUnderLayer(LayerStructure* pLayer) {
	if (!pLayer) {
		AfxMessageBox(_T("�������̾� ������ �������۵�"));
		return;
	}
	
	this->_pUnderLayer = pLayer;//�� �ù� �̰� UpperLayer�� �߸��Ǿ��־���. �׷��� ���̾� ������ �ȵƴ��ž� �ù� �̰ɷ� �Ϸ縦 �����ٴ� ����
}
void LayerStructure::SetUpperLayer(LayerStructure* pLayer) {
	if (!pLayer){
		AfxMessageBox(_T("�������̾� ������ �������۵�"));
		return;
	}

	this->_pUpperLayer = pLayer;
	this->_upperLayerCount++;
}
LayerStructure* LayerStructure::GetUpperLayer(int index) {
	if (index < 0 || index > this->_upperLayerCount || this->_upperLayerCount < 0)
		return NULL;

	return this->_pUpperLayer;
}
LayerStructure* LayerStructure::GetUpperLayer() {
	if (!this->_pUpperLayer)
		return NULL;

	return this->_pUpperLayer;
}
LayerStructure* LayerStructure::GetUnderLayer() {
	if (!this->_pUnderLayer)
		return NULL;

	return this->_pUnderLayer;
}
char* LayerStructure::GetLayerName() {
	return this->_pLayerName;
}
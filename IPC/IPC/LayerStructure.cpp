#include "stdafx.h"
#include "LayerStructure.h"

//생성자와 소멸자
LayerStructure::LayerStructure(char* pName)
: _upperLayerCount(0), _pUnderLayer(NULL) {
	this->_pLayerName = pName;
}
LayerStructure::~LayerStructure(){}

//주요 공개함수들
void LayerStructure::SetUnderLayer(LayerStructure* pLayer) {
	if (!pLayer) {
		AfxMessageBox(_T("하위레이어 설정이 비정상작동"));
		return;
	}
	
	this->_pUnderLayer = pLayer;//와 시발 이게 UpperLayer로 잘못되어있었어. 그래서 레이어 연결이 안됐던거야 시발 이걸로 하루를 보내다니 씨발
}
void LayerStructure::SetUpperLayer(LayerStructure* pLayer) {
	if (!pLayer){
		AfxMessageBox(_T("상위레이어 설정이 비정상작동"));
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
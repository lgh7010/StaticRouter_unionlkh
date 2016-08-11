#include "stdafx.h"
#include "LayerStructure.h"

//생성자와 소멸자
LayerStructure::LayerStructure(char* name)
: _upperLayerCount(0), _underLayer(NULL) {
	this->_layerName = name;
}
LayerStructure::~LayerStructure(){}

//주요 공개함수들
void LayerStructure::SetUnderLayer(LayerStructure* layer) {
	if (!layer)//이건 if(layer == null)과 같다
		return;
	
	this->_upperLayer = layer;
}
void LayerStructure::SetUpperLayer(LayerStructure* layer) {
	if (!layer)//이건 if(layer == null)과 같다
		return;

	this->_underLayer = layer;
}
LayerStructure* LayerStructure::GetUpperLayer(int index) {
	if (index < 0 || index > this->_upperLayerCount || this->_upperLayerCount < 0)
		return NULL;

	return this->_upperLayer;
}
LayerStructure* LayerStructure::GetUnderLayer() {
	if (!this->_underLayer)
		return NULL;

	return this->_underLayer;
}
char* LayerStructure::GetLayerName() {
	return this->_layerName;
}
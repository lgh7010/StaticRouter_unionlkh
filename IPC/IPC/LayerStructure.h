#pragma once
class LayerStructure
{
public:
	LayerStructure(char* pName = NULL);//각 '계층'을 의미하는 객체들은 생성과 동시에 이름이 정해지는게 좋다.
	~LayerStructure();

	char*				GetLayerName();
	LayerStructure*		GetUnderLayer();
	LayerStructure*		GetUpperLayer(int index);
	LayerStructure*		GetUpperLayer();
	//지금은 상하위 레이어가 하나씩 뿐이지만, 나중에는 상위레이어가 여러개인 경우도 있을 수 있다. 인덱스로 어떤 상위레이어인지 찾는다
	void				SetUnderLayer(LayerStructure* pLayer = NULL);
	void				SetUpperLayer(LayerStructure* pLayer = NULL);

	/*
		위에 있는 공개함수들(SetUnderLayer...등)과는 달리, 아래 두개의 Send Receive함수는
		각각의 레이어 객체마다 동작하는 로직이 다르다.

		virtual로 선언하는것은 말 그대로 추상함수를 만드는 것이다. 즉, 이름은 같지만
		내용이 다른 함수들을 만들기 위한 뼈대라는 것이다.

		virtual로 선언하지 않으면, 예를들어서 EthernetLayer에서 Receive를 호출했을 때,
		이 LayerStructure 클래스에 있는 함수를 호출한다. 그러나 LayerStructure클래스로 만들어진
		객체는 실제로 존재하지 않기 때문에(이 클래스는 다른 계층 클래스에 상속을 주기만 한다)
		함수를 호출할 수 없다는 메시지가 뜰 것이다.
	*/
	virtual BOOL		Send(unsigned char*, int) { return FALSE; }//다만 virtual로 선언할 때는, 이렇게 '기본 리턴값'을 지정해줘야 한다.
	virtual BOOL		Receive(unsigned char* ppayload) { return FALSE; }

protected:
	char*				_pLayerName;
	LayerStructure*		_pUnderLayer;//현재는 레이어가 위아래로 무조건 하나이다.
	LayerStructure*		_pUpperLayer;//나중에 다중연결이 필요한 경우에는 배열로 바꿔야 한다.
	int					_upperLayerCount;//이 객체(예를들어, LayerStructure를 상속받은 EthernetLayer의 입장에서)의 직계 상위 레이어가 몇개인지. 지금은 항상 1
};


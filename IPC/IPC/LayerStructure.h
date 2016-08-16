#pragma once
class LayerStructure
{
public:
	LayerStructure(char* pName = NULL);//�� '����'�� �ǹ��ϴ� ��ü���� ������ ���ÿ� �̸��� �������°� ����.
	~LayerStructure();

	char*				GetLayerName();
	LayerStructure*		GetUnderLayer();
	LayerStructure*		GetUpperLayer(int index);
	LayerStructure*		GetUpperLayer();
	//������ ������ ���̾ �ϳ��� ��������, ���߿��� �������̾ �������� ��쵵 ���� �� �ִ�. �ε����� � �������̾����� ã�´�
	void				SetUnderLayer(LayerStructure* pLayer = NULL);
	void				SetUpperLayer(LayerStructure* pLayer = NULL);

	/*
		���� �ִ� �����Լ���(SetUnderLayer...��)���� �޸�, �Ʒ� �ΰ��� Send Receive�Լ���
		������ ���̾� ��ü���� �����ϴ� ������ �ٸ���.

		virtual�� �����ϴ°��� �� �״�� �߻��Լ��� ����� ���̴�. ��, �̸��� ������
		������ �ٸ� �Լ����� ����� ���� ������ ���̴�.

		virtual�� �������� ������, ������ EthernetLayer���� Receive�� ȣ������ ��,
		�� LayerStructure Ŭ������ �ִ� �Լ��� ȣ���Ѵ�. �׷��� LayerStructureŬ������ �������
		��ü�� ������ �������� �ʱ� ������(�� Ŭ������ �ٸ� ���� Ŭ������ ����� �ֱ⸸ �Ѵ�)
		�Լ��� ȣ���� �� ���ٴ� �޽����� �� ���̴�.
	*/
	virtual BOOL		Send(unsigned char*, int) { return FALSE; }//�ٸ� virtual�� ������ ����, �̷��� '�⺻ ���ϰ�'�� ��������� �Ѵ�.
	virtual BOOL		Receive(unsigned char* ppayload) { return FALSE; }

protected:
	char*				_pLayerName;
	LayerStructure*		_pUnderLayer;//����� ���̾ ���Ʒ��� ������ �ϳ��̴�.
	LayerStructure*		_pUpperLayer;//���߿� ���߿����� �ʿ��� ��쿡�� �迭�� �ٲ�� �Ѵ�.
	int					_upperLayerCount;//�� ��ü(�������, LayerStructure�� ��ӹ��� EthernetLayer�� ���忡��)�� ���� ���� ���̾ �����. ������ �׻� 1
};


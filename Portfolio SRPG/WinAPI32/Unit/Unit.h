//#pragma once
//
///*
//�������� pull�� push �ܾ�� �Ȱ��� �� ����.
//�������� ����� Ǯ��İ� ���ִ� ����� Ǫ������� �ִ�.
//*/
//
//
//// ����'���ϴ�' Ŭ����
//class Observable {
//protected:
//	std::vector<Observer*> observerList;        // �ڽ��� �����ϴ� ��ü���� ����Ʈ�� ����������
//	virtual ~Observable() {};
//	bool changed = false;						//Ʈ�簡 �� ������ ������Ʈ�����ش�.
//
//public:
//	virtual void AddObserver(Observer*) = 0;     // �� �޼ҵ带 �̿��Ͽ� ������ �ϰڴٰ� ����
//	virtual void RemoveObserver(Observer*)=0;	// �� �޼ҵ带 �̿��Ͽ� ������ ��ġ�ڴٰ� ����
//	virtual void NotifyObservers() = 0;			//�˸��� ���ش�.
//
//	//change�� Ʈ��� �ٲ��ش�.
//	void SetChanged()
//	{
//		changed = true;
//	}
//	//change�� Ʈ��� �ٲ��ش�.
//	void ClearChanged()
//	{
//		changed = false;
//	}
//	//chnged�� ���� ��ȯ�Ѵ�.
//	bool hasChange()
//	{
//		return changed;
//	}
//};
//
//
//// ����'�ϴ�' Ŭ����
//class Observer {
//public:
//	virtual void update() {};				//�������� ���°� �����ϸ� ���� �� Ŭ����
//};
//
//
//
//
///*
//��������
//�ٲ����� �ൿ �˰����� ĸ��ȭ�Ѵ�.
//
//ĸ���� �ٲ�� �������� �ٸ� �˰����� ������ �� �ִ� ���� ����
//*/
//
//
//__interface IWeaponBehavior
//{
//
//public:
//	void UseWeapon();
//};
//
////Į
//class Knife : public IWeaponBehavior
//{
//
//private:
//
//
//public:
//
//	// IWeaponBehavior��(��) ���� ��ӵ�
//	virtual void UseWeapon() override
//	{
//		cout << "Į�� ����" << endl;
//	}
//
//};
//
////Ȱ��ȭ��
//class BowAndArrow : public IWeaponBehavior
//{
//
//private:
//
//
//public:
//
//	// IWeaponBehavior��(��) ���� ��ӵ�
//	virtual void UseWeapon() override
//	{
//		cout << "Ȱ�� ���" << endl;
//	}
//
//};
//
////����
//class Axe : public IWeaponBehavior
//{
//
//private:
//
//
//public:
//
//	// IWeaponBehavior��(��) ���� ��ӵ�
//	virtual void UseWeapon() override
//	{
//		cout << "������ �������" << endl;
//	}
//
//};
//
//
//class Sword : public IWeaponBehavior
//{
//
//private:
//
//
//public:
//
//	// IWeaponBehavior��(��) ���� ��ӵ�
//	virtual void UseWeapon() override
//	{
//		cout << "���� �ֵθ���!" << endl;
//	}
//
//};
//
//
//class Staff : public IWeaponBehavior
//{
//
//private:
//
//
//public:
//
//	// IWeaponBehavior��(��) ���� ��ӵ�
//	virtual void UseWeapon() override
//	{
//		cout << "������ �ֵθ���!" << endl;
//	}
//
//};
//
//
//
//
//class Unit
//{
//protected:
//	IWeaponBehavior* weapon;
//
//public:
//
//	void Fight()
//	{
//		weapon->UseWeapon();
//	}
//
//	void SetWeapon(IWeaponBehavior* weapon_)
//	{
//		weapon = weapon_;
//	}
//
//};
//
//class Acher : public Unit
//{
//
//public:
//	Acher()
//	{
//		weapon = new BowAndArrow();
//	}
//
//};
//
//class Wizard : public Unit
//{
//
//public:
//	Wizard()
//	{
//		weapon = new Staff();
//	}
//
//};
//
//class Inspection : public Unit
//{
//public:
//	Inspection()
//	{
//		weapon = new Sword();
//	}
//};
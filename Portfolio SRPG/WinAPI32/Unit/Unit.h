//#pragma once
//
///*
//깃허브브의 pull과 push 단어와 똑같은 것 같다.
//가져오는 방식인 풀방식과 보주는 방식인 푸쉬방식이 있다.
//*/
//
//
//// 관찰'당하는' 클래스
//class Observable {
//protected:
//	std::vector<Observer*> observerList;        // 자신을 관찰하는 객체들을 리스트로 가지고있음
//	virtual ~Observable() {};
//	bool changed = false;						//트루가 될 때마다 업데이트시켜준다.
//
//public:
//	virtual void AddObserver(Observer*) = 0;     // 이 메소드를 이용하여 관찰을 하겠다고 선언
//	virtual void RemoveObserver(Observer*)=0;	// 이 메소드를 이용하여 관찰을 마치겠다고 선언
//	virtual void NotifyObservers() = 0;			//알림을 해준다.
//
//	//change를 트루로 바꿔준다.
//	void SetChanged()
//	{
//		changed = true;
//	}
//	//change를 트루로 바꿔준다.
//	void ClearChanged()
//	{
//		changed = false;
//	}
//	//chnged의 값을 반환한다.
//	bool hasChange()
//	{
//		return changed;
//	}
//};
//
//
//// 관찰'하는' 클래스
//class Observer {
//public:
//	virtual void update() {};				//옵저버가 상태가 변경하면 실행 될 클래스
//};
//
//
//
//
///*
//전략패턴
//바뀌어야할 행동 알고리즘군을 캡슐화한다.
//
//캡슐만 바뀌면 언제든지 다른 알고리즘을 실행할 수 있는 편한 패턴
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
////칼
//class Knife : public IWeaponBehavior
//{
//
//private:
//
//
//public:
//
//	// IWeaponBehavior을(를) 통해 상속됨
//	virtual void UseWeapon() override
//	{
//		cout << "칼로 베기" << endl;
//	}
//
//};
//
////활과화살
//class BowAndArrow : public IWeaponBehavior
//{
//
//private:
//
//
//public:
//
//	// IWeaponBehavior을(를) 통해 상속됨
//	virtual void UseWeapon() override
//	{
//		cout << "활을 쏘기" << endl;
//	}
//
//};
//
////도끼
//class Axe : public IWeaponBehavior
//{
//
//private:
//
//
//public:
//
//	// IWeaponBehavior을(를) 통해 상속됨
//	virtual void UseWeapon() override
//	{
//		cout << "도끼로 내려찍기" << endl;
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
//	// IWeaponBehavior을(를) 통해 상속됨
//	virtual void UseWeapon() override
//	{
//		cout << "검을 휘두르기!" << endl;
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
//	// IWeaponBehavior을(를) 통해 상속됨
//	virtual void UseWeapon() override
//	{
//		cout << "지팡이 휘두르기!" << endl;
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
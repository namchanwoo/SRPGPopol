#pragma once


//전방선언 일일이 해주는 게 너무 귀찮을수도?
template <class T>
class StateMachine;


template<class T>
class State
{

private:



protected:

	StateMachine<T> stateMachine;
	T context;

public:

	

	State()
	{}

	//스테이트 머신과 자기자신
	void SetStateMachineAndContext(StateMachine<T> stateMachine_,T context_)
	{
		stateMachine = stateMachine_;
		context = context_;
		Init();
	}

	//초기화
	virtual void Init()
	{

	}

	//상태가 되었을 때
	virtual void OnEnter()
	{

	}

	//실행해야 할상태
	virtual void Update()
	{

	}

	//종료될 때
	virtual void OnExit()
	{

	}


};



template <class T>
class StateMachine
{

private:

	T context;

	//현재 상태
	State<T> currentState;

	//그 전 상태
	State<T> priviousState;


	//현재 상태로 전환되었을 때 흐른 시간
	float elapsedTimeInState = 0.0f;



	//키와 스테이트를 가지고 있는 건데
	unordered_map<string, State<T>> states;

public:


	StateMachine(T context_, State<T> initialState_)
	{
		context = context_;
		AddState(initialState_);
		currentState = initialState_;
		currentState.OnEnter();
	}

	//스테이트 추가하기
	void AddState(State<T> state)
	{
		state.SetStateMachineAndContext(this, context);
		states.insert(make_pair(typeid(state).name, state));
	}



	void Update()
	{
		elapsedTimeInState += DeltaTime;
		currentState.Update();
	}



	void ChangeState(State<T> chageState)
	{

		//만약 바꾸려고 하는 스테이트가 현재와 같다면 바꾸지마라
		if (typeid(currentState).name == typeid(chageState).name)
		{
			return;
		}

		if (currentState != NULL)
		{
			currentState.OnExit();
		}

		//그 전 상태를 갱신해준다.
		priviousState = currentState;
		//현재 상태를 바꾸자
		currentState = chageState;
		//현재 상태의 처음을 실행해라 이닛이랑 고민중
		currentState.OnEnter();
		//시간을 0초로
		elapsedTimeInState = 0.0f;

	}


	
	//currState Getter
	MAKEGET(State<T>, currentState)
	//priviousState
	MAKEGET(State<T>, priviousState)
	//elapsedTimeInState  Getter
	MAKEGET(float, elapsedTimeInState)

};






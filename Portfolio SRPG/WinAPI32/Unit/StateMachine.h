#pragma once


//���漱�� ������ ���ִ� �� �ʹ� ����������?
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

	//������Ʈ �ӽŰ� �ڱ��ڽ�
	void SetStateMachineAndContext(StateMachine<T> stateMachine_,T context_)
	{
		stateMachine = stateMachine_;
		context = context_;
		Init();
	}

	//�ʱ�ȭ
	virtual void Init()
	{

	}

	//���°� �Ǿ��� ��
	virtual void OnEnter()
	{

	}

	//�����ؾ� �һ���
	virtual void Update()
	{

	}

	//����� ��
	virtual void OnExit()
	{

	}


};



template <class T>
class StateMachine
{

private:

	T context;

	//���� ����
	State<T> currentState;

	//�� �� ����
	State<T> priviousState;


	//���� ���·� ��ȯ�Ǿ��� �� �帥 �ð�
	float elapsedTimeInState = 0.0f;



	//Ű�� ������Ʈ�� ������ �ִ� �ǵ�
	unordered_map<string, State<T>> states;

public:


	StateMachine(T context_, State<T> initialState_)
	{
		context = context_;
		AddState(initialState_);
		currentState = initialState_;
		currentState.OnEnter();
	}

	//������Ʈ �߰��ϱ�
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

		//���� �ٲٷ��� �ϴ� ������Ʈ�� ����� ���ٸ� �ٲ�������
		if (typeid(currentState).name == typeid(chageState).name)
		{
			return;
		}

		if (currentState != NULL)
		{
			currentState.OnExit();
		}

		//�� �� ���¸� �������ش�.
		priviousState = currentState;
		//���� ���¸� �ٲ���
		currentState = chageState;
		//���� ������ ó���� �����ض� �̴��̶� �����
		currentState.OnEnter();
		//�ð��� 0�ʷ�
		elapsedTimeInState = 0.0f;

	}


	
	//currState Getter
	MAKEGET(State<T>, currentState)
	//priviousState
	MAKEGET(State<T>, priviousState)
	//elapsedTimeInState  Getter
	MAKEGET(float, elapsedTimeInState)

};






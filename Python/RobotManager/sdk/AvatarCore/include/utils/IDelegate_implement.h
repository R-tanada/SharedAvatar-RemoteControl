

#define MakeOne(Name1, Name2) MakeOne1(Name1, Name2)
#define MakeOne1(Name1, Name2) Name1##Name2

#define IDELEGATE_NAME MakeOne(IDelegate, DELEG_PCOUNT)
#define ClassDelegate_NAME MakeOne(ClassDelegate, DELEG_PCOUNT)
#define MethodDelegate_NAME MakeOne(MethodDelegate, DELEG_PCOUNT)
#define DelegateEvent_NAME MakeOne(DelegateEvent, DELEG_PCOUNT)
#define newClassDelegate_NAME MakeOne(newClassDelegate, DELEG_PCOUNT)
#define newMethodDelegate_NAME MakeOne(newMethodDelegate, DELEG_PCOUNT)

template <typename RETVAL, DELEG_PARAMS_T>
class IDELEGATE_NAME
{
	std::string m_name;

public:
	IDELEGATE_NAME(const std::string &n)
	{
		m_name = n;
	}
	virtual ~IDELEGATE_NAME()
	{
	}
	virtual RETVAL operator()(DELEG_PARAMS) = 0;
	const std::string &getName() { return m_name; }
	void setName(const std::string &n) { m_name = n; }
};

template <class TObj, typename RETVAL, DELEG_PARAMS_T>
class ClassDelegate_NAME : public IDELEGATE_NAME<RETVAL, DELEG_ARGS_T>
{
	typedef RETVAL (TObj::*PMethod)(DELEG_PARAMS);
	TObj *m_o;
	PMethod m_p;

public:
	ClassDelegate_NAME(const std::string &n, TObj *o, PMethod p) // : IDELEGATE_NAME(n)
	{
		m_o = o;
		m_p = p;
	}
	virtual RETVAL operator()(DELEG_PARAMS)
	{
		return (m_o->*m_p)(DELEG_ARGS);
	}
};
template <typename RETVAL, DELEG_PARAMS_T>
class MethodDelegate_NAME : public IDELEGATE_NAME<RETVAL, DELEG_ARGS_T>
{

	typedef RETVAL (*PMethod)(DELEG_PARAMS);
	PMethod m_p;

public:
	MethodDelegate_NAME(const std::string &n, PMethod p) //: IDELEGATE_NAME(n)
	{
		m_p = p;
	}
	virtual RETVAL operator()(DELEG_PARAMS)
	{
		return (*m_p)(DELEG_ARGS);
	}
};

template <class TObj, typename RETVAL, DELEG_PARAMS_T>
IDELEGATE_NAME<RETVAL, DELEG_ARGS_T> *newClassDelegate_NAME(const std::string &name, TObj *o, RETVAL (TObj::*PMethod)(DELEG_PARAMS))
{
	return new ClassDelegate_NAME<TObj, RETVAL, DELEG_ARGS_T>(name, o, PMethod);
}

template <typename RETVAL, DELEG_PARAMS_T>
IDELEGATE_NAME<RETVAL, DELEG_ARGS_T> *newMethodDelegate_NAME(const std::string &name, RETVAL (*PMethod)(DELEG_PARAMS))
{
	return new MethodDelegate_NAME<RETVAL, DELEG_ARGS_T>(name, PMethod);
}

template <DELEG_PARAMS_T>
class DelegateEvent_NAME : public IDelegateEvent
{
public:
	typedef std::list<IDELEGATE_NAME<void, DELEG_ARGS_T> *> DelegateList;

protected:
	DelegateList m_delegates;

public:
	DelegateEvent_NAME() : IDelegateEvent(aT(""))
	{
	}
	DelegateEvent_NAME(const std::string &name) : IDelegateEvent(name)
	{
	}
	virtual ~DelegateEvent_NAME() {}

	std::list<IDELEGATE_NAME<void, DELEG_ARGS_T> *> getDelegates() { return m_delegates; }

	void operator+=(IDELEGATE_NAME<void, DELEG_ARGS_T> *d)
	{
		m_delegates.push_back(d);
	}

	void operator-=(IDELEGATE_NAME<void, DELEG_ARGS_T> *d)
	{
		typename DelegateList::iterator it = m_delegates.begin();
		typename DelegateList::iterator end = m_delegates.end();
		for (; it != end; ++it)
		{
			if (*it == d)
			{
				m_delegates.erase(it);
				break;
			}
		}
	}

	void operator()(DELEG_PARAMS)
	{
		typename DelegateList::iterator it = m_delegates.begin();
		typename DelegateList::iterator end = m_delegates.end();
		for (; it != end; ++it)
		{
			(**it)(DELEG_ARGS);
		}
	}
	template <class TObj, typename RETVAL>
	void Connect(const std::string &name, TObj *o, RETVAL (TObj::*PMethod)(DELEG_PARAMS))
	{
		m_delegates.push_back(new ClassDelegate_NAME<TObj, RETVAL, DELEG_ARGS_T>(name, o, PMethod));
	}
	template <typename RETVAL>
	void Connect(const std::string &name, RETVAL (*PMethod)(DELEG_PARAMS))
	{
		m_delegates.push_back(new MethodDelegate_NAME<RETVAL, DELEG_ARGS_T>(name, PMethod));
	}
};

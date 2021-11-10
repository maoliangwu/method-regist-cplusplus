#pragma once
#include "i_comInterface.h"
#include "CheckArgType.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QList>
#include <QPair>
#include <QDebug>

//#include "reader.h"
Q_DECLARE_METATYPE(std::string)

//对字符串类型的参数，需要考虑形参是std::string，入参是字符串，则QVariant自动转换出的是QString，需要手动tostdstring,否则导致实参与实际传值步匹配
#define TranslateVariant(obj, i, X,lst)\
if (QString::fromStdString(obj[obj.keys()[i]].toVariant().typeName()) == "QString"&& qMetaTypeId<X>() == qMetaTypeId<std::string>())\
{\
	lst.push_back(QVariant::fromValue<std::string>(obj[obj.keys()[i]].toString().toStdString()));\
}\
else\
{\
	lst.push_back(obj[obj.keys()[i]].toVariant());\
}

template<class R, typename... Args>
class SimpleMethod
{
public:
	SimpleMethod()
	{

	}
	SimpleMethod(std::function<R(Args...)> func):signal_func_(func){};
	~SimpleMethod()
	{

	};
	R operator()(Args... args)
	{
		return signal_func_(args...);
	};
private:
	std::function<R(Args...)>  signal_func_;
};

template< class R, typename... Args>
class ComMethod :
	public IComInterface
{
public:
	ComMethod(std::function<R(Args...)> func, const QList<QPair<QVariant::Type, std::string> >& params):
		params_(params)
	{
		mem_func_ = SimpleMethod<R, Args...>(func);
	}

	bool checkArg(const char* pchReq)
	{
		QJsonObject obj;
		if (GetJsonParams(pchReq, obj))
		{
			int req_param_counts = obj.keys().size();
			if (req_param_counts != params_.size())
			{
				return false;
			}
			QVariant ret;
			for (int i = 0; i < req_param_counts; i++)
			{
				if (obj.keys()[i].toStdString() != params_[i].second)
				{
					return false;
				}
				bool flag; 
				CHECK_ARG(obj, params_, i, flag)
				if (!flag)
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	bool invoke(const char* pchReq, const std::function<void(QVariant, QJsonObject&)> func, QJsonObject&dealresult)
	{
		return true;
	}
	bool ArgEqual(const QList<QPair<QVariant::Type, std::string> >& params)
	{
		if (params_.size() != params.size())
		{
			return false;
		}
		int size = params_.size();
		for (int i = 0; i < size; i++)
		{
			if (params_[i] != params[i])
			{
				return false;
			}
		}
		return true;
	}
	QList<QPair<QVariant::Type, std::string> > Get_Args()
	{
		return params_;
	}
protected:
	bool GetJsonParams(const char* pchReq, QJsonObject& json)
	{
		if (strlen(pchReq) > 0)
		{
			QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(pchReq).toLocal8Bit());
			json = doc.object();
		}
		return !json.isEmpty();
	}
	SimpleMethod<R, Args...> mem_func_;
	QList<QPair<QVariant::Type, std::string> > params_;
};


template< class R, class A>
class ComMethod1 :
	public ComMethod<R, A>
{
public:
	ComMethod1(std::function<R(A)> func, const QList<QPair<QVariant::Type, std::string> >& params) :ComMethod(func, params)
	{

	}
	bool invoke(const char* pchReq, const std::function<void(QVariant, QJsonObject&)> func, QJsonObject&dealresult)
	{
		QJsonObject obj;
		if (GetJsonParams(pchReq, obj))
		{
			QList<QVariant> lst;
			int req_param_counts = obj.keys().size();
			R ret;
			switch (req_param_counts)
			{
			case 1:
			{
				TranslateVariant(obj, 0, A,lst);
				ret = mem_func_(lst[0].value<A>());
			}
			break;
			default:
				break;
			}
			func(QVariant::fromValue<R>(ret), dealresult);
			return true;
		}
		else
		{
			return false;
		}
	}
};


template< class R, class A, class B>
class ComMethod2 :
	public  ComMethod<R, A, B>
{
public:
	ComMethod2(std::function<R(A,B)> func, const QList<QPair<QVariant::Type, std::string> >& params) :ComMethod(func, params)
	{

	}
	bool invoke(const char* pchReq, const std::function<void(QVariant, QJsonObject&)> func, QJsonObject&dealresult)
	{
		QJsonObject obj;
		if (GetJsonParams(pchReq, obj))
		{
			QList<QVariant> lst;
			int req_param_counts = obj.keys().size();
			R ret;
			switch (req_param_counts)
			{
			case 2:
			{
				TranslateVariant(obj, 0,A, lst);
				TranslateVariant(obj, 1,B, lst);
				ret = mem_func_(lst[0].value<A>(),lst[1].value<B>());
			}
			break;
			default:
				break;
			}
			func(QVariant::fromValue<R>(ret), dealresult);
			return true;
		}
		else
		{
			return false;
		}
	}
};


template< class R, class A, class B, class C>
class ComMethod3 :
	public ComMethod<R, A,B,C>
{
public:
	ComMethod3(std::function<R(A, B, C)> func, const QList<QPair<QVariant::Type, std::string> >& params) :ComMethod(func, params)
	{

	}
	bool invoke(const char* pchReq, const std::function<void(QVariant, QJsonObject&)> func, QJsonObject&dealresult)
	{
		QJsonObject obj;
		if (GetJsonParams(pchReq, obj))
		{
			QList<QVariant> lst;
			int req_param_counts = obj.keys().size();
			R ret;
			switch (req_param_counts)
			{
			case 3:
			{
				TranslateVariant(obj, 0, A, lst);
				TranslateVariant(obj, 1, B, lst);
				TranslateVariant(obj, 2, C, lst);
				/*
				lst.push_back(QVariant::fromValue(obj[obj.keys()[0]].toVariant().value<A>()));
				lst.push_back(QVariant::fromValue(obj[obj.keys()[1]].toVariant().value<B>()));
				lst.push_back(QVariant::fromValue(obj[obj.keys()[2]].toVariant().value<C>()));*/
				ret = mem_func_(lst[0].value<A>(),lst[1].value<B>(),lst[2].value<C>());
			}
			break;
			default:
				break;
			}
			func(QVariant::fromValue<R>(ret), dealresult);
			return true;
		}
		else
		{
			return false;
		}
	}
};

#pragma once
#include "i_comInterface.h"
#include "CheckArgType.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QList>
#include <QPair>
#include <QDebug>
#include <memory>

//#include "reader.h"
Q_DECLARE_METATYPE(std::string)

//对字符串类型的参数，需要考虑形参是std::string，入参是字符串，则QVariant自动转换出的是QString，需要手动tostdstring,否则导致实参与实际传值步匹配
#define TranslateVariant(obj, i, params, X,lst)\
if (QString::fromStdString(obj[params[i].second.c_str()].toVariant().typeName()) == "QString"&& qMetaTypeId<X>() == qMetaTypeId<std::string>())\
{\
	lst.push_back(QVariant::fromValue<std::string>(obj[params[i].second.c_str()].toString().toStdString()));\
}\
else\
{\
	lst.push_back(obj[params[i].second.c_str()].toVariant());\
}

template<class R, class FuncObj, typename... Args>
class SimpleMethod
{
public:
	SimpleMethod()
	{

	}
	SimpleMethod(std::shared_ptr<FuncObj> obj, std::function<R(Args...)> func) :signal_func_(func), obj_(obj) {};
	~SimpleMethod()
	{
		obj_ = nullptr;
	};
	R operator()(Args... args)
	{
		return signal_func_(args...);
	};
private:
	std::function<R(Args...)>  signal_func_;
	std::shared_ptr<FuncObj> obj_{ nullptr };
};

template< class R, class FuncObj, typename... Args>
class ComMethod :
	public IComInterface
{
public:
	ComMethod(std::function<R(Args...)> func, std::shared_ptr<FuncObj> obj, const QList<QPair<QVariant::Type, std::string> >& params) :
		params_(params)
	{
		mem_func_ = std::make_shared <SimpleMethod<R, FuncObj, Args...>>(obj, func);
	}
	~ComMethod()
	{
		mem_func_ = nullptr;
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
				if (!std::any_of(params_.begin(), params_.end(), [obj, i](const QPair<QVariant::Type, std::string>& p) {
					return p.second == obj.keys()[i].toStdString();
				}))
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
			auto s = QString::fromUtf8(pchReq);
			auto b = s.toUtf8();
			QJsonDocument doc = QJsonDocument::fromJson(QString::fromUtf8(pchReq).toUtf8());
			json = doc.object();
		}
		return !json.isEmpty();
	}
	std::shared_ptr<SimpleMethod<R, FuncObj, Args...>> mem_func_;
	QList<QPair<QVariant::Type, std::string> > params_;
};


template< class R, class FuncObj, class A>
class ComMethod1 :
	public ComMethod<R, FuncObj, A>
{
public:
	ComMethod1(std::function<R(A)> func, std::shared_ptr<FuncObj> obj, const QList<QPair<QVariant::Type, std::string> >& params) :ComMethod(func, obj, params)
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
				TranslateVariant(obj, 0, params_, A, lst);
				(*mem_func_)(lst[0].value<A>());
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


template< class R, class FuncObj, class A, class B>
class ComMethod2 :
	public  ComMethod<R, FuncObj, A, B>
{
public:
	ComMethod2(std::function<R(A, B)> func, std::shared_ptr<FuncObj> obj, const QList<QPair<QVariant::Type, std::string> >& params) :ComMethod(func, obj, params)
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
				TranslateVariant(obj, 0, params_, A, lst);
				TranslateVariant(obj, 1, params_, B, lst);
				ret = (*mem_func_)(lst[0].value<A>(), lst[1].value<B>());
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


template< class R, class FuncObj, class A, class B, class C>
class ComMethod3 :
	public ComMethod<R, FuncObj, A, B, C>
{
public:
	ComMethod3(std::function<R(A, B, C)> func, std::shared_ptr<FuncObj> obj, const QList<QPair<QVariant::Type, std::string> >& params) :ComMethod(func, obj, params)
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
				TranslateVariant(obj, 0, params_, A, lst);
				TranslateVariant(obj, 1, params_, B, lst);
				TranslateVariant(obj, 2, params_, C, lst);
				/*
				lst.push_back(QVariant::fromValue(obj[obj.keys()[0]].toVariant().value<A>()));
				lst.push_back(QVariant::fromValue(obj[obj.keys()[1]].toVariant().value<B>()));
				lst.push_back(QVariant::fromValue(obj[obj.keys()[2]].toVariant().value<C>()));*/
				ret = (*mem_func_)(lst[0].value<A>(), lst[1].value<B>(), lst[2].value<C>());
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

template< class R, class FuncObj, class A, class B, class C, class D, class E>
class ComMethod5 :
	public ComMethod<R, FuncObj, A, B, C, D, E>
{
public:
	ComMethod5(std::function<R(A, B, C, D, E)> func, std::shared_ptr<FuncObj> obj, const QList<QPair<QVariant::Type, std::string> >& params) :ComMethod(func, obj, params)
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
			case 6:
			{
				TranslateVariant(obj, 0, params_, A, lst);
				TranslateVariant(obj, 1, params_, B, lst);
				TranslateVariant(obj, 2, params_, C, lst);
				TranslateVariant(obj, 3, params_, D, lst);
				TranslateVariant(obj, 4, params_, E, lst);
				ret = (*mem_func_)(lst[0].value<A>(), lst[1].value<B>(), lst[2].value<C>(), lst[3].value<D>(), lst[4].value<E>());
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

template< class R, class FuncObj, class A, class B, class C, class D, class E, class F>
class ComMethod6 :
	public ComMethod<R, FuncObj, A, B, C, D, E, F>
{
public:
	ComMethod6(std::function<R(A, B, C, D, E, F)> func, std::shared_ptr<FuncObj> obj, const QList<QPair<QVariant::Type, std::string> >& params) :ComMethod(func, obj, params)
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
			case 6:
			{
				TranslateVariant(obj, 0, params_, A, lst);
				TranslateVariant(obj, 1, params_, B, lst);
				TranslateVariant(obj, 2, params_, C, lst);
				TranslateVariant(obj, 3, params_, D, lst);
				TranslateVariant(obj, 4, params_, E, lst);
				TranslateVariant(obj, 5, params_, F, lst);
				ret = (*mem_func_)(lst[0].value<A>(), lst[1].value<B>(), lst[2].value<C>(), lst[3].value<D>(), lst[4].value<E>(), lst[5].value<F>());
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
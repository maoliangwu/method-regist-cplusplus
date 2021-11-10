#pragma once
#include <string>
#include <functional>
#include <QVariant>
#include <QList>
#include <QPair>
__interface IComInterface
{
public:
	bool invoke(const char* pchReq, const std::function<void(QVariant, QJsonObject&)> func, QJsonObject&dealresult) = 0;
	bool checkArg(const char* pchReq) = 0;
	bool ArgEqual(const QList<QPair<QVariant::Type, std::string> >& params) = 0;
	QList<QPair<QVariant::Type, std::string> > Get_Args() = 0;
};


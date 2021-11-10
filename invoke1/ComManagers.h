#pragma once
#include "com_method.h"
#include <map>
#include <string>
#include <List>
#include <QPointer>
class ComManagers
{
public:
	ComManagers();
	~ComManagers();
	
	bool Conflict(std::string, IComInterface*com);
	bool Register(std::string, IComInterface* com);
	bool UnRegister(std::string, QList<QPair<QVariant::Type, std::string> > params);

	QVariant Invoke(std::string req);
	QVariant Invoke(std::string funcname, const char* params);

	void ResultHandle(QVariant in_var, QJsonObject& out_var);
private:
	std::map<std::string, std::list<IComInterface*>> com_func_map_;
};
